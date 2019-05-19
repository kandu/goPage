#define _MONITOR

#include <QRegExp>
#include <QDebug>
#include <QMessageBox>
#include "monitor.hpp"
#include "tray.hpp"

QRegExp ebf("ebf://(\\d+)((\\+|-)(\\d+))?@(.*)");

Monitor::Monitor(QClipboard* clipboard, QObject* parent)
    : QObject(parent)
{
    this->clipboard= clipboard;
#ifdef Q_OS_MACOS
    latest= clipboard->text();

    timer.setTimerType(Qt::PreciseTimer);

    connect(
        &timer,
        SIGNAL(timeout()),
        this,
        SLOT(check())
    );
    timer.setInterval(200);
    timer.start();
#else
    connect(
        clipboard,
        SIGNAL(dataChanged()),
        this,
        SLOT(monitor())
    );
#endif
    return;
}

void Monitor::monitor(void)
{
    QString text= clipboard->text();
    if (ebf.exactMatch(text)) {
        emit entry(ebf.cap(5), ebf.cap(1).toInt() + ebf.cap(2).toInt());
    }
    return;
}

void Monitor::check(void)
{
    QString text= clipboard->text();

    if (text != latest) {
        latest= text;
        if (ebf.exactMatch(text)) {
            emit entry(ebf.cap(5), ebf.cap(1).toInt() + ebf.cap(2).toInt());
        }
    }
    return;
}
