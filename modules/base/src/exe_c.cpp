
#include <windows.h>
#include <iostream>
#include <Process.hpp>

int main()
{
    base::Process p;
    p.setProcess("cmd");
    std::cout << p.stdOut() << std::endl;

#if 0
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    saAttr.bInheritHandle = TRUE; 
    saAttr.lpSecurityDescriptor = NULL; 

    HANDLE hChild_IN[2];
    HANDLE hChild_OUT[2];
    HANDLE hChild_ERR[2];

    // 0=>read - 1=>write
    CreatePipe(hChild_OUT, (hChild_OUT+1), &saAttr, 0);
    SetHandleInformation(hChild_OUT[0], HANDLE_FLAG_INHERIT, 0);

    STARTUPINFO siStartInfo;
    PROCESS_INFORMATION piProcInfo; 

    ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
 
    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.
 
    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.hStdError = hChild_OUT[1];
    siStartInfo.hStdOutput = hChild_OUT[1];
    siStartInfo.hStdInput = hChild_IN[0];
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    bool bSuccess = CreateProcess(NULL, 
                                  TEXT("cmd"),  // command line 
                                  NULL, // process security attributes 
                                  NULL, // primary thread security attributes 
                                  TRUE, // handles are inherited 
                                  0, // creation flags 
                                  NULL, // use parent's environment 
                                  NULL, // use parent's current directory 
                                  &siStartInfo,  // STARTUPINFO pointer 
                                  &piProcInfo
                                 );  // receives PROCESS_INFORMATION 

CHAR buf[4096];
memset(buf, '\0', 4096);
DWORD dwRead, dwWritten; 
bSuccess = ReadFile( hChild_OUT[0], buf, 4096, &dwRead, NULL);

std::cout << "buf:" << buf << std::endl;

    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);

    std::cout << "sale" << std::endl;
#endif
}
