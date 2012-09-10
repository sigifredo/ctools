
#include <Thread.hpp>

#include <cstdio>
#include <utility>

#define BUF_SIZE	4096

using namespace base;

Thread::Thread(HANDLE * hOUT, QObject * pParent):
    QObject(pParent)
{
    _hOUT[0] = hOUT[0];
    _hOUT[1] = hOUT[1];

    _threadHandler = NULL;
    _eStatus = Stopped;

    start();
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
        _threadHandler = CreateThread(0, 0, checkStdOut, (void *)this, 0, 0);
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
        _threadHandler = NULL;
    }
    else
        perror("Can't stop the thread because it is not running.");
}

DWORD WINAPI Thread::checkStdOut(void * pthread)
{
    Thread * pThread = (Thread *)pthread;
    CHAR szBuffer[BUF_SIZE];
    memset(szBuffer, '\0', BUF_SIZE);
    DWORD dwRead; 

    while(pThread->_eStatus == Running)
    {
        ReadFile( pThread->_hOUT[0], szBuffer, BUF_SIZE, &dwRead, NULL);
        // for(int i = 0; i < dwRead; i++)
        //     printf("%d - %c\n", szBuffer[i], szBuffer[i]);
        pThread->emit print(QString(szBuffer));
        memset(szBuffer, '\0', dwRead);
    }

    return 0;
}
