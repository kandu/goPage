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
        void rmpLoaded(Rmp const &);
    private:
        Monitor* monitor;
        QMap<QString, Rmp> rmps;
        QDir rmpDir;
    public slots:
        void addDict(QString const & path);
        void updateOffset(QString const & book, int offset);
        void updatePath(QString const & book, QString const & path);
        void update(QString const & book, int offset, QString const & path);
        void remove(QString const & book);
        void query(QString const & book, int page);
};

#undef EXTERN
#define EXTERN extern
#endif // _MANAGER_H

