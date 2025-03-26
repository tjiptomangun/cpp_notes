/**
 * linux
 * $CC -Wall -ggdb3 -D_LE_ xuimsgsndrcv.c -o xuimsgsndrcv
 * aix
 * $CC -Wall -ggdb3 -D_BE_ xuimsgsndrcv.c getopt_long.c -o xuimsgsndrcv
 * usage :
 *   ./xuimsgsndrcv -p 10024 -> as server listen on 10024
 *   ./xuimsgsndrcv -c 192.168.0.24 -p 10024 -> connect to spec address in port 
 *   ./xuimsgsndrcv -r read
 **/

#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#ifdef _LE_
#include <stdint.h>
#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#ifdef _LE_
#include <getopt.h>
#else
#include "getopt_long.h"
#endif
#define XL_ATM_MAX_DENOMINATION_TYPE 8
#define XL_ATM_LEN_TRANS_NO	10
#define USINT16	unsigned short
#define USINT32 unsigned
#define BYTE unsigned char
#define PORT  0
#define MAXMSG 10000 
#define STDIN_BUFFSZ 1
#define CONVERTING_BUFFSZ 4096
#define XL_OBJECT_NAME_SIZE	20
#define XL_MAX_OBJECT_DATA_SIZE	1000
#define XL_TIMESTAMP_SIZE	20
#define XUI_MSG_SYSRUN_REJECT					0

// Control on demand
#define XUI_MSG_SYSRUN_START					1
#define XUI_MSG_SYSRUN_STOP						2
#define XUI_MSG_SYSRUN_IN_SERVICE				3
#define XUI_MSG_SYSRUN_OUT_SERVICE				4
#define XUI_MSG_SYSRUN_ECHO						5
#define XUI_MSG_SYSRUN_STATUS_REFRESH			6
#define XUI_MSG_SYSRUN_NET_CUT					7
#define XUI_MSG_SYSRUN_INST_CUT					8
#define XUI_MSG_SYSRUN_DEV_CUT					9
#define XUI_MSG_SYSRUN_SCHEDULE_OFFLINE			10
#define XUI_MSG_SYSRUN_ALARM_ACK				11
#define XUI_MSG_SYSRUN_ALARM_DELETE				12
#define XUI_MSG_SYSRUN_SHUTDOWN					13
#define XUI_MSG_SYSRUN_RECYCLE_CORE				14
#define XUI_MSG_SYSRUN_RECYCLE_ALL				15
#define XUI_MSG_SYSRUN_LOAD_PARAM				16
#define XUI_MSG_SYSRUN_PROC_CONFIG				17
#define XUI_MSG_SYSRUN_EXEC_PROG				18
#define XUI_MSG_SYSRUN_PROFILE_CUT				19

// Object Property Page on demand
#define XUI_MSG_SYSRUN_PROPERTY					21
#define XUI_MSG_SYSRUN_PROCESS_INFO				22
#define XUI_MSG_SYSRUN_ROUTING_INFO				23
#define XUI_MSG_SYSRUN_IP_INFO					24

// Broadcasted message
#define XUI_MSG_SYSRUN_ALARM					31
#define XUI_MSG_SYSRUN_TRACE					32

// Hot Configuration (database has been modified)
#define XUI_MSG_SYSRUN_ADD						41
#define XUI_MSG_SYSRUN_DELETE					42
#define XUI_MSG_SYSRUN_UPDATE					43

#define XUI_MSG_SYSRUN_INFO_UPDATED				48
#define XUI_MSG_SYSRUN_FORCE_COMPLETE			49

#define XUI_MSG_SYSNET_SIGNON					51
#define XUI_MSG_SYSNET_SIGNOFF					52

#define XUI_MSG_SYSRUN_LOAD_MSG					53
#define XUI_MSG_SYSRUN_UNLOAD_MSG				54

#define XUI_MSG_SYSRUN_FAILOVER_XTS				55
#define XUI_MSG_SYSRUN_SHUTDOWN_XTS2			56
#define XUI_MSG_SYSRUN_FAILOVER_XDS				57
#define XUI_MSG_SYSRUN_SHUTDOWN_XDS2			58

#define XUI_MSG_SYSRUN_REPLACE_KEY				60
#define XUI_MSG_SYSRUN_GENERATE_PIN				61
#define XUI_MSG_SYSRUN_SETUP_PRINT_PIN			62
#define XUI_MSG_SYSRUN_PRINT_PIN				63
#define XUI_MSG_SYSRUN_GENERATE_CVV				64
#define XUI_MSG_SYSRUN_TRANSLATE_PIN			65

#define XUI_MSG_TYPE_MAX						65

#pragma pack(1)
typedef struct XUIRequest
{
	unsigned short		unMsgType;
	unsigned short		unObjectType;
	char			strObjectName[XL_OBJECT_NAME_SIZE];
	unsigned short		unPointId;
	
} XUI_REQUEST;

typedef struct xl_dev_status
{
	unsigned short		nBusinessCycle;
	unsigned short		nActivation;
	unsigned short		nCommunication;
	unsigned short		nServiceMode;

} XL_DEV_STATUS;

typedef struct XUIReply
{
	unsigned short	unMsgType;
	unsigned short	unObjectType;
	char		strObjectName[XL_OBJECT_NAME_SIZE];
	unsigned short	unPointId;
	unsigned short	unMsgStatus;
	char		strObjectData[XL_MAX_OBJECT_DATA_SIZE];
	
} XUI_REPLY;


typedef struct XUISysRunAlarm
{
	unsigned short	unPointStatus;
	char		strTimestamp[XL_TIMESTAMP_SIZE];
	char		strUserId[XL_OBJECT_NAME_SIZE];
	unsigned int	unSeq;
}XUI_SYSRUN_ALARM;

typedef struct xl_ndc_hw_config
{
	USINT16			nProductClass;
	USINT16			nMcrw;
	USINT16			nCashRejectBin;
	USINT16			nDepository;
	USINT16			nReceiptPrinter;
	USINT16			nJournalPrinter;
	USINT16			nStatementPrinter;
	USINT16			nNightSafeDepository;
	USINT16			nEncryptor;
	USINT16			nCamera;
	USINT16			nDoorAccess;
	USINT16			nFlexDisk;
	USINT16			nTamperInd;
	USINT16			nCardholderKb;
	USINT16			nOperatorKb;
	USINT16			nCardholderDisplay;

} XL_NDC_HW_CONFIG;

typedef struct xui_ndc_central_counters_cdm_8cass
{
	time_t			LastBalancingTimestamp;
	USINT16			nLastBusinessCycle;
	USINT32			nLastStan;
	USINT16			nUnused;				/*	HOLE  */
	char			cLastMsgCoordinationNo;
	USINT16			nLastTrxSerialNo;
	USINT16			LastNotesDispensedCassette[XL_ATM_MAX_DENOMINATION_TYPE];
	USINT32                 nAccumTrxCount;
	USINT16                 NotesInCassette[XL_ATM_MAX_DENOMINATION_TYPE];
	USINT16                 NotesDispensedCassette[XL_ATM_MAX_DENOMINATION_TYPE];
	// NH 20110309 Handle Cash Deposit Machine (CDM)
	unsigned short  nBNA;
} XUI_NDC_CENTRAL_COUNTERS_CDM_8CASS;

typedef struct xui_ndc_hw_fitness_cdm_8cass
{
#ifdef _LE_
	unsigned short	nTimeOfDayClock		:4;
	unsigned short	nHighOrderComm		:4;
	unsigned short	nSystemDisk			:4;
	unsigned short	nCardReader			:4;		/* 990428 nMcrw */
	unsigned short	nDispenser			:4;		/* 990428 nCashHandler */
	unsigned short	nDepository			:4;
	unsigned short	nReceiptPrinter		:4;
	unsigned short	nJournalPrinter		:4;
	unsigned short	nNightSafeDepository:4;
	unsigned short	nEncryptor			:4;
	unsigned short	nCamera				:4;
	unsigned short	nDoorAccess			:4;
	unsigned short	nFlexDisk			:4;
	unsigned short	nUNUSED				:12;/* 20120917 removed, Xmon does not have this*/
	unsigned		Cassette[XL_ATM_MAX_DENOMINATION_TYPE];
	unsigned short	nStatementPrinter	:4;
  unsigned short	nCDM:4;
  unsigned short	nUNUSED2:8;			/* 20120917 removed, Xmon does not have this*/

#else
	unsigned short	nHighOrderComm		:4;
	unsigned short	nTimeOfDayClock		:4;
	unsigned short	nCardReader			:4;
	unsigned short	nSystemDisk			:4;
	unsigned short	nDepository			:4;
	unsigned short	nDispenser			:4;
	unsigned short	nJournalPrinter		:4;
	unsigned short	nReceiptPrinter		:4;
	unsigned short	nEncryptor			:4;
	unsigned short	nNightSafeDepository:4;
	unsigned short	nDoorAccess		:4;
	unsigned short	nCamera				:4;
	unsigned short	nUNUSED_A			:4;
	unsigned short	nFlexDisk			:4;
	unsigned short	nUNUSED_B			:8;
	unsigned		Cassette[XL_ATM_MAX_DENOMINATION_TYPE];
  unsigned short  nCDM:4;
	unsigned short	nStatementPrinter	:4;
  unsigned short  nUNUSED2:8;
#endif

} XUI_NDC_HW_FITNESS_CDM_8CASS;

typedef struct xui_ndc_supplies_cdm_8cass
{
#ifdef _LE_
	unsigned short	nCardCaptureBin		:4;
	unsigned short	nCashRejectBin		:4;
	unsigned short	nDepositBin			:4;
	unsigned short	nReceiptPaper		:4;
	unsigned short	nReceiptRibbon		:4;
	unsigned short	nReceiptPrintHead	:4;
	unsigned short	nReceiptKnife		:4;
	unsigned short	nJournalSpace		:4;
	unsigned short	nNightSafeDepository:4;
	unsigned short	nUNUSED				:12;
	unsigned 		Cassette[XL_ATM_MAX_DENOMINATION_TYPE];
	unsigned short	nStatementPaper		:4;
	unsigned short	nStatementRibbon	:4;
	unsigned short	nStatementPrintHead	:4;
	unsigned short  nStatementKnife         :4;
	unsigned short  nStatementCaptureBin:4;
	unsigned short  nCamera                         :4;
        // NH 20110309 Handle Cash Deposit Machine (CDM)
	unsigned short  nCDMBin                         :4;
	unsigned short  nUNUSED2                        :4;

#else
	unsigned short	nCashRejectBin		:4;
	unsigned short	nCardCaptureBin		:4;
	unsigned short	nReceiptPaper		:4;
	unsigned short	nDepositBin			:4;
	unsigned short	nReceiptPrintHead	:4;
	unsigned short	nReceiptRibbon		:4;
	unsigned short	nJournalSpace		:4;
	unsigned short	nReceiptKnife		:4;
	unsigned short	nUNUSED_A				:4;
	unsigned short	nNightSafeDepository:4;
	unsigned short	nUNUSED_B				:8;
	unsigned Cassette[XL_ATM_MAX_DENOMINATION_TYPE];
	unsigned short	nStatementRibbon	:4;
	unsigned short	nStatementPaper		:4;
	unsigned short  nStatementKnife         :4;
	unsigned short	nStatementPrintHead	:4;
	unsigned short  nCamera                         :4;
	unsigned short  nStatementCaptureBin:4;
        // NH 20110309 Handle Cash Deposit Machine (CDM)
	unsigned short  nUNUSED2                        :4;
	unsigned short  nCDMBin                         :4;
#endif

} XUI_NDC_SUPPLIES_CDM_8CASS;
typedef struct xui_ndc_sensors_cdm_8cass
{
#ifdef _LE_
	unsigned short	nSupervisorMode		:4;
	unsigned short	nVibrationHeat		:4;
	unsigned short	nDoorContact		:4;
	unsigned short	nSilentSignal		:4;
	unsigned short	nElectronicsEnclosure:4;
	unsigned short	nDepositBin			:4;
	unsigned short	nCardCaptureBin		:4;
	unsigned short	nCashRejectBin		:4;
	unsigned short	CassetteSlot[XL_ATM_MAX_DENOMINATION_TYPE];

#else
	unsigned short	nVibrationHeat		:4;
	unsigned short	nSupervisorMode		:4;
	unsigned short	nSilentSignal		:4;
	unsigned short	nDoorContact		:4;
	unsigned short	nDepositBin			:4;
	unsigned short	nElectronicsEnclosure:4;
	unsigned short	nCashRejectBin		:4;
	unsigned short	nCardCaptureBin		:4;
	unsigned short	CassetteSlot[XL_ATM_MAX_DENOMINATION_TYPE];
#endif

} XUI_NDC_SENSORS_CDM_8CASS;

typedef struct xui_ndc_counters_cdm_8cass
{
	time_t			LastUpdate;
	char			LastTrxSerialNo[XL_ATM_LEN_TRANS_NO+1];
	USINT32			nAccumTrxCount;

	USINT16			NotesInCassette[XL_ATM_MAX_DENOMINATION_TYPE];
	USINT16			NotesRejectedCassette[XL_ATM_MAX_DENOMINATION_TYPE];
	USINT16			NotesDispensedCassette[XL_ATM_MAX_DENOMINATION_TYPE];
	USINT16			LastNotesDispensedCassette[XL_ATM_MAX_DENOMINATION_TYPE];

	USINT16			nCardsCaptured;
        USINT16                 nEnvelopeDeposited;
        USINT16                 nLastEnvelopeSerialNo;
        USINT16                 nCameraFilm;
        // NH 20110309 Handle Cash Deposit Machine (CDM)
        unsigned short  nBNA;

} XUI_NDC_COUNTERS_CDM_8CASS;


typedef struct xui_ndc_status_cdm_8cass
{
#ifdef _LE_
	unsigned short			bFullDownload	:1;
	unsigned short			nUNUSED				:15;

#else
	unsigned short			nUNUSED_A			:7;
	unsigned short			bFullDownload	:1;
	unsigned short			nUNUSED_B			:8;
#endif

	unsigned				nConfigVer;
	USINT16					nLanguageIdx;
	time_t					nLastEchoTimestamp;
	time_t					nLastInactivityTimestamp;
	time_t					nLastKeyReplTimestamp;
	USINT32					nNotesDenomination[XL_ATM_MAX_DENOMINATION_TYPE];

/*	981007	XL_NDC_DAILY_COUNTERS	DailyCounters; */
	XUI_NDC_CENTRAL_COUNTERS_CDM_8CASS	CentralCounters;
	XL_NDC_HW_CONFIG		HWConfig;
	XUI_NDC_HW_FITNESS_CDM_8CASS		HWFitness;
	XUI_NDC_SUPPLIES_CDM_8CASS    	Supplies;
	XUI_NDC_SENSORS_CDM_8CASS			SensorStatus;
	XUI_NDC_COUNTERS_CDM_8CASS			SupplyCounters;
} XUI_NDC_STATUS_CDM_8CASS; 
#pragma pack()
USINT32 rvsInt (USINT32 v) 
{
	uint32_t result = v;
	result = (v << 24) & 0xFF000000;
  	result |= (v << 8) & 0x00FF0000;
  	result |= (v >> 8) & 0x0000FF00;
  	result |= (v >> 24) & 0x000000FF;  	
	return result;
} /* End of rvsInt */


USINT16 rvsShort (USINT16 v) 
{
	uint16_t result = v;
	result = (v << 8) & 0xFF00;
  	result |= (v >> 8) & 0x00FF;

	return result;
} /* End of rvsInt */
#ifdef _BE_ 
 #undef ntohs
 #undef htons
 #undef ntohl
 #undef htonl
 #define ntohs rvsShort
 #define htons rvsShort
 #define ntohl rvsInt
 #define htonl rvsInt
#else
 #define ntohs(x)	(x)
 #define htons(x)	(x)
 #define ntohl(x) 	(x)
 #define htonl(x) 	(x)
#endif

void printUSINT16 (USINT16 in, char *label)
{
	fprintf (stdout, "%s  =  %u\n", label, in);
}

void printString (char *in, char *label)
{

	fprintf (stdout, "%s  =  %s\n", label, in);
}
void printUSINT16Array (USINT16 *in, int max, char *label)
{
	int i = 0;
	for (i = 0; i < max; i++)
	{
		fprintf (stdout, "%s [%d]  =  %u\n", label, i, in[i]);
	}
}
void printUSINT32 (USINT32 in, char *label)
{
	fprintf (stdout, "%s  =  %u\n", label, in);
}
void printUSINT32Array (USINT32 *in, int max, char *label)
{
	int i = 0;
	for (i = 0; i < max; i++)
	{
		fprintf (stdout, "%s [%d]  =  %u\n", label, i, in[i]);
	}
}
void printTimestamp (time_t * in, char * label)
{ 
	struct tm *tmp;
	char outstr[200];
	tmp = localtime (in);
	strftime (outstr, sizeof (outstr), "%Y-%m-%d %H:%M:%S", tmp);
	fprintf (stdout, "%s = %s\n", label, outstr); 
}
void printCentralCounters (XUI_NDC_CENTRAL_COUNTERS_CDM_8CASS *in)
{ 
	fprintf (stdout, "\nCentral Counters\n=============\n"); 
	printTimestamp(&in->LastBalancingTimestamp, "LastBalancingTimestamp");
	printUSINT16(in->nLastBusinessCycle, "LastBusinessCycle");
	printUSINT32(in->nLastStan, "LastStan");
	fprintf (stdout,"LastMsgCoordinationNo = %c\n", in->cLastMsgCoordinationNo);
	printUSINT16(in->nLastTrxSerialNo, "LastTrxSerialNo");
	printUSINT16Array (in->LastNotesDispensedCassette, 
		XL_ATM_MAX_DENOMINATION_TYPE,"LastNotesDispensedCassette");
	printUSINT32(in->nAccumTrxCount, "AccumTrxCount");
	printUSINT16Array (in->NotesInCassette, 
		XL_ATM_MAX_DENOMINATION_TYPE, "NotesInCassette");
	printUSINT16Array (in->NotesDispensedCassette, 
		XL_ATM_MAX_DENOMINATION_TYPE, "NotesDispensedCassette");
}
void printHWConfig (XL_NDC_HW_CONFIG* in)
{
	
	fprintf (stdout, "\nHW Config\n=============\n"); 
	printUSINT16(in->nProductClass, "ProductClass");
	printUSINT16(in->nMcrw, "Mcrw");
	printUSINT16(in->nCashRejectBin, "CashRejectBin");
	printUSINT16(in->nDepository, "Depository");
	printUSINT16(in->nReceiptPrinter, "ReceiptPrinter");
	printUSINT16(in->nJournalPrinter, "JournalPrinter");
	printUSINT16(in->nStatementPrinter, "StatementPrinter");
	printUSINT16(in->nNightSafeDepository, "ightSafeDepository");
	printUSINT16(in->nEncryptor, "Encryptor");
	printUSINT16(in->nCamera, "Camera");
	printUSINT16(in->nDoorAccess, "DoorAccess");
	printUSINT16(in->nFlexDisk, "FlexDisk");
	printUSINT16(in->nTamperInd, "TamperInd");
	printUSINT16(in->nCardholderKb, "CardholderKb");
	printUSINT16(in->nOperatorKb, "OperatorKb");
	printUSINT16(in->nCardholderDisplay, "CardholderDisplay"); 
}

void printHWFitness (XUI_NDC_HW_FITNESS_CDM_8CASS *in)
{ 
	fprintf (stdout, "\nHW Fitness\n=============\n"); 
	printUSINT16(in->nTimeOfDayClock, "TimeOfDayClock");
	printUSINT16(in->nHighOrderComm, "HighOrderComm");
	printUSINT16(in->nSystemDisk, "SystemDisk");
	printUSINT16(in->nCardReader, "CardReader");
	printUSINT16(in->nDispenser, "Dispenser");
	printUSINT16(in->nDepository, "Depository");
	printUSINT16(in->nReceiptPrinter, "ReceiptPrinter");
	printUSINT16(in->nJournalPrinter, "JournalPrinter");
	printUSINT16(in->nNightSafeDepository, "NightSafeDepository");
	printUSINT16(in->nEncryptor, "Encryptor");
	printUSINT16(in->nCamera, "Camera");
	printUSINT16(in->nDoorAccess, "DoorAccess");
	printUSINT16(in->nFlexDisk, "FlexDisk"); 
	printUSINT32Array (in->Cassette, 
		XL_ATM_MAX_DENOMINATION_TYPE, "Cassette");
	printUSINT16(in->nStatementPrinter, "StatementPrinter");
	printUSINT16(in->nCDM, "CDM");

}

void printSupplies (XUI_NDC_SUPPLIES_CDM_8CASS *in)
{
	fprintf (stdout, "\nSupplies\n=============\n"); 
	printUSINT16(in->nCardCaptureBin, "CardCaptureBin");
	printUSINT16(in->nCashRejectBin, "CashRejectBin");
	printUSINT16(in->nDepositBin, "DepositBin");
	printUSINT16(in->nReceiptPaper, "ReceiptPaper");
	printUSINT16(in->nReceiptRibbon, "ReceiptRibbon");
	printUSINT16(in->nReceiptPrintHead, "ReceiptPrintHead");
	printUSINT16(in->nReceiptKnife, "ReceiptKnife");
	printUSINT16(in->nJournalSpace, "JournalSpace");
	printUSINT16(in->nNightSafeDepository, "NightSafeDepository"); 
	printUSINT32Array (in->Cassette, 
		XL_ATM_MAX_DENOMINATION_TYPE, "Cassette");
	printUSINT16(in->nStatementPaper, "StatementPaper");
	printUSINT16(in->nStatementRibbon, "StatementRibbon");
	printUSINT16(in->nStatementPrintHead, "StatementPrintHead");
	printUSINT16(in->nStatementKnife, "StatementKnife");
	printUSINT16(in->nStatementCaptureBin, "StatementCaptureBin");
	printUSINT16(in->nCamera, "Camera");
	printUSINT16(in->nCDMBin, "CDMBin");
} 
void printSensors (XUI_NDC_SENSORS_CDM_8CASS *in)
{
	fprintf (stdout, "\nSensors\n=============\n"); 
	printUSINT16(in->nSupervisorMode, "SupervisorMode");
	printUSINT16(in->nVibrationHeat, "VibrationHeat");
	printUSINT16(in->nDoorContact, "DoorContact");
	printUSINT16(in->nSilentSignal, "SilentSignal");
	printUSINT16(in->nElectronicsEnclosure, "ElectronicsEnclosure");
	printUSINT16(in->nDepositBin, "DepositBin");
	printUSINT16(in->nCardCaptureBin, "CardCaptureBin");
	printUSINT16(in->nCashRejectBin, "CashRejectBin");
	printUSINT16Array(in->CassetteSlot, 
		XL_ATM_MAX_DENOMINATION_TYPE,"CassetteSlot");
	
}
void printCounters (XUI_NDC_COUNTERS_CDM_8CASS *in)
{

	fprintf (stdout, "\nCounters\n=============\n"); 
	printTimestamp (&in->LastUpdate, "LastUpdate");
	printString (in->LastTrxSerialNo, "LastTrxSerialNo");
	printUSINT16(in->nAccumTrxCount, "AccumTrxCount");
	printUSINT16Array(in->NotesInCassette, 
		XL_ATM_MAX_DENOMINATION_TYPE, "NotesInCassette");
	printUSINT16Array(in->NotesRejectedCassette, 
		XL_ATM_MAX_DENOMINATION_TYPE, "NotesRejectedCassette");
	printUSINT16Array(in->NotesDispensedCassette, 
		XL_ATM_MAX_DENOMINATION_TYPE, "NotesDispensedCassette");
	printUSINT16Array(in->LastNotesDispensedCassette, 
		XL_ATM_MAX_DENOMINATION_TYPE, "LastNotesDispensedCassette");
	printUSINT16(in->nCardsCaptured, "CardsCaptured");
	printUSINT16(in->nEnvelopeDeposited, "EnvelopeDeposited");
	printUSINT16(in->nLastEnvelopeSerialNo, "LastEnvelopeSerialNo");
	printUSINT16(in->nCameraFilm, "CameraFilm");
	printUSINT16(in->nBNA, "BNA"); 
}
int printNdcStatus(XUI_NDC_STATUS_CDM_8CASS* in)
{	
	XUI_NDC_CENTRAL_COUNTERS_CDM_8CASS *pCC = &in->CentralCounters;;
	XL_NDC_HW_CONFIG *pHWC = &in->HWConfig;
	XUI_NDC_HW_FITNESS_CDM_8CASS *pHWF = &in->HWFitness;
	XUI_NDC_SUPPLIES_CDM_8CASS *pSup = &in->Supplies;
	XUI_NDC_SENSORS_CDM_8CASS *pSensor = &in->SensorStatus;
	XUI_NDC_COUNTERS_CDM_8CASS *pSC = &in->SupplyCounters; 
	fprintf (stdout, "\nstatus\n=======\n"); 
	printUSINT16 (in->bFullDownload, "FullDownload");
	printUSINT32 (in->nConfigVer, "ConfigVer");
	printUSINT16 (in->nLanguageIdx, "LanguageIdx");
	printTimestamp (&in->nLastEchoTimestamp, "LastEchoTimestamp");
	printTimestamp (&in->nLastInactivityTimestamp, "LastInactivityTimestamp");
	printTimestamp (&in->nLastKeyReplTimestamp, "LastKeyReplTimestamp");
	printUSINT32Array (in->nNotesDenomination, 
		XL_ATM_MAX_DENOMINATION_TYPE, "NotesDenomination");
	printCentralCounters (pCC);
	printHWConfig (pHWC);
	printHWFitness (pHWF);
	printSupplies (pSup);
	printSensors (pSensor);
	printCounters (pSC);
	
	return 0;
}

int printSysRunAlarm (XUI_SYSRUN_ALARM *in)
{
	printUSINT16 (in->unPointStatus, "Point Status");
	printString (in->strTimestamp, "Timestamp");
	printString (in->strUserId, "UserId");
	printUSINT32 (in->unSeq,"Seq");
	return 0;
}

int printXuiRpNdc (XUI_REPLY *in)
{
	XL_DEV_STATUS *pDevStatus = (XL_DEV_STATUS *) in->strObjectData;
	printUSINT16(in->unMsgType, "MsgType");
	printUSINT16(in->unObjectType, "ObjectType"); 
	printString (in->strObjectName, "ObjectName");
	printUSINT16(in->unPointId, "PointId");
	printUSINT16(in->unMsgStatus, "MsgStatus");
	printUSINT16(pDevStatus->nBusinessCycle, "BusinessCycle");
	printUSINT16(pDevStatus->nActivation, "Activation");
	printUSINT16(pDevStatus->nCommunication, "Communication");
	printUSINT16(pDevStatus->nServiceMode, "ServiceMode");
	pDevStatus ++;
	printNdcStatus ((XUI_NDC_STATUS_CDM_8CASS*) pDevStatus);
	return 0;
}

int printXuiRpSysRunAlarm (XUI_REPLY *in)
{
	XUI_SYSRUN_ALARM *pAlarm = (XUI_SYSRUN_ALARM *) in->strObjectData;
	printUSINT16(in->unMsgType, "MsgType");
	printUSINT16(in->unObjectType, "ObjectType"); 
	printString (in->strObjectName, "ObjectName");
	printUSINT16(in->unPointId, "PointId");
	printUSINT16(in->unMsgStatus, "MsgStatus");
	printSysRunAlarm (pAlarm);
	return 0;

}
void initNdcCentralCounterTest (XUI_NDC_CENTRAL_COUNTERS_CDM_8CASS *in)
{
	in->LastBalancingTimestamp = 643432;
	in->nLastBusinessCycle = 7656;
	in->nLastStan = 95433;
	in->cLastMsgCoordinationNo = '1';
	in->nLastTrxSerialNo = 71;

	in->LastNotesDispensedCassette [0] = 8;
	in->LastNotesDispensedCassette [1] = 7;
	in->LastNotesDispensedCassette [2] = 6;
	in->LastNotesDispensedCassette [3] = 5;
	in->LastNotesDispensedCassette [4] = 4;
	in->LastNotesDispensedCassette [5] = 3;
	in->LastNotesDispensedCassette [6] = 2;
	in->LastNotesDispensedCassette [7] = 1;

	in->nAccumTrxCount = 1325;
	
	in->NotesInCassette [0] = 823;
	in->NotesInCassette [1] = 723;
	in->NotesInCassette [2] = 623;
	in->NotesInCassette [3] = 523;
	in->NotesInCassette [4] = 423;
	in->NotesInCassette [5] = 323;
	in->NotesInCassette [6] = 223;
	in->NotesInCassette [7] = 123;

	in->NotesDispensedCassette [0] = 8001;
	in->NotesDispensedCassette [1] = 7001;
	in->NotesDispensedCassette [2] = 6001;
	in->NotesDispensedCassette [3] = 5001;
	in->NotesDispensedCassette [4] = 4001;
	in->NotesDispensedCassette [5] = 3001;
	in->NotesDispensedCassette [6] = 2001;
	in->NotesDispensedCassette [7] = 1001;

	in->nBNA = 143; 
}

void initNdcHWConfigTest (XL_NDC_HW_CONFIG *in)
{
	in->nProductClass = 200;
	in->nMcrw = 201;
	in->nCashRejectBin = 202;
	in->nDepository = 203;
	in->nReceiptPrinter = 204;
	in->nJournalPrinter = 205;
	in->nStatementPrinter = 206;
	in->nNightSafeDepository = 207;
	in->nEncryptor = 208;
	in->nCamera = 209;
	in->nDoorAccess = 210;
	in->nFlexDisk = 211;
	in->nTamperInd = 212;
	in->nCardholderKb = 213;
	in->nOperatorKb = 214;
	in->nCardholderDisplay = 215;
}

void initNdcHWFitnessTest (XUI_NDC_HW_FITNESS_CDM_8CASS *in)
{
	in->nTimeOfDayClock = 1;
	in->nHighOrderComm = 2;
	in->nSystemDisk = 3;
	in->nCardReader = 4;
	in->nDispenser = 5;
	in->nDepository = 6;
	in->nReceiptPrinter = 7;
	in->nJournalPrinter = 8;
	in->nNightSafeDepository = 9;
	in->nEncryptor = 10;
	in->nCamera = 11;
	in->nDoorAccess = 12;
	in->nFlexDisk = 13;
	in->Cassette[0] = 1001;
	in->Cassette[1] = 2002;
	in->Cassette[2] = 3003;
	in->Cassette[3] = 4004;
	in->Cassette[4] = 5005;
	in->Cassette[5] = 6001;
	in->Cassette[6] = 7007;
	in->Cassette[7] = 8008;
	in->nStatementPrinter = 14;
  	in->nCDM = 15; 
}

void initNdcSuppliesTest (XUI_NDC_SUPPLIES_CDM_8CASS *in)
{
	in->nCardCaptureBin	 = 15;
	in->nCashRejectBin	 = 14;
	in->nDepositBin		 = 13;
	in->nReceiptPaper	 = 12;
	in->nReceiptRibbon	 = 11;
	in->nReceiptPrintHead = 10;
	in->nReceiptKnife	 = 9;
	in->nJournalSpace	 = 8;
	in->nNightSafeDepository  = 7;
	in->Cassette[0] = 16;
	in->Cassette[1] = 116;
	in->Cassette[2] = 216;
	in->Cassette[3] = 316;
	in->Cassette[4] = 416;
	in->Cassette[5] = 516;
	in->Cassette[6] = 616;
	in->Cassette[7] = 716;
	in->nStatementPaper	 = 6;
	in->nStatementRibbon = 5;
	in->nStatementPrintHead = 4;
	in->nStatementKnife = 3;
	in->nStatementCaptureBin = 2;
	in->nCamera = 1;
	in->nCDMBin = 0;

}

void initNdcSensorsTest (XUI_NDC_SENSORS_CDM_8CASS *in)
{
	in->nVibrationHeat	 = 0;
	in->nSupervisorMode	 = 11;
	in->nSilentSignal	 = 12;
	in->nDoorContact	 = 13;
	in->nDepositBin		 = 14;
	in->nElectronicsEnclosure = 15;
	in->nCashRejectBin	 = 7;
	in->nCardCaptureBin	 = 8;
	in->CassetteSlot[0]  = 11;
	in->CassetteSlot[1]  = 12;
	in->CassetteSlot[2]  = 13;
	in->CassetteSlot[3]  = 14;
	in->CassetteSlot[4]  = 15;
	in->CassetteSlot[5]  = 16;
	in->CassetteSlot[6]  = 17;
	in->CassetteSlot[7]  = 18;
}

void initNdcCountersTest (XUI_NDC_COUNTERS_CDM_8CASS *in)
{
	in->LastUpdate  = 4343545;
	sprintf (in->LastTrxSerialNo,"%s", "470");
	in->nAccumTrxCount = 100;
	in->NotesInCassette [0] = 7015;
	in->NotesInCassette [1] = 7014;
	in->NotesInCassette [2] = 7013;
	in->NotesInCassette [3] = 7012;
	in->NotesInCassette [4] = 7011;
	in->NotesInCassette [5] = 7010;
	in->NotesInCassette [6] = 7009;
	in->NotesInCassette [7] = 7008;

	in->NotesRejectedCassette [0] = 815;
	in->NotesRejectedCassette [1] = 814;
	in->NotesRejectedCassette [2] = 813;
	in->NotesRejectedCassette [3] = 812;
	in->NotesRejectedCassette [4] = 811;
	in->NotesRejectedCassette [5] = 810;
	in->NotesRejectedCassette [6] = 809;
	in->NotesRejectedCassette [7] = 808;
	
	in->NotesDispensedCassette [0] = 88;
	in->NotesDispensedCassette [1] = 87;
	in->NotesDispensedCassette [2] = 86;
	in->NotesDispensedCassette [3] = 85;
	in->NotesDispensedCassette [4] = 84;
	in->NotesDispensedCassette [5] = 83;
	in->NotesDispensedCassette [6] = 82;
	in->NotesDispensedCassette [7] = 81;
	
	in->LastNotesDispensedCassette [0] = 8;
	in->LastNotesDispensedCassette [1] = 7;
	in->LastNotesDispensedCassette [2] = 6;
	in->LastNotesDispensedCassette [3] = 5;
	in->LastNotesDispensedCassette [4] = 4;
	in->LastNotesDispensedCassette [5] = 3;
	in->LastNotesDispensedCassette [6] = 2;
	in->LastNotesDispensedCassette [7] = 1;	
	
	in->nCardsCaptured = 19;
	in->nEnvelopeDeposited = 29;
	in->nLastEnvelopeSerialNo = 39;
	in->nCameraFilm = 49;
	in->nBNA = 56;
}
void initNdcStatusTest (XUI_NDC_STATUS_CDM_8CASS *in)
{
	in->bFullDownload = 1;
	in->nConfigVer = 4343;
	in->nLanguageIdx = 7869;
	in->nLastEchoTimestamp = 243543543;
	in->nLastInactivityTimestamp = 243543543;
	in->nLastKeyReplTimestamp = 243543543;
	in->nNotesDenomination [0] = 10000;
	in->nNotesDenomination [1] = 20000;
	in->nNotesDenomination [2] = 50000;
	in->nNotesDenomination [3] = 100000;
	in->nNotesDenomination [4] = 200000;
	in->nNotesDenomination [5] = 5000;
	in->nNotesDenomination [6] = 2000;
	in->nNotesDenomination [7] = 1000;
	initNdcCentralCounterTest (&in->CentralCounters);
	initNdcHWConfigTest (&in->HWConfig);
	initNdcHWFitnessTest (&in->HWFitness);
	initNdcSuppliesTest (&in->Supplies);
	initNdcSensorsTest (&in->SensorStatus);
	initNdcCountersTest (&in->SupplyCounters); 
}

void initSysRunAlarmTest (XUI_SYSRUN_ALARM *in)
{
	struct tm *tmp;
	time_t tTime;
	tmp = localtime (&tTime);
	strftime (in->strTimestamp, sizeof (in->strTimestamp), "%Y-%m-%d %H:%M:%S", tmp);	
	in->unPointStatus = 1;
	sprintf (in->strUserId, "xlink");
	in->unSeq = 10; 
}

void initXuiRpNdc (XUI_REPLY *in)
{
	XL_DEV_STATUS	*pDevStatus = (XL_DEV_STATUS *)in->strObjectData;
	in->unMsgType = 21;
	in->unObjectType = 801;
	memset (in->strObjectName, 0, XL_OBJECT_NAME_SIZE);
	sprintf (in->strObjectName, "%s", "ObjectTest");
	in->unPointId = 1;
	in->unMsgStatus = 100; 
	pDevStatus->nBusinessCycle = 23232;
	pDevStatus->nActivation = 3432;
	pDevStatus->nCommunication = 64554;
	pDevStatus->nServiceMode = 5443;
	pDevStatus ++;
	initNdcStatusTest ((XUI_NDC_STATUS_CDM_8CASS*)pDevStatus); 
	
}

void initXuiRpSysRunAlarm (XUI_REPLY *in)
{
	XUI_SYSRUN_ALARM *pAlarm= (XUI_SYSRUN_ALARM*)in->strObjectData; 
	in->unMsgType = 31;
	in->unObjectType = 801;
	memset (in->strObjectName, 0, XL_OBJECT_NAME_SIZE);
	sprintf (in->strObjectName, "%s", "ObjectTest");
	in->unPointId = 1;
	in->unMsgStatus = 100; 
	initSysRunAlarmTest ((XUI_SYSRUN_ALARM *) pAlarm);
}

void packNdcCentralCounters (XUI_NDC_CENTRAL_COUNTERS_CDM_8CASS *in)
{
	int n = 0;
	in->LastBalancingTimestamp = htonl (in->LastBalancingTimestamp);
	in->nLastBusinessCycle 	= htons (in->nLastBusinessCycle);
	in->nLastStan 				= htonl (in->nLastStan);
	in->cLastMsgCoordinationNo = in->cLastMsgCoordinationNo;
	in->nLastTrxSerialNo 		= htons (in->nLastTrxSerialNo);
 	for (n=0; n<XL_ATM_MAX_DENOMINATION_TYPE; n++)
	{
 		in->LastNotesDispensedCassette[n] =
 			htons (in->LastNotesDispensedCassette[n]);
 		in->NotesInCassette[n] =
 			htons (in->NotesInCassette[n]);
 		in->NotesDispensedCassette[n] =
 			htons (in->NotesDispensedCassette[n]);
	}
 	in->nAccumTrxCount = htonl (in->nAccumTrxCount);
 	in->nBNA = htons(in->nBNA); 
}

void packNdcHWConfig (XL_NDC_HW_CONFIG *in)
{ 
	in->nProductClass = htons (in->nProductClass);
	in->nMcrw = htons (in->nMcrw);
	in->nCashRejectBin = htons (in->nCashRejectBin);
	in->nDepository = htons (in->nDepository);
	in->nReceiptPrinter = htons (in->nReceiptPrinter);
	in->nJournalPrinter = htons (in->nJournalPrinter);
	in->nStatementPrinter 	= htons (in->nStatementPrinter);
	in->nNightSafeDepository 	= htons (in->nNightSafeDepository);
	in->nEncryptor = htons (in->nEncryptor);
	in->nCamera = htons (in->nCamera);
	in->nDoorAccess 			= htons (in->nDoorAccess);
	in->nFlexDisk 			= htons (in->nFlexDisk);
	in->nTamperInd 			= htons (in->nTamperInd);
	in->nCardholderKb 		= htons (in->nCardholderKb);
	in->nOperatorKb 			= htons (in->nOperatorKb);
	in->nCardholderDisplay = htons (in->nCardholderDisplay); 
}

void packNdcHWFitness (XUI_NDC_HW_FITNESS_CDM_8CASS* in)
{
	int n = 0; 
	for (n=0; n<XL_ATM_MAX_DENOMINATION_TYPE; n++)
	{
		in->Cassette[n] = htonl (in->Cassette[n]);
	} 
}

void packNdcSupplies (XUI_NDC_SUPPLIES_CDM_8CASS *in)
{
	int n = 0;
	for (n = 0; n < XL_ATM_MAX_DENOMINATION_TYPE; ++n)
	{
		in->Cassette[n] = htonl (in->Cassette[n]);
	}
}

void packNdcSensors (XUI_NDC_SENSORS_CDM_8CASS *in)
{
	int n = 0;
	for (n = 0; n < XL_ATM_MAX_DENOMINATION_TYPE; ++n)
	{
		in->CassetteSlot[n] = htonl (in->CassetteSlot[n]);
	} 
}

void packNdcSupplyCounters (XUI_NDC_COUNTERS_CDM_8CASS *in)
{

	int n = 0;
	in->LastUpdate 		= htonl (in->LastUpdate);
	in->nAccumTrxCount 	= htonl (in->nAccumTrxCount);

	for (n=0; n<XL_ATM_MAX_DENOMINATION_TYPE; n++) 
	{
		in->NotesInCassette[n] =
			htons (in->NotesInCassette[n]);
		in->NotesRejectedCassette[n] =
			htons (in->NotesRejectedCassette[n]);
		in->NotesDispensedCassette[n] =
			htons (in->NotesDispensedCassette[n]);
		in->LastNotesDispensedCassette[n] =
			htons (in->LastNotesDispensedCassette[n]);
	}
	in->nCardsCaptured 	= htons (in->nCardsCaptured);
	in->nEnvelopeDeposited = htons (in->nEnvelopeDeposited);
	in->nLastEnvelopeSerialNo = htons (in->nLastEnvelopeSerialNo);
	in->nCameraFilm 	= htons (in->nCameraFilm);
	in->nBNA 	= htons (in->nBNA);

}

void packNdcStatus (XUI_NDC_STATUS_CDM_8CASS *in)
{
	int n = 0;
	in->nConfigVer				  = htonl(in->nConfigVer);
	in->nLanguageIdx				  = htons(in->nLanguageIdx);
	in->nLastEchoTimestamp		  = htonl(in->nLastEchoTimestamp);
	in->nLastInactivityTimestamp	  = htonl(in->nLastInactivityTimestamp);
	in->nLastKeyReplTimestamp	  = htonl(in->nLastKeyReplTimestamp);
	for (n=0; n<XL_ATM_MAX_DENOMINATION_TYPE; n++) 
	{
		in->nNotesDenomination[n] = htonl (in->nNotesDenomination[n]);
	}
	packNdcCentralCounters (&in->CentralCounters);
	packNdcHWConfig (&in->HWConfig);
	packNdcHWFitness (&in->HWFitness);
	packNdcSupplies (&in->Supplies);
	packNdcSensors (&in->SensorStatus);
	packNdcSupplyCounters (&in->SupplyCounters);
}

void packXuiRpNdc (XUI_REPLY *in)
{
	XL_DEV_STATUS *pDevStatus = (XL_DEV_STATUS *)in->strObjectData;
	in->unMsgType = htons (in->unMsgType);
	in->unObjectType = htons (in->unObjectType);
	in->unPointId = htons (in->unPointId);
	in->unMsgStatus = htons (in->unMsgStatus);
	pDevStatus->nBusinessCycle = htons (pDevStatus->nBusinessCycle);
	pDevStatus->nActivation	= htons (pDevStatus->nActivation);
	pDevStatus->nCommunication = htons (pDevStatus->nCommunication);
	pDevStatus->nServiceMode = htons (pDevStatus->nServiceMode);
	pDevStatus++;
	packNdcStatus ((XUI_NDC_STATUS_CDM_8CASS *)pDevStatus); 
}

void packSysRunAlarm (XUI_SYSRUN_ALARM *in)
{
	in->unPointStatus = htons (in->unPointStatus);
	in->unSeq = htonl (in->unSeq);
}

void packXuiRpSysRunAlarm (XUI_REPLY *in)
{
	XUI_SYSRUN_ALARM *pAlarm = (XUI_SYSRUN_ALARM*)in->strObjectData;
	in->unMsgType = htons (in->unMsgType);
	in->unObjectType = htons (in->unObjectType);
	in->unPointId = htons (in->unPointId);
	in->unMsgStatus = htons (in->unMsgStatus);
	packSysRunAlarm (pAlarm);

}
int make_socket (uint16_t nport)
{
	int sock;
	struct sockaddr_in name;
	
	/* Create the socket */
	sock = socket (PF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		perror ("socket");
		exit (EXIT_FAILURE);
	}

	/* Give the socket a name */
	name.sin_family = AF_INET;
	name.sin_port = htons (nport);
	name.sin_addr.s_addr = htonl (INADDR_ANY);
	
	if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
	{
		perror ("bind");
		exit (EXIT_FAILURE);
	}
	return sock;
}

int read_from_client (int filedes)
{
	unsigned char buffer [MAXMSG];
	int nbytes; 
	XUI_REPLY *status = (XUI_REPLY *) buffer;

	nbytes = read (filedes, buffer, MAXMSG);
	if (nbytes < 0)
	{
		/* Read error */
		perror ("read");
		exit (EXIT_FAILURE);
	}
	else if (nbytes == 0)
	{
		/* End of file */
		return -1; 
	}
	else
	{
		/* Data read */
		fprintf (stderr, "Server : got message : %d\n", nbytes);
		packXuiRpNdc (status);
		printXuiRpNdc (status);
		return 0;
	}


}/* end of read_from_client */

int read_from_remote (int filedes)
{
	char buffer [MAXMSG];
	int nbytes;
	memset (buffer, 0, MAXMSG);

	nbytes = read (filedes, buffer, MAXMSG);
	if (nbytes < 0)
	{
		/* Read error */
		perror ("read");
		exit (EXIT_FAILURE);
	}
	else if (nbytes == 0)
	{
		/* End of file */
		return -1; 
	}
	else
	{
		/* Data read */
		fprintf (stderr, "Server : got message: '%s'\n", buffer);
		return 0;
	}
}/* end of read_from_remote */
int isServer = 1;
int port = PORT;
int isRead = 0;
char * host_name = NULL;
char * file_name = NULL;
int server()
{ 
	int sock;
	fd_set active_fd_set, read_fd_set;
	int i;
	struct sockaddr_in clientname;
	size_t size; 

	/* Create the socket and set it up to accept connections */
	sock = make_socket (port);
	if (listen (sock, 1) < 0)
	{
		perror ("listen");
		exit (EXIT_FAILURE);
	}

	/*Initialize the set of active sockets*/

	FD_ZERO (&active_fd_set);
	FD_SET (sock, &active_fd_set);

	while (1)
	{
		/* Block until input arrives on one or more active sockets */
		read_fd_set = active_fd_set;
		if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
		{
			perror ("select");
			exit (EXIT_FAILURE);
		}

		/* Service all the sockets with input pending */
		for (i = 0; i < FD_SETSIZE; i++)
		{
			if (!FD_ISSET (i, &read_fd_set))
				continue; 

			if (i != sock)
			{
			/* Data arriving on an already-connected socket */
				if (read_from_client (i) >=0)
					continue;
				close (i);
				FD_CLR (i, &active_fd_set); 
				continue;
			}
			/*Connection request on original socket*/
			int new;
			size = sizeof (clientname);
			new = accept (sock, (struct sockaddr *) &clientname,
					(socklen_t *) &size);
			if (new < 0)
			{
				perror ("accept");
				exit (EXIT_FAILURE);	
			}
			fprintf (stderr, "Server : connect from host %s, port %hd.\n",
				inet_ntoa (clientname.sin_addr),
				ntohs (clientname.sin_port));
			FD_SET (new, &active_fd_set); 
			
		}
	}
}
int client ()
{

	struct sockaddr_in my_addr; 
	int hsock;
	int err;
	int *p_int; 
	struct timeval tv;
	int packets = 0;
	int ret; 
	XUI_REPLY status ;
	initXuiRpNdc (&status);
	printXuiRpNdc (&status);
	packXuiRpNdc (&status);
	

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	fd_set active_fd_set, read_fd_set, write_fd_set;

	hsock = socket (AF_INET, SOCK_STREAM, 0); /* create the socket descriptor */
	if (hsock == -1)
	{
		fprintf (stderr, "error initializing socket %d\n", errno);
		exit (EXIT_FAILURE);
	} 
	p_int = (int *) calloc (1, sizeof (int));
	*p_int = 1;

	if ((setsockopt (hsock, SOL_SOCKET, SO_REUSEADDR, (char *)p_int, sizeof (int)) == -1) ||
		(setsockopt (hsock, SOL_SOCKET, SO_KEEPALIVE, (char *)p_int, sizeof (int)) == -1))
	{
		fprintf (stderr, "error setting options %d\n", errno);
		free (p_int);
		close (hsock);
		exit (1);
	}
	free (p_int);
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons (port);
	
	memset (&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr= inet_addr (host_name);
	fprintf (stdout,"client to %s:%d\n", host_name, port);

	if (connect (hsock, (struct sockaddr *) &my_addr, sizeof (my_addr)) == -1)
	{
		if ((err = errno) != EINPROGRESS)
		{
			fprintf (stderr, "error connection socket %d\n", errno);
			close (hsock);
			exit (EXIT_FAILURE);
		}
	}
	while (1)
	{

		FD_ZERO (&active_fd_set);
		FD_SET (hsock, &active_fd_set);
		
		write_fd_set = active_fd_set;
		/* select returns 0 if timeout, -1 if error*/
		/* http://www.gnu.org/software/libc/manual/html_node/Waiting-for-I_002fO.html */
		if ((ret = select (hsock + 1, NULL, &write_fd_set, NULL, &tv))<0)
		{
			fprintf (stderr, "error write select %d errno %d", ret, errno);
			close (hsock);
			exit (EXIT_FAILURE);
		}
		else if (ret == 0)
		{
			//no bits set
			fprintf (stderr, "no bits set: timeout write\n"); 
			sleep (2);
		}
		else
		{
			if (!packets)
			{
				fprintf (stdout, "write : %d\n", sizeof (status));
				write (hsock, &status,sizeof (status));
				packets ++;
			}
		}
		read_fd_set = active_fd_set;
		if ((ret = select (hsock + 1, &read_fd_set, NULL, NULL, &tv))<0)
		{
			fprintf (stderr, "error write select %d errno %d", ret, errno);
			close (hsock);
			exit (EXIT_FAILURE);
		}
		else if (ret == 0)
		{
			//no bits set
			//fprintf (stderr, "no bits set: timeout read\n");
			sleep (100);
		}
		else
		{
			if (packets)
			{
				read_from_remote (hsock);
//				packets --;
			}
		}
	} 
}

BYTE CharHex2Dec (char *sBil)
{
	int 	nI;

	for (nI = 0; nI < 2; nI++)
	{
		if ((sBil[nI] >= 0x30 && sBil[nI] <= 0x39) ||
             (sBil[nI] >= 0x41 && sBil[nI] <= 0x46) ||
		     (sBil[nI] >= 0x61 && sBil[nI] <= 0x66))
		{
			if ((sBil[nI] >= 0x41 && sBil[nI] <= 0x46) ||
				 (sBil[nI] >= 0x61 && sBil[nI] <= 0x66))
			{
				sBil[nI] = sBil[nI] + 9;
			}
		}
		else
		{
			return (0);
		}
	}
	return ((sBil[1] & 0x0f) + (16 * (sBil[0] & 0x0f)));
}

int ConvertBuffHex2Dec(char  *inBuff, BYTE *outBuff, int inBuffSz, int outBuffSz)
{
	int inIdx = 0, outIdx = 0;
	if(inBuffSz &1 )
		return -1;
	for (inIdx = 0, outIdx =0 ; inIdx < inBuffSz && outIdx < outBuffSz; inIdx+=2,outIdx+=1 )
	{
		outBuff[outIdx] = CharHex2Dec(&inBuff[inIdx]);
	}	
	return outIdx;
}


int printMessage(BYTE *inBuffer)
{

	XUI_REPLY 	*pXuiRp;
	pXuiRp = (XUI_REPLY *) inBuffer;
	int nRet = 0;
	unsigned short msgType = 0;
	unsigned char tmp [2];
	memcpy (tmp, inBuffer,2);
	msgType = ntohs (*((unsigned short *)&tmp));
	
	fprintf(stdout, "MsgType = %d\n", msgType);
	switch(msgType)
	{
		case XUI_MSG_SYSRUN_STATUS_REFRESH:
		case XUI_MSG_SYSRUN_PROPERTY:
			packXuiRpNdc (pXuiRp);
			nRet = printXuiRpNdc(pXuiRp);
			break;
		case XUI_MSG_SYSRUN_ALARM:
			packXuiRpSysRunAlarm (pXuiRp);
			nRet = printXuiRpSysRunAlarm(pXuiRp);
		default:
			break; 
	}
	return nRet; 
}

void readFilePtr(FILE *instream)
{
		
	char 	stdin_buffer[STDIN_BUFFSZ];
	char	converting_buffer[CONVERTING_BUFFSZ];
	BYTE	result_buffer[CONVERTING_BUFFSZ]; 
	int	bytes_read;
	int	stdin_buffsz=0;
	int 	converting_idx=0;
	if (instream == NULL)
		return;
	stdin_buffsz = STDIN_BUFFSZ * sizeof(BYTE);
	/*openning stdin for reading*/ 
	/*did it open*/
	if( instream != NULL )
	{
		
		while(( bytes_read = fread(&stdin_buffer, stdin_buffsz, 1, instream)) == stdin_buffsz)
		{
			if( !(stdin_buffer[0] == 0x0d || stdin_buffer[0] == 0x0a) )
			{
				converting_buffer[converting_idx++] = stdin_buffer[0];		
			}
			if(converting_idx >= CONVERTING_BUFFSZ)
			{
				converting_idx=0;
				ConvertBuffHex2Dec(converting_buffer, result_buffer,
					CONVERTING_BUFFSZ, CONVERTING_BUFFSZ);
			}
			//fprintf(stdout,".");
		}
		if(converting_idx & 1)
		{
			fprintf(stderr,"error odd size input");
			exit(0);
		}
		if(converting_idx < CONVERTING_BUFFSZ)
		{
	
			//fprintf(stdout,".");
			ConvertBuffHex2Dec(converting_buffer, result_buffer,
					converting_idx, CONVERTING_BUFFSZ); 
			printMessage(result_buffer);
		}
	}
	else
	{
		fprintf(stderr,"error openning stdin. aborting.\n");
		exit(0);
	} 
	
}


int main (int argc, char **argv)
{
	int c; 
	int hlen = 0;
	FILE *instream;
#ifdef _LE_
	fprintf (stdout, "_LE_\n");
#else
	fprintf (stdout, "_BE_\n");
#endif
	static struct option long_options[] =
	{
		{"server", no_argument, &isServer, 1},
		{"client", required_argument, &isServer, 'c'},
		{"port", required_argument, 0, 'p'},
		{"read", optional_argument, 0, 'r'},
		{0,0,0,0}
	};
	while ((c = getopt_long (argc, argv, ":sc:p:r::W;", long_options, NULL))>=0)
	{
		switch (c)
		{
			case 'p':
				port = atoi (optarg);
				break;
			case 's':
				isServer=1;
				break;
			case 'c':
				isServer=0;
				hlen = strlen (optarg);
				host_name = (char *) calloc (1, hlen+1);
				strcpy (host_name, optarg);
				break;
			case ':':
				switch (optopt)
				{
					case 'r':
						isRead = 1;
						break;
					default:
						break;
				}
			case 'r':
				if(optarg)
					hlen = strlen (optarg);
				isRead = 1;
				if (!optarg)
					continue;
				file_name = (char *) calloc (1, hlen+1);
				strcpy (file_name, optarg);
				break;
			
		}
	}
 	if (isRead)
	{
		if (file_name)
		{
			fprintf (stdout, "fname : %s",file_name);
			instream = fopen (file_name, "r");
			if (!instream)
			{
				fprintf(stderr, "error reading file\n");
				exit (-1);
			} 
			
		}
		else
		{
		
			instream = fdopen (fileno (stdin), "r");
		}
		readFilePtr (instream);
		fclose (instream);
		exit  (0);
	}
	if (isServer)
	{
		fprintf (stdout, "server\n");
		server ();
	}
	else
	{
		fprintf (stdout, "client\n");
		client ();
	}
	exit(0);	
}
