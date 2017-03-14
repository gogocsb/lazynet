#include <iostream>
#include <bitset>
#include <sys/time.h>
#include <string>

#include <glog/logging.h>

#include "StreamMgr.h"
#include "../codec/ExternalMsgCodec.h"
#include "../protocol/MS_DS.pb.h"


StreamMgr::StreamMgr():
    prePlans_(),
    curPlans_(),
    storeSess_(),
    loadSess_()
{
    port_ = 3000;
    portInterval_ = 10;
}

//day0 means sunday, change day0 to day7
uint32_t StreamMgr::getCurDay()
{
    time_t t;
    time(&t);
    uint32_t wd = localtime(&t)->tm_wday;
    return (wd == 0 ? 7 : wd);
}

uint32_t StreamMgr::getCurHour()
{
    time_t t;
    time(&t);
    return localtime(&t)->tm_hour;
}


void StreamMgr::createStream(string streamid)
{
    uint64_t sesId = sesServicePtr_->newStoreSes(port_);
    LOG(WARNING) << "after newStore " << sesId;
    if(sesId != 0)
    {
        //after recv INVITEACK, session attatch to a loop
        StoreSesInfo ses(sesId);
        storeSess_[streamid] = ses;
        LOG(INFO) << "stream " << streamid << " add StoreSession " << sesId;
        inviteStream(streamid, port_);
    }
    else
    {
        LOG(INFO) << "stream " << streamid << " add StoreSession fail";
    }
    port_ += portInterval_;
}

//TODO send bye and stop all session, clear map
void StreamMgr::stopAllStream()
{
    for(auto ses : storeSess_)
        stopStream(ses.first);
    //TODO clear map
    storeSess_.clear();
    return;
}

//TODO send bye and stop session, erase session in map
void StreamMgr::stopStream(string streamid)
{
    byeStream(streamid);
    sesServicePtr_->getStoreSes(storeSess_[streamid].sessionId)->stopStore();
    //TODO erase mmap
    storeSess_.erase(streamid);
    return;
}

void StreamMgr::inviteStream(string streamid, uint16_t port)
{
    MS_DS::ReadyStoreStreamInfo msg;
    msg.set_streamid(streamid);
    msg.set_recvcameraport(to_string(port));
    ExternalMsgCodec::encode(&msg, 0, 0, 0, msConn_->getOutput());
    msConn_->sendOutput();
    LOG(INFO) << "send Invite Camera";
    return;
}

void StreamMgr::startStream(string streamid, uint32_t p1, uint32_t p2 ,uint32_t err)
{
    if(err != 0)
    {
        LOG(WARNING) << "invite Camera Fail, wait for next time";
        // stop session and erase storesessionMap
        sesServicePtr_->getStoreSes(storeSess_[streamid].sessionId)->stopStore();
        storeSess_.erase(streamid);
    }
    else
    {
        storeSess_[streamid].para1 = p1,
        storeSess_[streamid].para2 = p2;
        uint64_t sesid = storeSess_[streamid].sessionId;
        sesServicePtr_->getStoreSes(sesid)->startStore();
        return;
    }
}


//TODO add Proto
void StreamMgr::byeStream(string streamid)
{
    uint32_t para1 = storeSess_[streamid].para1;
    uint32_t para2 = storeSess_[streamid].para2;
    ExternalMsgCodec::encode("MS_DS.Bye", para1, para2, 0, msConn_->getOutput());
    msConn_->sendOutput();
    LOG(WARNING) << "send Bye Camera";
    return;
}

bool StreamMgr::existStoreSes(string streamid)
{
    return (storeSess_.find(streamid) == storeSess_.end()) ? false : true;
}

bool StreamMgr::existLoadSes(string streamid)
{
    return (loadSess_.find(streamid) == loadSess_.end()) ? false : true;
}

uint64_t StreamMgr::getStoreSesId(string streamid)
{
    if(existStoreSes(streamid))
        return storeSess_[streamid].sessionId;
    else
        return 0;
}

uint64_t StreamMgr::getLoadSesId(string streamid)
{
    if(existLoadSes(streamid))
       return loadSess_[streamid].sessionId;
    else
        return 0;
}

void StreamMgr::setSesServicePtr(shared_ptr<SessionService> ptr)
{
    sesServicePtr_ = ptr;
}

void StreamMgr::setMSConn(handy::TcpConnPtr conn)
{
    msConn_ = conn;
}

void StreamMgr::updateStorePlan(StorePlan* msg)
{
    if(msg->empty())
    {
        curPlans_.clear();
        prePlans_.clear();
        //TODO
        //stop all task
        //this->stopAllTask();
    }
    else
    {
        prePlans_.clear();
        swap(prePlans_, curPlans_);
        for(int32_t i = 0; i < msg->streamplaninfo_size(); i++)
        {
           StreamPlanInfo planInfo = msg->streamplaninfo(i);
           PlanPtr planPtr = make_shared<Plan>(planInfo);
           curPlans_[planInfo.streamid()] = planPtr;
        }
        LOG(INFO) << "Current Stream Size: " << curPlans_.size();
    }
    diffStorePlan();
}

void StreamMgr::diffStorePlan()
{/*
    for(auto& plan : curPlans_)
    {
        auto it = prePlans_.find(plan.first);
        if(it != prePlans_.end())
        {
            LOG(INFO) << "****** StreamId: " << plan.first;
            for(uint32_t i = 1; i < 8; i++)
            {
                LOG(INFO) << "CurPlan: " << i << " " << bitset<24>(plan.second->timePhrase[i]);
                LOG(INFO) << "PrePlan: " << i << " " << bitset<24>(prePlans_[plan.first]->timePhrase[i]);

                uint32_t diffAdd =
                    (prePlans_[plan.first]->timePhrase[i] ^ plan.second->timePhrase[i]) &
                     plan.second->timePhrase[i];

                uint32_t diffDel =
                    (prePlans_[plan.first]->timePhrase[i] ^ plan.second->timePhrase[i]) &
                     ~plan.second->timePhrase[i];

                LOG(INFO) << "Day" << i;
                LOG(INFO) << "CurPlan: " << bitset<24>(plan.second->timePhrase[i]);
                LOG(INFO) << "AddPlan: " << bitset<24>(diffAdd);
                LOG(INFO) << "DelPlan: " << bitset<24>(diffDel);
            }
        }
        else{//curPlans exist, prePlans not exist
            for(uint32_t i = 1; i < 8; i++)
            {
                LOG(INFO) << "Day" << i;
                LOG(INFO) << "CurPlan: " << bitset<24>(plan.second->timePhrase[i]);
                LOG(INFO) << "AddPlan: " << bitset<24>(plan.second->timePhrase[i]);
            }
        }
    }*/
    for(auto& prePlan : prePlans_) //prePlans exist, curPlans not exist
    {
        auto it = curPlans_.find(prePlan.first);
        if(it == prePlans_.end())
        {
            LOG(INFO) << "****** StreamId: " << prePlan.first;
            delPlans_.insert(prePlan);
            for(uint32_t i = 1; i < 8; i++)
            {
                LOG(INFO) << "Day" << i;
                LOG(INFO) << "delPlan: " << bitset<24>(prePlan.second->timePhrase[i]);
            }
        }
    }
}

void StreamMgr::checkStorePlan()
{
    LOG(INFO) << "Check StorePlan";
    uint32_t curDay = getCurDay();
    uint32_t curHour = getCurHour();
    for(auto& plan : curPlans_)
    {

        if(plan.second->timePhrase[curDay]&(1<<curHour)) //if plan is set now
        {
            if(existStoreSes(plan.second->streamId))
            {
                LOG(WARNING) << "cur set 1, exist StorePlan";
            }
            else
            {
                LOG(WARNING) << "not existfind del";
                auto it = delPlans_.find(plan.second->streamId);
                if(it != delPlans_.end())
                {
                    LOG(INFO) << "in delPLan";
                    delPlans_.erase(it);
                }
                //createSes and invite
                createStream(plan.second->streamId);
            }
        }


        else //if plan is unset now
        {
            LOG(WARNING) << "curplan set 0";
            if(existStoreSes(plan.second->streamId))
            {
                LOG(WARNING) << "streamid not exist" << plan.second->streamId << "stop";
                //stopSes
                stopStream(plan.second->streamId);
            }
            else
            {
                LOG(WARNING) << "no exist";
            }
        }
    }
    for(auto& delPlan : delPlans_)
    {
        LOG(WARNING) << "In delPlan " << delPlan.second->streamId;
        //stopSes
        stopStream(delPlan.second->streamId);
        delPlans_.erase(delPlan.first);
    }
}


