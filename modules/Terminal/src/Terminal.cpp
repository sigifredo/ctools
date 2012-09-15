
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
    QWidget(parent), _sPrompt(""), _eScrollBarLocation(ScrollBarLeft)
{
    init();
}


void Terminal::init()
{
    QFontMetrics fm(font());
    _iFontHeight = fm.height();
    _iFontWidth = qRound((double)fm.width(REPCHAR)/(double)strlen(REPCHAR));

    _pStdOutHistory = new History;

    _pBackgroundColor = new QColor(BACKGROUND_COLOR);
    _pForegroundColor = new QColor(FOREGROUND_COLOR);

    _pCurrentPoint = new INITIAL_POINT;
    _pInitialPoint = new INITIAL_POINT;

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
    delete _pStdOutHistory;
}

void Terminal::printStdOut(QString str)
{
    if(str.trimmed() != "")
    {
        str.remove("\x0d");
        QStringList lst = str.split("\x0a");
        _pStdOutHistory->append(lst);

        _pCurrentPoint->setY(_pCurrentPoint->y() + _iFontHeight*lst.length());

        updateImage();
    }
}

void Terminal::setScroll(int iCursor, int iLines)
{
    if(_pScrollBar->isVisible())
    {
        disconnect(_pScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarPositionChanged(int)));
        _pScrollBar->setRange(0, iLines - lines);
        _pScrollBar->setSingleStep(1);
        _pScrollBar->setPageStep(lines);
        _pScrollBar->setValue(iCursor);
        connect(_pScrollBar, SIGNAL(valueChanged(int)), this, SLOT(scrollBarPositionChanged(int)));
    }
}

void Terminal::drawBackground(QPainter &painter, QRect &rect)
{
    painter.save();

    painter.fillRect(rect, *_pBackgroundColor);

    painter.restore();
}

void Terminal::drawContents(QPainter &painter)
{
    QPoint pnt = *_pInitialPoint;

    for(History::iterator it = _pStdOutHistory->begin(); it != _pStdOutHistory->end(); it++)
    {
        QRect r(pnt.x(), pnt.y(), _iFontWidth*(*it).length(), _iFontHeight);

        painter.drawText(r, *it);
        pnt.setY(pnt.y() + _iFontHeight);
    }

    QRect r(pnt.x(), pnt.y(), _iFontWidth*_sPrompt.length(), _iFontHeight);
    painter.drawText(r, _sPrompt);

    _pCurrentPoint->setX(_iLeftMargin + _iFontWidth*_sPrompt.length());
}

void Terminal::drawCursor(QPainter &painter, QRect & rect)
{
//     QRect cursorRect = rect;
//     painter.fillRect(cursorRect, _cursorColor.isValid() ? _cursorColor : _pForegroundColor);
    painter.fillRect(rect, *_pForegroundColor);
}

#warning "pensado para optimizar la escritura de texto en el widget"
// void Terminal::drawTextFragment(QPainter& painter , const QRect& rect, const QString& text)
// {
//     painter.save();
// 
//     painter.restore();
// }

void Terminal::keyPressEvent(QKeyEvent* event)
{
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_D)
    {
        event->accept();
        exit(0);
    }
    elif(event->key() == Qt::Key_Return)
    {
        _sPrompt = _sPrompt.trimmed();

        _pStdOutHistory->append(_sPrompt);
        QString sStdIn = _sPrompt;

        if(_sPrompt != "")
            _sPrompt = QString("");

        _pCurrentPoint->setY(_pCurrentPoint->y() + _iFontHeight);

        emit sendToStdIn(sStdIn);
    }
    elif(event->key() == Qt::Key_Backspace)
        _sPrompt = _sPrompt.remove(_sPrompt.size()-1, 1);
    else
        _sPrompt += event->text();

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

    QRect rectCursor(_pCurrentPoint->x(), _pCurrentPoint->y(), _iFontWidth, _iFontHeight);

    drawCursor(painter, rectCursor);
}

void Terminal::resizeEvent(QResizeEvent*)
{
    if(_eScrollBarLocation != NoScrollBar)
    {
        _pScrollBar->resize(_pScrollBar->sizeHint().width(), contentsRect().height());
        _iLeftMargin = _pScrollBar->width();
        _pCurrentPoint->setX(_iLeftMargin);
        _pInitialPoint->setX(_iLeftMargin);
        lines = height()/_iFontHeight;
        if(lines*_iFontHeight != height())
        {
            setGeometry(x(), y(), width(), lines*_iFontHeight);
        }
    }
}

void Terminal::updateImage()
{
    setScroll( _pStdOutHistory->length(), _pStdOutHistory->length()+1);
    int top = height() - ((_pStdOutHistory->length()+1)*_iFontHeight);
    if(top < 0)
    {
        _pInitialPoint->setY(top);
        _pCurrentPoint->setY(height() - _iFontHeight);
    }
    update();
}

void Terminal::scrollBarPositionChanged(int value)
{
//     _pCurrentPoint->setY(_pCurrentPoint->y()-(_iFontHeight*value));
    _pInitialPoint->setY(-_iFontHeight*value);

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
