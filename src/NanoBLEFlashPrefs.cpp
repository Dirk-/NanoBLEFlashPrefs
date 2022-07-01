#include <Arduino.h>
#include "NanoBLEFlashPrefs.h"

// See https://infocenter.nordicsemi.com/topic/sdk_nrf5_v17.0.2/lib_fds.html

#define FILE_ID 0x0001    // The ID of the file to write the records into.
#define RECORD_KEY 0x1111 // A key for the preferences record.

// Array to map FDS return values to strings.
char const *fds_err_str[] =
    {
        "FDS_SUCCESS",
        "FDS_ERR_OPERATION_TIMEOUT",
        "FDS_ERR_NOT_INITIALIZED",
        "FDS_ERR_UNALIGNED_ADDR",
        "FDS_ERR_INVALID_ARG",
        "FDS_ERR_NULL_ARG",
        "FDS_ERR_NO_OPEN_RECORDS",
        "FDS_ERR_NO_SPACE_IN_FLASH",
        "FDS_ERR_NO_SPACE_IN_QUEUES",
        "FDS_ERR_RECORD_TOO_LARGE",
        "FDS_ERR_NOT_FOUND",
        "FDS_ERR_NO_PAGES",
        "FDS_ERR_USER_LIMIT_REACHED",
        "FDS_ERR_CRC_CHECK_FAILED",
        "FDS_ERR_BUSY",
        "FDS_ERR_INTERNAL",
};


// Static class members

bool NanoBLEFlashPrefs::opCompleted;

ret_code_t NanoBLEFlashPrefs::lastResult = FDS_SUCCESS;


// Simple event handler. Gets called when the async operations fds_record_write(),
// fds_record_update(), fds_init() or fds_gc() complete.
void NanoBLEFlashPrefs::fdsEventHandler(fds_evt_t const *fdsEvent)
{
  // Signal end of operation
  opCompleted = true;

  // Make result of operation accessible. This is a different value than the
  // immediate return value of the functions.
  lastResult = fdsEvent->result;

  // We probably need this later...
  switch (fdsEvent->id)
  {
  case FDS_EVT_INIT:
    break;
  case FDS_EVT_WRITE:
    break;
  case FDS_EVT_UPDATE:
    break;
  case FDS_EVT_DEL_RECORD:
    break;
  case FDS_EVT_DEL_FILE:
    break;
  case FDS_EVT_GC:
    break;
  default:
    break;
  }
}

// Constructor
NanoBLEFlashPrefs::NanoBLEFlashPrefs()
{
  opCompleted = false;

  if (fds_register(fdsEventHandler) != FDS_SUCCESS)
  {
    // Registering of the FDS event handler has failed.
  }

  if (fds_init() == FDS_SUCCESS) // Wait for completion
  {
    while (!opCompleted)
    {
      // wait
    }
  }
  else
  {
    // Give up.
  }
}

int8_t NanoBLEFlashPrefs::writePrefs(void *value, unsigned int lengthInByte)
{
  fds_record_t record;
  fds_record_desc_t record_desc;
  fds_find_token_t ftok;

  // Data buffer for write and update operations. 4 byte alignment is crucial.
  // (https://infocenter.nordicsemi.com/topic/sdk_nrf5_v17.0.2/group__fds.html#ga0114083241dc287c7145fe113c9adc2c)
  uint8_t __attribute__((aligned(4))) dataBuffer[4076];

  // It is required to zero the token before first use.
  memset(&ftok, 0x00, sizeof(fds_find_token_t));

  opCompleted = false; // Will be set to true in event handler

  // Copy data to static variable.
  uint32_t const len = lengthInByte < sizeof(dataBuffer) ? lengthInByte : sizeof(dataBuffer);
  memset(dataBuffer, 0x00, sizeof(dataBuffer));
  memcpy(dataBuffer, value, len);

  // Set up record.
  record.file_id = FILE_ID;
  record.key = RECORD_KEY;
  record.data.p_data = dataBuffer;
  // Calculate length in words, take into account any eventual remainder of the division.
  record.data.length_words = (len + 3) / 4;

  ret_code_t ret = FDS_SUCCESS;
  if (fds_record_find(FILE_ID, RECORD_KEY, &record_desc, &ftok) == FDS_SUCCESS)
  {
    ret = fds_record_update(&record_desc, &record);
    // Serial.println(errorString(ret));
  }
  else
  {
    ret = fds_record_write(&record_desc, &record);
    // Serial.println(errorString(ret));
  }

  if (ret == FDS_SUCCESS)
  { // Call successful, wait for result
    while (!opCompleted)
    {
      /* wait */
    }

    ret = lastResult;
  }

  opCompleted = true; // In case of an error it will not get set by the event handler, so set it here

  return ret;
}

int8_t NanoBLEFlashPrefs::readPrefs(void *value, unsigned int lengthInByte)
{
  fds_flash_record_t flash_record;
  fds_record_desc_t record_desc;
  fds_find_token_t ftok;

  // It is required to zero the token before first use.
  memset(&ftok, 0x00, sizeof(fds_find_token_t));

  ret_code_t rc = FDS_ERR_NOT_FOUND;
  // Loop until all records with the given key and file ID have been found.
  while (fds_record_find(FILE_ID, RECORD_KEY, &record_desc, &ftok) == FDS_SUCCESS)
  {
    rc = fds_record_open(&record_desc, &flash_record);
    if (rc != FDS_SUCCESS)
    { // leave while loop in case of problems
      return rc;
    }

    // Access the record through the flash_record structure.
    // uint16_t length = flash_record.p_header.length_words;
    // lengthInByte = length * 4;
    memcpy(value, flash_record.p_data, lengthInByte);

    // Close the record when done.
    rc = fds_record_close(&record_desc);
  }

  return rc;
}

int8_t NanoBLEFlashPrefs::deletePrefs()
{
  fds_record_desc_t record_desc;
  fds_find_token_t ftok;

  // It is required to zero the token before first use.
  memset(&ftok, 0x00, sizeof(fds_find_token_t));

  ret_code_t rc = FDS_ERR_NOT_FOUND;

  // Loop until all records with the given key and file ID have been found.
  while (fds_record_find(FILE_ID, RECORD_KEY, &record_desc, &ftok) == FDS_SUCCESS)
  {
    opCompleted = false; // Will be set to true in event handler
    rc = fds_record_delete(&record_desc);
    if (rc != FDS_SUCCESS)
    { // leave while loop in case of problems
      break;
    }
    while (!opCompleted)
    {
      /* wait */
    }
    rc = lastResult;
  }

  opCompleted = true; // In case of an error it will not get set by the event handler, so set it here

  return rc;
}

int8_t NanoBLEFlashPrefs::garbageCollection()
{
  opCompleted = false; // Will be set to true in event handler
  ret_code_t rc = fds_gc();
  if (rc == FDS_SUCCESS)
  {
    while (!opCompleted)
    {
      /* wait */
    }
    rc = lastResult;
  }

  opCompleted = true; // In case of an error it will not get set by the event handler, so set it here

  return rc;
}

bool NanoBLEFlashPrefs::operationCompleted()
{
  return opCompleted;
}

const char *NanoBLEFlashPrefs::statusString()
{
  fds_stat_t stats;
  fds_stat(&stats);

  static char str[100];

  sprintf(str, "%d pages available, %d open records, %d valid records, %d dirty, %d corruption",
    stats.pages_available,
    stats.open_records,
    stats.valid_records,
    stats.dirty_records,
    stats.corruption);

  return str;
}

const char *NanoBLEFlashPrefs::errorString(int8_t returnCode)
{
  if (returnCode >= 0 && returnCode < 16)
  {
    return fds_err_str[returnCode];
  }

  return "UNKNOWN_CODE";
}
