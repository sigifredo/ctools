/********************************************************************************
*   Copyright (C) 2008 - 2011 ... by Omar Andrés Zapata Mesa (Head Developer)   *
*   email:andresete.chaos@gmail.com                                             *
*   Copyright (C) 2008 - 2011 ... by Sigifredo Escobar Gómez(Developer)         *
*   email:sigifredo89@gmail.com                                                 *
*   Orbital Mechanics Group                                                     *
*   Aerospace Engineering                                                       *
*   University of Texas At Austin USA                                           *
*   Grupo de Física y Astrofísica Computacional (FACom)                         *
*   División de Programación en Ciencias de la Computación (FACom dev )         *
*   Universidad de Antioquia At Medellin - Colombia                             *
*                                                                               *
********************************************************************************/


#ifndef XTERMINALTYPES_H
#define XTERMINALTYPES_H

#include<QList>

typedef QList<QString> History;
typedef QList<QString>::iterator HistoryIterator;

enum XTerminalStatus { waitingResponse, waitingCommand };
enum MsgType { CommandStatus, CommandFinish, CommandError };
enum ScrollBarLocation { NoScrollBar, ScrollBarLeft, ScrollBarRight };


class XCommand
{
public:
};

#endif
