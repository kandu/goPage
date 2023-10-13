TEMPLATE = app
TARGET = goPage
DEPENDPATH += .
INCLUDEPATH += .

QT += core gui widgets
CONFIG += c++14

DESTDIR= build

isEmpty(LUPDATE) {
    LUPDATE= lupdate
}
isEmpty(LRELEASE) {
    LRELEASE= lrelease
}
system($$LUPDATE goPage.pro && $$LRELEASE goPage.pro)

isEmpty(PREFIX) {
    PREFIX= "/usr/local"
}

isEmpty(SYSCONFDIR) {
    SYSCONFDIR= $$sprintf("%1/%2", $$PREFIX, "etc")
}

DEFINES += PREFIX=\\\"$${PREFIX}\\\"
DEFINES += SYSCONFDIR=\\\"$${SYSCONFDIR}\\\"


!equals(debug, n) {
    CONFIG += debug
}

# Input

FORMS += goPage.ui migrateFrom.ui migrateAll.ui about.ui
RESOURCES += res/goPage.qrc

HEADERS += config.h tray.hpp mw.hpp monitor.hpp rmp.hpp invoker.hpp manager.hpp dlgMigFrom.hpp dlgMigAll.hpp
SOURCES += init.cpp goPage.cpp mw.cpp monitor.cpp rmp.cpp invoker.cpp manager.cpp dlgMigFrom.cpp dlgMigAll.cpp

#Temp Dir
OBJECTS_DIR = build/obj
UI_DIR = build/ui
MOC_DIR = build/moc
RCC_DIR = build/rcc


TRANSLATIONS += res/zh_CN.ts
TRANSLATIONS += res/zh_HK.ts
TRANSLATIONS += res/zh_TW.ts

etc.path= $$sprintf("%1/goPage", $$SYSCONFDIR)
etc.files += reader.cfg

bin.path= $$sprintf("%1/bin", $$PREFIX)
bin.files += $$sprintf("%1/%2", $$DESTDIR, $$TARGET)


xdg_icon.path= $$sprintf("%1/share/icons/hicolor/256x256/apps", $$PREFIX)
xdg_icon.files += res/goPage.png
xdg_desktop.path = $$sprintf("%1/share/applications", $$PREFIX)
xdg_desktop.files += res/goPage.desktop

INSTALLS += bin etc

macx {
    HEADERS += noNap.h
    OBJECTIVE_SOURCES += noNap.mm
    LIBS += -framework Foundation
}

!win32&!macx {
    INSTALLS += xdg_icon xdg_desktop
}

