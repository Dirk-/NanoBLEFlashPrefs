#ifndef NanoBLEFlashPrefs_h
#define NanoBLEFlashPrefs_h

#include <fds.h>

class NanoBLEFlashPrefs
{
  	public:
      /**
       * Constructor. Initializes the Flash Data Storage (FDS) module.
       */
      NanoBLEFlashPrefs();

      /**
       * Write any preference variable or struct to flash file system.
       *
       * @param value Any kind of variable, preferably a struct
       * @param lengthInByte Length of value in byte
       * @return Return code of Flash Data Storage module.
       */
      int8_t writePrefs(void *value, int lengthInByte);

      /**
       * Read content of preference record into value.
       *
       * @param value Any kind of variable, preferably a struct
       * @param lengthInByte Length of value in byte
       * @return Return code of Flash Data Storage module.
       */
      int8_t readPrefs(void *value, int lengthInByte);

      /**
       * Deletes (actually invalidates) preference record.
       *
       * @return Return code of Flash Data Storage module.
       */
      int8_t deletePrefs();

      /**
       * Clears file system from all invalid preference records.
       *
       * @return Return code of Flash Data Storage module.
       */
      int8_t garbageCollection();

      /**
       * Test if writePrefs or deletePrefs is done. (Initialization, 
       * writing or erasing flash memory are asynchronous operations)
       *
       * @return TRUE if successfully completed, FALSE if not.
       */
      bool operationCompleted();

      /**
       * Returns a meaningful error string when given a valid return code.
       *
       * @return Error string from Flash Data Storage module.
       */
      const char *errorString(int8_t rc);

    private:
  		static bool opCompleted;
		  static void fdsEventHandler(fds_evt_t const * fdsEvent);
  		
};

#endif
