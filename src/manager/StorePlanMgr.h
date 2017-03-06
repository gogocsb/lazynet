#ifndef STOREPLANMGR_H_
#define STOREPLANMGR_H_

#include <map>
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <atomic>

#include "Plan.h"
#include "StoreTask.h"
#include "../protocol/MS_DS.pb.h"

using namespace std;


class StorePlanMgr
{
public:
    StorePlanMgr();
    void updateStorePlan(StorePlanPtr msg);
    void diffStorePlan();
    void checkStorePlan();
    uint32_t getCurDay();
    uint32_t getCurHour();

    void stopAllTask();
    void stopTask(string);

    bool existTask(string streamid);
private:
    typedef map<string, PlanPtr> StorePlanMap; //streamid<->plan
    typedef map<string, StoreTaskPtr> Plan2TaskMap;   //streamid<->taskid
    typedef unordered_map<string, PlanPtr> StorePlanHashMap;
    StorePlanMap prePlans_;
    StorePlanMap curPlans_;
    StorePlanHashMap delPlans_;
    Plan2TaskMap tasks_;
};

#endif
