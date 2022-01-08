#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <X11/extensions/Xinerama.h>

#include "config.h"
unsigned int width, height;
Display* display;
Drawable window;
int screen;
cairo_surface_t* surface;
cairo_t* cairo;

int cell_size = 0;

float animation_time = 0;

static void
redraw() {
	unsigned int i, j;
	int s;

	cairo_push_group(cairo);
		cairo_set_source_rgb(cairo, bg_r, bg_g, bg_b);
		cairo_paint(cairo);
		cairo_set_source_rgb(cairo, fg_r, fg_g, fg_b);
		for (i = 0; i < width; i+= cell_size) {
			for (j = 0; j < height; j+= cell_size) {
				s = (cell_size-cell_padding)*sin(i+j+animation_time);
				cairo_rectangle(cairo, i + (cell_size - s)/2, j + (cell_size - s)/2, s, s);
				cairo_fill(cairo);
			}
		}
	cairo_pop_group_to_source(cairo);
	cairo_paint(cairo);
	cairo_surface_flush(surface);
}

static void
xinit() {
	unsigned int i;
	Atom stop_atom;
	unsigned int n;
	int curx, cury;
	union {Window w; int x;} junk;
	Window w;
	XWindowAttributes wa;
	XSetWindowAttributes swa = {
		.override_redirect = True,
		.event_mask = ExposureMask | StructureNotifyMask | KeyPressMask
	};

	display = XOpenDisplay(NULL);
	if (!display) {
		fprintf(stderr, "Unable to open display\n");
		exit(1);
	}
	screen = DefaultScreen(display);

	XGetWindowAttributes(display, RootWindow(display, screen), &wa);

	width = wa.width;
	height = wa.height;

	window = XCreateWindow(display, DefaultRootWindow(display), 0, 0,
		width, height, 0, CopyFromParent, CopyFromParent, CopyFromParent,
		CWOverrideRedirect | CWEventMask, &swa);
	XMapWindow(display, window);
	XLowerWindow(display, window);

	surface = cairo_xlib_surface_create(display, window, DefaultVisual(display, screen), width, height);
	cairo_xlib_surface_set_size(surface, width, height);
	cairo = cairo_create(surface);

	stop_atom = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &stop_atom, 1);
}

int
main(int argc, char** argv) {
	XEvent e;
	unsigned int i;

	xinit();

	cell_size = width/cell_count;

	for (;;) {
		while (XPending(display)) {
			XNextEvent(display, &e);
			switch (e.type) {
				case ConfigureNotify:
					width = e.xconfigure.width;
					height = e.xconfigure.height;
					cairo_xlib_surface_set_size(surface, width, height);
					break;
				case Expose:
					if (e.xexpose.count) break;
					redraw();
					break;
			}
		}
		usleep(66666);
		animation_time += 0.03;
		redraw();
	}
}
