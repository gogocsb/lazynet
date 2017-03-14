#include <thread>
#include <glog/logging.h>

#include <handy/handy.h>

#include "../src/service/DSServer.h"
#include "../src/manager/StreamMgr.h"

int main(int argc, char** argv)
{
    google::InitGoogleLogging((const char *)argv[0]);
    google::SetStderrLogging(google::INFO);
    FLAGS_colorlogtostderr=true;
    Logger::getLogger().setLogLevel(Logger::LWARN);
    handy::EventBase adminLoop;
    handy::EventBase storeLoop;
    handy::Ip4Addr loacl("127.0.0.1", 2017);
    handy::Ip4Addr remote("127.0.0.1", 6666);
    DSServer DS(loacl, remote);
    DS.initAdmin(&adminLoop);
    DS.initStore(&storeLoop);
    DS.registerToMS();
    LOG(INFO) << "t prepare";
    thread t([&storeLoop]{ storeLoop.loop(); LOG(INFO)<<"aaa"; });
    //t.join();
    LOG(INFO) << "t start";
    adminLoop.loop();
}
