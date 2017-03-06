#ifndef STORETASK_H_
#define STORETASK_H_

#include <atomic>
#include <string>
#include <vector>
#include "Plan.h"

using namespace std;

enum StoreState
{
    kPrepare = 1,
    kStart = 2,
    kFinish = 3,
    kNext = 4,
    kError = 5,
};


struct StoreTaskImp
{

};

class StoreTask
{
    StoreTask(Plan plan)
    {
        static std::atomic<int64_t> id(0);
        taskId_ = id++;
        streamId_ = plan.streamId;
        cameraIp_ = plan.cameraIp;
        cameraPort_ = plan.cameraPort;
        extraInfo_ = plan.extraInfo;
        expiryDays_ = plan.expiryDays;
        codeRate_ = plan.codeRate;
        frameRate_ = plan.frameRate;
    }
    uint64_t taskId();
    void set_sfd(uint64_t);
private:
    uint64_t taskId_;
    string streamId_;
    string cameraIp_;
    string cameraPort_;
    string extraInfo_;
    uint32_t expiryDays_;
    uint32_t codeRate_;
    float frameRate_;

    uint64_t sfd;
    uint32_t streamSrc;
    vector<uint32_t> streamDestVec;
};
typedef shared_ptr<StoreTask> StoreTaskPtr;


#endif
