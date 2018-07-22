#include <QApplication>
#include <QTextEncoder>
#include <QString>
#include <QTranslator>
#include <QLibraryInfo>
#include "mw.hpp"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load(
        "qt_" + QLocale::system().name(),
        QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QString qmPath(":/translations");
    QTranslator appTranslator;
    appTranslator.load(QLocale::system().name(), qmPath);
    app.installTranslator(&appTranslator);

    QIcon icon(":/images/goPage.png");
    app.setWindowIcon(icon);

    Mw mw;
    mw.show();

    return app.exec();
}

