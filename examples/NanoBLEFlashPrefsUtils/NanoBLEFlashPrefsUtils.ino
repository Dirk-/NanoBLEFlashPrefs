/*
  NanoBLEFlashPrefsUtils

  Demonstrates the use of the NanoBLEFlashPrefs library for getting
  information on or deleting of flash memory

  created 2023
  by Dirk Fröhling

  https://github.com/Dirk-/NanoBLEFlashPrefs
*/
#include <NanoBLEFlashPrefs.h>

NanoBLEFlashPrefs myFlashPrefs; // Access to the library

void setup()
{
  Serial.begin(9600);
  delay(2000);
}

void loop()
{
  Serial.println("Enter i for info, d to delete, g for garbage collection.");
  delay(1000);
  if (Serial.available() > 0)
  {
    String cmdStr = Serial.readString();
    cmdStr.trim();

    if (cmdStr == "i")
    {
      Serial.print("-- Info: ");
      Serial.println(myFlashPrefs.statusString());
    }
    else if (cmdStr == "d")
    {
      Serial.print("-- Delete: ");
      int rc = myFlashPrefs.deletePrefs(); // Only marks records for deletion, use garbage collection to free memory
      Serial.println(myFlashPrefs.errorString(rc));
      Serial.print("-- Info: ");
      Serial.println(myFlashPrefs.statusString());
    }
    else if (cmdStr == "g")
    {
      Serial.print("-- Garbage Collection: ");
      int rc = myFlashPrefs.garbageCollection();
      Serial.println(myFlashPrefs.errorString(rc));
      Serial.print("-- Info: ");
      Serial.println(myFlashPrefs.statusString());
    }
  }
  delay(1000);
}
