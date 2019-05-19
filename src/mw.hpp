#ifndef _MW_H
#define _MW_H

#include "config.h"
#include <QMainWindow>
#include <QSpinBox>
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

class BookOffset: public QSpinBox {
    Q_OBJECT
    public:
        BookOffset(QString const & book, QWidget* parent= 0);
    signals:
        void offsetChanged(QString const & book, int i);
    private slots:
        void changed(int i);
    private:
        QString book;
};

class Mw: public QMainWindow {
    Q_OBJECT
    public:
        Mw(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    private:
        Manager* manager;
        QMenu* trayMenu;
        Ui::goPage ui;
        DelKeyFilter delKeyFilter;
        QMap<QString, QTableWidgetItem*> items;
        QDialog* dlgAbout;
    public slots:
        void appendBook(Rmp const & rmp);
        void refreshBook(Rmp const & rmp);
        void delRows(QList<int> const &);
        void importDict();
        void importRmp();
        void pathChanged(QTableWidgetItem*);
        void offsetChanged(QString const & book, int offset);
    private slots:
        void cellClicked(int row, int column);
        void removeSelected();
        void toggle();
        void trayActive(QSystemTrayIcon::ActivationReason);
        void quit();
        void migFrom();
        void migAll();
};

#undef EXTERN
#define EXTERN extern
#endif // _MW_H

