# Rpi-pico-i2c-example

This example demonstrates the infamous issue described at https://github.com/raspberrypi/pico-sdk/issues/708 

Depending on timing in which the slave/worker sends the data, the master/controller either sees it (and everything works as expected) or it doesn't (and the controller hangs, whereas the worker obliviously continue running without blocking, despite it is calling `i2c_write_raw_blocking` which is supposed to block)

```
pico I2C controller/worker example
This is what was I have: 0 0 0 0 
Worker about to send data
Worker sent data
This is what I received: 0 1 2 3 
This is what was I have: 0 0 0 0 
Worker about to send data
Worker sent data
This is what I received: 0 1 2 3 
This is what was I have: 0 0 0 0 
Worker about to send data
Worker sent data
This is what I received: 0 1 2 3 
```

```
pico I2C controller/worker example
Worker about to send data
This is what was I have: Worker sent data
0 0 0 0 
Worker about to send data
Worker sent data
Worker about to send data
Worker sent data
Worker about to send data
Worker sent data
Worker about to send data
Worker sent data
Worker about to send data
Worker sent data
```
