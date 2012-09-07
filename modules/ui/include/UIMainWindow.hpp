
#ifndef UIMAINWINDOW_HPP
#define UIMAINWINDOW_HPP

// Own
#include <UIGlobal.hpp>

// Qt
#include <QMainWindow>

namespace UI
{

class UI_EXPORT MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget * pParent = 0);
};

}

#endif
