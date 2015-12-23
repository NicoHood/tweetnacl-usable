### Building

A simple makefile is provided to compile the source for linux PCs.
The .ino file can be compiled with the latest Arduino IDE for AtMega2560 and ESP8266.

```bash
make cleanall
make
./tweetnacl
```

### Arduino Speed Tests

Tests are only a rough estimate to get the speed of the devices.
Only a few bytes (~80bytes) were used for testing.

On Arduino Uno there was not enough RAM available and the program crashed.
There was no avrNaCl optimization used, just TweetNaCl.

The results clearly shows that even on (not optimized) avr
the encryption can be very fast, if you split the encryption into two parts.

### Single step encryption

| Platform              | crypto_box_keypair | crypto_box | crypto_box_open | 
| :-------------------- | -----------------: | ---------: | --------------: | 
| Arduino Mega 16MHz    |             31,1s  |     31,1s  |          31,1s  |
| ESP8266 80Mhz         |             1400ms |     1400ms |          1400ms |
| ESP8266 160Mhz        |              700ms |      700ms |           700ms |
| Raspberry PiB+ 700MHz |              140ms |      140ms |           140ms |
| Raspberry PiZero 1GHz |              100ms |      100ms |           100ms |
| Raspberry Pi2 900MHz  |               90ms |       90ms |            90ms |
| Laptop i7 ~2,2GHz     |                3ms |        3ms |             3ms |

#### Two step encryption

| Platform              | crypto_box_beforenm | crypto_box_afternm | crypto_box_open_afternm |
| :-------------------- | ------------------: | -----------------: | ----------------------: | 
| Arduino Mega 16MHz    |              31,1s  |               47ms |                    63ms |
| ESP8266 80Mhz         |              1400ms |             1050us |                  1440us |
| ESP8266 160Mhz        |               700ms |              530us |                   720us |
| Raspberry PiB+ 700MHz |                     |                    |                         |
| Raspberry PiZero 1GHz |                     |                    |                         |
| Raspberry Pi2 900MHz  |                     |                    |                         |
| Laptop i7 ~2,2GHz     |                     |                    |                         |

### Optimization Tests

Different gcc optimization levels seem to not influence the speed in any noticable way (about a ms difference).
However the program size differs slighty, but not much. As a result no special optimization parameters were used.
Program was compiled for ESP8266 (32bit).

```
OS
Sketch uses 210,132 bytes (20%) of program storage space. Maximum is 1,044,464 bytes.
Global variables use 33,510 bytes (40%) of dynamic memory, leaving 48,410 bytes for local variables. Maximum is 81,920 bytes.

O0
Sketch uses 211,796 bytes (20%) of program storage space. Maximum is 1,044,464 bytes.
Global variables use 33,510 bytes (40%) of dynamic memory, leaving 48,410 bytes for local variables. Maximum is 81,920 bytes.

O1
Sketch uses 210,388 bytes (20%) of program storage space. Maximum is 1,044,464 bytes.
Global variables use 33,510 bytes (40%) of dynamic memory, leaving 48,410 bytes for local variables. Maximum is 81,920 bytes.

O2
Sketch uses 210,068 bytes (20%) of program storage space. Maximum is 1,044,464 bytes.
Global variables use 33,510 bytes (40%) of dynamic memory, leaving 48,410 bytes for local variables. Maximum is 81,920 bytes.

O3
Sketch uses 212,292 bytes (20%) of program storage space. Maximum is 1,044,464 bytes.
Global variables use 33,510 bytes (40%) of dynamic memory, leaving 48,410 bytes for local variables. Maximum is 81,920 bytes.
```

### Links

* [NaCl](http://nacl.cr.yp.to/box.html)
* [NaCl paper](http://cr.yp.to/highspeed/naclcrypto-20090310.pdf)
* [TweetNaCl](http://tweetnacl.cr.yp.to/software.html)
* [TweetNaCl paper](http://tweetnacl.cr.yp.to/tweetnacl-20140917.pdf)
* [uNaCl](http://munacl.cryptojedi.org/index.shtml)
* [uNaCl paper](https://cryptojedi.org/papers/avrnacl-20130514.pdf)

### Licence
TODO
