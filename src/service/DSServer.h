#ifndef DSSERVER_H_
#define DSSERVER_H_

#include <handy/handy.h>

#include "../codec/ExternalMsgCodec.h"
#include "../manager/StreamMgr.h"


//TODO
//add threadpool for load
//add one thread for store

class DSServer
{
public:
    //TODO add KeepAlive
    //add PlanCheck
    DSServer(handy::Ip4Addr, handy::Ip4Addr, int);
    void initAdmin(handy::EventBase*);
    void initStore(handy::EventBase*);
    void initLoad();
    void registerToMS();
    void keepAlive();
private:
    handy::Ip4Addr localAddr_;
    handy::Ip4Addr remoteAddr_;
    handy::EventBase* adminLoop_;
    handy::EventBase* storeLoop_;
    handy::ThreadPool storeThread_;
    handy::ThreadPool loadThreadPool_;
    handy::TcpConnPtr msAgent_;
    ExternalMsgDispatcher dispatcher_;
    shared_ptr<StreamMgr> streamMgr_;
    shared_ptr<SessionService> sesService_;
};

#endif
