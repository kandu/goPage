#include <QApplication>
#include <QTextEncoder>
#include <QString>
#include <QTranslator>
#include <QLibraryInfo>
#include "mw.hpp"
#include "init.h"

int main(int argc, char *argv[])
{
    init();
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

    app.setQuitOnLastWindowClosed(false);
    return app.exec();
}

