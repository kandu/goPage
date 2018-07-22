#ifndef _RMP_H
#define _RMP_H

#include "config.h"
#include <QObject>
#include <QApplication>
#include <QClipboard>

#ifdef _RMP
    #undef EXTERN
    #define EXTERN
#endif

class Rmp: public QObject {
    Q_OBJECT
    public:
        Rmp(QString book, QObject* parent= 0);
        Rmp& fromFile();
        bool exist();
        int offset();
        QString path();
    private:
        QString book;
        bool _exist= false;
        int _offset= 0;
        QString _path= "";
};

#undef EXTERN
#define EXTERN extern
#endif // _RMP_H

