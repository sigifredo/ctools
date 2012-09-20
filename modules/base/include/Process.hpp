
#ifndef BASE_PROCESS_HPP
#define BASE_PROCESS_HPP

#include <BaseGlobal.hpp>
#include <windows.h>
#include <string>
#include <Thread.hpp>

namespace base
{

class BASE_EXPORT Process: public QObject
{
    Q_OBJECT
public:
    Process(QObject * pParent = 0);
    ~Process();

    bool setProcess(char * szProcess);

    bool isRunning();

    static DWORD WINAPI checkStdOut(void * pRunning);

public slots:
    void stdIn(const QString &sMessage);

signals:
    void printInStdOut(QString sMessage);
    void printInStdErr(QString sMessage);

protected:
    PROCESS_INFORMATION _processInformation; 

    HANDLE _hIN[2];
    HANDLE _hOUT[2];
    HANDLE _hERR[2];

    Thread * _pStdOutThread;
    Thread * _pStdErrThread;
    bool _bProcessRunning;
};

}

#endif
