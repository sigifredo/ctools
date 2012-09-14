
#ifndef UI_TYPES_HPP
#define UI_TYPES_HPP

#include <QList>

namespace UI
{

typedef QList<QString> History;

enum ScrollBarLocation { NoScrollBar, ScrollBarLeft, ScrollBarRight };

typedef unsigned int height_t;
typedef unsigned int width_t;

}

#endif
