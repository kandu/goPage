#define _MANAGER

#include <QRegExp>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include "manager.hpp"

Manager::Manager(QObject* parent)
    : QObject(parent)
{
    monitor= new Monitor(QApplication::clipboard(), this);
    connect(
        monitor,
        SIGNAL(entry(QString, int)),
        this,
        SLOT(query(QString, int))
        );
    QDir rmpDir(QDir::homePath() + USERCONFDIR + "/rmp");
    rmpDir.setNameFilters(QStringList() << "*.rmp");
    rmpDir.setFilter(QDir::Files);
    qDebug() << rmpDir.entryList();
}

void Manager::query(QString book, int page) {
    Rmp* rmp;
    auto rmp_i= rmps.find(book);
    if (rmp_i != rmps.end()) {
        rmp= rmp_i.value();
    } else {
        rmp= new Rmp(book, this);
        rmps.insert(book, rmp);
    }

    if (rmp->ifExist()) {
        Invoker invoker(this);
        invoker.open(rmp->getPath(), QString::number(page + rmp->getOffset()));
    } else {
        QMessageBox(
            QMessageBox::Warning,
            QObject::tr("error"),
            QObject::tr("dict doesn't exist: ") + rmp->getPath()
        ).exec();
    }
}

