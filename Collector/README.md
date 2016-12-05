# Collector

This part of the project describes the temprature sensor that has reads the
water temperature and uploads the measured value to Thingspeak. It will also
keep track of the remaining battery power.

As the collector will run on batteries, the power consumption should be as low
as possible. Between measurements, the device will go into deep sleep mode.

## Hardware

* ESP8266-07 - As the physical distance between access point and the water is
about 10 meters, we use an ESP with an external antenna connected to it.
* Temperature probe: ds18b20
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

## Voltage regulator (ht7333)

We want to power the collector from batteries. The ESP8266 wants a constant
voltage of 3.2 volt and has been known to draw some current when powered on.


## Battery voltage (ADC)
As we want to minimize the power consumption of the collector, but know when it
is time to replace the batteries, we want to read the ADC. The ADC pin allows an
input range between 0 and 1V. This results in a value between 0 and 1024,
depending on the read voltage.

As we are interested in the power that is left in the batteries (and not what is
  available after the ht7333), we need to measure Vin and use a voltage divider
  to lower the input range between 0 and 1 volt.

Depending the used batteries, the Vin might be higher or lower. In our case, we
used 4 AA batteries, resulting in a Vmax of 4 * 1.5V = 6V.

![Voltage divider](./adc1.png?raw=true)


```
float val = (float)analogRead(A0);
float voltage =  val * (( r1 + r2 ) / r2 ) / 1023;
```

Even with high valued resistors, there will be a constant current flow over the
resistors, which results in loosing power and draining the batteries. Therefor,
we need to only power the voltage divider, when we want to measure data.


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
