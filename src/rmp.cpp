#define _RMP

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QRegExp>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include "rmp.hpp"

QRegExp rmpFmt("^((\\+|-)?\\d+)?@(.*)");

Rmp::Rmp(QString book, QObject* parent)
    : QObject(parent)
{
    QString rmpDir(QDir::homePath() + USERCONFDIR + "/rmp");
    this->book= book;
    QString rmpPath= rmpDir + "/" + book + ".rmp";
    QFile rmp(rmpPath);
    if (rmp.exists()) {
        rmp.open(QIODevice::ReadOnly);
        auto cfg= QTextStream(&rmp).readLine();
        rmp.close();
        if (rmpFmt.exactMatch(cfg)) {
            _offset= QString(rmpFmt.cap(1)).toInt();
            _path= rmpFmt.cap(3).replace('\\', '/');
        }
        else {
            QMessageBox(
                QMessageBox::Warning,
                QObject::tr("error"),
                book + QObject::tr(".rmp format error.")
            ).exec();
        }
    } else {
        _offset= 0;
        _path= QDir::currentPath() + "/" + book;
    }
    return;
}

bool Rmp::exist() {
    return _exist;
}

int Rmp::offset() {
    return _offset;
}

QString Rmp::path() {
    return _path;
}

