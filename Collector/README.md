

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
