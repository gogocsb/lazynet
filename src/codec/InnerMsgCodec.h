#ifndef INNERCODEC_H_
#define INNERCODEC_H_

#include <string>
#include <map>

#include <google/protobuf/message.h>

#include <handy/conn.h>


using namespace handy;

typedef google::protobuf::Message Message;
typedef google::protobuf::Descriptor Descriptor;
typedef std::map<uint32_t, std::string> Cmd2TypeNameMap;
typedef std::map<std::string, uint32_t> TypeName2CmdMap;

struct BaseHead
{
    uint32_t cmd;
    uint32_t length;
    uint32_t err;
    uint32_t para1;
    uint32_t para2;
    BaseHead() :
        cmd(0),
        length(0),
        err(0),
        para1(0),
        para2(0)
    {

    }
    ~BaseHead()
    {

    }
};
typedef struct BaseHead BaseHead;
typedef std::function<void(TcpConnPtr con, BaseHead info, Message* msg)> ProtoCallBack;


struct InnerMsgCodec{
    static void encode(Message* msg, Buffer& buf);
    static void encode(Message* msg, uint32_t error, uint32_t para1, uint32_t para2, Buffer& buf);
    static Message* decode(Buffer& buf, BaseHead& info);
    static bool msgComplete(Buffer& buf);
    static void onCodec(uint32_t cmd, std::string type);
    static uint32_t getCmd(std::string type);
    static std::string getType(uint32_t cmd);
private:
    static Cmd2TypeNameMap cmd2Type_;
    static TypeName2CmdMap type2Cmd_;
};

struct InnerMsgDispatcher{
    void handle(TcpConnPtr con, BaseHead info, Message* msg);
    template<typename M> void onMsg(std::function<void(TcpConnPtr con, BaseHead info, M* msg)> cb){
        protocbs_[M::descriptor()] = [cb](TcpConnPtr con, BaseHead info, Message* msg){
            cb(con, info, dynamic_cast<M*>(msg));
        };
    }
private:
    std::map<const Descriptor*, ProtoCallBack> protocbs_;
};

inline bool InnerMsgCodec::msgComplete(Buffer& buf){
    return buf.size() >= 20;
}

#endif
