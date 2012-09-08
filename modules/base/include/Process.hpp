
#ifndef BASE_PROCESS_HPP
#define BASE_PROCESS_HPP

#include <BaseGlobal.hpp>
#include <windows.h>
#include <string>
#include <Thread.hpp>

namespace base
{

class BASE_EXPORT Process
{
public:
    Process();
    ~Process();

    bool setProcess(char * szProcess);

    std::string stdOut();
    void stdIn(const std::string &sMessage);

    bool isRunning();

    static DWORD WINAPI checkStdOut(void * pRunning);

protected:
    PROCESS_INFORMATION _processInformation; 

    HANDLE _hIN[2];
    HANDLE _hOUT[2];
    HANDLE _hERR[2];

    Thread * _pStdOutThread;
    bool _bProcessRunning;
};

}

#endif
