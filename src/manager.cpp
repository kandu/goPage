#define _MANAGER

#include <QRegExp>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include "manager.hpp"

Manager::Manager(QObject* parent)
    : QObject(parent)
{
    pdfViewer= new PdfViewer(this);
    monitor= new Monitor(QApplication::clipboard(), this);
    connect(
        monitor,
        SIGNAL(entry(QString, int)),
        this,
        SLOT(query(QString, int))
        );
}

void Manager::query(QString book, int page) {
    qDebug() << book << page;
    Rmp* rmp;
    auto rmp_i= rmps.find(book);
    if (rmp_i != rmps.end()) {
        rmp= rmp_i.value();
    } else {
        rmp= new Rmp(book, this);
        rmps.insert(book, rmp);
    }

    if (QFile(rmp->path()).exists()) {
        QProcess pdf;
        QString app= pdfViewer->app();
        QStringList arg= QStringList()
            << rmp->path()
            << pdfViewer->opt_page ()
            << QString::number(page + rmp->offset());

        pdf.startDetached(app, arg);
    } else {
        QMessageBox(
            QMessageBox::Warning,
            "錯誤",
            "無法找到詞典文件：" + rmp->path()
        ).exec();
    }
}

