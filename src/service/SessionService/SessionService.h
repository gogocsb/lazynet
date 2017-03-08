#ifndef SESSIONSERVICE_H_
#define SESSIONSERVICE_H_

#include <map>
#include <vector>

#include <glog/logging.h>

#include <handy/handy.h>

#include "StoreSession.h"

using namespace std;

//SessionService only run in admin_thread
//but Session run across admin_thread and store_thread/load_thread


//TODO
//SessionService
    //init service set loop
    //has loadloop_pool
    //has storeloop
//StoreSession
    //create
    //stop
    //attatchLive
    //detachLive
//LoadSession
    //create
    //stop

class SessionService
{
public:
    SessionService(handy::EventBase* admin,
            handy::EventBase* store,
            handy::EventBase* load);
    //StoreSession interface
    uint64_t newStoreSes();
    void startStoreSes(uint64_t);
    void stopStoreSes(uint64_t);
    void attatchLive(uint64_t, handy::Ip4Addr);
    void detachLive(uint64_t, handy::Ip4Addr);
    bool existStoreSes(uint64_t);
    StoreSesPtr getStoreSes(uint64_t);
    //TODO LoadSession interface
    //uint64_t newLoadSes();
    //void startLoadSes();
    //void stopLoadSes();
private:
    typedef map<uint64_t, StoreSesPtr> Id2StoreMap;
    uint64_t storeSesCount_;
    uint64_t loadSesCount_;
    uint64_t liveSesCount_;
    uint64_t id_;
    uint16_t port_;
    uint16_t portInterval_;
    uint64_t maxConn_;
    Id2StoreMap storeSess_;
    //map<uint64_t, > loadMap_;
    handy::EventBase* adminLoop_;
    handy::EventBase* storeLoop_;
    handy::EventBase* loadLoop_;
};

#endif
