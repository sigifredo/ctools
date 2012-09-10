
#ifndef UI_TERMINALWIDGET_HPP
#define UI_TERMINALWIDGET_HPP

// Own
#include <UIGlobal.hpp>
#include <Process.hpp>

// Qt
#include <QWidget>

class QLineEdit;
class QTextEdit;

namespace UI
{

class UI_EXPORT TerminalWidget: public QWidget
{
    Q_OBJECT
public:
    TerminalWidget(QWidget * pParent = 0);

signals:
    void sendMessage(const QString & sMessage);

public slots:
    void sendMessage();
    void printStdOut(QString sMessage);
    void printStdErr(QString sMessage);

protected:
    QTextEdit * _pDisplayTextEdit;
    QLineEdit * _pPromptLineEdit;
    base::Process * _pCMDProcess;
};

}

#endif
