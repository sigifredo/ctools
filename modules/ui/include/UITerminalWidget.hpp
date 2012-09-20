
#ifndef UI_TERMINALWIDGET_HPP
#define UI_TERMINALWIDGET_HPP

#include<QWidget>
#include<UIGlobal.hpp>
#include<UITypes.hpp>

class QScrollBar;

namespace UI
{

class UI_EXPORT TerminalWidget: public QWidget
{
    Q_OBJECT
public:
    TerminalWidget(QWidget * parent = 0);
    ~TerminalWidget();

    /**
     * \brief
     *
     * \param iCursor
     * \param iLines
     *
     */
    void setScroll(int iCursor, int iLines);

    void setScrollBarLocation(ScrollBarLocation location);

public slots:
    void printStdOut(QString str);

signals:
    void sendToStdIn(const QString &command);

protected:
    int _iFontHeight;
    int _iFontWidth;
    int _iLeftMargin;
    int _iRightMargin;

    int lines;

    int _pScrollBarValue;

    QString _sPrompt;
    QPoint * _pCurrentPoint;
    QPoint * _pInitialPoint;
    History * _pStdOutHistory;

    QColor * _pBackgroundColor;
    QColor * _pForegroundColor;

    QScrollBar * _pScrollBar;
    ScrollBarLocation _eScrollBarLocation;

    void drawBackground(QPainter &painter, QRect &rect);
    void drawContents(QPainter &painter);
    void drawCursor(QPainter &painter, QRect & rect);
    // void drawTextFragment(QPainter& painter , const QRect& rect, const QString& text);

    virtual void keyPressEvent(QKeyEvent* pEvent);
    virtual void paintEvent(QPaintEvent * pEvent);

    virtual void resizeEvent(QResizeEvent* pEvent);

    void updateImage();

protected slots:
    void scrollBarPositionChanged(int iValue);

private:
    // init grapics elements
    void init();
};

inline void TerminalWidget::setScrollBarLocation(ScrollBarLocation location)
{
    _eScrollBarLocation = location;
}

}

#endif
