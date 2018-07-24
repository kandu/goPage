#define _MW

#include <QMenu>
#include <QFileDialog>
#include <QSpinBox>
#include <QKeyEvent>
#include <QDebug>
#include <limits>
#include "mw.hpp"

bool DelKeyFilter::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent *>(event);
        QTableWidget* tableWidget = static_cast<QTableWidget*>(obj);
        if (keyEvent->key() == Qt::Key_Delete
            && tableWidget->currentItem()->isSelected()
            && tableWidget->currentColumn() == 0)
        {
            auto items= tableWidget->selectedItems();
            QList<int> rows;

            QListIterator<QTableWidgetItem*> i(items);
            while (i.hasNext()) {
                auto item= i.next();
                if (item->column() == 0) {
                    rows.append(item->row());
                }
            }

            emit del(rows);
            return true;
        } else {
            return QObject::eventFilter(obj, event);
        }
    } else {
        return QObject::eventFilter(obj, event);
    }
}


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
        ui.actionImportDict,
        SIGNAL(triggered(bool)),
        this,
        SLOT(importDict()));

    connect(
        ui.actionImportRmp,
        SIGNAL(triggered(bool)),
        this,
        SLOT(importRmp()));

    connect(
        ui.actionRemoveSelected,
        SIGNAL(triggered(bool)),
        this,
        SLOT(removeSelected()));


    connect(
        &delKeyFilter,
        SIGNAL(del(QList<int> const &)),
        this,
        SLOT(delRows(QList<int> const &)));

    trayIcon= new QSystemTrayIcon(this);
    trayMenu= new QMenu(this);
    setupTray(this, trayMenu, trayIcon);

    QHeaderView* header= ui.tableWidget->horizontalHeader();
    header->setStretchLastSection(true);

    manager= new Manager(this);
    QObject::connect(
        manager,
        SIGNAL(rmpLoaded(Rmp const &)),
        this,
        SLOT(appendBook(Rmp const &))
        );
    manager->loadRmps();

    ui.tableWidget->installEventFilter(&delKeyFilter);
}

void Mw::appendBook(Rmp const & rmp) {
    QTableWidget* qmlTable= ui.tableWidget;
    int row= qmlTable->rowCount();
    qmlTable->insertRow(row);

    QSpinBox* offsetBox= new QSpinBox(qmlTable);
    offsetBox->setRange(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max());
    offsetBox->setValue(rmp.getOffset());
    qmlTable->setCellWidget(row, 1, offsetBox);

    auto nameItem= new QTableWidgetItem(rmp.getName());
    nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);
    auto offsetItem= new QTableWidgetItem(rmp.getOffset());
    auto pathItem= new QTableWidgetItem(rmp.getPath());

    qmlTable->setItem(row, 0, nameItem);
    qmlTable->setItem(row, 1, offsetItem);
    qmlTable->setItem(row, 2, pathItem);
}

void Mw::delRows(QList<int> const & _rows) {
    // delete from bottom to top
    auto rows= _rows;
    qSort(rows.begin(), rows.end());

    QListIterator<int> i(rows);
    i.toBack();
    while(i.hasPrevious()) {
        int row= i.previous();
        manager->remove(ui.tableWidget->item(row, 0)->text());
        ui.tableWidget->removeRow(row);
    }
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

void Mw::removeSelected() {
    auto items= ui.tableWidget->selectedItems();
    QList<int> rows;

    QListIterator<QTableWidgetItem*> i(items);
    while (i.hasNext()) {
        auto item= i.next();
        if (item->column() == 0) {
            rows.append(item->row());
        }
    }

    delRows(rows);
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

