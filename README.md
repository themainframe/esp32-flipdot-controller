# `esp32-flipdot-controller`

**Directly controls a Hanover Displays Ltd. 7020-20-01 dotboard from an R014B-model 20x14 flipdot display.**

## Pinout

**GPIO** numbers refer to the ESP32 GPIO pins.
**Pin** numbers refer to the dotboard's 14-pin flex connector.

* **GPIO 32** - Enable Display (**Pin 2**)
* **GPIO 33** - Column Advance (**Pin 7**)
* **GPIO 34** - Reset (**Pin 8**)
* **GPIO 35** - Set/Unset (**Pin 9**)
* **GPIO 36** - Row Advance (**Pin 10**)

In addition, **GPIO 39** controls the coil drive on **Pin 12** via a _FET/BJT_.

## Progress/Plan

- [ ] Working control bitbanging
- [ ] Control via UDP/Wi-Fi/HTTP server (whichever is most useful)
- [ ] Cook steamed hams