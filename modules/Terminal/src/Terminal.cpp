
#include<Terminal.hpp>
// Qt
#include<QPainter>
#include<QKeyEvent>
#include<QScrollBar>
#include<QAbstractItemView>
#include<QDebug>

#define REPCHAR   "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
                  "abcdefgjijklmnopqrstuvwxyz" \
                  "0123456789./+@"

#define FONT_FAMILY		"DejaVu Sans Mono"


#define INITIAL_POINT		QPoint(_iLeftMargin, 0/*_iFontHeight*/)
#define BACKGROUND_COLOR	0x33, 0x33, 0x33
#define FOREGROUND_COLOR	0xff, 0xff, 0xff

#ifndef elif
#  define elif			else if
#endif

Terminal::Terminal(QWidget * parent):
    QWidget(parent), afterCommand(-1), prompt(""), _eScrollBarLocation(ScrollBarLeft)
{
    init();
}


void Terminal::init()
{
    QFontMetrics fm(font());
    _iFontHeight = fm.height();
    _iFontWidth = qRound((double)fm.width(REPCHAR)/(double)strlen(REPCHAR));

    commandsHistory = new History;
    linesHistory = new History;

    _pBackgroundColor = new QColor(BACKGROUND_COLOR);
    _pForegroundColor = new QColor(FOREGROUND_COLOR);

    pntCurrent = INITIAL_POINT;
    pntInitial = INITIAL_POINT;

    _pScrollBar = new QScrollBar(this);
    _pScrollBar->setCursor( Qt::ArrowCursor );
    _pScrollBarValue = 0;
//     _pScrollBar->hide();
    connect(_pScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarPositionChanged(int)));

    _iLeftMargin = _pScrollBar->width();

    setScroll(0, 0);
    setCursor(Qt::IBeamCursor);
}

Terminal::~Terminal()
{
    delete _pScrollBar;
    delete _pForegroundColor;
    delete _pBackgroundColor;
    delete linesHistory;
    delete commandsHistory;
}

void Terminal::printStdOut(QString str)
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
        pntCurrent.setY(pntCurrent.y() + _iFontHeight*lst.length());
        updateImage();
    }
}

void Terminal::setScroll(int cursor, int slines)
{
    if(!_pScrollBar->isVisible())
        return;

    disconnect(_pScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarPositionChanged(int)));
    _pScrollBar->setRange(0, slines - lines);
    _pScrollBar->setSingleStep(1);
    _pScrollBar->setPageStep(lines);
    _pScrollBar->setValue(cursor);
    connect(_pScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarPositionChanged(int)));
}

void Terminal::termSignal()
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
    pntCurrent.setY(pntCurrent.y() + _iFontHeight);
}

void Terminal::drawBackground(QPainter &painter, QRect &rect)
{
    painter.save();

    painter.fillRect(rect, *_pBackgroundColor);

    painter.restore();
}

void Terminal::drawContents(QPainter &painter)
{
    QPoint pnt = pntInitial;

    for(HistoryIterator it = linesHistory->begin(); it != linesHistory->end(); it++)
    {
        QRect r(pnt.x(), pnt.y(), _iFontWidth*(*it).length(), _iFontHeight);

        painter.drawText(r, *it);
        pnt.setY(pnt.y() + _iFontHeight);
    }

    QRect r(pnt.x(), pnt.y(), _iFontWidth*prompt.length(), _iFontHeight);
    painter.drawText(r, prompt);
}

void Terminal::drawCursor(QPainter &painter, QRect & rect)
{
//     QRect cursorRect = rect;
//     painter.fillRect(cursorRect, _cursorColor.isValid() ? _cursorColor : _pForegroundColor);
    painter.fillRect(rect, *_pForegroundColor);
}

void Terminal::drawTextFragment(QPainter& painter , const QRect& rect, const QString& text)
{
    painter.save();

    painter.restore();
}

void Terminal::keyPressEvent(QKeyEvent* event)
{
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
    
    if(event->key() == Qt::Key_Up)
    {
        if(afterCommand > -1)
            afterCommand--;
        goto END_EVENT;
    }
    elif(event->key() == Qt::Key_Down)
    {
        if(afterCommand < commandsHistory->size()-1)
            afterCommand++;
        goto END_EVENT;
    }
    elif(event->key() == Qt::Key_Return)
    {
        if(afterCommand != (commandsHistory->length()-1))
        {
            prompt = commandsHistory->at(afterCommand+1);
            afterCommand = commandsHistory->length()-1;
        }
        linesHistory->append(prompt.trimmed());
        QString smdCommand = prompt;
        if(prompt.trimmed() == "")
        {
            pntCurrent.setY(pntCurrent.y() + _iFontHeight);
        }
        else
        {
            commandsHistory->append(prompt.trimmed());
// 	    pntInitial.setY(pntInitial.y() - _iFontHeight);
            afterCommand++;
            prompt = QString("");
            pntCurrent.setY(pntCurrent.y() + _iFontHeight);
        }
        emit sendCommand(smdCommand);
        goto END_EVENT;
    }
    elif(event->key() == Qt::Key_Backspace)
    {
        prompt = prompt.remove(prompt.size()-1, 1);
        goto END_EVENT;
    }

    prompt += event->text();
    emit keyPressedSignal(event);

END_EVENT:
    
    event->accept();
    updateImage();
}

void Terminal::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    QFont f(FONT_FAMILY, -1/*default value*/, _iFontWidth);
    painter.setFont(f);

    painter.setPen(*_pForegroundColor);

    QRect r = rect();

    drawBackground(painter, r);
    drawContents(painter);

    QFontMetrics fm(font());
    int width = _iLeftMargin;

    QRect rectCursor(width, pntCurrent.y(), _iFontWidth, _iFontHeight);

    drawCursor(painter, rectCursor);
}

void Terminal::resizeEvent(QResizeEvent*)
{
    if(_eScrollBarLocation != NoScrollBar)
    {
        _pScrollBar->resize(_pScrollBar->sizeHint().width(), contentsRect().height());
        _iLeftMargin = _pScrollBar->width();
        pntCurrent.setX(_iLeftMargin);
        pntInitial.setX(_iLeftMargin);
        lines = height()/_iFontHeight;
        if(lines*_iFontHeight != height())
        {
            setGeometry(x(), y(), width(), lines*_iFontHeight);
        }
    }
}

void Terminal::updateImage()
{
    setScroll( linesHistory->length(), linesHistory->length()+1);
    int top = height() - ((linesHistory->length()+1)*_iFontHeight);
    if(top < 0)
    {
        pntInitial.setY(top);
        pntCurrent.setY(height() - _iFontHeight);
    }
    update();
}

void Terminal::scrollBarPositionChanged(int value)
{
//     pntCurrent.setY(pntCurrent.y()-(_iFontHeight*value));
    pntInitial.setY(-_iFontHeight*value);

//   if ( !_screenWindow )
//       return;
//   OJO
//   _screenWindow->scrollTo( __pScrollBar->value() );

    // if the thumb has been moved to the bottom of the __pScrollBar then set
    // the display to automatically track new output,
    // that is, scroll down automatically
    // to how new _lines as they are added
//   const bool atEndOfOutput = (__pScrollBar->value() == __pScrollBar->maximum());
//   _screenWindow->setTrackOutput( atEndOfOutput );

    update();
}
