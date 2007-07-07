CXX	= g++
ORIGLIB = /usr/lib/libGL.so.1

debug:	ALL

release: ALL

ALL:	libGL.so.1 libmumble.so.1

%.o:	%.cpp
	g++ -c -pipe -O2 -Wall -W -fPIC -fvisibility=hidden -o $@ $^

%_p.o:	%.cpp
	g++ -c -pipe -O2 -Wall -W -fPIC -fvisibility=hidden -DPRELOAD -o $@ $^

libGL.link.so.1: libGL.original.so.1 Makefile
	g++ -shared -Wl,-soname,libGL.original.so.1 -o ./$@

libGL.original.so.1: $(ORIGLIB)
	ln -sf $^ $@

libGL.so.1: overlay.o libGL.link.so.1
	g++ -shared -Wl,-soname,$@ -o $@ $^ -lrt
	strip $@

libmumble.so.1: overlay_p.o
	g++ -shared -Wl,-soname,$@ -o $@ $^ -lrt
	strip $@
	
clean:
	-rm -f *~ *.o *.so.1
distclean: clean
	