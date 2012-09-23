
// Own
#include <UIMainWindow.hpp>
#include <UITerminalWidget.hpp>

#include <QBoxLayout>

using namespace UI;

MainWindow::MainWindow(QWidget * pParent):
    QMainWindow(pParent)
{
    QWidget * pCentralWidget = new QWidget(this);
    QBoxLayout * pLayout = new QBoxLayout(QBoxLayout::TopToBottom, pCentralWidget);

    _pTerminalWidget = new TerminalWidget(this);

    pLayout->addWidget(_pTerminalWidget);

    setCentralWidget(pCentralWidget);

    _pProcess = new base::Process(this);

    connect(_pTerminalWidget, SIGNAL(sendToStdIn(const QString &)), _pProcess, SLOT(stdIn(const QString&)));
    connect(_pProcess, SIGNAL(printInStdOut(QString)), _pTerminalWidget, SLOT(printStdOut(QString)));
    connect(_pProcess, SIGNAL(printInStdErr(QString)), _pTerminalWidget, SLOT(printStdOut(QString)));

    _pProcess->setProcess("cmd");
}

