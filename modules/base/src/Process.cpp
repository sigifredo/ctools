
#include <Process.hpp>

#define BUF_SIZE	4096

using namespace base;

Process::Process()
{
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    saAttr.bInheritHandle = TRUE; 
    saAttr.lpSecurityDescriptor = NULL; 

    // stdout
    CreatePipe(_hOUT, (_hOUT+1), &saAttr, 0);
    SetHandleInformation(_hOUT[0], HANDLE_FLAG_INHERIT, 0);
}

Process::~Process()
{
    CloseHandle(_processInformation.hProcess);
    CloseHandle(_processInformation.hThread);
}

bool Process::setProcess(char * szProcess)
{
    STARTUPINFO siStartInfo;

    ZeroMemory( &_processInformation, sizeof(PROCESS_INFORMATION) );

    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.hStdError = _hOUT[1];
    siStartInfo.hStdOutput = _hOUT[1];
    siStartInfo.hStdInput = _hIN[0];
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    CreateProcess(NULL, 
                  szProcess,  // command line 
                  NULL, // process security attributes 
                  NULL, // primary thread security attributes 
                  TRUE, // handles are inherited 
                  0, // creation flags 
                  NULL, // use parent's environment 
                  NULL, // use parent's current directory 
                  &siStartInfo,  // STARTUPINFO pointer 
                  &_processInformation
                 );  // receives PROCESS_INFORMATION 
}

std::string Process::stdOut()
{
    CHAR buf[BUF_SIZE];
    memset(buf, '\0', BUF_SIZE);
    DWORD dwRead; 
    ReadFile( _hOUT[0], buf, BUF_SIZE, &dwRead, NULL);

    return std::string(buf);
}