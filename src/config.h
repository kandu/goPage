#ifndef _CONFIG_H
#define _CONFIG_H

#undef EXTERN
#define EXTERN extern

#define UNUSED(param) /**/

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define USERCONFDIR ".config/goPage"
#define READERCFG "reader.cfg"

#define SYS_READERCFG (SYSCONFDIR "/" "goPage/" READERCFG)
#define USER_READERCFG (USERCONFDIR "/" READERCFG)

#endif // _CONFIG_H

