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


#include<XTerminal.h>
// Qt
#include<QPainter>
#include<QKeyEvent>
#include<QScrollBar>
#include<QAbstractItemView>
#include<QDebug>

#define REPCHAR   "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
                  "abcdefgjijklmnopqrstuvwxyz" \
                  "0123456789./+@"

#ifdef _WIN32
#define FONT_FAMILY		"Courier New"
#else
#define FONT_FAMILY		"DejaVu Sans Mono"
#endif


#define INITIAL_POINT		QPoint(leftMargin, 0/*fontHeight*/)
#define BACKGROUND_COLOR	0x33, 0x33, 0x33
#define FOREGROUND_COLOR	0xff, 0xff, 0xff


XTerminal::XTerminal(QString prompt_title, QWidget * parent):
    QWidget(parent), afterCommand(-1), prompt(""), promptTitle(prompt_title), scrollBarLocation(ScrollBarLeft)
{
    init();
}

XTerminal::XTerminal(QWidget * parent):
    QWidget(parent), afterCommand(-1), prompt(""), promptTitle("xterminal ~$"), scrollBarLocation(ScrollBarLeft)
{
    init();
}


void XTerminal::init()
{
    wordlist << "alpha" << "omega" << "omicron" << "zeta";
    completer = new QCompleter(wordlist, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWidget(this);

    QFontMetrics fm(font());
    fontHeight = fm.height();
    fontWidth = qRound((double)fm.width(REPCHAR)/(double)strlen(REPCHAR));

    commandsHistory = new History;
    linesHistory = new History;

    backgroundColor = new QColor(BACKGROUND_COLOR);
    foregroundColor = new QColor(FOREGROUND_COLOR);

    pntCurrent = INITIAL_POINT;
    pntInitial = INITIAL_POINT;

    scrollBar = new QScrollBar(this);
    scrollBar->setCursor( Qt::ArrowCursor );
    scrollBarValue = 0;
//     scrollBar->hide();
    connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarPositionChanged(int)));

    leftMargin = scrollBar->width();

    setScroll(0, 0);
    setCursor(Qt::IBeamCursor);
    status = waitingCommand;
}

XTerminal::~XTerminal()
{
    delete scrollBar;
    delete foregroundColor;
    delete backgroundColor;
    delete linesHistory;
    delete commandsHistory;
}

void XTerminal::print(QString str)
{
    if(str.trimmed() != "")
    {
        str.remove("\x0d");
        QStringList lst = str.split("\x0a");
        linesHistory->append(lst);
//         for(QList<QString>::iterator it = lst.begin(); it != lst.end(); it++)
//         {
//             QString str = *it;
//             int i = 0;
//             for(QString::iterator jt = str.begin(); jt != str.end(); jt++)
//             {
//                 QString s = "char: " + str[i] + " int: " + QString::number((int)(*jt).toLatin1());
// // 	      linesHistory->append(s);
//                 i++;
//             }
//         }
        pntCurrent.setY(pntCurrent.y() + fontHeight*lst.length());
        updateImage();
    }
}

void XTerminal::setScroll(int cursor, int slines)
{
    if(!scrollBar->isVisible())
        return;

    disconnect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarPositionChanged(int)));
    scrollBar->setRange(0, slines - lines);
    scrollBar->setSingleStep(1);
    scrollBar->setPageStep(lines);
    scrollBar->setValue(cursor);
    connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarPositionChanged(int)));
}

void XTerminal::recvResponse(QString message, MsgType type)
{
    print(message);
    if(type == CommandFinish || type == CommandError)
        status = waitingCommand;
}

void XTerminal::termSignal()
{
    if(afterCommand == (commandsHistory->length()-1))
    {
        linesHistory->append(prompt.trimmed()+"^C");
    }
    else
    {
        linesHistory->append(commandsHistory->at(afterCommand+1) + "^C");
        afterCommand = commandsHistory->length()-1;
    }
    prompt = QString("");
    pntCurrent.setY(pntCurrent.y() + fontHeight);
}

void XTerminal::drawBackground(QPainter &painter, QRect &rect)
{
    painter.save();

    painter.fillRect(rect, *backgroundColor);

    painter.restore();
}

void XTerminal::drawContents(QPainter &painter)
{
    QPoint pnt = pntInitial;

    for(HistoryIterator it = linesHistory->begin(); it != linesHistory->end(); it++)
    {
        QRect r(pnt.x(), pnt.y(), fontWidth*(*it).length(), fontHeight);

        painter.drawText(r, *it);
        pnt.setY(pnt.y() + fontHeight);
    }

    if(status == waitingCommand)
    {
        QString promptLine;
        if(afterCommand == (commandsHistory->size()-1))
            promptLine = promptTitle + " " + prompt;
        else
            promptLine = promptTitle + " " + commandsHistory->at(afterCommand + 1);
        QRect r(pnt.x(), pnt.y(), fontWidth*promptLine.length(), fontHeight);
        painter.drawText(r, promptLine);
    }
}

void XTerminal::drawCursor(QPainter &painter, QRect & rect)
{
//     QRect cursorRect = rect;
//     painter.fillRect(cursorRect, _cursorColor.isValid() ? _cursorColor : foregroundColor);
    painter.fillRect(rect, *foregroundColor);
}

void XTerminal::drawTextFragment(QPainter& painter , const QRect& rect, const QString& text)
{
    painter.save();

    painter.restore();
}

void XTerminal::keyPressEvent(QKeyEvent* event)
{
    QString promptLine;
    if(event->modifiers() == Qt::ControlModifier)
    {
        if(event->key() == Qt::Key_D)
        {
            event->accept();
            exit(0);
        }
        else if(event->key() == Qt::Key_C)
        {
            termSignal();
            goto END_EVENT;
        }
    }
    
    else if(status == waitingResponse)
    {
        goto END_EVENT;
    }

    if(event->key() == Qt::Key_Up)
    {
        if(afterCommand > -1)
            afterCommand--;
        goto END_EVENT;
    }
    else if(event->key() == Qt::Key_Down)
    {
        if(afterCommand < commandsHistory->size()-1)
            afterCommand++;
        goto END_EVENT;
    }
    else if(event->key() == Qt::Key_Return)
    {
        if(completer->completionCount()>0&&completer->completionCount()<wordlist.count())
	{
	      prompt=completer->currentCompletion();
	      goto END_EVENT;
	}

        if(afterCommand != (commandsHistory->length()-1))
        {
            prompt = commandsHistory->at(afterCommand+1);
            afterCommand = commandsHistory->length()-1;
        }
        linesHistory->append(promptTitle + " " + prompt.trimmed());
        QString smdCommand = prompt;
        if(prompt.trimmed() == "")
        {
            pntCurrent.setY(pntCurrent.y() + fontHeight);
        }
        else
        {
            commandsHistory->append(prompt.trimmed());
// 	    pntInitial.setY(pntInitial.y() - fontHeight);
            afterCommand++;
            prompt = QString("");
            pntCurrent.setY(pntCurrent.y() + fontHeight);
        }
        status = waitingResponse;
        emit sendCommand(smdCommand);
        goto END_EVENT;
    }
    else if(event->key() == Qt::Key_Backspace)
    {
        prompt = prompt.remove(prompt.size()-1, 1);
        goto END_EVENT;
    }

    prompt += event->text();
    emit keyPressedSignal(event);

END_EVENT:
    
    completer->setCompletionPrefix(prompt);
    completer->popup()->keyboardSearch(prompt);
    promptLine = promptTitle + " " + prompt;
    completer->complete(QRect( pntCurrent.x(),pntCurrent.y(),fontWidth*promptLine.length(), fontHeight));
    if(completer->completionCount()>0&&completer->completionCount()<wordlist.count())
    {
      qDebug()<<"FOUND"<<completer->completionCount();
     completer->popup()->setVisible(true); 
    }else{
     completer->popup()->setVisible(false);       
    }

    event->accept();
    updateImage();
}

void XTerminal::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    QFont f(FONT_FAMILY, -1/*default value*/, fontWidth);
    painter.setFont(f);

    painter.setPen(*foregroundColor);

    QRect r = rect();

    drawBackground(painter, r);
    drawContents(painter);

    QFontMetrics fm(font());
    int width = leftMargin;
    if(status == waitingCommand)
    {
        QString str;
        if(afterCommand == (commandsHistory->size()-1))
            str = prompt+" "+promptTitle;
        else
            str = commandsHistory->at(afterCommand+1) + " " + promptTitle;
        width += fontWidth*str.length();
    }
    QRect rectCursor(width, pntCurrent.y(), fontWidth, fontHeight);

    drawCursor(painter, rectCursor);
}

void XTerminal::resizeEvent(QResizeEvent*)
{
    if(scrollBarLocation != NoScrollBar)
    {
        scrollBar->resize(scrollBar->sizeHint().width(), contentsRect().height());
        leftMargin = scrollBar->width();
        pntCurrent.setX(leftMargin);
        pntInitial.setX(leftMargin);
        lines = height()/fontHeight;
        if(lines*fontHeight != height())
        {
            setGeometry(x(), y(), width(), lines*fontHeight);
        }
    }
}

void XTerminal::updateImage()
{
    setScroll( linesHistory->length(), linesHistory->length()+1);
    int top = height() - ((linesHistory->length()+1)*fontHeight);
    if(top < 0)
    {
        pntInitial.setY(top);
        pntCurrent.setY(height() - fontHeight);
    }
    update();
}

void XTerminal::scrollBarPositionChanged(int value)
{
//     pntCurrent.setY(pntCurrent.y()-(fontHeight*value));
    pntInitial.setY(-fontHeight*value);

//   if ( !_screenWindow )
//       return;
//   OJO
//   _screenWindow->scrollTo( _scrollBar->value() );

    // if the thumb has been moved to the bottom of the _scrollBar then set
    // the display to automatically track new output,
    // that is, scroll down automatically
    // to how new _lines as they are added
//   const bool atEndOfOutput = (_scrollBar->value() == _scrollBar->maximum());
//   _screenWindow->setTrackOutput( atEndOfOutput );

    update();
}
