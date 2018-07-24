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
            emit newRmp(rmp);
        }
    }
}

QMap<QString, Rmp> const & Manager::getRmps() {
    return rmps;
}

void Manager::query(QString book, int page) {
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

