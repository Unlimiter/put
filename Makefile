PREFIX = /usr/local

all: put

put: src/put.cpp src/put.hpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS) src/put.cpp -o $@

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 0755 put $(DESTDIR)$(PREFIX)/bin

clean:
	rm -f put
