#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include <stdexcept>

template <typename T>
class Singleton
{
public:
    template<typename... Args>
    static T* Instance(Args&&... args)
    {
        if(pInstance_ == nullptr)
        {
            pInstance_ = new T(std::forward<Args>(args)...);
        }
        return pInstance_;
    }

    static T* getInstance()
    {
        if(pInstance_ == nullptr)
            throw std::logic_error("the Instance is not init.");
        return pInstance_;
    }
private:
    Singleton(void);
    virtual ~Singleton(void);
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
private:
    static T* pInstance_;
};

template <class T> T* Singleton<T>::pInstance_ = nullptr;

#endif
