#ifndef _DLGMIGFROM_H
#define _DLGMIGFROM_H

#include "config.h"
#include <QDialog>
#include "ui_migrateFrom.h"

#ifdef _DLGMIGFROM
    #undef EXTERN
    #define EXTERN
#endif

class DlgMigFrom: public QDialog {
    Q_OBJECT
    public:
        DlgMigFrom(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    private:
        Ui::dlgMigFrom ui;
};

#undef EXTERN
#define EXTERN extern
#endif // _DLGMIGFROM_H

