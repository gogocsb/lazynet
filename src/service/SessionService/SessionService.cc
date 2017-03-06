#include <memory>

#include <glog/logging.h>
#include <handy/handy.h>

#include "SessionService.h"

using namespace std;

SessionService::SessionService(handy::EventBase* admin,
        handy::EventBase* store,
        handy::EventBase* load) :
    storeSesCount_(0),
    loadSesCount_(0),
    liveSesCount_(0),
    id_(0),
    storeSess_(),
    adminLoop_(admin),
    storeLoop_(store),
    loadLoop_(load)
{
    port_ = 3000;
    maxConn_ = 200;
}

uint64_t SessionService::newStoreSes()
{
    if(storeSesCount_ < maxConn_)
    {
        ++id_;
        StoreSesPtr storeSes = make_shared<StoreSession>();
        if(storeSes != nullptr)
        {
            LOG(INFO) << "session " << id_ << " is created";
            storeSess_[id_] = storeSes;
            return id_;
        }
        else
        {
            LOG(WARNING) << "StoreSession create Error";
            return 0;
        }
    }
    else
    {
        LOG(WARNING) << "too much conns";
        return 0;
    }
}

void SessionService::startStoreSes(uint64_t sesid)
{
    if(existStoreSes(sesid))
    {
        LOG(INFO) << "session " << sesid << " start store";
        storeSesCount_++;
        storeSess_[sesid]->startStore();
        return;
    }
    else
    {
        return;
    }
}

void SessionService::stopStoreSes(uint64_t sesid)
{
    if(existStoreSes(sesid))
    {
        LOG(INFO) << "session " << sesid << " stop store";
        storeSesCount_--;
        storeSess_[sesid]->stopStore();
        return;
    }
    else
    {
        return;
    }
}

void SessionService::attatchLive(uint64_t sesid, handy::Ip4Addr addr)
{
    if(existStoreSes(sesid))
    {
        LOG(INFO) << "session " << sesid <<" attatch Live(" << addr.ip() << ":" << addr.port() <<  ")";
        liveSesCount_++;
        storeSess_[sesid]->addOutStream(addr);
        return;
    }
    else
    {
        return;
    }
}

void SessionService::detachLive(uint64_t sesid, handy::Ip4Addr addr)
{
    if(existStoreSes(sesid))
    {
        LOG(INFO) << "session " << sesid << " detach Live(" << addr.ip() << ":" << addr.port() <<  ")";
        liveSesCount_--;
        storeSess_[sesid]->removeOutStream(addr);
        return;
    }
    else
    {
        return;
    }
}

bool SessionService::existStoreSes(uint64_t sesid)
{
    return (storeSess_.find(sesid) != storeSess_.end());
}

StoreSesPtr SessionService::getStoreSes(uint64_t sesid)
{
    if(existStoreSes(sesid))
        return storeSess_[sesid];
    else
        return nullptr;
}

