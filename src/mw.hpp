#ifndef _MW_H
#define _MW_H

#include "config.h"
#include <QMainWindow>
#include <QSystemTrayIcon>
#include "manager.hpp"
#include "ui_goPage.h"

#ifdef _MW
    #undef EXTERN
    #define EXTERN
#endif

class DelKeyFilter: public QObject {
    Q_OBJECT
    public:
        bool eventFilter(QObject *obj, QEvent *event);
    signals:
        void del(QList<int> const &);
};

class Mw: public QMainWindow {
    Q_OBJECT
    public:
        Mw(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    private:
        Manager* manager;
        QSystemTrayIcon* trayIcon;
        QMenu* trayMenu;
        Ui::goPage ui;
        DelKeyFilter delKeyFilter;
    public slots:
        void appendBook(Rmp const & rmp);
        void delRows(QList<int> const &);
        void importDict();
        void importRmp();
    private slots:
        void removeSelected();
        void toggle();
        void trayActive(QSystemTrayIcon::ActivationReason);
        void quit();
};

#undef EXTERN
#define EXTERN extern
#endif // _MW_H

