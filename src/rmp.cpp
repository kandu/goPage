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

QRegExp rmpFmt("^((\\+|-)?\\d+)?@(.*)?");

Rmp::Rmp(QString book, QObject* parent)
    : QObject(parent)
{
    QString appDir= QApplication::applicationDirPath();
    this->book= book;
    QString rmpPath= appDir + "/" + book + ".rmp";
    QFile rmp(rmpPath);
    if (rmp.exists()) {
        rmp.open(QIODevice::ReadOnly);
        auto cfg= QTextStream(&rmp).readLine();
        rmp.close();
        if (rmpFmt.exactMatch(cfg)) {
            _offset= QString(rmpFmt.cap(1)).toInt();
            _path= rmpFmt.cap(3);
            if (_path.isEmpty()) {
                _path= appDir + "/" + book + ".pdf";
            } else {
                QDir dict(appDir);
                dict.cd(QFileInfo(_path).dir().path());
                _path= dict.path()
                    + "/"
                    + QFileInfo(_path).fileName();
            }
        }
        else {
            QMessageBox(
                QMessageBox::Warning,
                "錯誤",
                book + ".rmp 文件格式錯誤。"
            ).exec();
        }
    } else {
        _offset= 0;
        _path= appDir + "/" + book + ".pdf";
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

