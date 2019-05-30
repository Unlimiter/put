PREFIX = /usr/local
MANPREFIX = /man/man1

all: put

put: src/put.cpp src/put.hpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS) src/put.cpp -o $@

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/man/man1
	install -m 0755 put $(DESTDIR)$(PREFIX)/bin
	install -m 0444 put.1 $(DESTDIR)$(PREFIX)$(MANPREFIX)/man1

clean:
	rm -f put
