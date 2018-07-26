#define _MANAGER

#include <QRegExp>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include "manager.hpp"

Manager::Manager(QObject* parent)
    : QObject(parent)
    , rmpDir(QDir::homePath() + USERCONFDIR + "/rmp")
{
    monitor= new Monitor(QApplication::clipboard(), this);
    connect(
        monitor,
        SIGNAL(entry(QString, int)),
        this,
        SLOT(query(QString, int))
        );
    rmpDir.setNameFilters(QStringList() << "*.rmp");
    rmpDir.setFilter(QDir::Files);
}

void Manager::loadRmps() {
    QListIterator<QString> i(rmpDir.entryList());
    while (i.hasNext()) {
        QString rmpFileName= i.next();
        QFileInfo fi(rmpDir.filePath(rmpFileName));
        Rmp rmp(fi.baseName());
        if (rmp.load(rmpDir) == Rmp::ok) {
            rmps.insert(rmp.getName(), rmp);
            emit rmpLoaded(rmp);
        }
    }
}

QMap<QString, Rmp> const & Manager::getRmps() {
    return rmps;
}

void Manager::query(QString const & book, int page) {
    Rmp rmp(book);
    auto rmp_i= rmps.find(book);
    if (rmp_i != rmps.end()) {
        rmp= rmp_i.value();
    } else if (rmp.load(rmpDir) == Rmp::ok) {
        rmps.insert(book, rmp);
    }

    if (rmp.isDictExist()) {
        Invoker invoker(this);
        invoker.open(rmp.getPath(), QString::number(page + rmp.getOffset()));
    } else {
        QMessageBox(
            QMessageBox::Warning,
            QObject::tr("error"),
            QObject::tr("dict doesn't exist: ") + rmp.getPath()
        ).exec();
    }
}

void Manager::addDict(QString const & path) {
    QFileInfo fi(path);
    auto name= fi.baseName();
    if (fi.exists()) {
        if (rmps.contains(name)) {
            auto rmp= rmps.find(name).value();
            rmp.setPath(path);
            rmp.save(rmpDir);
            rmps.insert(name, rmp);
            emit(rmpUpdated(rmp));
        } else {
            Rmp rmp(name);
            rmp.setPath(path);
            rmp.save(rmpDir);
            rmps.insert(name, rmp);
            emit(rmpLoaded(rmp));
        }
    }
}

void Manager::addRmp(QString const & path) {
    QFileInfo fi(path);
    auto name= fi.baseName();
    if (fi.exists() && !rmps.contains(name)) {
        Rmp rmp(name);
        if (rmp.load(fi.absoluteDir()) == Rmp::ok) {
            rmp.save(rmpDir);
            rmps.insert(name, rmp);
            emit(rmpLoaded(rmp));
        }
    }
}

void Manager::updateOffset(QString const & book, int offset) {
    auto i= rmps.find(book);
    if (i != rmps.end()) {
        auto rmp= i.value();
        rmp.setOffset(offset);
        rmp.save(rmpDir);
        rmps.insert(book, rmp);
    }
}

void Manager::updatePath(QString const & book, QString const & path) {
    auto i= rmps.find(book);
    if (i != rmps.end()) {
        auto rmp= i.value();
        rmp.setPath(path);
        rmp.save(rmpDir);
        rmps.insert(book, rmp);
    }
}

void Manager::update(QString const & book, int offset, QString const & path) {
    auto i= rmps.find(book);
    if (i != rmps.end()) {
        auto rmp= i.value();
        rmp.setOffset(offset);
        rmp.setPath(path);
        rmp.save(rmpDir);
        rmps.insert(book, rmp);
    }
}

void Manager::remove(QString const & book) {
    auto rmp_i= rmps.find(book);
    if (rmp_i != rmps.end()) {
        Rmp rmp= rmp_i.value();
        rmps.remove(book);
        QFile(rmpDir.filePath(book + ".rmp")).remove();
    }
}
