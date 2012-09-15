
#ifndef UI_TYPES_HPP
#define UI_TYPES_HPP

#include <QList>

#ifdef _WIN32
#  define EOL           "\r\n"
#else
#  define EOL           "\n"
#endif
#define EOL_WIN         "\r\n"
#define EOL_LINUX       "\n"

namespace UI
{

typedef QList<QString> History;

enum ScrollBarLocation { NoScrollBar, ScrollBarLeft, ScrollBarRight };

typedef unsigned int height_t;
typedef unsigned int width_t;

}

#endif
