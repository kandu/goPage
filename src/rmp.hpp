#ifndef _RMP_H
#define _RMP_H

#include "config.h"
#include <QDir>
#include <QApplication>
#include <QClipboard>

#ifdef _RMP
    #undef EXTERN
    #define EXTERN
#endif

class Rmp {
    public:
        enum State { ok, notExist, formatErr };

    public:
        Rmp(QString const & name);

        QString const & getName() const;

        State load(QDir const & dir);
        bool save(QDir const & dir);
        void del();

        bool isRmpExist();
        bool isDictExist();

        int getOffset() const;
        void setOffset(int offset);

        QString const & getPath() const;
        void setPath(QString const & path);

        State getState() const;
        void setState(State state);
    private:
        State state;
        QString name;
        bool rmpExist= false;
        bool dictExist= false;

        // rmp cfg
        int offset= 0;
        QString path;
};

#undef EXTERN
#define EXTERN extern
#endif // _RMP_H

