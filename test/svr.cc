#include <glog/logging.h>

#include <handy/handy.h>

#include "../src/service/DSServer.h"
#include "../src/manager/StreamMgr.h"

int main()
{
    handy::EventBase base;
    handy::Ip4Addr loacl("127.0.0.1", 6666);
    handy::Ip4Addr remote("127.0.0.1", 2017);
    DSServer DS(&base, loacl, remote);
    DS.initServer();
    DS.registerToMS();
    base.loop();
}
