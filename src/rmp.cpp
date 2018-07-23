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

Rmp::Rmp(QString const & book, QObject* parent)
    : QObject(parent)
{
    this->book= book;
    return;
}

bool Rmp::ifExist() {
    return exist;
}
void Rmp::setExist(bool exist) {
    this->exist= exist;
}

int Rmp::getOffset() {
    return offset;
}
void Rmp::setOffset(int offset) {
    this->offset= offset;
}

QString const & Rmp::getPath() {
    return path;
}
void Rmp::setPath(QString const & path) {
    this->path= path;
}

Rmp::State Rmp::getState() {
    return state;
}
void Rmp::setState(State state) {
    this->state= state;
}

Rmp::State Rmp::load(QString const & rmpPath) {
    QFile rmp(rmpPath);
    if (rmp.exists()) {
        rmp.open(QIODevice::ReadOnly);
        auto cfg= QTextStream(&rmp).readLine();
        rmp.close();
        if (rmpFmt.exactMatch(cfg)) {
            offset= QString(rmpFmt.cap(1)).toInt();
            path= rmpFmt.cap(3).replace('\\', '/');
            state= ok;
        }
        else {
            state= formatErr;
        }
    } else {
        state= notExist;
    }
    return state;
}

bool Rmp::save(QString const & rmpPath) {
    QFile rmp(rmpPath);
    if (rmp.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QString cfg= QString("%1@%2\n").arg(offset).arg(path);
        QTextStream rmpFile(&rmp);
        rmpFile << cfg;
        rmpFile.flush();
        rmp.close();
        return true;
    } else {
        return false;
    }
}

