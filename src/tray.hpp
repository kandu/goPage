#ifndef _TRAY_H
#define _TRAY_H

#include "config.h"
#include <QSystemTrayIcon>

#ifdef _TRAY
    #undef EXTERN
    #define EXTERN
#endif

EXTERN QSystemTrayIcon* trayIcon;

#undef EXTERN
#define EXTERN extern
#endif // _TRAY_H

