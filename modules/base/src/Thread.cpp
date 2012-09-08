
#include <Thread.hpp>

#include <cstdio>

using namespace base;

Thread::Thread(DWORD WINAPI (*pthreadFunction)(void*), void * pfunctionParameter)
{
    _pthreadFunction = pthreadFunction;
    _eStatus = Stopped;
}

Thread::~Thread()
{
    if(_eStatus == Running)
        CloseHandle(_threadHandler);
}

bool Thread::start()
{
    if(_eStatus == Stopped)
    {
        _threadHandler = CreateThread(0, 0, _pthreadFunction, (void *)_pfunctionParameter, 0, 0);
        if(_threadHandler == NULL)
        {
            perror("Can't start the thread.");
            return false;
        }
        else
            _eStatus = Running;
    }
    else
        perror("Can't start the thread because it is already running.");
}

bool Thread::stop()
{
    if(_eStatus == Running)
    {
        CloseHandle(_threadHandler);
        _eStatus = Stopped;
    }
    else
        perror("Can't stop the thread because it is not running.");
}
