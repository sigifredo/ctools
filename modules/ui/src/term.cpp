
// Own
#include <UITerminalWidget.hpp>

// Qt
#include <QApplication>
#include <QTextCodec>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // QTextCodec * pCodec = QTextCodec::codecForName("Latin-1");
    // QTextCodec::setCodecForTr(pCodec);
    // QTextCodec::setCodecForCStrings(pCodec);
    // QTextCodec::setCodecForLocale(pCodec);

    UI::TerminalWidget term;
    term.show();

    return app.exec();
}
