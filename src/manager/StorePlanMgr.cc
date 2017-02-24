#include <iostream>
#include <bitset>
#include <sys/time.h>

#include <glog/logging.h>

#include "StorePlanMgr.h"

StorePlanMgr::StorePlanMgr():
    prePlans_(),
    curPlans_(),
    tasks_()
{
}

//day0 means sunday
uint32_t StorePlanMgr::getCurDay()
{
    time_t t;
    time(&t);
    uint32_t wd = localtime(&t)->tm_wday;
    return (wd == 0 ? 7 : wd);
}

uint32_t StorePlanMgr::getCurHour()
{
    time_t t;
    time(&t);
    return localtime(&t)->tm_hour;
}

bool StorePlanMgr::existTask(string streamId)
{
    return (tasks_.find(streamid) == tasks_.end()) ? false : true;
}

void StorePlanMgr::updateStorePlan(StorePlanPtr msg)
{
    if(msg->empty())
    {
        curPlans_.clear();
        prePlans_.clear();
        //stop all task
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
        LOG(INFO) << "Current StorePlan Size: " << curPlans_.size();
    }
}

void StorePlanMgr::diffStorePlan()
{
    for(auto& plan : curPlans_)
    {
        if(prePlans_.find(plan.first) != prePlans_.end())
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

                LOG(INFO) << "Day" << i;
                LOG(INFO) << "CurPlan: " << bitset<24>(plan.second->timePhrase[i]);
                LOG(INFO) << "AddPlan: " << bitset<24>(diffAdd);
                LOG(INFO) << "DelPlan: " << bitset<24>(diffDel);
            }
        }
        else{
            for(uint32_t i = 1; i < 7; i++)
            {
                LOG(INFO) << "Day" << i;
                LOG(INFO) << "CurPlan: " << bitset<24>(plan.second->timePhrase[i]);
                LOG(INFO) << "AddPlan: " << bitset<24>(plan.second->timePhrase[i]);
            }
        }
    }
}

void StorePlanMgr::checkStorePlan()
{
    uint32_t curDay = getCurDay();
    uint32_t curHour = getCurHour();
    for(auto& plan : curPlans_)
    {
        if(plan.second->timePhrase[curDay]&(1<<curHour))
        {
            if(existTask(plan.second->streamId))
                continue;
            else{
                //startTask
            }
        }
        else{
            if(existTask(plan.second->streamId))
                //stopTask;
        }
    }
}


