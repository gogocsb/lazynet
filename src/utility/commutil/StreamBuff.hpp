#ifndef STREAMBUFF_H_
#define STREAMBUFF_H_

#include <memory>
#include <string.h>

#define BUFSIZE 10*1024*1024
#define LOWATER 1470

struct StreamBuf
{
    StreamBuf() : b_(2), e_(BUFSIZE)
    {
        buf_ = new char[BUFSIZE];
        next_ = new char[BUFSIZE];
    }
    ~StreamBuf()
    {
        delete[] buf_;
        delete[] next_;
    }
    size_t cap() const{return e_ - b_;}
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
    bool isFull()
    {
        if (cap() < LOWATER)
            return true;
        else
            return false;
    }

    void resetBuf()
    {
        char* tmp = buf_;
        buf_ = next_;
        next_ = tmp;
        b_ = 0;
    }
private:
    char* buf_;
    char* next_;
    size_t b_, e_, cap_;
};

#endif
