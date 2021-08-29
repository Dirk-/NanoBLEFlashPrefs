# NanoBLEFlashPrefs

Substitute for EEPROM storage on Arduino Nano BLE and BLE Sense.

## Info

Arduino Nano BLE and BLE Sense do not have an EEPROM where you can store any parameters
permanently. This library allows to store parameters and preferences between program restarts
and even reflashing.

These Arduino Boards use a nRF52840 microcontroller from Nordic Semiconductor. This chip
features a very simple file system for flash storage. This is used here to store and retrieve
an arbitrary struct of preferences for your program.

## Installation

- Select `Download ZIP` from the GitHub `Code` button. Unzip the archive if it isn't already.
You will end up with a folder `NanoBLEFlashPrefs-master` in your download folder. 

- Rename that folder to `NanoBLEFlashPrefs`.
- Move it to `Arduino/libraries` in your documents folder.
- Restart Arduino IDE.

## TODO

### Library

- Lots.

### Documentation

- Clean up library and examples.

## Credits

