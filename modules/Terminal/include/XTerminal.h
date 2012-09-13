
#ifndef XTERMINAL_H
#define XTERMINAL_H

#include<QWidget>
#include<XTerminalTypes.h>
#include<QCompleter>

class QScrollBar;

class XTerminal: public QWidget
{
    Q_OBJECT
public:
    XTerminal(QWidget * parent = 0);
    ~XTerminal(void);

    void printStdOut(QString str);
    void setScroll(int cursor, int slines);
    void setScrollBarLocation(ScrollBarLocation location);

public slots:
    void termSignal(void);

signals:
    void keyPressedSignal(QKeyEvent *e);
    void sendCommand(QString &command);

protected:
    int _iFontHeight;
    int _iFontWidth;
    int _iLeftMargin;
    int _iRightMargin;

    int lines;

    int afterCommand;
    int _pScrollBarValue;

    QString prompt;
    QPoint pntCurrent;
    QPoint pntInitial;
    History * commandsHistory;
    History * linesHistory;

    QColor * _pBackgroundColor;
    QColor * _pForegroundColor;

    QScrollBar * _pScrollBar;
    ScrollBarLocation _eScrollBarLocation;

    void drawBackground(QPainter &painter, QRect &rect);
    void drawContents(QPainter &painter);
    void drawCursor(QPainter &painter, QRect & rect);
    void drawTextFragment(QPainter& painter , const QRect& rect, const QString& text);

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void paintEvent( QPaintEvent * event);

    virtual void resizeEvent(QResizeEvent*);

    void updateImage(void);

protected slots:
    void scrollBarPositionChanged(int value);

private:
    // init grapics elements
    void init(void);
};

inline void XTerminal::setScrollBarLocation(ScrollBarLocation location)
{
    _eScrollBarLocation = location;
}

#endif
