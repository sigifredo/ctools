
#ifndef UI_TERMINALWIDGET_HPP
#define UI_TERMINALWIDGET_HPP

#include<QWidget>
#include<UITypes.hpp>

class QScrollBar;

namespace UI
{

class TerminalWidget: public QWidget
{
    Q_OBJECT
public:
    TerminalWidget(QWidget * parent = 0);
    ~TerminalWidget();

    void printStdOut(QString str);

    /**
     * \brief
     *
     * \param iCursor
     * \param iLines
     *
     */
    void setScroll(int iCursor, int iLines);

    void setScrollBarLocation(ScrollBarLocation location);

signals:
    void sendToStdIn(QString &command);

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

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void paintEvent( QPaintEvent * event);

    virtual void resizeEvent(QResizeEvent*);

    void updateImage();

protected slots:
    void scrollBarPositionChanged(int value);

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
