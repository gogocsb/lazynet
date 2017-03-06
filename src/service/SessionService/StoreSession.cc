#include <handy/handy.h>

#include <glog/logging.h>

#include "StoreSession.h"


StoreSession::StoreSession(handy::EventBase* loop, handy::Ip4Addr addr)
    : addr_(addr), storeLoop_(loop)
{

}

void StoreSession::startStore()
{

}

void StoreSession::stopStore()
{

}

void StoreSession::addOutStream(handy::Ip4Addr outStream)
{
    storeLoop_->safeCall(
            [=]{
                outStreamVec_.push_back(outStream);
            });
}

void StoreSession::removeOutStream(handy::Ip4Addr outStream)
{
    storeLoop_->safeCall(
            [=]{
                auto it = find(outStreamVec_.begin(), outStreamVec_.end(), outStream);
                if(it == outStreamVec_.end())
                    return;
                else
                {
                    outStreamVec_.erase(it);
                    return;
                }
            });
}


void StoreSession::readCallBack()
{

}
