
#include<UITerminalWidget.hpp>
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

using namespace UI;

TerminalWidget::TerminalWidget(QWidget * parent):
    QWidget(parent), _sPrompt(""), _eScrollBarLocation(ScrollBarLeft)
{
    init();
}


void TerminalWidget::init()
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

    setFocusPolicy(Qt::StrongFocus);
}

TerminalWidget::~TerminalWidget()
{
    delete _pScrollBar;
    delete _pForegroundColor;
    delete _pBackgroundColor;
    delete _pStdOutHistory;
}

void TerminalWidget::printStdOut(QString str)
{
    if(str.trimmed() != "")
    {
        str.remove("\x0d");
        QStringList lst = str.split("\x0a");

        for(QStringList::iterator it = lst.begin(); it != lst.end(); ++it)
        {
            int iLength = (*it).length();
            int iWidthAllowed = width() - (_iLeftMargin+_iRightMargin);

            if((iLength*(_iFontWidth+1)) > iWidthAllowed)
            {
                int iCharactersAllowed = iWidthAllowed / _iFontWidth;

                QString s2 = (*it);
                (*it).remove(iCharactersAllowed, iLength);
                s2.remove(0, iCharactersAllowed);

#warning "hay un error a la hora de insertar la parte faltante de la linea"
                // lst.insert(it, s2);
            }
        }

        _pStdOutHistory->append(lst);

        _pCurrentPoint->setY(_pCurrentPoint->y() + _iFontHeight*lst.length());

        updateImage();
    }
}

void TerminalWidget::setScroll(int iCursor, int iLines)
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

void TerminalWidget::drawBackground(QPainter &painter, QRect &rect)
{
    painter.save();

    painter.fillRect(rect, *_pBackgroundColor);

    painter.restore();
}

void TerminalWidget::drawContents(QPainter &painter)
{
    QPoint pnt = *_pInitialPoint;

    History::iterator last;
    for(History::iterator it = _pStdOutHistory->begin(); it != _pStdOutHistory->end(); last = it++)
    {
        int iLength = (*it).length();
        QRect r(pnt.x(), pnt.y(), _iFontWidth*iLength + iLength, _iFontHeight);

        painter.drawText(r, *it);

        pnt.setY(pnt.y() + _iFontHeight);
    }

    // QRect r(pnt.x(), pnt.y(), _iFontWidth*_sPrompt.length(), _iFontHeight);
    if(_pStdOutHistory->length() > 0)
    {
        pnt.setX((_iFontWidth + 1)*(*last).length() + pnt.x());
        pnt.setY(pnt.y() - _iFontHeight);
    }

    QRect r(pnt.x(), pnt.y(), width(), _iFontHeight);
    painter.drawText(r, _sPrompt);

    _pCurrentPoint->setX(pnt.x() + (_iFontWidth+1)*_sPrompt.length());
    _pCurrentPoint->setY(pnt.y());
}

void TerminalWidget::drawCursor(QPainter &painter, QRect & rect)
{
//     QRect cursorRect = rect;
//     painter.fillRect(cursorRect, _cursorColor.isValid() ? _cursorColor : _pForegroundColor);
    painter.fillRect(rect, *_pForegroundColor);
}

#warning "pensado para optimizar la escritura de texto en el widget"
// void TerminalWidget::drawTextFragment(QPainter& painter , const QRect& rect, const QString& text)
// {
//     painter.save();
// 
//     painter.restore();
// }

void TerminalWidget::keyPressEvent(QKeyEvent* event)
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

        emit sendToStdIn(sStdIn + "\n");
    }
    elif(event->key() == Qt::Key_Backspace)
        _sPrompt = _sPrompt.remove(_sPrompt.size()-1, 1);
    else
        _sPrompt += event->text();

    event->accept();

    updateImage();
}

void TerminalWidget::paintEvent(QPaintEvent * pEvent)
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

void TerminalWidget::resizeEvent(QResizeEvent* pEvent)
{
    if(_eScrollBarLocation != NoScrollBar)
    {
        _pScrollBar->resize(_pScrollBar->sizeHint().width(), contentsRect().height());
        _iLeftMargin = _pScrollBar->width();
        _pCurrentPoint->setX(_iLeftMargin);
        _pInitialPoint->setX(_iLeftMargin);

#ifndef _WIN32
        lines = height()/_iFontHeight;
        if(lines*_iFontHeight != height())
        {
            setGeometry(x(), y(), width(), lines*_iFontHeight);
        }
#endif
    }
}

void TerminalWidget::updateImage()
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

void TerminalWidget::scrollBarPositionChanged(int iValue)
{
//     _pCurrentPoint->setY(_pCurrentPoint->y()-(_iFontHeight*value));
    _pInitialPoint->setY(-_iFontHeight*iValue);

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
