TEMPLATE = app
TARGET = goPage
DEPENDPATH += .
INCLUDEPATH += .

DESTDIR= build

system(lupdate goPage.pro && lrelease goPage.pro)

QMAKE_CXXFLAGS += -std=c++11

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

HEADERS += config.h mw.hpp monitor.hpp rmp.hpp invoker.hpp manager.hpp dlgMigFrom.hpp dlgMigAll.hpp
SOURCES += init.cpp goPage.cpp mw.cpp monitor.cpp rmp.cpp invoker.cpp manager.cpp dlgMigFrom.cpp dlgMigAll.cpp

#Temp Dir
OBJECTS_DIR = build/obj
UI_DIR = build/ui
MOC_DIR = build/moc
RCC_DIR = build/rcc


TRANSLATIONS += res/zh_CN.ts
TRANSLATIONS += res/zh_TW.ts

etc.path= $$sprintf("%1/goPage", $$SYSCONFDIR)
etc.files += reader.cfg

bin.path= $$sprintf("%1/bin", $$PREFIX)
bin.files += $$sprintf("%1/%2", $$DESTDIR, $$TARGET)


INSTALLS += bin etc

