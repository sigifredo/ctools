
// Own
#include <UITerminalWidget.hpp>

// Qt
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    UI::TerminalWidget term;
    term.show();

    return app.exec();
}