#define _MW

#include <QMenu>
#include <QFileDialog>
#include <QDebug>
#include "mw.hpp"

void setupTray(QWidget* mw, QMenu* menu, QSystemTrayIcon* trayIcon)
{
    QAction* toggle= new QAction(trayIcon);
    toggle->setText(QObject::tr("toggle"));

    QAction* quit= new QAction(trayIcon);
    quit->setText(QObject::tr("quit"));

    menu->addAction(toggle);
    menu->addSeparator();
    menu->addAction(quit);
    trayIcon->setContextMenu(menu);
    trayIcon->setIcon(QIcon(":/images/goPage.png"));

    QObject::connect(
        trayIcon,
        SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        mw,
        SLOT(trayActive(QSystemTrayIcon::ActivationReason))
    );

    QObject::connect(
        toggle,
        SIGNAL(triggered(bool)),
        mw,
        SLOT(toggle())
    );

    QObject::connect(
        quit,
        SIGNAL(triggered(bool)),
        mw,
        SLOT(quit())
    );

    trayIcon->show();
}


Mw::Mw(QWidget* parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);

    connect(
        ui.actionTray,
        SIGNAL(triggered(bool)),
        this,
        SLOT(toggle()));

    connect(
        ui.actionQuit,
        SIGNAL(triggered(bool)),
        this,
        SLOT(quit()));

    connect(
        ui.actionDict,
        SIGNAL(triggered(bool)),
        this,
        SLOT(importDict()));

    connect(
        ui.actionRmp,
        SIGNAL(triggered(bool)),
        this,
        SLOT(importRmp()));

    trayIcon= new QSystemTrayIcon(this);
    trayMenu= new QMenu(this);
    setupTray(this, trayMenu, trayIcon);

    manager= new Manager(this);

    return;
}

void Mw::importDict() {
    QFileDialog fd(this);
    fd.setFileMode(QFileDialog::ExistingFiles);
    fd.setNameFilters(QStringList()
        << QObject::tr("pdf files (*.pdf)")
        << QObject::tr("djvu files (*.djvu *.djv)")
        << QObject::tr("Any files (*)"));

    QStringList fileNames;
    if (fd.exec()) {
        fileNames = fd.selectedFiles();
        qDebug() << fileNames;
    } 
}

void Mw::importRmp() {
    qDebug() << "importRmp";
}

void Mw::trayActive(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        case QSystemTrayIcon::Trigger:
            toggle();
            break;
        default:
            toggle();
    }
}

void Mw::toggle() {
    this->setVisible(! this->isVisible());
}

void Mw::quit() {
    QApplication::quit();
}

