#include <thread>
#include <sys/syscall.h>
#include <unistd.h>

#include <glog/logging.h>

#include "DSServer.h"
#include "../protocol/MS_DS.pb.h"
#include "../codec/SSCmdCode.h"

#define gettidv1() syscall(__NR_gettid)
#define gettidv2() syscall(SYS_gettid)

DSServer::DSServer(handy::Ip4Addr local, handy::Ip4Addr remote):
    localAddr_(local), remoteAddr_(remote),
    storeThread_(1)
{
}

//TODO
//Init codec and dispatcher
//cmd : register  storeplan  loadsession addlive  removelive
//Init Timer for Plan Check
//
void DSServer::initAdmin(handy::EventBase* loop)
{
    adminLoop_ = loop;
    streamMgr_ =  make_shared<StreamMgr>();
    //MS_DS_Register
    ExternalMsgCodec::onCodec(MSG_MS_DS_DSREGISTER, "MS_DS.DSRegister");
    //MS_DS_RegisterACK
    ExternalMsgCodec::onCodec(MSG_MS_DS_DSREGISTERACK, "MS_DS.DSRegisterACK");
    dispatcher_.onMsg<MS_DS::DSRegisterACK>(
            [](handy::TcpConnPtr, BaseHead, MS_DS::DSRegisterACK* msg)
            {LOG(INFO) << "recv " << msg->GetTypeName();});
    //MS_DS_KeepAlive

    //MS_DS_Report

    //MS_DS_StorePlan
    ExternalMsgCodec::onCodec(MSG_MS_DS_STOREPLAN, "MS_DS.StorePlan");
    dispatcher_.onMsg<MS_DS::StorePlan>(
            [this](handy::TcpConnPtr, BaseHead, MS_DS::StorePlan* msg)
            {
                LOG(INFO) << "recv " << msg->GetTypeName();
                streamMgr_->updateStorePlan(msg);
            });

    //DS_Check_StorePlan
    adminLoop_->runAfter(6000, [this]{streamMgr_->checkStorePlan();}, 6000);
    //MS_DS_Invite
    ExternalMsgCodec::onCodec(MSG_DS_MS_INVITE_CAMERA, "MS_DS.ReadyStoreStreamInfo");
    ExternalMsgCodec::onCodec(MSG_DS_MS_INVITE_CAMERA_ACK, "MS_DS.ReadyStoreStreamInfoACK");
    dispatcher_.onMsg<MS_DS::ReadyStoreStreamInfoACK>(
            [this](handy::TcpConnPtr, BaseHead info, MS_DS::ReadyStoreStreamInfoACK* msg)
            {
                LOG(INFO) << "recv " << msg->GetTypeName();
                streamMgr_->startStream(msg->streamid(), info.para1, info.para2, info.err);
            });
    //MS_DS_BYE
    ExternalMsgCodec::onCodec(MSG_DS_MS_STORE_FINISH, "MS_DS.Bye");
}


void DSServer::registerToMS()
{
    msAgent_ = handy::TcpConn::createConnection(adminLoop_, remoteAddr_.ip(), remoteAddr_.port());
    msAgent_->setReconnectInterval(5*1000);
    msAgent_->onState(
            [this](const TcpConnPtr& con)
            {
                if (con->getState() == TcpConn::Connected)
                {
                    MS_DS::DSRegister msg;
                    msg.set_ip(localAddr_.ip());
                    msg.set_port(localAddr_.port());
                    msg.set_pid((long int)gettidv1());
                    ExternalMsgCodec::encode(&msg, 0, 0, 0, con->getOutput());
                    con->sendOutput();
                    LOG(INFO) << "send Register";
                    streamMgr_->setMSConn(con);
                    ExternalMsgCodec::encode(&msg, 0, 0, 0, con->getOutput());
                    con->sendOutput();
                }
            });
    msAgent_->onRead(
            [this](const TcpConnPtr& con)
            {
                BaseHead info;
                if (ExternalMsgCodec::msgComplete(con->getInput()))
                {
                    Message* msg = ExternalMsgCodec::decode(con->getInput(), info);
                    if(msg || info.length == 0)
                    {
                        dispatcher_.handle(con, info, msg);
                    }
                    else
                    {
                        LOG(ERROR) << "bad Msg from peer";
                    }
                }
            });
}

//TODO add KeepAlive
void DSServer::keepAlive()
{

}

void DSServer::initStore(handy::EventBase* loop)
{
    storeLoop_ = loop;
    sesService_ = make_shared<SessionService>(adminLoop_, storeLoop_);
    streamMgr_->setSesServicePtr(sesService_);
}

void DSServer::initLoad()
{
    return;
}

