#ifndef _MANAGER_H
#define _MANAGER_H

#include "config.h"
#include <QObject>
#include <QApplication>
#include <QMap>
#include <QDir>
#include "rmp.hpp"
#include "monitor.hpp"
#include "invoker.hpp"

#ifdef _MANAGER
    #undef EXTERN
    #define EXTERN
#endif

class Manager: public QObject {
    Q_OBJECT
    public:
        Manager(QObject* parent= 0);
        void loadRmps();
        QMap<QString, Rmp> const & getRmps();
    signals:
        void newRmp(Rmp const &);
    private:
        Monitor* monitor;
        QMap<QString, Rmp> rmps;
        QDir rmpDir;
    private slots:
        void query(QString book, int page);
};

#undef EXTERN
#define EXTERN extern
#endif // _MANAGER_H

