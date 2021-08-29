#include "Arduino.h"
#include "NanoBLEFlashPrefs.h"
#include <fds.h>

// See https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v15.0.0%2Flib_fstorage.html

#define FILE_ID         0x0001  /* The ID of the file to write the records into. */
#define RECORD_KEY_1    0x1111  /* A key for the first record. */
#define RECORD_KEY_2    0x2222  /* A key for the second record. */


// Initialization, writing or erasing flash memory is an asynchronous operation.

/* Array to map FDS return values to strings. */
char const * fds_err_str[] =
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

/* Array to map FDS events to strings. */
static char const * fds_evt_str[] =
{
    "FDS_EVT_INIT",
    "FDS_EVT_WRITE",
    "FDS_EVT_UPDATE",
    "FDS_EVT_DEL_RECORD",
    "FDS_EVT_DEL_FILE",
    "FDS_EVT_GC",
};


#ifdef __cplusplus
extern "C" {
#endif


// Simple event handler to handle errors.
static void fds_evt_handler(fds_evt_t const * p_fds_evt)
{
    switch (p_fds_evt->id)
    {
        case FDS_EVT_INIT:
            if (p_fds_evt->result != NRF_SUCCESS)
            {
                // Initialization failed.
            }
            break;
        case FDS_EVT_WRITE:
            if (p_fds_evt->result != NRF_SUCCESS)
            {
                // Write failed.
            }
            break;
        case FDS_EVT_UPDATE:
            if (p_fds_evt->result != NRF_SUCCESS)
            {
                // Write failed.
            }
            break;
        case FDS_EVT_DEL_RECORD:
            if (p_fds_evt->result != NRF_SUCCESS)
            {
                // Delete failed.
            }
            break;
        default:
            break;
    }
}


#ifdef __cplusplus
}
#endif


NanoBLEFlashPrefs::NanoBLEFlashPrefs()
{
	ret_code_t ret = fds_register(fds_evt_handler);
	if (ret != NRF_SUCCESS)
	{
	    // Registering of the FDS event handler has failed.
	}
	
	ret = fds_init();
	if (ret != NRF_SUCCESS)
	{
    	// Handle error.
	}
}


void NanoBLEFlashPrefs::writePrefs(void* value, int lengthInByte)
{
	fds_record_t        record;
	fds_record_desc_t   record_desc;
	
	// Set up record.
	record.file_id           = FILE_ID;
	record.key               = RECORD_KEY_1;
	record.data.p_data       = value;
	/* The following calculation takes into account any eventual remainder of the division. */
	record.data.length_words = (lengthInByte + 3) / 4;
	ret_code_t rc;
	rc = fds_record_write(&record_desc, &record);
	if (rc != NRF_SUCCESS)
	{
		/* Handle error. */
	}
}


void NanoBLEFlashPrefs::readPrefs(void* value, int lengthInByte)
{
	fds_flash_record_t  flash_record;
	fds_record_desc_t   record_desc;
	fds_find_token_t    ftok;
	
	/* It is required to zero the token before first use. */
	memset(&ftok, 0x00, sizeof(fds_find_token_t));
	
	/* Loop until all records with the given key and file ID have been found. */
	while (fds_record_find(FILE_ID, RECORD_KEY_1, &record_desc, &ftok) == NRF_SUCCESS)
	{
		if (fds_record_open(&record_desc, &flash_record) != NRF_SUCCESS)
		{
			/* Handle error. */
		}
		
		/* Access the record through the flash_record structure. */
		// uint16_t length = flash_record.p_header.length_words;
		// lengthInByte = length * 4;
		memcpy(value, flash_record.p_data, lengthInByte);
		
		/* Close the record when done. */
		if (fds_record_close(&record_desc) != NRF_SUCCESS)
		{
			/* Handle error. */
		}
	}
}


void NanoBLEFlashPrefs::deletePrefs()
{
	fds_record_desc_t   record_desc;
	fds_find_token_t    ftok;
	
	/* It is required to zero the token before first use. */
	memset(&ftok, 0x00, sizeof(fds_find_token_t));
	
	/* Loop until all records with the given key and file ID have been found. */
	while (fds_record_find(FILE_ID, RECORD_KEY_1, &record_desc, &ftok) == NRF_SUCCESS)
	{

		ret_code_t ret = fds_record_delete(&record_desc);
		if (ret != FDS_SUCCESS)
		{
			/* Error. */
		}
	}
}


