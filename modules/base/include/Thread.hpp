
#ifndef BASE_THREAD_HPP
#define BASE_THREAD_HPP

#include <windows.h>

namespace base
{

enum Status { Running, Stopped };

class Thread
{
public:
    Thread(HANDLE * hOUT);
    ~Thread();

    bool start();
    bool stop();

    Status status() const;

protected:
    HANDLE _threadHandler;
    Status _eStatus;
    HANDLE _hOUT[2];

    static DWORD WINAPI checkStdOut(void * pThread);

};

inline Status Thread::status() const
{
    return _eStatus;
}

}

#endif
