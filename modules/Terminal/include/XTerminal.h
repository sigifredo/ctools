/********************************************************************************
*   Copyright (C) 2008 - 2011 ... by Omar Andrés Zapata Mesa (Head Developer)   *
*   email:andresete.chaos@gmail.com                                             *
*   Copyright (C) 2008 - 2011 ... by Sigifredo Escobar Gómez(Developer)         *
*   email:sigifredo89@gmail.com                                                 *
*   Orbital Mechanics Group                                                     *
*   Aerospace Engineering                                                       *
*   University of Texas At Austin USA                                           *
*   Grupo de Física y Astrofísica Computacional (FACom)                         *
*   División de Programación en Ciencias de la Computación (FACom dev )         *
*   Universidad de Antioquia At Medellin - Colombia                             *
*                                                                               *
********************************************************************************/


#ifndef XTERMINAL_H
#define XTERMINAL_H

#include<QWidget>
#include<XTerminalTypes.h>
#include<QCompleter>

class QScrollBar;
class XTerminal: public QWidget {
    Q_OBJECT
public:
    XTerminal(QString prompt_title = QString("xterminal ~$"), QWidget * parent = 0);
    XTerminal(QWidget * parent);
    ~XTerminal(void);

    void setPromptTitle(QString prompt_title) {
        promptTitle = prompt_title;
    }

    void print(QString str);
    void setScroll(int cursor, int slines);
    void setScrollBarLocation(ScrollBarLocation location);

public slots:
    void recvResponse(QString message, MsgType type);
    void termSignal(void);


signals:
    void keyPressedSignal(QKeyEvent *e);
    void sendCommand(QString &command);

protected:
    int fontHeight;
    int fontWidth;
    int leftMargin;
    int rightMargin;

    int lines;

    XTerminalStatus status;

    int afterCommand;
    int scrollBarValue;

    QString prompt;
    QString promptTitle;
    QPoint pntCurrent;
    QPoint pntInitial;
    History * commandsHistory;
    History * linesHistory;

    QColor * backgroundColor;
    QColor * foregroundColor;

    QScrollBar * scrollBar;
    ScrollBarLocation scrollBarLocation;

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
    QCompleter *completer;
    QStringList wordlist;
};

inline void XTerminal::setScrollBarLocation(ScrollBarLocation location)
{
    scrollBarLocation = location;
}

#endif
