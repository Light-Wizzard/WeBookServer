#ifndef CUTELOGGER_GLOBAL_H
#define CUTELOGGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CUTELOGGER_LIBRARY)
#  define CUTELOGGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CUTELOGGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CUTELOGGER_GLOBAL_H
/******************************* End of File *********************************/