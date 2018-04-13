#define _PDFVIEWER

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include "pdfViewer.hpp"

QRegExp cfgFmt("^([^ ]*) *, *([^ ]*)");

PdfViewer::PdfViewer(QObject* parent)
    : QObject(parent)
{
    connect(
        &xdg_mime,
        SIGNAL(finished(int, QProcess::ExitStatus)),
        this,
        SLOT(setViewer (int, QProcess::ExitStatus))
        );
    xdg_mime.start("xdg-mime query default application/pdf");

    QFile pdfViewerCfg(QApplication::applicationDirPath() + "/pdfViewer.cfg");
    if (pdfViewerCfg.open(QIODevice::ReadOnly)) {
        QTextStream cfg(&pdfViewerCfg);
        while (!cfg.atEnd()) {
            if (cfgFmt.exactMatch(cfg.readLine())) {
                pdfCfg.insert(cfgFmt.cap(1), cfgFmt.cap(2));
            }
        }
        pdfViewerCfg.close();
    }

    return;
}

void PdfViewer::setViewer(int, QProcess::ExitStatus status) {
    bool found= false;
    if (status == QProcess::NormalExit) {
        QString mimeInfo= QTextStream(xdg_mime.readAllStandardOutput()).readLine();

        QMapIterator<QString, QString> i(pdfCfg);
        while (i.hasNext() && !found) {
            i.next();
            if (QRegExp(i.key()).indexIn(mimeInfo) != -1) {
                viewer_app= i.key();
                viewer_opt_page= i.value();
                found= true;
            }
        }
    }

    if (!found) {
        QMessageBox(
            QMessageBox::Critical,
            "錯誤",
            "無法找到默認 pdf 閱讀器。"
        ).exec();
    }
}

QString PdfViewer::app() {
    return viewer_app;
}

QString PdfViewer::opt_page() {
    return viewer_opt_page;
}

