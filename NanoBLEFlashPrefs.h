#ifndef NanoBLEFlashPrefs_h
#define NanoBLEFlashPrefs_h

#include <fds.h>

class NanoBLEFlashPrefs
{
  	public:
  		// Constructor
    	NanoBLEFlashPrefs();
		
		// Write any variable or struct to flash file system.
    	void writePrefs(void* value, int lengthInByte);
    	
    	// Read content of preference record into value.
		int8_t readPrefs(void* value, int lengthInByte);
		
		// Deletes (actually invalidates) preference record.
		void deletePrefs();

		// Clears file system from all invalid preference records.
		void garbageCollection();
		
		// Initialization, writing or erasing flash memory is an asynchronous operation.
		// You can test with operationCompleted() if writePrefs or deletePrefs is done.
  		bool operationCompleted();
  	
	private:
  		static bool opCompleted;
		static void fdsEventHandler(fds_evt_t const * fdsEvent);
  		
};

#endif
