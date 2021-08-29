# NanoBLEFlashPrefs

Substitute for missing EEPROM storage on Arduino Nano 33 BLE and 33 BLE Sense.

## Info

Arduino Nano 33 BLE and 33 BLE Sense do not have an EEPROM where you can store any parameters
permanently. This library allows to store parameters and preferences between program restarts
and even reflashing.

The Arduino boards mentioned above use a nRF52840 microcontroller from Nordic Semiconductor. This chip
features a very simple file system for flash storage. This is used here to store and retrieve
an arbitrary struct of preferences of your program.

## Installation

- Select `Download ZIP` from the GitHub `Code` button. Unzip the archive if it isn't already.
You will end up with a folder `NanoBLEFlashPrefs-master` in your download folder. 

- Rename that folder to `NanoBLEFlashPrefs`.
- Move it to `Arduino/libraries` in your documents folder.
- Restart Arduino IDE.

The library has been tested with Arduino Nano 33 BLE and version 2.4.1 of this board's
SDK. Make sure you have at least this version of `Arduino Mbed OS Nano Boards` installed.

## Usage

See the enclosed example. You can store any preferences you like in a struct like this:

	// Preferences structure. Arbitrary, but must not exeed 1019 words (4076 byte)
	typedef struct flashStruct {
    	char someString[64];
    	bool aSetting;
    	int  someNumber;
    	float anotherNumber;
	} flashPrefs;

This is how you read and write prefs:

	NanoBLEFlashPrefs myFlashPrefs;
	flashPrefs prefs;

    int rc = myFlashPrefs.readPrefs(&prefs, sizeof(prefs));

	myFlashPrefs.writePrefs(&prefs, sizeof(prefs));
	
Deleting prefs and garbage collection is also supported.

## TODO

### Library

- Complete error handling
- Test on Arduino Nano 33 BLE Sense

### Documentation

- Expand.

## Credits

See [nRF5 SDK](https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v15.0.0%2Flib_fstorage.html)
by Nordic Semiconductor for sample code and additional information.

