#include <Foundation/NSProcessInfo.h>

void noNap() {
    id m_activity=
        [[NSProcessInfo processInfo]
            beginActivityWithOptions:NSActivityBackground
            reason:@"watch clipboard"];
}

