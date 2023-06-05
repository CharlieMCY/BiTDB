#ifndef BITDB_TA_H
#define BITDB_TA_H

/*
 * This UUID is generated with uuidgen
 * the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html
 */
#define BITDB_TA_UUID \
	{ 0xcb917dde, 0x3abc, 0x4452, \
		{ 0x93, 0xbc, 0x02, 0x33, 0xd4, 0x8d, 0xbe, 0x3f} }

/* The function IDs implemented in this TA */
#define TA_BITDB_CMD_SELECT_GPS     51
#define TA_BITDB_CMD_TPC_TEST	52

extern char* DB_NAME;

#endif /*BITDB_TA_H*/

