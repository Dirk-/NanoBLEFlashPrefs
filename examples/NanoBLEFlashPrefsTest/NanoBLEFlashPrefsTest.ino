#include <Arduino.h>
#include <NanoBLEFlashPrefs.h>

NanoBLEFlashPrefs myFlashPrefs;

// Preferences structure. Arbitrary, but must not exeed 1019 words (4076 byte)
typedef struct flashStruct {
    char someString[64];
    bool aSetting;
    int  someNumber;
    float anotherNumber;
} flashPrefs;

// Our preferences. All functions can read and write, but to make them permanent
// the struct has to be written to flash explicitly (see below).
flashPrefs prefs;

void setup() {
    // Another preferences variable just to show it actually works.
    // You could use a single variable to read from and write to flash.
    flashPrefs prefsOut; 

    Serial.begin(115200);

    // Give user a chance to open the terminal
    delay(5000);

    /*
    // Uncomment this if you always want to see what happens if there are no prefs to read.
    // You also see what happens if you run the sketch for the first time on your Arduino Nano BLE.
    myFlashPrefs.deletePrefs();
    while (!myFlashPrefs.operationCompleted()) {
    }
    myFlashPrefs.garbageCollection();
    */

    // See if we already have a preference record
    Serial.println("Read record...");
    int rc = myFlashPrefs.readPrefs(&prefsOut, sizeof(prefsOut));
    if (rc == FDS_SUCCESS) {
        Serial.println("Preferences found: ");
        Serial.println(prefsOut.someString);
        Serial.println(prefsOut.aSetting);
        Serial.println(prefsOut.someNumber);
        Serial.println(prefsOut.anotherNumber);
    } else {
        Serial.print("No preferences found. Return code: ");
        Serial.print(rc);
        Serial.print(", ");
        Serial.println(myFlashPrefs.errorString(rc));
    }
    Serial.println("");

    // Prepare preference record for writing
    strcpy(prefs.someString, "NanoBLEFlashPrefs Test");
    prefs.aSetting = true;
    prefs.someNumber = 42;
    prefs.anotherNumber = 3.14;

    // Write preference record
    Serial.println("Write preferences...");
    myFlashPrefs.writePrefs(&prefs, sizeof(prefs));
    // Wait until completion
    while (!myFlashPrefs.operationCompleted()) {
    }
    Serial.println("");
    
    // Read preference record
    Serial.println("Read preferences...");
    rc = myFlashPrefs.readPrefs(&prefsOut, sizeof(prefsOut));
    if (rc == FDS_SUCCESS) {
        Serial.println("Preferences read: ");
        Serial.println(prefsOut.someString);
        Serial.println(prefsOut.aSetting);
        Serial.println(prefsOut.someNumber);
        Serial.println(prefsOut.anotherNumber);
    } else {
        Serial.print("Return code: ");
        Serial.print(rc);
        Serial.print(", ");
        Serial.println(myFlashPrefs.errorString(rc));
    }
    Serial.println("");

    delay(1000);
    
    // Change preference record
    strcpy(prefs.someString, "NanoBLEFlashPrefs Test 2");
    prefs.aSetting = false;
    prefs.someNumber = 5050;
    prefs.anotherNumber = 2.72;

    // Write preference record
    Serial.println("Write another preference record...");
    myFlashPrefs.writePrefs(&prefs, sizeof(prefs));
    // Wait until completion
    while (!myFlashPrefs.operationCompleted()) {
    }
    Serial.println("");
    
    // Read preference record
    Serial.println("Read preferences...");
    rc = myFlashPrefs.readPrefs(&prefsOut, sizeof(prefsOut));
    if (rc == FDS_SUCCESS) {
        Serial.println("Preferences read: ");
        Serial.println(prefsOut.someString);
        Serial.println(prefsOut.aSetting);
        Serial.println(prefsOut.someNumber);
        Serial.println(prefsOut.anotherNumber);
    } else {
        Serial.print("Return code: ");
        Serial.print(rc);
        Serial.print(", ");
        Serial.println(myFlashPrefs.errorString(rc));
    }

    Serial.println("");
    Serial.println("Done. Press reset button to see that again.");
    Serial.println("");
}


void loop() {
    // put your main code here, to run repeatedly:

}