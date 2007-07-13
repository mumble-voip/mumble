CXX	= gcc
ORIGLIB = /usr/lib/libGL.so.1

debug:	ALL

release: ALL

ALL:	libGL.so.1 libmumble.so.1

%.o:	%.c
	$(CXX) -c -pipe -O2 -Wall -W -fPIC -fvisibility=hidden -o $@ $^

%_p.o:	%.c
	$(CXX) -c -pipe -O2 -Wall -W -fPIC -fvisibility=hidden -DPRELOAD -o $@ $^

libGL.link.so.1: libGL.original.so.1 Makefile
	$(CXX) -shared -Wl,-soname,libGL.original.so.1 -o ./$@

libGL.original.so.1: $(ORIGLIB)
	ln -sf $^ $@

libGL.so.1: overlay.o libGL.link.so.1
	$(CXX) -shared -Wl,-soname,$@ -o $@ $^ -lrt
	strip $@

libmumble.so.1: overlay_p.o
	$(CXX) -shared -Wl,-soname,$@ -o $@ $^ -lrt -ldl
	strip $@
	
clean:
	-rm -f *~ *.o *.so.1
distclean: clean

indent:
	indent -kr -l999 overlay.c

test:	libGL.so.1
	LD_LIBRARY_PATH=$$(pwd):$$LD_LIBRARY_PATH glxgears

quake:	libGL.so.1
	LD_LIBRARY_PATH=$$(pwd):$$LD_LIBRARY_PATH quake4-demo
