# Locking in Murmur

## Murmur's threads

Murmur makes use of multiple operating system threads.

Most logic in Murmur happens on the main thread.

Each virtual server in Murmur is represented by the
`Server` class. Most of the `Server` class runs on the
main thread, such as all control channel traffic (TLS).
Murmur's RPC systems usually run on a separate thread --
but all RPC methods are run on the main thread via
`ExecEvent` (see `Server.cpp`/`Server.h`).

The `Server` class is a subclass of `QThread`. The
thread that `Server` runs, is the *voice thread*.
This thread handles incoming UDP packets (ping and
voice), and rebroadcasts them as necessary.
The methods that run on the voice thread are:

- `void Server::run()`
- `void Server::processMsg(ServerUser *u, const char *data, int len)`
- `void Server::sendMessage(ServerUser *u, const char *data, int len, QByteArray &cache, bool force)`
- `bool Server::checkDecrypt(ServerUser *u, const char *encrypt, char *plain, unsigned int len)`

but these methods may call other methods
(such as `ChanACL::hasPermission()`) in order to
do their job.

In general, when speaking about the *voice thread*,
it refers to any code running in the `Server` methods
listed above.

The voice thread methods access various data in the
`Server` class to do their job. Besides being accessed
by the voice thread, this data is also read and written
to by the main thread.

To properly synchronize the access to the data shared
between the two threads, the `Server` class contains
a non-recursive reader-writer lock, `Server->qrwlVoiceThread`. 

This lock provides synchronization between the
main thread and the voice thread. These are the
only two threads that access a `Server`'s data.

The easiest way to understand the locking strategy
and synchronization between the main thread and the
`Server`'s voice thread is by using the concept of
ownership.

## Understanding data ownership in Murmur

A thread owning an object means that it is the only
thread that is allowed to write to that object. To
make changes to it.

Most data in the `Server` class is owned by the main
thread. That means that the main thread is the only
thread that writes/updates those structures.

When processing incoming voice data (and re-
broadcasting that voice data), the `Server`'s voice
thread needs to access various parts of `Server`'s data.

To ensure correct synchronization between the two
threads it is important that the rules for accessing
data in `Server` are clearly documented.

Most of these ownership rules relate to the use of
`Server->qrwlVoiceThread` mentioned earlier. It is
the main reader/writer lock between the voice thread
and the main thread. It is important to know *how*
exactly Murmur uses this lock, because it is not
immediately obvious.

Since the data protected by `Server->qrwlVoiceThread`
is only ever accessed by two threads, Murmur is clever in
the way that it uses the lock.

It uses the ownership rule defined above ("a thread owning
an object means that it is the only thread allowed to write
to that object") to avoid taking locks in some situations.
This is to avoid littering the code with locks, but also to
avoid deadlocks and/or misuse of the non-recursive reader/write
locker.

When reading data in the main thread that Murmur knows is
only ever written to by the main thread (that is, the data
is *owned* by the main thread), Murmur will avoid taking a
read lock on `Server->qrwlVoiceThread`.

However, not all data in `Server` is owned by the main thread.
When data is owned by the voice thread, the same kind of rule
applies. The voice thread will not take a read lock when reading
data that it knows only *it* will write to.

This behavior is not immediately obvious, but it is important
to know for understanding how the `Server->qrwlVoiceThread`
lock is used throughout the code base.

Another important detail to keep in mind is that while the
main thread can avoid locking `Server->qrwlVoiceThread`
during most of its operation, the opposite it true for
the voice thread. Most of the time, when processing incoming
packets, and especially in the
`void Server::processMsg(ServerUser *u, const char *data, int len)`
method, the voice thread holds a read lock `Server->qrwlVoiceThread`.
This is to ensure that the main thread is properly excluded when the
voice thread is reading data, and the voice thread is excluded when
the main thread is writing data.

## Ownership of shared data between multiple threads

This section documents the owners of data in Murmur that is
accessed by multiple threads at once. It documents how one
should synchronize access to that data to avoid introducing
data races in Murmur.

### Data owned by the voice thread

These are never accessed by the main thread, except in `ServerUser`'s constructor.
There is no synchronization on these.

- `ServerUser->sUdpSocket`
- `ServerUser->saiUdpAddress`

### Data owned by the main thread

The rules for accessing these objects are:

- To read from the main thread: No lock is required.
  A read lock is implied because the main thread is the owner.
- To read from the voice thread: The voice thread must hold read lock on qrwlVoiceThread.
- To write from the main thread: The main thread must hold write lock on qrwlVoiceThread.
- To write from the voice thread: Illegal. Disallowed.

The objects are:

- `Server->qhUsers`
- `Server->qhChannels`
- `ServerUser->uiSession`
- `ServerUser->sState`
- `ServerUser->bMute`
- `ServerUser->bSuppress`
- `ServerUser->bSelfMute`
- `ServerUser->bDeaf`
- `ServerUser->bSelfDeaf`
- `ServerUser->ssContext`
- `ServerUser->cChannel`
- `ServerUser->qmTargets`
- `ServerUser->qmWhisperRedirect`
- `Channel->qlUsers`
- `Channel->qhLinks`
- `Channel->qhPermLinks`
- `Channel->qlChannels`
- `Channel->qhGroups`
- `Channel->qlACL`
- `WhisperTarget->qlChannels`
- `WhisperTarget->bLinks`
- `WhisperTarget->bChildren`
- `WhisperTarget->qlSessions`

### Data with shared ownership (main thread and voice thread)

The rules for accessing these objects are:

- To read from the main thread: The main thread must hold a read lock on qrwlVoiceThread.
- To read from the voice thread: The voice thread must hold a read lock on qrwlVoiceThread.
- To write from the main thread: The main thread must hold a write lock on qrwlVoiceThread.
- To write from the voice thread: The voice thread must hold a write lock on qrwlVoiceThread.

The objects with shared ownership are:

- `Server->qhHostUsers`
- `Server->qhPeerUsers`
-  `ServerUser->qmTargetCache`

### Data with no ownership (synchronized via atomic types)

These objects are explicitly synchronized by using atomic types
and need no extra synchronization when read or written to.
The objects are:

- `ServerUser->aiUdpFlag`

### Data with no ownership (synchronized via mutexes)

These objects use their own locking schemes. This is can be
an internal mutex that is automatically taken when the object's
methods are called. It can also be an external mutex that must
be held before accessing the object itself.

- `ServerUser->csCrypt` (Locked via `User->qmCrypt` mutex.)
- `ServerUser->bwr` (Internal locking inside `BandwidthRecord`. All methods can be called without extra synchronization.)
- `Server->acCache` (Locked via `Server->qmCache` mutex.)
