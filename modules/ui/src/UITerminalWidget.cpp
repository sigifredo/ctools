
// Own
#include <UITerminalWidget.hpp>

// Qt
#include <QBoxLayout>
#include <QLineEdit>
#include <QTextEdit>

using namespace UI;

TerminalWidget::TerminalWidget(QWidget * pParent):
    QWidget(pParent)
{
    QBoxLayout * pLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);

    _pDisplayTextEdit = new QTextEdit(this);
    _pDisplayTextEdit->setEnabled(false);
    _pPromptLineEdit = new QLineEdit(this);

    pLayout->addWidget(_pDisplayTextEdit);
    pLayout->addWidget(_pPromptLineEdit);

    connect(_pPromptLineEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
}

void TerminalWidget::sendMessage()
{
    QString sMessage = _pPromptLineEdit->text();
    _pPromptLineEdit->setText(QString(""));

    emit sendMessage(sMessage);
}

