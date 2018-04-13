#include <QApplication>
#include <QTextEncoder>
#include <QString>
#include <QDebug>
#include "manager.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    Manager manager;
    return app.exec();
}

