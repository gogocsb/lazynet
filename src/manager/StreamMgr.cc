#include <iostream>
#include <bitset>
#include <sys/time.h>

#include <glog/logging.h>

#include "StreamMgr.h"



StreamMgr::StreamMgr():
    prePlans_(),
    curPlans_(),
    storeSess_(),
    loadSess_()
{
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
        return storeSess_[streamid];
    else
        return 0;
}

uint64_t StreamMgr::getLoadSesId(string streamid)
{
    if(existLoadSes(streamid))
       return loadSess_[streamid];
    else
        return 0;
}

void StreamMgr::updateStorePlan(StorePlan* msg)
{
    if(msg->empty())
    {
        curPlans_.clear();
        prePlans_.clear();
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
{
    for(auto& plan : curPlans_)
    {
        auto it = prePlans_.find(plan.first);
        if(it != prePlans_.end())
        {
            LOG(INFO) << "****** StreamId: " << plan.first;
            for(uint32_t i = 1; i < 7; i++)
            {
                uint32_t diffAdd =
                    (prePlans_[plan.first]->timePhrase[i] ^ plan.second->timePhrase[i]) &
                     plan.second->timePhrase[i];

                uint32_t diffDel =
                    (prePlans_[plan.first]->timePhrase[i] ^ plan.second->timePhrase[i]) &
                     ~plan.second->timePhrase[i];

                prePlans_.erase(it);
                LOG(INFO) << "Day" << i;
                LOG(INFO) << "CurPlan: " << bitset<24>(plan.second->timePhrase[i]);
                LOG(INFO) << "AddPlan: " << bitset<24>(diffAdd);
                LOG(INFO) << "DelPlan: " << bitset<24>(diffDel);
            }
        }
        else{//curPlans exist, prePlans not exist
            for(uint32_t i = 1; i < 7; i++)
            {
                LOG(INFO) << "Day" << i;
                LOG(INFO) << "CurPlan: " << bitset<24>(plan.second->timePhrase[i]);
                LOG(INFO) << "AddPlan: " << bitset<24>(plan.second->timePhrase[i]);
            }
        }
    }
    for(auto& delPlan : prePlans_) //prePlans exist, curPlans not exist
    {
        LOG(INFO) << "****** StreamId: " << delPlan.first;
        delPlans_.insert(delPlan);
        for(uint32_t i = 1; i < 7; i++)
        {
            LOG(INFO) << "Day" << i;
            LOG(INFO) << "delPlan: " << bitset<24>(delPlan.second->timePhrase[i]);
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
                continue;
            }
            else
            {
                auto it = delPlans_.find(plan.second->streamId);
                if(it != delPlans_.end())
                {
                    delPlans_.erase(it);
                }
                //createSes and startSes;
            }
        }
        else //if plan is unset now
        {
            if(existStoreSes(plan.second->streamId))
            {
                //stopSes
            }
            else
            {
                continue;
            }
        }
    }
    for(auto& delPlan : delPlans_)
    {
        //stopSes
    }
}

