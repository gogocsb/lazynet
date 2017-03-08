#ifndef DSSERVER_H_
#define DSSERVER_H_

#include <handy/handy.h>

#include "../codec/ExternalMsgCodec.h"
#include "../manager/StreamMgr.h"

class DSServer
{
public:
    //TODO add KeepAlive
    //add PlanCheck
    DSServer(handy::EventBase*, handy::Ip4Addr, handy::Ip4Addr);
    void initServer();
    void registerToMS();
    void keepAlive();
private:
    handy::EventBase *adminLoop_;
    handy::Ip4Addr localAddr_;
    handy::Ip4Addr remoteAddr_;
    handy::TcpConnPtr msAgent_;
    ExternalMsgCodec codec_;
    ExternalMsgDispatcher dispatcher_;
    StreamMgr streamMgr_;
};

#endif
