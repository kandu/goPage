#define _DLGMIGALL

#include <QFileDialog>
#include <QDebug>
#include "dlgMigAll.hpp"

DlgMigAll::DlgMigAll(QWidget* parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
{
    ui.setupUi(this);
    connect(
        ui.toolButton,
        SIGNAL(clicked()),
        this,
        SLOT(selectDir())
        );
}

QString DlgMigAll::getDir() {
    return ui.lineEdit->text();
}

bool DlgMigAll::isMoveFile() {
    return ui.checkBox->isChecked();
}

void DlgMigAll::selectDir() {
    QFileDialog fd(this);
    fd.setFileMode(QFileDialog::Directory);
    fd.setOption(QFileDialog::ShowDirsOnly, true);
    if (fd.exec()) {
        auto dir= fd.selectedFiles()[0];
        ui.lineEdit->setText(dir);
    }
}

