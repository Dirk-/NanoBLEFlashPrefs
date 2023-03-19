/*
  NanoBLEFlashPrefsTest

  Demonstrates the use of the NanoBLEFlashPrefs library for storing
  preferences and other values in flash memory

  created 2021
  by Dirk Fröhling

  https://github.com/Dirk-/NanoBLEFlashPrefs
*/
#include <Arduino.h>
#include <NanoBLEFlashPrefs.h>

// Access to the library
NanoBLEFlashPrefs myFlashPrefs;

// Structure of preferences. You determine the fields.
// Must not exeed 1019 words (4076 byte).
typedef struct flashStruct
{
  char someString[64];
  bool aSetting;
  int someNumber;
  float anotherNumber;
} flashPrefs;

// Our preferences. All functions here can read and modify these values, but to make
// them permanent, the struct must be written to flash explicitly (see below).
flashPrefs globalPrefs;

void setup()
{
  // Another preferences variable just to show it actually works
  // (We use globalPrefs for writing and localPrefs for reading).
  // You could use a single variable to read from and write to flash.
  flashPrefs localPrefs;

  Serial.begin(9600);

  // Give user a chance to open the terminal
  delay(5000);
  Serial.println("----- NanoBLEFlashPrefs Test -----");

  // See if we already have a preference record
  Serial.println("Read preference record...");
  int rc = myFlashPrefs.readPrefs(&localPrefs, sizeof(localPrefs));
  if (rc == FDS_SUCCESS)
  {
    printPreferences(localPrefs);
  }
  else
  {
    Serial.println("No preferences found."); // This should be the case when running for the first time on that particular board
    printReturnCode(rc);
  }
  Serial.println("");

  // Prepare preference record for writing
  strcpy(globalPrefs.someString, "NanoBLEFlashPrefs Test");
  globalPrefs.aSetting = true;
  globalPrefs.someNumber = 42;
  globalPrefs.anotherNumber = 3.14;

  // Write preference record
  Serial.println("Write preferences...");
  printReturnCode(myFlashPrefs.writePrefs(&globalPrefs, sizeof(globalPrefs)));
  Serial.println("");

  // Read preference record
  Serial.println("Read preferences...");
  rc = myFlashPrefs.readPrefs(&localPrefs, sizeof(localPrefs));
  if (rc == FDS_SUCCESS)
  {
    printPreferences(localPrefs);
  }
  else
  {
    printReturnCode(rc);
  }
  Serial.println("");

  delay(1000);

  // Change preference record
  strcpy(globalPrefs.someString, "NanoBLEFlashPrefs Test 2");
  globalPrefs.aSetting = false;
  globalPrefs.someNumber = 5050;
  globalPrefs.anotherNumber = 2.72;

  // Write preference record
  Serial.println("Write another preference record...");
  printReturnCode(myFlashPrefs.writePrefs(&globalPrefs, sizeof(globalPrefs)));
  Serial.println("");

  // Read preference record
  Serial.println("Read preferences...");
  rc = myFlashPrefs.readPrefs(&localPrefs, sizeof(localPrefs));
  if (rc == FDS_SUCCESS)
  {
    printPreferences(localPrefs);
  }
  else
  {
    printReturnCode(rc);
  }

  Serial.println("");
  Serial.println("Done. Press reset button to see that again or take look at");
  Serial.println("the NanoBLEFlashPrefsUtils example for more info.");
  Serial.println("");
}

void loop()
{
  // put your main code here, to run repeatedly:
}

// Print preference record to Serial.
// You have to adapt this to your particular structure.
void printPreferences(flashPrefs thePrefs)
{
  Serial.println("Preferences: ");
  Serial.println(thePrefs.someString);
  Serial.println(thePrefs.aSetting);
  Serial.println(thePrefs.someNumber);
  Serial.println(thePrefs.anotherNumber);
}

// Print return code infos to Serial.
void printReturnCode(int rc)
{
  Serial.print("Return code: ");
  Serial.print(rc);
  Serial.print(", ");
  Serial.println(myFlashPrefs.errorString(rc));
}
