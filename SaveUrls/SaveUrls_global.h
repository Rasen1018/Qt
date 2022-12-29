#ifndef SAVEURLS_GLOBAL_H
#define SAVEURLS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SAVEURLS_LIBRARY)
#  define SAVEURLS_EXPORT Q_DECL_EXPORT
#else
#  define SAVEURLS_EXPORT Q_DECL_IMPORT
#endif

#endif // SAVEURLS_GLOBAL_H
