#ifndef _DLGMIGALL_H
#define _DLGMIGALL_H

#include "config.h"
#include <QDialog>
#include "ui_migrateAll.h"

#ifdef _DLGMIGALL
    #undef EXTERN
    #define EXTERN
#endif

class DlgMigAll: public QDialog {
    Q_OBJECT
    public:
        DlgMigAll(QWidget* parent = 0, Qt::WindowFlags flags = 0);
        QString getDir();
        bool isMoveFile();
    public slots:
        void selectDir();
    private:
        Ui::dlgMigAll ui;
};

#undef EXTERN
#define EXTERN extern
#endif // _DLGMIGALL_H

