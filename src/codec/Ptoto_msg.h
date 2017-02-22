#ifndef PROTO_MSG_H_
#define PROTO_MSG_H_

#include <google/protobuf/message.h>
#include <handy/conn.h>
#include <map>

typedef google::protobuf::Message Message;
typedef google::protobuf::Descriptor Descriptor;
typedef std::function<void (handy::TcpConnPtr con, Message* msg)> ProtoCallBack;

struct ProtoMsgCodec{
    static void encode(Message* msg, handy::Buffer& buf);
    static Message* decode(handy::Buffer* buf);
    static bool msgComplete(handy::Buffer& buf);
};

struct ProtoMsgDispatcher{
    void handle(handy::TcpConnPtr con, Message* msg);
    template<typename M> void onMsg(std::function<void(handy::TcpConnPtr con, M* msg)> cb){
        protocbs_[M::descriptor()] = [cb] (handy::TcpConnPtr con, Message* msg){
            cb(con, dynamic_cast<M*>(msg));
        };
    }
private:
    std::map<const Descriptor*, ProtoCallBack> protocbs_;
};

inline bool ProtoMsgCodec::msgComplete(handy::Buffer& buf){
    return buf.size() >= 4 && buf.size() >= *(uint32_t*)buf.begin();
}
#endif
