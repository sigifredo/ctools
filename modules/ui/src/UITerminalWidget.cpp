
// Own
#include <UITerminalWidget.hpp>

// Qt
#include <QDebug>
#include <QBoxLayout>
#include <QPainter>
#include <QScrollBar>
#include <QKeyEvent>

#include <cstdio>

#define REPCHAR		"ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
			"abcdefgjijklmnopqrstuvwxyz" \
			"0123456789./+@"

#define FONT_FAMILY	"DejaVu Sans Mono"

#define BACKGROUND_COLOR	0x33, 0x33, 0x33
#define FOREGROUND_COLOR	0xff, 0xff, 0xff

using namespace UI;

TerminalWidget::TerminalWidget(QWidget* pParent):
    QWidget(pParent)
{
_pScrollBar = new QScrollBar(this);

QFontMetrics fm(font());
_hFontHeight = fm.height();
_wFontWidth = qRound((double)fm.width(REPCHAR)/(double)strlen(REPCHAR));
_eScrollBarLocation = ScrollBarLeft;

_pStdOutHistory = new History();
_pBackgroundColor = new QColor(BACKGROUND_COLOR);
_pForegroundColor = new QColor(FOREGROUND_COLOR);

_pInitialPoint = new QPoint(_pScrollBar->width(), 0);
_pCurrentPoint = _pInitialPoint;

setCursor(Qt::IBeamCursor);
}

TerminalWidget::~TerminalWidget()
{
    delete _pInitialPoint;
    delete _pStdOutHistory;
}

void TerminalWidget::sendMessage()
{
}

void TerminalWidget::printStdOut(QString sMessage)
{
}

void TerminalWidget::printStdErr(QString sMessage)
{
}

void TerminalWidget::drawBackground(QPainter &painter, QRect &rect)
{
    painter.save();

    painter.fillRect(rect, *_pBackgroundColor);

    painter.restore();
}

void TerminalWidget::drawContents(QPainter &painter)
{
//    QPoint pnt = *_pInitialPoint;
//
//    for(History::iterator it = _pStdOutHistory->begin(); it != _pStdOutHistory->end(); it++)
//    {
//        QRect r(pnt.x(), pnt.y(), _wFontWidth*(*it).length(), _hFontHeight);
//
//        painter.drawText(r, *it);
//        pnt.setY(pnt.y() + _hFontHeight);
//    }

QRect r(_pCurrentPoint->x(), _pCurrentPoint->y(), _wFontWidth*_sCurrentLine.length(), _hFontHeight);
painter.drawText(r, _sCurrentLine);
}

void TerminalWidget::keyPressEvent(QKeyEvent* pEvent)
{
_sCurrentLine += pEvent->text();
}

void TerminalWidget::paintEvent(QPaintEvent * pEvent)
{
    QPainter painter(this);
    QFont f(FONT_FAMILY, -1/*default value*/, _wFontWidth);
    painter.setFont(f);

    painter.setPen(*_pForegroundColor);

    QRect r = rect();

    drawBackground(painter, r);
    drawContents(painter);

    // QFontMetrics fm(font());
    // int width = leftMargin;
    // if(status == waitingCommand)
    // {
    //     QString str;
    //     if(afterCommand == (commandsHistory->size()-1))
    //         str = prompt+" "+promptTitle;
    //     else
    //         str = commandsHistory->at(afterCommand+1) + " " + promptTitle;
    //     width += _wFontWidth*str.length();
    // }
    // QRect rectCursor(width, pntCurrent.y(), _wFontWidth, _hFontHeight);

    // drawCursor(painter, rectCursor);
}

void TerminalWidget::resizeEvent(QResizeEvent* pEvent)
{
    QWidget::resizeEvent(pEvent);

if(_eScrollBarLocation == ScrollBarLeft)
    _pInitialPoint->setX(_pScrollBar->sizeHint().width());

    _pScrollBar->resize(_pScrollBar->sizeHint().width(), contentsRect().height());

// 
//     int iLines = height() / _hFontHeight;
// 
//     if((iLines*_hFontHeight) != height())
//         setGeometry(x(), y(), width(), 100);
}
