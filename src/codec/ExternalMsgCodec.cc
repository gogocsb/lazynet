#include <string>
#include <endian.h>


#include <glog/logging.h>
#include <google/protobuf/descriptor.h>

#include "ExternalMsgCodec.h"

#define HTONL

using namespace std;
using namespace google::protobuf;

Cmd2TypeNameMap ExternalMsgCodec::cmd2Type_;
TypeName2CmdMap ExternalMsgCodec::type2Cmd_;

void ExternalMsgDispatcher::handle(TcpConnPtr con, BaseHead info, Message* msg)
{
    auto p = protocbs_.find(msg->GetDescriptor());
    if(p != protocbs_.end())
    {
         p->second(con, info, msg);
    }
    else{
        error("unknown message type %s", msg->GetTypeName().c_str());
    }
}


Message* ExternalMsgCodec::decode(Buffer& buf, BaseHead& info)
{
    if(buf.size() < 20)
    {
        error("buffer is too small size: %lu", buf.size());
        return NULL;
    }
    char* p = buf.data();
#ifdef HTONL
    info.cmd = htonl(*(uint32_t*)p);
    info.length = htonl(*(uint32_t*)(p+4));
    info.err = htonl(*(uint32_t*)(p+8));
    info.para1 = htonl(*(uint32_t*)(p+12));
    info.para2 = htonl(*(uint32_t*)(p+16));
#endif

#ifndef HTONL
    info.cmd = *(uint32_t*)p;
    info.length = *(uint32_t*)(p+4);
    info.err = *(uint32_t*)(p+8);
    info.para1 = *(uint32_t*)(p+12);
    info.para2 = *(uint32_t*)(p+16);
#endif

    std::string typeName = getType(info.cmd);
    Message* msg = NULL;
    const Descriptor* des = DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
    if(des)
    {
        const Message* proto = MessageFactory::generated_factory()->GetPrototype(des);
        if(proto)
        {
            msg = proto->New();
        }
    }
    if(msg == NULL)
    {
        error("cannot create Message for %s", typeName.c_str());
        return NULL;
    }
    int ret = msg->ParsePartialFromArray(p + sizeof(BaseHead), info.length);
    if(!ret)
    {
        error("bad msg for protobuf");
        delete msg;
        return NULL;
    }
    buf.consume(sizeof(BaseHead) + info.length);
    return msg;
}

void ExternalMsgCodec::onCodec(uint32_t cmd, string type)
{
    LOG(INFO) << "reg cmd" << cmd <<" type" << type;
    cmd2Type_[cmd] = type;
    type2Cmd_[type] = cmd;
}

uint32_t ExternalMsgCodec::getCmd(string typeName)
{
    return type2Cmd_[typeName];
}

string ExternalMsgCodec::getType(uint32_t cmd)
{
    LOG(INFO) << "cmd " << cmd << "getType "<< cmd2Type_[cmd];
    return cmd2Type_[cmd];
}


void ExternalMsgCodec::encode(Message* msg, Buffer& buf)
{
    size_t offset = buf.size();
    buf.appendValue((uint32_t)0);
    const string& typeName = msg->GetDescriptor()->full_name();
    buf.appendValue((uint32_t)typeName.size());
    buf.append(typeName.data(), typeName.size());
    msg->SerializeToArray(buf.allocRoom(msg->ByteSize()), msg->ByteSize());
    *(uint32_t*)(buf.begin() + offset) = buf.size() - offset;
}

void ExternalMsgCodec::encode(Message* msg, uint32_t err, uint32_t para1, uint32_t para2, Buffer& buf)
{
    BaseHead info;
    const string& typeName = msg->GetDescriptor()->full_name();
    LOG(INFO) << "encode typename " << typeName;
    info.cmd = htonl(getCmd(typeName));
#ifdef HTONL
    info.length = htonl(msg->ByteSize());
    info.err = htonl(err);
    info.para1 = htonl(para1);
    info.para2 = htonl(para2);
#endif

#ifndef HTONL
    info.length = msg->ByteSize();
    info.err = err;
    info.para1 = para1;
    info.para2 = para2;
#endif
    LOG(INFO) << "buf append" ;
    buf.appendValue(info.cmd);
    buf.appendValue(info.length);
    buf.appendValue(info.err);
    buf.appendValue(info.para1);
    buf.appendValue(info.para2);

    msg->SerializeToArray(buf.allocRoom(msg->ByteSize()), msg->ByteSize());
}


