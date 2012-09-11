
#include <QApplication>
#include <XTerminal.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    XTerminal t;
    t.showMaximized();
    
    return app.exec();
}
