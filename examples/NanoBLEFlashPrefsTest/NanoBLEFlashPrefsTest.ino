#include <Arduino.h>
#include "NanoBLEFlashPrefs.h"

NanoBLEFlashPrefs myFlashPrefs;

// Preferences structure. Arbitrary, but cannot exeed 1019 words (4076 byte)
typedef struct flashStruct {
    char ssid[64];
    char pwd[64];
    char devname[64];
    int  someNumber;
    float anotherNumber;
} flashPrefs;


void setup() {
    flashPrefs prefs1 {
      "test", "secret", "fred", 3, 5.6
    }; 
    flashPrefs prefs2 {
      "test2", "secret2", "fred2", 4, 6.7
    }; 
    flashPrefs prefsOut; 

    Serial.begin(115200);
    delay(5000);
    
    myFlashPrefs.readPrefs(&prefsOut, sizeof(prefsOut));

    Serial.println(prefsOut.ssid);
    Serial.println(prefsOut.pwd);
    Serial.println(prefsOut.devname);
    Serial.println(prefsOut.someNumber);
    Serial.println(prefsOut.anotherNumber);

    delay(2000);
    
    myFlashPrefs.writePrefs(&prefs2, sizeof(prefs1));

    myFlashPrefs.readPrefs(&prefsOut, sizeof(prefsOut));

    Serial.println(prefsOut.ssid);
    Serial.println(prefsOut.pwd);
    Serial.println(prefsOut.devname);
    Serial.println(prefsOut.someNumber);
    Serial.println(prefsOut.anotherNumber);
}


void loop() {
    // put your main code here, to run repeatedly:

}
