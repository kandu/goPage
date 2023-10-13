#define _INVOKER

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QStringList>
#include <QMessageBox>
#include <QDebug>
#include "invoker.hpp"
#include "tray.hpp"

QRegExp cfgFmt("^ *([^ ]*) *, *([^/]*)");

Invoker::Invoker(QObject* parent)
    : QObject(parent)
{
    QFile readerCfgFile(QDir::homePath() + "/" + USERCONFDIR + "/" READERCFG);
    if (readerCfgFile.open(QIODevice::ReadOnly)) {
        QTextStream cfg(&readerCfgFile);
        while (!cfg.atEnd()) {
            if (cfgFmt.exactMatch(cfg.readLine())) {
                readerCfg.insert(
                    cfgFmt.cap(1).trimmed(),
                    cfgFmt.cap(2).trimmed());
            }
        }
        readerCfgFile.close();
    }

    return;
}

void Invoker::open(QString path, QString page) {
#ifdef Q_OS_MACOS
    auto param= QString{"file://%1#page=%2"}.arg(path).arg(page);
    auto fx= new QProcess();
    fx->connect(
        fx,
        qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
        [fx]() { fx->deleteLater(); }
        );
    fx->start("/Applications/Firefox.app/Contents/MacOS/firefox", QStringList{} << param);
#else
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

        xdg_mime.start(
            QString("xdg-mime"),
            QStringList() << "query" << "default" << filetype);
        if (xdg_mime.waitForFinished()) {

            QString defaultApp= QTextStream(xdg_mime.readAllStandardOutput()).readLine();

            QString reader_app;
            QString reader_opt;
            bool found= false;
            QMapIterator<QString, QString> i(readerCfg);
            while (i.hasNext() && !found) {
                i.next();
                if (QRegExp(i.key(), Qt::CaseInsensitive).indexIn(defaultApp) != -1) {
                    reader_app= i.key();
                    reader_opt= i.value();
                    found= true;
                }
            }

            if (!found) {
                mb_reader.exec();
            } else {
                auto opts= reader_opt.split(QRegExp("\\s+"));
                opts.replaceInStrings("%1", path);
                opts.replaceInStrings("%2", page);
                QProcess::startDetached(
                    reader_app,
                    opts);
            }
        } else {
            mb_reader.exec();
        }
    } else {
        mb_filetype.exec();
    }
#endif
}

