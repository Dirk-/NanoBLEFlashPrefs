#ifndef NanoBLEFlashPrefs_h
#define NanoBLEFlashPrefs_h

#include "Arduino.h"
#include <fds.h>

class NanoBLEFlashPrefs
{
  public:
    NanoBLEFlashPrefs();

    void writePrefs(void* value, int lengthInByte);
	void readPrefs(void* value, int lengthInByte);
	void deletePrefs();
};

#endif