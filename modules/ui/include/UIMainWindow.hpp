
#ifndef UIMAINWINDOW_HPP
#define UIMAINWINDOW_HPP

// Own
#include <UIGlobal.hpp>
#include <Process.hpp>

// Qt
#include <QMainWindow>

namespace UI
{

class TerminalWidget;

class UI_EXPORT MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget * pParent = 0);

protected:
    TerminalWidget * _pTerminalWidget;
    base::Process * _pProcess;
};

}

#endif
