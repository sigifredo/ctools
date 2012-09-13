
#ifndef XTERMINALTYPES_H
#define XTERMINALTYPES_H

#include<QList>

typedef QList<QString> History;
typedef QList<QString>::iterator HistoryIterator;

// enum XTerminalStatus { waitingResponse, waitingCommand };
// enum MsgType { CommandStatus, CommandFinish, CommandError };
enum ScrollBarLocation { NoScrollBar, ScrollBarLeft, ScrollBarRight };


class XCommand
{
public:
};

#endif
