
#ifndef UI_TERMINALWIDGET_HPP
#define UI_TERMINALWIDGET_HPP

// Own
#include <UITypes.hpp>
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
    ~TerminalWidget();

public slots:
    void sendMessage();
    void printStdOut(QString sMessage);
    void printStdErr(QString sMessage);

protected:
    height_t _hFontHeight;
    width_t _wFontWidth;
    ScrollBarLocation _eScrollBarLocation;

    QPoint * _pInitialPoint;
    QPoint * _pCurrentPoint;
    History * _pStdOutHistory;
    QColor * _pBackgroundColor;
    QColor * _pForegroundColor;
    QString _sCurrentLine;

    QScrollBar * _pScrollBar;

    void drawBackground(QPainter &painter, QRect &rect);
    void drawContents(QPainter &painter);

    virtual void keyPressEvent(QKeyEvent* pEvent);
    virtual void paintEvent(QPaintEvent * pEvent);
    virtual void resizeEvent(QResizeEvent* pEvent);

    void updateImage();
};

}

#endif
