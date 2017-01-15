This is the collector part of the project. The code uses the https://platform.io/ framework.

## Installation

Clone the repository first, then initialize the submodule(s):

    git clone https://github.com/EHCNB/DeBlauweKamp.git
    cd Collector
    pio platform install espressif8266

Then adjust ```lib/Credentials/Credentials.h``` to connect to the right network,
modify the Thingspeak api key and such.

## Troubleshooting


### OneWire.h

    fatal error: OneWire.h: No such file or directory
    #include <OneWire.h>
    ^
    compilation terminated.


Cause is that the ```OneWire.h``` library cannot be found. Solution:

    pio install OneWire



## Tips and tricks


### Serial interface

Baud rate is: 115200 baud

#### OS X

    screen /dev/tty.SLAB_USBtoUART 115200


#### Linux


#### Window

Use putty.

## Commands

### Upload code

    platformio run --target upload
