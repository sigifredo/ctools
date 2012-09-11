
#ifndef UI_TERMINALWIDGET_HPP
#define UI_TERMINALWIDGET_HPP

// Own
#include <UIGlobal.hpp>
#include <Process.hpp>

// Qt
#include <QWidget>

class QScrollBar;

namespace UI
{

class UI_EXPORT TerminalWidget: public QWidget
{
    Q_OBJECT
public:
    TerminalWidget(QWidget* pParent = 0);

public slots:
    void sendMessage();
    void printStdOut(QString sMessage);
    void printStdErr(QString sMessage);

protected:
    height_t _hFontHeight;
    width_t _wFontWidth;

    QScrollBar * _pScrollBar;

    virtual void resizeEvent(QResizeEvent* pEvent);
};

}

#endif
