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
    enum State { ok, notExist, formatErr };

    Q_OBJECT
    public:
        Rmp(QString const & book, QObject* parent= 0);

        State load(QString const & path);
        bool save(QString const & path);

        bool ifExist();
        void setExist(bool exist);

        int getOffset();
        void setOffset(int offset);

        QString const & getPath();
        void setPath(QString const & path);

        State getState();
        void setState(State state);
    private:
        State state;
        QString book;
        bool exist= false;
        int offset= 0;
        QString path= "";
};

#undef EXTERN
#define EXTERN extern
#endif // _RMP_H

