
#include <Process.hpp>

#define BUF_SIZE	4096

using namespace base;

Process::Process(QObject * pParent):
    QObject(pParent)
{
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    saAttr.bInheritHandle = TRUE; 
    saAttr.lpSecurityDescriptor = NULL; 

    CreatePipe(_hOUT, (_hOUT+1), &saAttr, 0);
    CreatePipe(_hIN, (_hIN+1), &saAttr, 0);
    CreatePipe(_hERR, (_hERR+1), &saAttr, 0);

    SetHandleInformation(_hOUT[0], HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(_hIN[1], HANDLE_FLAG_INHERIT, 0);

    _bProcessRunning = true;
    _pStdOutThread = NULL;
    _pStdErrThread = NULL;
}

Process::~Process()
{
    CloseHandle(_processInformation.hProcess);
    CloseHandle(_processInformation.hThread);

    if(_pStdOutThread != NULL)
    {
        delete _pStdOutThread;
        delete _pStdErrThread;
    }
}

bool Process::setProcess(char * szProcess)
{
    STARTUPINFO siStartInfo;

    ZeroMemory( &_processInformation, sizeof(PROCESS_INFORMATION) );

    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.hStdError = _hERR[1];
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

    _pStdOutThread = new Thread(_hOUT);
    _pStdErrThread = new Thread(_hERR);
    connect(_pStdOutThread, SIGNAL(print(QString)), this, SIGNAL(printInStdOut(QString)));
    connect(_pStdErrThread, SIGNAL(print(QString)), this, SIGNAL(printInStdErr(QString)));
}

void Process::stdIn(const QString &sMessage)
{
    DWORD dwWritten;

    WriteFile(_hIN[1], sMessage.toStdString().c_str(), sMessage.length(), &dwWritten, NULL);
}
