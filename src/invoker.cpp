#define _INVOKER

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QMessageBox>
#include <QDebug>
#include "invoker.hpp"

QRegExp cfgFmt("^ *([^ ]*) *, *([^/]*)");

Invoker::Invoker(QObject* parent)
    : QObject(parent)
{
    QFile readerCfgFile(QDir::homePath() + USERCONFDIR + "/reader.cfg");
    if (readerCfgFile.open(QIODevice::ReadOnly)) {
        QTextStream cfg(&readerCfgFile);
        while (!cfg.atEnd()) {
            if (cfgFmt.exactMatch(cfg.readLine())) {
                readerCfg.insert(cfgFmt.cap(1), cfgFmt.cap(2));
            }
        }
        readerCfgFile.close();
    }

    return;
}

void Invoker::open(QString path, QString page) {
    QMessageBox mb_filetype(
        QMessageBox::Critical,
        QObject::tr("error"),
        QObject::tr("unknown filetype!"));

    xdg_mime.start("xdg-mime",
        QStringList()
            << "query"
            << "filetype"
            << path);
    if (xdg_mime.waitForFinished()) {
        QString filetype= QTextStream(xdg_mime.readAllStandardOutput()).readLine();

        QMessageBox mb_reader(
            QMessageBox::Critical,
            QObject::tr("error"),
            QString(QObject::tr("no default reader for filetype %1.")).arg(filetype));

        xdg_mime.start(QString("xdg-mime query default %1").arg(filetype));
        if (xdg_mime.waitForFinished()) {

            QString defaultApp= QTextStream(xdg_mime.readAllStandardOutput()).readLine();

            QString reader_app;
            QString reader_opt;
            bool found= false;
            QMapIterator<QString, QString> i(readerCfg);
            while (i.hasNext() && !found) {
                i.next();
                if (QRegExp(i.key()).indexIn(defaultApp) != -1) {
                    reader_app= i.key();
                    reader_opt= i.value();
                    found= true;
                }
            }

            if (!found) {
                mb_reader.exec();
            } else {
                QProcess::startDetached(reader_app
                    + " "
                    + reader_opt.arg(path).arg(page));
            }
        } else {
            mb_reader.exec();
        }
    } else {
        mb_filetype.exec();
    }
}
