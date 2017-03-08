#include <glog/logging.h>

#include "DSServer.h"
#include "../protocol/MS_DS.pb.h"
#include "../codec/SSCmdCode.h"

DSServer::DSServer(handy::EventBase* loop, handy::Ip4Addr local, handy::Ip4Addr remote):
    adminLoop_(loop), localAddr_(local), remoteAddr_(remote)
{

}


//TODO
//Init codec and dispatcher
//cmd : register  storeplan  loadsession addlive  removelive
//Init Timer for Plan Check
//
void DSServer::initServer()
{
    codec_.onCodec(MSG_MS_DS_DSREGISTERACK, "MS_DS.DSRegisterACK");
    dispatcher_.onMsg<MS_DS::DSRegisterACK>(
            [](handy::TcpConnPtr, BaseHead, MS_DS::DSRegisterACK* msg)
            {LOG(INFO) << "recv " << msg->GetTypeName();});

    codec_.onCodec(MSG_MS_DS_STOREPLAN, "MS_DS.StorePlan");
    dispatcher_.onMsg<MS_DS::StorePlan>(
            [](handy::TcpConnPtr, BaseHead, MS_DS::StorePlan* msg)
            {
                LOG(INFO) << "recv " <<msg->GetTypeName();
            });
}

void DSServer::registerToMS()
{
    msAgent_ = handy::TcpConn::createConnection(adminLoop_, remoteAddr_.ip(), remoteAddr_.port());
    msAgent_->setReconnectInterval(10*1000);
    msAgent_->onState(
            [this](const TcpConnPtr& con)
            {
                if (con->getState() == TcpConn::Connected)
                {
                    MS_DS::DSRegister msg;
                    msg.set_ip(localAddr_.ip());
                    msg.set_port(localAddr_.port());
                    ExternalMsgCodec::encode(&msg, 0, 0, 0, con->getOutput());
                    con->sendOutput();
                    LOG(INFO) << "send Register";
                }
            });
    streamMgr_.checkStorePlan();
}
