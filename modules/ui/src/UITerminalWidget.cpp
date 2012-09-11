
// Own
#include <UITerminalWidget.hpp>

// Qt
#include <QBoxLayout>
#include <QLineEdit>
#include <QTextEdit>

#include <cstdio>

using namespace UI;

TerminalWidget::TerminalWidget(QWidget * pParent):
    QWidget(pParent)
{
    QBoxLayout * pLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    _pDisplayTextEdit = new QTextEdit(this);
    _pDisplayTextEdit->setReadOnly(true);
    _pDisplayTextEdit->setFontFamily("DejaVu Sans Mono");
    _pPromptLineEdit = new QLineEdit(this);

    pLayout->addWidget(_pDisplayTextEdit);
    pLayout->addWidget(_pPromptLineEdit);

    connect(_pPromptLineEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));

    _pCMDProcess = new base::Process(this);
    connect(_pCMDProcess, SIGNAL(printInStdOut(QString)), this, SLOT(printStdOut(QString)));
    connect(_pCMDProcess, SIGNAL(printInStdErr(QString)), this, SLOT(printStdErr(QString)));
    _pCMDProcess->setProcess("cmd");
}

void TerminalWidget::sendMessage()
{
    QString sMessage = _pPromptLineEdit->text() + "\n";
    _pPromptLineEdit->setText(QString(""));

    _pCMDProcess->stdIn(sMessage);
}

#include <QDebug>
void TerminalWidget::printStdOut(QString sMessage)
{
if(sMessage != "")
{
    sMessage = sMessage.replace(QString("\r\n"), QString(""));
qDebug() << sMessage;
    _pDisplayTextEdit->append(sMessage);
    _pDisplayTextEdit->append(sMessage);
}
}

void TerminalWidget::printStdErr(QString sMessage)
{
    _pDisplayTextEdit->append("<font color=red>" + sMessage + "</font>");
}

// #include <iostream>
// #define print(X) std::cout << __LINE__ << ": " << X << std::endl
// void TerminalWidget::keyPressEvent(QKeyEvent * e)
// {
// QTextEdit::keyPressEvent(e);
// 
// print(e->text());
// _pCMDProcess->stdIn(e->text());
// }
