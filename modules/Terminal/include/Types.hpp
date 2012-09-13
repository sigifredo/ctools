
#ifndef XTERMINALTYPES_H
#define XTERMINALTYPES_H

#include<QList>

#ifdef _WIN32
#  define EOL		"\r\n"
#else
#  define EOL		"\n"
#endif
#define EOL_WIN		"\r\n"
#define EOL_LINUX	"\n"

typedef QList<QString> History;

enum ScrollBarLocation { NoScrollBar, ScrollBarLeft, ScrollBarRight };

#endif
