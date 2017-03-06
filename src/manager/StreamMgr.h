#ifndef STREAMMGR_H_
#define STREAMMGR_H_

#include <map>
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <atomic>

#include "Plan.h"
#include "../SessionService/SessionService.h"
#include "../protocol/MS_DS.pb.h"

using namespace std;

//StreamManager only run in admin_thread

class StreamMgr
{
public:
    StreamMgr();
    void updateStorePlan(StorePlanPtr msg);
    void diffStorePlan();
    void checkStorePlan();
    uint32_t getCurDay();
    uint32_t getCurHour();

    void stopAllSes();
    void stopSes(string streamid);
//TODO
//add get stream <-> session interface
    bool existStoreSes(string streamid);
    bool existLoadSes(string streamid);
    uint64_t getStoreSesId(string streamid);
    uint64_t getLoadSesId(string streamid);
private:
    typedef map<string, PlanPtr> StorePlanMap; //streamid<->plan
    typedef unordered_map<string, PlanPtr> StorePlanHashMap;
    typedef map<string, uint64_t> Plan2SessionMap;   //streamid<->sessionid

    StorePlanMap prePlans_;
    StorePlanMap curPlans_;
    StorePlanHashMap delPlans_;
    //StoreSessions
    Plan2SessionMap storeSess_;
    //LoadSessions
    Plan2SessionMap loadSess_;
};

#endif
