### Arduino Speed Tests

Tests are not relyable, with a few bytes of data.

* Arduino Uno - Not enough ram
* Arduino Mega - 62,2s key generation, 32,2 en/decryption
* ESP8266 80Mhz - 2,8s key generation, 1,4s en/decryption
* ESP8266 160Mhz - 1,4s key generation, 0,7s en/decryption

### Optimization Tests

Different gcc optimization levels seem to not influence the speed in any noticable way (about a ms difference).
Program was compiled for ESP8266 (32bit).

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


###Links

* [NaCl](http://nacl.cr.yp.to/box.html)
* [NaCl paper](http://cr.yp.to/highspeed/naclcrypto-20090310.pdf)
* [TweetNaCl](http://tweetnacl.cr.yp.to/software.html)
* [TweetNaCl paper](http://tweetnacl.cr.yp.to/tweetnacl-20140917.pdf)
* [uNaCl](http://munacl.cryptojedi.org/index.shtml)
* [uNaCl paper](https://cryptojedi.org/papers/avrnacl-20130514.pdf)
