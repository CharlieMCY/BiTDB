#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include "./include/bitdb_ta.h"
#include <string.h>
#include <utee_syscalls.h>
#include "./include/sqlite3.h"
#include "./include/tpc-test.h"
#include <utee_defines.h>
#include <tee_api.h>

static TEE_Time start, end;
char* DB_GPSNAME = "./aes_cbc_128_plain.db";
char* DB_TPC_NAME = "./TPC-H-small.db";
static int gps_rec_num = 0;
static int rec_num = 0;
int write_time = 0;
int read_time = 0;
int lseek_time = 0;
int strcspn_time = 0;

TEE_Result TA_CreateEntryPoint(void)
{
	/* Nothing to do */
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
	/* Nothing to do */
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t __unused param_types,
				    TEE_Param __unused params[4],
				    void __unused **session)
{
	/* Nothing to do */
	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void __unused *session)
{
	/* Nothing to do */
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	rec_num ++;
	// for (int i=0; i < argc; i++) {
	//   IMSG("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");	
	// }
	return 0;
}

// int current, highwater;
static int callback_gpsselect(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	gps_rec_num ++;
	// for (int i=0; i < argc; i++) {
	//   IMSG("%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
	// }
	
	// sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current, &highwater, 0);
	// IMSG("current = %d, highwater = %d", current, highwater);

	return 0;
}


static inline uint32_t tee_time_to_ms(TEE_Time t)
{
	return t.seconds * 1000 + t.millis;
}

static inline uint32_t get_delta_time_in_ms(TEE_Time start, TEE_Time stop)
{
	return tee_time_to_ms(stop) - tee_time_to_ms(start);
}

static TEE_Result cmd_selectgps(uint32_t param_types,
	TEE_Param params[4]){
	
	TEE_Time t_start={};
	TEE_Time t_end={};
	
	const uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	char* data = NULL;
	uint32_t data_len = 0;
	sqlite3 *db = NULL;
	int rc;
	char *zErrMsg = NULL;

	DMSG("has been called");

	sqlite3_hard_heap_limit64(10 * 1024 * 1024);

	// void *heap_mem = malloc(8 * 1024 * 1024);
	// sqlite3_config(SQLITE_CONFIG_HEAP, heap_mem, 8 * 1024 * 1024, 1024);
	// DMSG("addr: %d\n", heap_mem);

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	data = params[0].memref.buffer;
	data_len = params[0].memref.size;

	rc = sqlite3_open(DB_GPSNAME, &db);
	if(rc){
		IMSG("Can't create database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return TEE_SUCCESS;
	}
	DMSG("Database %s is opened.\n", DB_GPSNAME);

	DMSG("DATA: %s, DATA_LEN: %d.\n", data, data_len);

	TEE_GetSystemTime(&t_start);
	rc = sqlite3_exec(db, data, callback_gpsselect, 0, &zErrMsg);
	if(rc != SQLITE_OK){
		IMSG("SQL error: %s\n", zErrMsg);
		IMSG("rc = %d\n", rc);
		sqlite3_close(db);
		sqlite3_free(zErrMsg);
		return TEE_SUCCESS;
	}
	
	// MSG("write: %d ms, read: %d ms, lseek: %d ms, strcspn: %d ms.\n", write_time, read_time, lseek_time, strcspn_time);
	write_time = 0;
	read_time = 0;
	lseek_time = 0;
	strcspn_time = 0;

	TEE_GetSystemTime(&t_end);
	int cost = get_delta_time_in_ms(t_start, t_end);


	MSG("The total number of gps table's records: %d, timecost: %d\n", gps_rec_num, cost);
	gps_rec_num = 0;
	sqlite3_close(db);

	return TEE_SUCCESS;
}

static TEE_Result cmd_tpc_test(uint32_t param_types,
	TEE_Param params[4])
{
	const uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");

	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;
	
	sqlite3 *db = NULL;
	char* err_msg = NULL;

	sqlite3_hard_heap_limit64(10 * 1024 * 1024);

    MSG("qr, rec_num, time\n");
    // for (int j = 0; j < 1; j++) {
    for (int i = 0; i < 22; i++) {
		int rc = sqlite3_open(DB_TPC_NAME, &db);
	    if (rc != SQLITE_OK)
	    {
		    MSG("Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
		    return TEE_SUCCESS;
	    }

        rec_num = 0;
        char * sql = qr[i];
		TEE_GetREETime(&start);

        if (i == 14)
        {
			// rc = sqlite3_exec(db, q15drop, callback, 0, &err_msg);
            rc = sqlite3_exec(db, q15view, callback, 0, &err_msg);
			if(rc != SQLITE_OK) {
				MSG("SQL error: %s\n", err_msg);
				MSG("rc = %d\n", rc);
				sqlite3_close(db);
				sqlite3_free(err_msg);
				return TEE_SUCCESS;
			}
        }
		// if (i == 12)
		// {
		// 	rc = sqlite3_exec(db, q13drop, callback, 0, &err_msg);
		// 	// rc = sqlite3_exec(db, q13view, callback, 0, &err_msg);
		// 	if(rc != SQLITE_OK) {
		// 		MSG("SQL error: %s\n", err_msg);
		// 		MSG("rc = %d\n", rc);
		// 		sqlite3_close(db);
		// 		sqlite3_free(err_msg);
		// 		return TEE_SUCCESS;
		// 	}
		// }

        rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
		if(rc != SQLITE_OK) {
			MSG("SQL error: %s\n", err_msg);
			MSG("rc = %d\n", rc);
			sqlite3_close(db);
			sqlite3_free(err_msg);
			return TEE_SUCCESS;
		}
		TEE_GetREETime(&end);
        MSG("%d, %d, %d\n", 
			i + 1, rec_num, (end.seconds - start.seconds) * 1000 + (end.millis - start.millis));
		// IMSG("write: %d ms, read: %d ms, lseek: %d ms, strcspn: %d ms.\n", write_time, read_time, lseek_time, strcspn_time);
		write_time = 0;
		read_time = 0;
		lseek_time = 0;
		strcspn_time = 0;

        if (i == 14)
        {
			rc = sqlite3_exec(db, q15drop, callback, 0, &err_msg);
            // rc = sqlite3_exec(db, q15view, callback, 0, &err_msg);
			if(rc != SQLITE_OK) {
				MSG("SQL error: %s\n", err_msg);
				MSG("rc = %d\n", rc);
				sqlite3_close(db);
				sqlite3_free(err_msg);
				return TEE_SUCCESS;
			}
        }
		
		sqlite3_close(db);    
	}
    // }
    
	return TEE_SUCCESS;
}

TEE_Result TA_InvokeCommandEntryPoint(void __unused *session,
				      uint32_t command,
				      uint32_t param_types,
				      TEE_Param params[4])
{
	switch (command) {
	case TA_BITDB_CMD_SELECT_GPS:
		return cmd_selectgps(param_types, params);
	case TA_BITDB_CMD_TPC_TEST:
		return cmd_tpc_test(param_types, params);
	default:
		EMSG("Command ID 0x%x is not supported", command);
		return TEE_ERROR_NOT_SUPPORTED;
	}
}
