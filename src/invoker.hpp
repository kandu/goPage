#ifndef _INVOKER_H
#define _INVOKER_H

#include "config.h"
#include <QObject>
#include <QProcess>
#include <QMap>

#ifdef _INVOKER
    #undef EXTERN
    #define EXTERN
#endif

class Invoker: public QObject {
    Q_OBJECT
    public:
        Invoker(QObject* parent= 0);
        void open(QString path, QString page);
    private:
        QProcess xdg_mime;
    private:
        QMap<QString, QString> readerCfg;
};

#undef EXTERN
#define EXTERN extern
#endif // _INVOKER_H

