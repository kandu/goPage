#ifndef _MONITOR_H
#define _MONITOR_H

#include <QObject>
#include <QClipboard>

#ifdef _MONITOR
    #undef EXTERN
    #define EXTERN
#endif

class Monitor: public QObject {
    Q_OBJECT
    public:
        Monitor(QClipboard* clipboard, QObject* parent= 0);
    public slots:
        void monitor(void);
    signals:
        void entry(QString book, int page);
    private:
        QClipboard* clipboard;
};

#undef EXTERN
#define EXTERN extern
#endif // _MONITOR_H

