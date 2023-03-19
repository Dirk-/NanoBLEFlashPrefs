# NanoBLEFlashPrefs

Substitute for missing EEPROM storage on Arduino Nano 33 BLE and 33 BLE Sense (not for Nano 33 IoT or other Nano boards).

## Info

Arduino Nano 33 BLE and 33 BLE Sense do not have an EEPROM where you can store any parameters
permanently. This library allows to store parameters and preferences between program restarts
and even reflashing.

The Arduino boards mentioned above use a nRF52840 microcontroller from Nordic Semiconductor. This chip
features a very simple file system for flash storage. It is used here to easily store and retrieve
an arbitrary struct of preferences of your sketch.

**Please note:** Starting with version 1.1 of the library, all functions are synchronous. Therefore you should remove waiting loops with calls to `operationCompleted()` in your existing code.

## Installation

- Search for `NanoBLEFlashPrefs` in the library manager of your Arduino IDE and install it.

Alternatively:

- Select `Download ZIP` from the GitHub `Code` button. Unzip the archive if it isn't already.
  You will end up with a folder `NanoBLEFlashPrefs-master` in your download folder.

- Rename that folder to `NanoBLEFlashPrefs`.
- Move it to `Arduino/libraries` in your documents folder.
- Restart Arduino IDE.

The library has been tested with Arduino Nano 33 BLE and version 2.4.1 of this board's
SDK. Make sure you have at least this version of `Arduino Mbed OS Nano Boards` installed.

## Usage

See the enclosed `NanoBLEFlashPrefsTest` example. You can store any preferences you like in a struct like this:

    // Preferences structure. Arbitrary, but must not exeed 1019 words (4076 byte)
    typedef struct flashStruct {
    	char someString[64];
    	bool aSetting;
    	int  someNumber;
    	float anotherNumber;
    } flashPrefs;

This is how you read and write preferences:

    NanoBLEFlashPrefs myFlashPrefs;
    flashPrefs prefs;

    int rc = myFlashPrefs.readPrefs(&prefs, sizeof(prefs));

    // Prepare preference record for writing
    strcpy(prefs.someString, "NanoBLEFlashPrefs Test");
    prefs.aSetting = true;
    prefs.someNumber = 42;
    prefs.anotherNumber = 3.14;
    // Write it to flash memory
    myFlashPrefs.writePrefs(&prefs, sizeof(prefs));

In order to understand the return values of the functions, use `errorString()` like this:

    int rc = myFlashPrefs.readPrefs(&prefs, sizeof(prefs));
    Serial.println(myFlashPrefs.errorString(rc));

If you want to learn more about the status of your Nano 33 BLE flash file system, use `statusString()` like this:

    Serial.println(myFlashPrefs.statusString());

For interpretation of this information look at the [nRF5 SDK website](https://infocenter.nordicsemi.com/topic/sdk_nrf5_v17.1.0/lib_fds.html).

Garbage collection is also supported. `deletePrefs()` actually only invalidates a preference record, so flash usage gradually increases. Use `garbageCollection()` to clear the file system from all invalid (deleted) preference records. Please see the `NanoBLEFlashPrefsUtils` sample code for more information.

## TODO

### Library

- Test on Arduino Nano 33 BLE Sense

### Documentation

- Expand.

## Credits

See [nRF5 SDK](https://infocenter.nordicsemi.com/topic/sdk_nrf5_v17.1.0/lib_fds.html)
by Nordic Semiconductor for sample code and additional information.
