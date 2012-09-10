
#ifndef BASE_THREAD_HPP
#define BASE_THREAD_HPP

#include <windows.h>
#include <QObject>

namespace base
{

enum Status { Running, Stopped };

class Thread: public QObject
{
    Q_OBJECT
public:
    Thread(HANDLE * hOUT, QObject * pParent = 0);
    ~Thread();

    bool start();
    bool stop();

    Status status() const;

signals:
    void print(QString sMessageOut);

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
