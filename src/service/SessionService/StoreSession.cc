#include <fcntl.h>
#include <unistd.h>
#include <handy/handy.h>

#include <glog/logging.h>

#include "StoreSession.h"


StoreSession::StoreSession(handy::EventBase* adminloop,
        handy::EventBase* storeloop,
        handy::Ip4Addr addr)
    : fd_(0), addr_(addr), adminLoop_(adminloop), storeLoop_(storeloop)
{
}

int StoreSession::bindAddr()
{
    fd_ = socket(AF_INET, SOCK_DGRAM, 0);
    int ret = handy::net::setReuseAddr(fd_);
    //LOG ERROR when ret != 0
    LOG_IF(ERROR, ret) << "set socket reuse option failed";
    ret = handy::util::addFdFlag(fd_, FD_CLOEXEC);
    LOG_IF(ERROR, ret) << "addFdFlag FD_CLOEXEC failed";
    ret = ::bind(fd_,(struct sockaddr *)&addr_.getAddr(),sizeof(struct sockaddr));
    if (ret) {
        ::close(fd_);
        LOG(ERROR) << "bind to " << addr_.toString().c_str()
            << " failed " << errno << " " <<strerror(errno);
        return errno;
    }
    handy::net::setNonBlock(fd_);
    LOG(INFO) << "udp fd " << fd_ << " bind to " << addr_.toString().c_str();
    return 0;
}

//TODO ReadCallBack
void StoreSession::attatchToLoop()
{
    channel_ = new handy::Channel(storeLoop_, fd_, handy::kReadEvent);
    channel_->onRead([this]{
            LOG(INFO) << "onRead";
            /*
            Buffer buf;
            struct sockaddr_in raddr;
            socklen_t rsz = sizeof(raddr);
            if (!channel_ || channel_->fd() < 0)
            {
                return;
            }
            int fd = channel_->fd();
            ssize_t rn = recvfrom(fd, buf.makeRoom(kUdpPacketSize), kUdpPacketSize, 0, (sockaddr*)&raddr, &rsz);
            if (rn < 0)
            {
                error("udp %d recv failed: %d %s", fd, errno, strerror(errno));
                return;
            }
            buf.addSize(rn);
            trace("udp %d recv %ld bytes from %s", fd, rn, Ip4Addr(raddr).toString().data());
            this->msgcb_(shared_from_this(), buf, raddr);*/
            });
    return;
}

//TODO after close send buf
void StoreSession::closeUDP()
{
    if(!channel_)
        return;
    auto p = channel_;
    channel_=NULL;
    storeLoop_->safeCall([p](){ delete p; });
}

void StoreSession::startStore()
{
    this->attatchToLoop();
}

void StoreSession::stopStore()
{
    this->closeUDP();
}

void StoreSession::addOutStream(handy::Ip4Addr outStream)
{
    storeLoop_->safeCall(
            [=]{
                outStreamVec_.push_back(outStream);
            });
}

void StoreSession::removeOutStream(handy::Ip4Addr outStream)
{
    storeLoop_->safeCall(
            [=]{
                auto it = find(outStreamVec_.begin(), outStreamVec_.end(), outStream);
                if(it == outStreamVec_.end())
                    return;
                else
                {
                    outStreamVec_.erase(it);
                    return;
                }
            });
}

//TODO
//when recv, send packet to (auto i : outStreamVec)
//when buf reach 10M, flush buffer, exchange buf and buf->next

void StoreSession::readCallBack()
{

}
