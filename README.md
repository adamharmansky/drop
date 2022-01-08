# Drop

![](drop.gif)

Simple X11 live wallpaper:

 - uses about 3% CPU (single core)
 - does not draw directly on the root window
 - no Xinerama, EWMH nor compositor required


## Dependencies

## Building

 - Xlib
 - cairo

To build and install, just run:

```
sudo make install
```

To configure, edit `config.h` and recompile.

Then, run the command `drop` to start drop.

### Note

If you want to tinker with the drawing, make sure to pass whole numbers to all the cairo functions, so that they can just call the underlying Xlib functions without compositing. This reduces CPU usage from 50% to 3% ☺.
