#ifndef _PDFVIEWER_H
#define _PDFVIEWER_H

#include <QObject>
#include <QProcess>
#include <QMap>

#ifdef _PDFVIEWER
    #undef EXTERN
    #define EXTERN
#endif

class PdfViewer: public QObject {
    Q_OBJECT
    public:
        PdfViewer(QObject* parent= 0);
        QString app();
        QString opt_page();
    private:
        QProcess xdg_mime;
    private slots:
        void setViewer(int code, QProcess::ExitStatus status);
    private:
        QMap<QString, QString> pdfCfg;
        QString viewer_app= "";
        QString viewer_opt_page= "";
};

#undef EXTERN
#define EXTERN extern
#endif // _PDFVIEWER_H

