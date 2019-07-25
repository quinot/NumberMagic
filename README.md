Arduino NumberMagic
===================

This sketch is a simple math game posing multiplication problems.
It features several devices that can be found in Arduino starter kits.

Keypad
------

4x4 membrane keypad connected to pins 2..9.

LCD 1602
--------

16 column, 2 row LCD display driven directly through a 74HC595
shift register.

Seven segment LED display
-------------------------

http://wayoda.github.io/LedControl/pages/software

The four-digit, seven-segment LED display is a 5641AS-1
common cathode unit. The forward current is 1.8V, and the
desired current is 20 mA (absolute maximum 30 mA).

It is driven by a MAX7219. Rset is set to 26.7 kOhm,
which should give approximately the desired current.

The LED display indicates the score; decimal points
indicate how many tries are left.

Segments numbering:

```
      1
     ---
  6 | 7 | 2
     ---
  5 |   | 3
     ---    . 0
      4
```

LedControl          | 0  | 1 | 2 | 3 | 4 | 5 | 6 | 7 
--------------------|----|---|---|---|---|---|---|---
MAX7219, 5641AS-1   | DP | A | B | C | D | E | F | G 

Data pins multiplexing
----------------------

We never write simultaneously to the LCD and the LED driver.
They share the hardware SPI data (MOSI, 11) and clock (13) pins.
