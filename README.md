ahrsDemo
========

This is a simple AHRS visualization that receives quaternions over OSC and
visualizes the orientation by drawing a set of axes.

It expects the OSC messages to come in on port 10001, and the messages should
be formatted as:

```
/orientation w x y z
```

Where w, x, y, and z are the quaternion components.

This project also includes a little PD patch that can send the quaternion
components in the proper format so you can make sure things are working.

Building
--------

This makefile expects the project to be located in `${OF_ROOT}/apps/myApps`, or
at least that it is 2 levels below the OpenFrameworks root directory. You can
also just set the variable `OF_ROOT` to point to your OpenFrameworks directory.

Then you should be able to build just by typing `make`. `make run` will run the
compiled application, and open a window with some axes on it.
