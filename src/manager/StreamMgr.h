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
#include "../service/SessionService/SessionService.h"
#include "../protocol/MS_DS.pb.h"

using namespace std;

//StreamManager only run in admin_thread

struct StoreSesInfo
{
    uint64_t sessionId;
    uint32_t para1;
    uint32_t para2;
    StoreSesInfo(uint64_t id = 0, uint32_t p1 = 0, uint32_t p2 = 0):sessionId(id), para1(p1), para2(p2){}
};

typedef struct StoreSesInfo StoreSesInfo;

class StreamMgr
{
public:
    StreamMgr();
    void updateStorePlan(StorePlan* msg);
    void diffStorePlan();
    void checkStorePlan();
    uint32_t getCurDay();
    uint32_t getCurHour();

    void createStream(string streamid);
    void stopAllStream();
    void stopStream(string streamid);

    void inviteStream(string streamid, uint16_t port);
    //after recv inviteACK, start Stream
    void startStream(string streamid, uint32_t p1, uint32_t p2, uint32_t err);
    void byeStream(string streamid);
//TODO
//add get stream <-> session interface
    bool existStoreSes(string streamid);
    bool existLoadSes(string streamid);
    uint64_t getStoreSesId(string streamid);
    uint64_t getLoadSesId(string streamid);
    void setSesServicePtr(shared_ptr<SessionService>);
    void setMSConn(handy::TcpConnPtr);
private:
    typedef map<string, PlanPtr> StorePlanMap; //streamid<->plan
    typedef unordered_map<string, PlanPtr> StorePlanHashMap;
    typedef map<string, StoreSesInfo> Plan2SessionMap;   //streamid<->sessionid

    StorePlanMap prePlans_;
    StorePlanMap curPlans_;
    StorePlanHashMap delPlans_;
    //StoreSessions
    Plan2SessionMap storeSess_;
    //LoadSessions
    Plan2SessionMap loadSess_;
    shared_ptr<SessionService> sesServicePtr_;
    handy::TcpConnPtr msConn_;
    uint16_t port_;
    uint16_t portInterval_;
};

#endif
