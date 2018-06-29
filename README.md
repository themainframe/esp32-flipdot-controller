# `esp32-flipdot-controller`

**Directly controls a Hanover Displays Ltd. 7020-20-01 dotboard from an R014B-model 20x14 flipdot display.**

![Image of the FDC board](https://damow.net/assets/images/2018/06/assembled-board.jpg)

See the associated [blog post on damow.net](https://damow.net/fun-with-flip-dot-displays/)!

## Pinout

**GPIO** numbers refer to the ESP32 GPIO pins.
**Pin** numbers refer to the dotboard's 14-pin flex connector.

* **GPIO 21** - Enable Display (**Pin 2**)
* **GPIO 18** - Column Advance (**Pin 7**)
* **GPIO 19** - Reset (**Pin 8**)
* **GPIO 17** - Set/Unset (**Pin 9**)
* **GPIO 5** - Row Advance (**Pin 10**)

In addition, **GPIO 4** controls the coil drive on **Pin 12** via a _FET/BJT_.
