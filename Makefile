PROGNM =  wm
PREFIX ?= /usr/local
BINDIR ?= $(DESTDIR)$(PREFIX)/bin
LIBDIR ?= $(DESTDIR)$(PREFIX)/lib
INCDIR ?= $(DESTDIR)$(PREFIX)/include
MKDIR ?= mkdir -p

include Makerules
CFLAGS += -Wno-disabled-macro-expansion

.PHONY: all bin clean scan-build cov-build test install uninstall

all: dist bin

bin:
	@$(CC) $(CFLAGS) -fPIE $(LDFLAGS) src/main.c -o dist/$(PROGNM)

clean:
	@rm -rf -- dist cov-int $(PROGNM).tgz make.sh ./src/*.plist

dist:
	@$(MKDIR) ./dist

cov-build: dist
	@cov-build --dir cov-int make bin
	@tar czvf $(PROGNM).tgz cov-int

scan-build:
	@scan-build --use-cc=$(CC) make all

test: all
	startx ./dist/$(PROGNM)

install:
	@install -Dm755 dist/$(PROGNM) $(BINDIR)/$(PROGNM)

uninstall:
	@rm -f -- $(BINDIR)/$(PROGNM)
