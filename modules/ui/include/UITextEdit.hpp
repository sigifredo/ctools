
#ifndef UI_LINEEDIT_HPP
#define UI_LINEEDIT_HPP

#include <QLineEdit>

namespace UI
{

class LineEdit: public QLineEdit
{
    Q_OBJECT
public:
    LineEdit(QWidget * pParent = 0);

protected:
    void keyPressEvent(QKeyEvent * e);
};

}

#endif
