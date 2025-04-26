/* Simple Plugin API
 *
 * Copyright © 2019 Wim Taymans
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef SPA_SYSTEM_H
#define SPA_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

struct itimerspec;

#include <time.h>
#include <sys/types.h>

#include <spa/utils/defs.h>
#include <spa/utils/hook.h>

/**
 * a collection of core system functions
 */
#define SPA_TYPE_INTERFACE_System	SPA_TYPE_INFO_INTERFACE_BASE "System"
#define SPA_TYPE_INTERFACE_DataSystem	SPA_TYPE_INFO_INTERFACE_BASE "DataSystem"

#define SPA_VERSION_SYSTEM		0
struct spa_system { struct spa_interface iface; };

/* IO events */
#define SPA_IO_IN	(1 << 0)
#define SPA_IO_OUT	(1 << 2)
#define SPA_IO_ERR	(1 << 3)
#define SPA_IO_HUP	(1 << 4)

/* flags */
#define SPA_FD_CLOEXEC			(1<<0)
#define SPA_FD_NONBLOCK			(1<<1)
#define SPA_FD_EVENT_SEMAPHORE		(1<<2)
#define SPA_FD_TIMER_ABSTIME		(1<<3)
#define SPA_FD_TIMER_CANCEL_ON_SET	(1<<4)

struct spa_poll_event {
	uint32_t events;
	void *data;
};

struct spa_system_methods {
#define SPA_VERSION_SYSTEM_METHODS	0
	uint32_t version;

	/* read/write/ioctl */
	ssize_t (*read) (void *object, int fd, void *buf, size_t count);
	ssize_t (*write) (void *object, int fd, const void *buf, size_t count);
	int (*ioctl) (void *object, int fd, unsigned long request, ...);
	int (*close) (void *object, int fd);

	/* clock */
	int (*clock_gettime) (void *object,
			int clockid, struct timespec *value);
	int (*clock_getres) (void *object,
			int clockid, struct timespec *res);

	/* poll */
	int (*pollfd_create) (void *object, int flags);
	int (*pollfd_add) (void *object, int pfd, int fd, uint32_t events, void *data);
	int (*pollfd_mod) (void *object, int pfd, int fd, uint32_t events, void *data);
	int (*pollfd_del) (void *object, int pfd, int fd);
	int (*pollfd_wait) (void *object, int pfd,
			struct spa_poll_event *ev, int n_ev, int timeout);

	/* timers */
	int (*timerfd_create) (void *object, int clockid, int flags);
	int (*timerfd_settime) (void *object,
			int fd, int flags,
			const struct itimerspec *new_value,
			struct itimerspec *old_value);
	int (*timerfd_gettime) (void *object,
			int fd, struct itimerspec *curr_value);
	int (*timerfd_read) (void *object, int fd, uint64_t *expirations);

	/* events */
	int (*eventfd_create) (void *object, int flags);
	int (*eventfd_write) (void *object, int fd, uint64_t count);
	int (*eventfd_read) (void *object, int fd, uint64_t *count);

	/* signals */
	int (*signalfd_create) (void *object, int signal, int flags);
	int (*signalfd_read) (void *object, int fd, int *signal);
};

#define spa_system_method_r(o,method,version,...)			\
({									\
	int _res = -ENOTSUP;						\
	struct spa_system *_o = o;					\
	spa_interface_call_res(&_o->iface,				\
			struct spa_system_methods, _res,		\
			method, version, ##__VA_ARGS__);		\
	_res;								\
})


#define spa_system_read(s,...)			spa_system_method_r(s,read,0,__VA_ARGS__)
#define spa_system_write(s,...)			spa_system_method_r(s,write,0,__VA_ARGS__)
#define spa_system_ioctl(s,...)			spa_system_method_r(s,ioctl,0,__VA_ARGS__)
#define spa_system_close(s,...)			spa_system_method_r(s,close,0,__VA_ARGS__)

#define spa_system_clock_gettime(s,...)		spa_system_method_r(s,clock_gettime,0,__VA_ARGS__)
#define spa_system_clock_getres(s,...)		spa_system_method_r(s,clock_getres,0,__VA_ARGS__)

#define spa_system_pollfd_create(s,...)		spa_system_method_r(s,pollfd_create,0,__VA_ARGS__)
#define spa_system_pollfd_add(s,...)		spa_system_method_r(s,pollfd_add,0,__VA_ARGS__)
#define spa_system_pollfd_mod(s,...)		spa_system_method_r(s,pollfd_mod,0,__VA_ARGS__)
#define spa_system_pollfd_del(s,...)		spa_system_method_r(s,pollfd_del,0,__VA_ARGS__)
#define spa_system_pollfd_wait(s,...)		spa_system_method_r(s,pollfd_wait,0,__VA_ARGS__)

#define spa_system_timerfd_create(s,...)	spa_system_method_r(s,timerfd_create,0,__VA_ARGS__)
#define spa_system_timerfd_settime(s,...)	spa_system_method_r(s,timerfd_settime,0,__VA_ARGS__)
#define spa_system_timerfd_gettime(s,...)	spa_system_method_r(s,timerfd_gettime,0,__VA_ARGS__)
#define spa_system_timerfd_read(s,...)		spa_system_method_r(s,timerfd_read,0,__VA_ARGS__)

#define spa_system_eventfd_create(s,...)	spa_system_method_r(s,eventfd_create,0,__VA_ARGS__)
#define spa_system_eventfd_write(s,...)		spa_system_method_r(s,eventfd_write,0,__VA_ARGS__)
#define spa_system_eventfd_read(s,...)		spa_system_method_r(s,eventfd_read,0,__VA_ARGS__)

#define spa_system_signalfd_create(s,...)	spa_system_method_r(s,signalfd_create,0,__VA_ARGS__)
#define spa_system_signalfd_read(s,...)		spa_system_method_r(s,signalfd_read,0,__VA_ARGS__)

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* SPA_SYSTEM_H */
