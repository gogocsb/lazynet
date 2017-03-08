#ifndef STREAMBUFF_H_
#define STREAMBUFF_H_

#include <memory>
#include <string.h>

#define BUFSIZE 10*1024*1024

struct StreamBuf
{
    StreamBuf() : b_(2), e_(0), cap_(BUFSIZE)
    {
        buf_ = new char[BUFSIZE];
        next_ = new char[BUFSIZE];
    }
    ~StreamBuf()
    {
        delete[] buf_;
        delete[] next_;
    }
    size_t size() const{return e_ - b_;}
    char* data() const{return buf_;}
    char* begin() const{return buf_ + b_;}
    char* next() const{return next_;}
    StreamBuf& appendValue(const char* p, uint16_t value)
    {
        memcpy((void*)p, &value, sizeof(value));
        return *this;
    }
    StreamBuf& consume(size_t len)
    {
        appendValue(buf_ + b_ - 2, (uint16_t)len);
        b_ = b_ + len + 2;
        return *this;
    }
private:
    char* buf_;
    char* next_;
    size_t b_, e_, cap_;
};


#endif
