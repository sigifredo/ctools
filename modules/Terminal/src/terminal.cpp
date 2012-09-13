
#include <QApplication>
#include <Terminal.hpp>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    Terminal t;
    t.showMaximized();
    
    return app.exec();
}
