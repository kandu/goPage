#define _MONITOR

#include <QRegExp>
#include <QDebug>
#include "monitor.hpp"

QRegExp ebf("ebf://(\\d+)((\\+|-)(\\d+))?@(.*)");

Monitor::Monitor(QClipboard* clipboard, QObject* parent)
    : QObject(parent)
{
    this->clipboard= clipboard;
    connect(
        clipboard,
        SIGNAL(dataChanged()),
        this,
        SLOT(monitor())
    );
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

