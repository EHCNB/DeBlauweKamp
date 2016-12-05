# Collector

This part of the project describes the temprature sensor that has reads the
water temperature and uploads the measured value to Thingspeak.

As the collector will run of batteries, the power consumption should be as low
as possible. Between measurements, the device will go into deep sleep mode.

## Hardware

* ESP8266-07 - As the physical distance between access point and the water is
about 10 meters, we use an ESP with an external antenna connected to it.
* power regulator: ht7333
* antenna
* batteries
* enclosure


## Deep sleep
The GPIO16 pin needs to be connected to the reset pin. After sleeping for the
given time, the GPIO16 pin is set to HIGH and the ESP8266 gets a reset,
reboots, measures the temparture and goes into deep sleep again.

Deep sleep time is given in microseconds, hence the 1000000 multiplication.

```
ESP.deepSleep(120 * 1000000);

```

## Serial interface

Baud rate is: 115200 baud

### OS X
```
screen /dev/tty.SLAB_USBtoUART 115200

```

### Linux


### Window

Use putty.

## Commands

### Upload code
```
platformio run --target upload

```


## Troubleshooting


### OneWire.h

```
fatal error: OneWire.h: No such file or directory
#include <OneWire.h>
^
compilation terminated.
```

Cause is that the OneWire.h library cannot be found. Solution:

```
platformio search OneWire
platformio install 1
```
