PKGCONFIG ?= pkg-config
CFLAGS = --std=gnu99 -ggdb
GLIB = `$(PKGCONFIG) --cflags --libs glib-2.0 gio-2.0`
JSON = `$(PKGCONFIG) --cflags --libs json-glib-1.0`
MOSQUITTO = -lmosquitto
LIBGPS = `$(PKGCONFIG) --cflags --libs libgps`

all: gwctrl gwctrl.stripped

gwctrl: gwctrl.c gwctrl.h \
		location.c location.h
	$(MAKE) -C mosquittomainloop
	$(CC) $(GLIB) $(JSON) $(MOSQUITTO) $(LIBGPS) $(CFLAGS) $(filter %.c,$^) -o $@ mosquittomainloop/mosquittomainloop.o
	
gwctrl.stripped: gwctrl
	strip -o gwctrl.stripped gwctrl

.PHONY: clean

clean:
	rm -rf gwctrl