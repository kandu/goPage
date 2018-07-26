#define _DLGMIGFROM

#include <QFileDialog>
#include <QDebug>
#include "dlgMigFrom.hpp"

DlgMigFrom::DlgMigFrom(QWidget* parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
{
    ui.setupUi(this);
    connect(
        ui.selector_from,
        SIGNAL(clicked()),
        this,
        SLOT(selectFromDir())
        );
    connect(
        ui.selector_to,
        SIGNAL(clicked()),
        this,
        SLOT(selectToDir())
        );
}


QString DlgMigFrom::getFromDir() {
    return ui.lineEdit_from->text();
}

QString DlgMigFrom::getToDir() {
    return ui.lineEdit_to->text();
}

bool DlgMigFrom::isMoveFile() {
    return ui.checkBox->isChecked();
}

void DlgMigFrom::selectFromDir() {
    QFileDialog fd(this);
    fd.setFileMode(QFileDialog::Directory);
    fd.setOption(QFileDialog::ShowDirsOnly, true);
    if (fd.exec()) {
        auto dir= fd.selectedFiles()[0];
        ui.lineEdit_from->setText(dir);
    }
}

void DlgMigFrom::selectToDir() {
    QFileDialog fd(this);
    fd.setFileMode(QFileDialog::Directory);
    fd.setOption(QFileDialog::ShowDirsOnly, true);
    if (fd.exec()) {
        auto dir= fd.selectedFiles()[0];
        ui.lineEdit_to->setText(dir);
    }
}

