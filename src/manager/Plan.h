#ifndef PLAN_H_
#define PLAN_H_

#include <string>
#include <memory>
#include <vector>

#include "../protocol/MS_DS.pb.h"

using namespace std;

typedef MS_DS::StreamPlanInfo StreamPlanInfo;
typedef shared_ptr<MS_DS::StreamPlanInfo> StreamPlanInfoPtr;
typedef shared_ptr<MS_DS::StorePlan> StorePlanPtr;
typedef shared_ptr<MS_DS::StoreTimePhrase> StoreTimePhrasePtr;

struct Plan
{
    string streamId;
    string cameraIp;
    string cameraPort;
    string streamType;
    string extraInfo;
    uint32_t expiryDays;
    uint32_t codeRate;
    float frameRate;
    vector<uint32_t> timePhrase;

    Plan(StreamPlanInfo info) :
        streamId(info.streamid()),
        cameraIp(info.cameraip()),
        cameraPort(info.cameraport()),
        streamType(info.streamtype()),
        extraInfo(info.moreinformation()),
        expiryDays(info.expireddays()),
        codeRate(info.coderate()),
        frameRate(info.framerate()),
        timePhrase(8 ,0)
    {
        for(int i = 0; i < info.storetimephrase_size(); i++)
        {
            int index = info.storetimephrase(i).weekday();
            timePhrase[index] = info.storetimephrase(i).storetime();
        }
    }
};

typedef struct Plan Plan;
typedef shared_ptr<Plan> PlanPtr;

#endif
