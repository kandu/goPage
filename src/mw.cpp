#define _MW
#define _TRAY

#include <QMenu>
#include <QFileDialog>
#include <QFileInfo>
#include <QSpinBox>
#include <QKeyEvent>
#include <QDebug>
#include <limits>
#include "dlgMigFrom.hpp"
#include "dlgMigAll.hpp"
#include "tray.hpp"
#include "mw.hpp"
#include "ui_about.h"

#ifdef Q_OS_MACOS
    #include "noNap.h"
#endif

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


BookOffset::BookOffset(QString const & book, QWidget* parent)
    : QSpinBox(parent), book(book)
{
    connect(
        this,
        SIGNAL(valueChanged(int)),
        this,
        SLOT(changed(int)));
}

void BookOffset::changed(int i) {
    emit offsetChanged(book, i);
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

    Ui::dlgAbout dlgAboutUi;
    dlgAbout= new QDialog(this, Qt::WindowCloseButtonHint);
    dlgAboutUi.setupUi(dlgAbout);

    QAction *aboutAction = ui.menubar->addAction(tr("&About"));
    connect(aboutAction, SIGNAL(triggered()), dlgAbout, SLOT(exec()));

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
        ui.actionMigFrom,
        SIGNAL(triggered(bool)),
        this,
        SLOT(migFrom()));

    connect(
        ui.actionMigAll,
        SIGNAL(triggered(bool)),
        this,
        SLOT(migAll()));

    connect(
        &delKeyFilter,
        SIGNAL(del(QList<int> const &)),
        this,
        SLOT(delRows(QList<int> const &)));

    connect(
        ui.tableWidget,
        SIGNAL(cellClicked(int, int)),
        this,
        SLOT(cellClicked(int, int)));

    trayIcon= new QSystemTrayIcon(this);
    trayMenu= new QMenu(this);
    setupTray(this, trayMenu, trayIcon);

    QHeaderView* header= ui.tableWidget->horizontalHeader();
    header->setStretchLastSection(true);

    manager= new Manager(this);
    connect(
        manager,
        SIGNAL(rmpLoaded(Rmp const &)),
        this,
        SLOT(appendBook(Rmp const &))
        );
    manager->loadRmps();

    connect(
        manager,
        SIGNAL(rmpUpdated(Rmp const &)),
        this,
        SLOT(refreshBook(Rmp const &))
        );

    connect(
        ui.actionIgnore_dangling_ebf,
        &QAction::toggled,
        manager,
        &Manager::setIgnoreDanglingEbf);
    manager->setIgnoreDanglingEbf(ui.actionIgnore_dangling_ebf->isChecked());

    // connect after all rmps are loaded
    connect(
        ui.tableWidget,
        SIGNAL(itemChanged(QTableWidgetItem*)),
        this,
        SLOT(pathChanged(QTableWidgetItem*)));

    ui.tableWidget->installEventFilter(&delKeyFilter);

#ifdef Q_OS_MACOS
    noNap();
#endif
}

void Mw::appendBook(Rmp const & rmp) {
    QTableWidget* qmlTable= ui.tableWidget;
    int row= qmlTable->rowCount();
    qmlTable->insertRow(row);

    BookOffset* offsetBox= new BookOffset(rmp.getName(), qmlTable);
    offsetBox->setRange(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max());
    offsetBox->setValue(rmp.getOffset());
    qmlTable->setCellWidget(row, 1, offsetBox);

    auto name= rmp.getName();
    auto nameItem= new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);
    auto offsetItem= new QTableWidgetItem(rmp.getOffset());
    auto pathItem= new QTableWidgetItem(rmp.getPath());

    qmlTable->setItem(row, 0, nameItem);
    qmlTable->setItem(row, 1, offsetItem);
    qmlTable->setItem(row, 2, pathItem);

    items.insert(name, nameItem);

    connect(
        offsetBox,
        SIGNAL(offsetChanged(QString const &, int)),
        this,
        SLOT(offsetChanged(QString const &, int)));
}

void Mw::refreshBook(Rmp const & rmp) {
    auto item= items[rmp.getName()];
    int row= item->row();
    ui.tableWidget->item(row, 2)->setText(rmp.getPath());
    ((BookOffset*)ui.tableWidget->cellWidget(row, 1))->setValue(rmp.getOffset());
}

void Mw::delRows(QList<int> const & _rows) {
    // delete from bottom to top
    auto rows= _rows;
    std::sort(rows.begin(), rows.end());

    QListIterator<int> i(rows);
    i.toBack();
    while(i.hasPrevious()) {
        int row= i.previous();
        auto book= ui.tableWidget->item(row, 0)->text();
        manager->remove(book);
        items.remove(book);
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

    if (fd.exec()) {
        QStringList fileNames= fd.selectedFiles();
        QListIterator<QString> i(fileNames);
        while (i.hasNext()) {
            auto path= i.next();
            manager->addDict(path);
        }
    }
}

void Mw::importRmp() {
    QFileDialog fd(this);
    fd.setFileMode(QFileDialog::ExistingFiles);
    fd.setNameFilter(QObject::tr("rmp files (*.rmp)"));

    if (fd.exec()) {
        QStringList fileNames= fd.selectedFiles();
        QListIterator<QString> i(fileNames);
        while (i.hasNext()) {
            auto path= i.next();
            manager->addRmp(path, ui.actionRmp_update_offset->isChecked());
        }
    }
}

void Mw::pathChanged(QTableWidgetItem* item) {
    int row= item->row();
    int column= item->column();
    if (column == 2) {
        QString book= ui.tableWidget->item(row, 0)->text();
        QString path= ui.tableWidget->item(row, 2)->text();
        manager->updatePath(book, path);
    }
}

void Mw::offsetChanged(QString const & book, int offset) {
    manager->updateOffset(book, offset);
}

void Mw::cellClicked(int row, int column) {
    if (column == 2) {
        QFileDialog fd(this);
        fd.setFileMode(QFileDialog::ExistingFile);
        fd.setNameFilters(QStringList()
            << QObject::tr("pdf files (*.pdf)")
            << QObject::tr("djvu files (*.djvu *.djv)")
            << QObject::tr("Any files (*)"));

        if (fd.exec()) {
            auto fileNames= fd.selectedFiles();
            auto newPath= fileNames[0];
            auto book= ui.tableWidget->item(row, 0)->text();
            ui.tableWidget->item(row, column)->setText(newPath);
            manager->updatePath(book, newPath);
        }
    }
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
#ifdef Q_OS_MAC
#else
    switch (reason) {
        case QSystemTrayIcon::Trigger:
            toggle();
            break;
        default:
            toggle();
    }
#endif
}

void Mw::toggle() {
    this->setVisible(! this->isVisible());
}

void Mw::quit() {
    QApplication::quit();
}

void Mw::migFrom() {
    DlgMigFrom dlgMigFrom(this);
    if (dlgMigFrom.exec()) {
        auto fromPath= dlgMigFrom.getFromDir();
        auto toPath= dlgMigFrom.getToDir();
        if (!fromPath.isEmpty() && !toPath.isEmpty()) {
            QDir fromDir(fromPath);
            QDir toDir(toPath);
            bool isMoveFile= dlgMigFrom.isMoveFile();
            manager->moveFrom(fromDir, toDir, isMoveFile);
        }
    }
}

void Mw::migAll() {
    DlgMigAll dlgMigAll(this);
    if (dlgMigAll.exec()) {
        auto path= dlgMigAll.getDir();
        if (!path.isEmpty()) {
            QDir dir(path);
            bool isMoveFile= dlgMigAll.isMoveFile();
            manager->moveAll(dir, isMoveFile);
        }
    }
}

