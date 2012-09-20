
// Own
#include <UIMainWindow.hpp>

// Qt
#include <QApplication>
#include <QTextCodec>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    UI::MainWindow wind;
    wind.show();

    return app.exec();
}
