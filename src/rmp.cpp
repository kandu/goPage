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

Rmp::Rmp(QString const & name)
{
    this->name= name;
    return;
}

QString const & Rmp::getName() const {
    return name;
}

bool Rmp::isRmpExist() {
    return rmpExist;
}

bool Rmp::isDictExist() {
    return QFile(path).exists();
}

int Rmp::getOffset() const {
    return offset;
}
void Rmp::setOffset(int offset) {
    this->offset= offset;
}

QString const & Rmp::getPath() const {
    return path;
}
void Rmp::setPath(QString const & path) {
    this->path= path;
}

Rmp::State Rmp::getState() const {
    return state;
}
void Rmp::setState(State state) {
    this->state= state;
}

Rmp::State Rmp::load(QDir const & rmpDir) {
    QFile rmp(rmpDir.filePath(name + ".rmp"));
    if (rmp.exists()) {
        rmpExist= true;
        rmp.open(QIODevice::ReadOnly);
        auto cfg= QTextStream(&rmp).readLine();
        rmp.close();
        if (rmpFmt.exactMatch(cfg)) {
            offset= QString(rmpFmt.cap(1)).toInt();
            path= rmpFmt.cap(3)/*.replace('\\', '/')*/;
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

bool Rmp::save(QDir const & rmpDir) {
    QFile rmp(rmpDir.filePath(name + ".rmp"));
    if (rmp.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QString cfg= QString("%1@%2\n").arg(offset).arg(path);
        QTextStream rmpFile(&rmp);
        rmpFile << cfg;
        rmpFile.flush();
        rmp.close();
        setState(ok);
        return true;
    } else {
        return false;
    }
}

void Rmp::del() {
    QFile(path).remove();
}

