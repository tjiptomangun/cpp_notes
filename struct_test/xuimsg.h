#ifndef _XUIMSG_H_
#define _XUIMSG_H_
#include <time.h>


#define XL_ATM_MAX_CASSETTE_SLOT                4
#define XL_ATM_MAX_DENOMINATION_TYPE            8
#define USINT16	unsigned short
#define USINT32 unsigned int
#define XL_NDC_MAX_CASSETTE                     4
#define XL_ATM_LEN_TRANS_NO                     10
#define XL_OBJECT_NAME_SIZE			20
#define XL_MAX_OBJECT_DATA_SIZE                 1000

#ifdef _aix_51_
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
#pragma pack(1)
typedef struct XUIReply
{
	unsigned short	unMsgType;
	unsigned short	unObjectType;
	char			strObjectName[XL_OBJECT_NAME_SIZE];
	unsigned short	unPointId;
	unsigned short	unMsgStatus;
	char			strObjectData[XL_MAX_OBJECT_DATA_SIZE];
	
} XUI_REPLY;

typedef struct xl_dev_status
{
	unsigned short		nBusinessCycle;
	unsigned short		nActivation;
	unsigned short		nCommunication;
	unsigned short		nServiceMode;

} XL_DEV_STATUS;


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

#define XL_NDC_HW_FIT_OK				0
#define XL_NDC_HW_FIT_ERROR				1
#define XL_NDC_HW_FIT_WARNING			2
#define XL_NDC_HW_FIT_SUSPEND			3
#define XL_NDC_HW_FIT_FATAL				4


typedef struct xl_atm_hw_fitness
{
	unsigned short	nTimeOfDayClock		:4;
	unsigned short	nHighOrderComm		:4;
	unsigned short	nSystemDisk			:4;
	unsigned short	nFlexDisk			:4;
	unsigned short	nCardReader			:4;
	unsigned short	nDispenser			:4;
	unsigned short	nUNUSED				:8;
	unsigned short	nDenomination[XL_ATM_MAX_DENOMINATION_TYPE];
	unsigned short	nCassetteSlot[XL_ATM_MAX_CASSETTE_SLOT];

	unsigned short	nDepository			:4;
	unsigned short	nNightSafeDepository:4;		
					/* Securomatic Depository */
	unsigned short	nReceiptPrinter		:4;
	unsigned short	nJournalPrinter		:4;
	unsigned short	nStatementPrinter	:4;
	unsigned short	nPassbookPrinter	:4;
	unsigned short	nCamera				:4;
	unsigned short	nEncryptor			:4;
	unsigned short	nDoorAccess			:4;
	unsigned short	nVandalShield		:4;
	unsigned short	nUNUSED2			:8; 
	char				Unused[30];

} XL_ATM_HW_FITNESS;




typedef struct xl_ndc_hw_fitness
{
#ifdef _LE_
	unsigned short	nTimeOfDayClock		:4;
	unsigned short	nHighOrderComm		:4;
	unsigned short	nSystemDisk			:4;
	unsigned short	nCardReader			:4;
	unsigned short	nDispenser			:4;
	unsigned short	nDepository			:4;
	unsigned short	nReceiptPrinter		:4;
	unsigned short	nJournalPrinter		:4;
	unsigned short	nNightSafeDepository:4;
	unsigned short	nEncryptor			:4;
	unsigned short	nCamera				:4;
	unsigned short	nDoorAccess			:4;
	unsigned short	nFlexDisk			:4;
	unsigned short	nUNUSED				:12;
	unsigned		Cassette[XL_NDC_MAX_CASSETTE];
	unsigned short	nStatementPrinter	:4;
	unsigned short	nUNUSED2:12;
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


	unsigned short	nDoorAccess			:4;
	unsigned short	nCamera				:4;

	unsigned short	nUNUSED_A	:4;
	unsigned short	nFlexDisk	:4;
	unsigned short	nUNUSED_B	:8;

	unsigned		Cassette[XL_NDC_MAX_CASSETTE];

	unsigned short	nUNUSED2_A:4; 
	unsigned short	nStatementPrinter:4;
	unsigned short	nUNUSED2_B:8;
#endif
	
} XL_NDC_HW_FITNESS;

#define XL_NDC_SUPPLIES_NOT_CONFIG		0
#define XL_NDC_SUPPLIES_GOOD_STATE		1
#define XL_NDC_SUPPLIES_MEDIA_LOW 		2
#define XL_NDC_SUPPLIES_MEDIA_OUT 		3
#define XL_NDC_SUPPLIES_OVERFILL  		4

typedef struct xl_ndc_supplies
{
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
	unsigned 		Cassette[XL_NDC_MAX_CASSETTE];
	unsigned short	nStatementPaper		:4;
	unsigned short	nStatementRibbon	:4;
	unsigned short	nStatementPrintHead	:4;
	unsigned short	nStatementKnife		:4;
	unsigned short	nStatementCaptureBin:4;
	unsigned short	nCamera				:4;
	unsigned short	nUNUSED2			:8;
	
} XL_NDC_SUPPLIES;


#define XL_NDC_SENSORS_INACTIVE			0
#define XL_NDC_SENSORS_ACTIVE			1

typedef struct xl_ndc_sensors
{
	unsigned short	nSupervisorMode		:4;
	unsigned short	nVibrationHeat		:4;
	unsigned short	nDoorContact		:4;
	unsigned short	nSilentSignal		:4;
	unsigned short	nElectronicsEnclosure:4;
	unsigned short	nDepositBin			:4;
	unsigned short	nCardCaptureBin		:4;
	unsigned short	nCashRejectBin		:4;
	unsigned short	CassetteSlot[XL_NDC_MAX_CASSETTE];
	
} XL_NDC_SENSORS;


typedef struct xl_ndc_counters
{
	time_t			LastUpdate;
	char			LastTrxSerialNo[XL_ATM_LEN_TRANS_NO+1];
	USINT32			nAccumTrxCount;
	
	USINT16			NotesInCassette[XL_NDC_MAX_CASSETTE];
	USINT16			NotesRejectedCassette[XL_NDC_MAX_CASSETTE];
	USINT16			NotesDispensedCassette[XL_NDC_MAX_CASSETTE];
	USINT16			LastNotesDispensedCassette[XL_NDC_MAX_CASSETTE];
	
	USINT16			nCardsCaptured;
	USINT16			nEnvelopeDeposited;
	USINT16			nLastEnvelopeSerialNo;
	USINT16			nCameraFilm;

} XL_NDC_COUNTERS;



typedef struct xl_ndc_central_counters
{
	time_t			LastBalancingTimestamp;
	USINT16			nLastBusinessCycle;
	USINT32			nLastStan;
	USINT16			nUnused;				/*	HOLE  */
	char			cLastMsgCoordinationNo;
	USINT16			nLastTrxSerialNo;
	USINT16			LastNotesDispensedCassette[XL_NDC_MAX_CASSETTE];
	USINT32			nAccumTrxCount;
	USINT16			NotesInCassette[XL_NDC_MAX_CASSETTE];
	USINT16			NotesDispensedCassette[XL_NDC_MAX_CASSETTE];

} XL_NDC_CENTRAL_COUNTERS;



typedef struct xl_ndc_daily_counters
{
/*	981001
    USINT16         nRqTrx;
    USINT16         nRqReject;
    INT32           RqDebit[2];
    INT32           RqCredit[2];

    USINT16         nRqRvsTrx;
    INT32           RqRvsDebit[2];
    INT32           RqRvsCredit[2];
*/
	
	USINT16			nNumCr;
	double			TtCrAmt;
	double			dTtCrProcFee;
	double			dTtCrTFee;

	USINT16			nNumDb;
	double			TtDbAmt;
	double			dTtDbProcFee;
	double			dTtDbTFee;

	USINT16			nNumTrf;
	USINT16			nNumInq;
	USINT16			nNumAuth;
	USINT16			nNumReject;

	USINT16			nNumRvsCr;
	double			TtRvsCrAmt;
	USINT16			nNumRvsDb;
	double			TtRvsDbAmt;
	USINT16			nNumRvsTrf;

} XL_NDC_DAILY_COUNTERS;



typedef struct xl_ndc_status
{
	unsigned short			bFullDownload		:1;
	unsigned short			nUNUSED				:15;
	unsigned				nConfigVer;
	USINT16					nLanguageIdx;
	time_t					nLastEchoTimestamp;
	time_t					nLastInactivityTimestamp;
	time_t					nLastKeyReplTimestamp;
	USINT32					nNotesDenomination[4];

/*	981007	XL_NDC_DAILY_COUNTERS	DailyCounters; */
	XL_NDC_CENTRAL_COUNTERS	CentralCounters;
	XL_NDC_HW_CONFIG		HWConfig;
	XL_NDC_HW_FITNESS		HWFitness;
	XL_NDC_SUPPLIES			Supplies;
	XL_NDC_SENSORS			SensorStatus;
	XL_NDC_COUNTERS			SupplyCounters;

} XL_NDC_STATUS;

#define XL_TIMESTAMP_SIZE        20

typedef struct XUISysRunAlarm
{
	unsigned short	unPointStatus;
	char		strTimestamp[XL_TIMESTAMP_SIZE];
	char		strUserId[XL_OBJECT_NAME_SIZE];
	unsigned int	unSeq;
}XUI_SYSRUN_ALARM;



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


typedef struct  struct_test
{
        unsigned short a:4;
        unsigned short b:4;
        unsigned short c:4;
        unsigned short d:4;
        unsigned short e:4;
} STRUCT_TEST;

#pragma pack()

#endif
