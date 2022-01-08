all:
	cc drop.c -lX11 -lcairo -lm -o drop

run: all
	./drop

clean:
	rm -f drop

install: all
	install drop /usr/bin/
