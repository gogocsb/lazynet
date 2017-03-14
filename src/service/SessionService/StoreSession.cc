#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <handy/handy.h>

#include <glog/logging.h>

#include "StoreSession.h"

#define gettidv1() syscall(__NR_gettid)

StoreSession::StoreSession(handy::EventBase* adminloop,
        handy::EventBase* storeloop,
        handy::Ip4Addr addr)
    : fd_(0), buf_(), addr_(addr), adminLoop_(adminloop), storeLoop_(storeloop)
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
    LOG(WARNING) << "udp fd " << fd_ << " bind to " << addr_.toString().c_str();
    return 0;
}

int StoreSession::prePareStore()
{
    storeLoop_->safeCall([this]{ return bindAddr();});
}

//TODO ReadCallBack
//when recv, send packet to (auto i : outStreamVec)
//when buf reach 10M, flush buffer, exchange buf and buf->next
void StoreSession::attatchToLoop()
{
    channel_ = new handy::Channel(storeLoop_, fd_, handy::kReadEvent);
    channel_->onRead([this]{
            LOG(INFO) << "onRead";

            struct sockaddr_in raddr;
            socklen_t rsz = sizeof(raddr);
            if (!channel_ || channel_->fd() < 0)
            {
                return;
            }
            int fd = channel_->fd();
            ssize_t rn = recvfrom(fd, buf_.begin(), handy::kUdpPacketSize, 0, (sockaddr*)&raddr, &rsz);
            if (rn < 0)
            {
                LOG(INFO) << "udp " << fd << "  recv failed: " << fd << " " << strerror(errno);
                return;
            }
            //TODO outputStream
            //for(auto out : outStreamVec_)
            //{
            //    sendTo(buf_.begin(), rn, out);
            //}
            buf_.consume(rn);
            LOG(INFO) << "buf consume " << rn;
            if(buf_.isFull())
            {
                LOG(INFO) << "buf full write t SFS";
                LOG(INFO) << "buf_addr " << buf_.data();
                //TODO write to SFS
                buf_.resetBuf();
            }
            });
    LOG(WARNING) << "attatch successful";
    return;
}

void StoreSession::sendTo(const char* buf, size_t len, handy::Ip4Addr addr)
{
    if (!channel_ || channel_->fd() < 0)
    {
        LOG(WARNING) << "channel closed";
        return;
    }
    int fd = channel_->fd();
    int wn = ::sendto(fd, buf, len, 0, (sockaddr*)&addr.getAddr(), sizeof(sockaddr));
    if (wn < 0) {
        LOG(ERROR) << "udp  sendto "<< addr.toString().c_str() << " error";
    }
}

//TODO after close send buf
void StoreSession::closeUDP()
{
    if(!channel_)
        return;
    channel_->enableReadWrite(false, false);
    auto p = channel_;
    channel_= NULL;
    delete p;
}

void StoreSession::startStore()
{
    storeLoop_->safeCall([this]{ attatchToLoop();});
}

void StoreSession::stopStore()
{
    storeLoop_->safeCall( [this]{ closeUDP();});
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
