// Precompiled header for Mumble client/server object libraries.
// Added for build-performance optimization (vcperf analysis): the headers below
// are the most expensive + most widely included stable third-party/STL headers
// in the build trace. Precompiling them once avoids re-parsing per translation unit.
#ifndef MUMBLE_COMMON_PCH_H_
#define MUMBLE_COMMON_PCH_H_

#ifdef __cplusplus

// ---- C++ Standard Library (never change; Tier 1) ----
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

// ---- Qt Core (stable third-party; Tier 2) ----
#include <QtCore/QByteArray>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QMetaType>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QVariant>
#include <QtCore/QVector>

#endif // __cplusplus

#endif // MUMBLE_COMMON_PCH_H_
