# Rpi-pico-i2c-example

This example demonstrates the infamous issue described at https://github.com/raspberrypi/pico-sdk/issues/708 (and the various other places linked there).

#### Problem: operating the Raspberry Pi pico as a slave/worker is not functioning as intended when the latter is sending data and the master/controller is receiving
#### When the data direction is the reverse, everything works as expected.

Depending on timing in which the slave/worker sends the data, the master/controller either sees it (and everything works as expected) or it doesn't (and the controller hangs, whereas the worker obliviously continue running without blocking, despite it is calling `i2c_write_raw_blocking` which as the name implies is supposed to block.

To demonstrate this behavior this code is using a single pico taking both roles in two separate cores. This is just for simplicity of testing and wiring, I verified the exact identical behavior when the controller and the worker are both running on the main thread in core zero in two separate boards.



If [line 37](https://github.com/davidedelvento/Rpi-pico-i2c-example/blob/cf7505e05e7d4061ffb33ae7362bd04a9c569c5b/i2c_example/example.c#L37) is commented and [line 41](https://github.com/davidedelvento/Rpi-pico-i2c-example/blob/cf7505e05e7d4061ffb33ae7362bd04a9c569c5b/i2c_example/example.c#L41) is uncommented, this is the output of the program.

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

If [line 37](https://github.com/davidedelvento/Rpi-pico-i2c-example/blob/cf7505e05e7d4061ffb33ae7362bd04a9c569c5b/i2c_example/example.c#L37) is commented and [line 41](https://github.com/davidedelvento/Rpi-pico-i2c-example/blob/cf7505e05e7d4061ffb33ae7362bd04a9c569c5b/i2c_example/example.c#L41) is uncommented, this is the output of the program.

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

As it is obvious, `i2c_write_raw_blocking` it is not blocking and the data is sent before the controller is ready to receive, which causes the bus to stall.
Looking at the source code of the `i2c_write_raw_blocking` function, it seems correct to me, so I speculate this is a hardware issue.

## Possible workaround
Obviously in most cases one could not or would not want to add just waits in the worker code, so using interrupts is the obvious solution. I put some stud example of that approach commented out in the code. As is it's not working and my time to work on this issue today is over.

Credits: https://github.com/vmilea/pico_i2c_slave/
