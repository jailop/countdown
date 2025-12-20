CFLAGS = -Wall -g -std=c99
FLAGS = `pkg-config --cflags gtk4`
LIBS = `pkg-config --libs gtk4`
BIN = countdown-gtk countdown

all: $(BIN)

countdown-gtk: countdown-gtk.c
	gcc $(CFLAGS) $(FLAGS) -o countdown-gtk countdown-gtk.c $(LIBS)

countdown: countdown.c
	gcc $(CFLAGS) -o countdown countdown.c

install: $(BIN)
	cp $(BIN) /usr/local/bin

clean:
	rm -f $(BIN)
