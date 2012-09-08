
#ifndef BASE_THREAD_HPP
#define BASE_THREAD_HPP

#include <windows.h>

namespace base
{

enum Status { Running, Stopped };

class Thread
{
public:
    Thread(DWORD WINAPI (*pthreadFunction)(void*), void * pfunctionParameter);
    ~Thread();

    bool start();
    bool stop();

    Status status() const;

protected:
    DWORD WINAPI (*_pthreadFunction)(void*);
    void * _pfunctionParameter;
    HANDLE _threadHandler;
    Status _eStatus;
};

inline Status Thread::status() const
{
    return _eStatus;
}

}

#endif
