
#include <UILineEdit.hpp>

#include <QDebug>
#include <QKeyEvent>

using namespace UI;

LineEdit::LineEdit(QWidget * pParent):
    QLineEdit(pParent)
{
}

#include <iostream>
void LineEdit::keyPressEvent(QKeyEvent * e)
{
QLineEdit::keyPressEvent(e);
std::cout << e->text().toStdString() << std::endl;
}
