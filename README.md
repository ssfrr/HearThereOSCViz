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
