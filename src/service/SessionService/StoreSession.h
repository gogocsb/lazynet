#ifndef STORESESSION_H_
#define STORESESSION_H_

#include <memory>
#include <vector>
#include <string>

#include <glog/logging.h>

#include <handy/handy.h>

using namespace std;

//TODO
//session state
//session input
//session output
//storebuf
//workloop
//reportLoop


//in initfunc
//Session hook onRead CallBack

//attatch loop
//start
//stop



class StoreSession
{
public:
    StoreSession(handy::EventBase*, handy::Ip4Addr);
    void startStore();
    void stopStore();
    void addOutStream(handy::Ip4Addr);
    void removeOutStream(handy::Ip4Addr);
    void readCallBack();
private:
    handy::Ip4Addr addr_;
    handy::UdpConnPtr inStream_;
    vector<handy::Ip4Addr> outStreamVec_;
    handy::EventBase* adminLoop_;
    handy::EventBase* storeLoop_;
};
typedef shared_ptr<StoreSession> StoreSesPtr;

#endif
