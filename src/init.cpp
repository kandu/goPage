#include <QFile>
#include <QDir>
#include "init.h"

void init() {
    QDir::home().mkpath(USERCONFDIR "/rmp");
    auto userCfgPath= QDir::home().filePath(USER_READERCFG);
    if (!QFile(userCfgPath).exists()) {
        QFile(SYS_READERCFG).copy(userCfgPath);
    }
}

