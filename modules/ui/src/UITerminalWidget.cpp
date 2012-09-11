
// Own
#include <UITerminalWidget.hpp>

// Qt
#include <QDebug>
#include <QBoxLayout>
#include <QScrollBar>

#include <cstdio>

#define REPCHAR		"ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
			"abcdefgjijklmnopqrstuvwxyz" \
			"0123456789./+@"

#define FONT_FAMILY	"DejaVu Sans Mono"

using namespace UI;

TerminalWidget::TerminalWidget(QWidget* pParent):
    QWidget(pParent)
{
_pScrollBar = new QScrollBar(this);

QFontMetrics fm(font());
_hFontHeight = fm.height();
_wFontWidth = qRound((double)fm.width(REPCHAR)/(double)strlen(REPCHAR));

setCursor(Qt::IBeamCursor);
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

void TerminalWidget::resizeEvent(QResizeEvent* pEvent)
{
    QWidget::resizeEvent(pEvent);

    _pScrollBar->resize(_pScrollBar->sizeHint().width(), contentsRect().height());

// 
//     int iLines = height() / _hFontHeight;
// 
//     if((iLines*_hFontHeight) != height())
//         setGeometry(x(), y(), width(), 100);
}
