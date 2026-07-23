// Precompiled header for the Mumble client object library.
// Extends the common STL + QtCore PCH with the QtGui/QtWidgets base headers that
// dominate the client build trace (MainWindow and friends pull them into many TUs).
#ifndef MUMBLE_CLIENT_PCH_H_
#define MUMBLE_CLIENT_PCH_H_

#include "../mumble_common_pch.h"

#ifdef __cplusplus

#include <QtGui/QIcon>
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>

#endif // __cplusplus

#endif // MUMBLE_CLIENT_PCH_H_
