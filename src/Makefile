CFLAGS = -Wall -g -std=c99 `pkg-config --cflags gtk4`
LDLIBS = `pkg-config --libs gtk4`
BIN = countdown-gtk

all: $(BIN)

install: $(BIN)
	cp $(BIN) /usr/local/bin

clean:
	rm -f $(BIN)
