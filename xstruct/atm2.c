#include <stdio.h>
#include <stdlib.h>
#define XL_ATM_MAX_DENOMINATION_TYPE            8 
#define XL_ATM_LEN_TRANS_NO                     10
#define USINT16 unsigned short
#define USINT32 unsigned
#pragma pack(1)
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

typedef struct xl_ndc_hw_fitness
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
} XL_NDC_HW_FITNESS;
typedef struct xl_ndc_supplies
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
} XL_NDC_SUPPLIES;


#define XL_NDC_SENSORS_INACTIVE			0
#define XL_NDC_SENSORS_ACTIVE			1

typedef struct xl_ndc_sensors
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

	
} XL_NDC_SENSORS;


typedef struct xl_ndc_counters
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

} XL_NDC_COUNTERS;



typedef struct xl_ndc_central_counters
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
#ifdef _LE_
	unsigned short			bFullDownload		:1;
	unsigned short			nUNUSED				:15;
#else
	unsigned short			nUNUSED_A		:7;
	unsigned short			bFullDownload:1;
	unsigned short			nUNUSED_B		:8;
#endif
	unsigned				nConfigVer;
	USINT16					nLanguageIdx;
	time_t					nLastEchoTimestamp;
	time_t					nLastInactivityTimestamp;
	time_t					nLastKeyReplTimestamp;
	USINT32					nNotesDenomination[XL_ATM_MAX_DENOMINATION_TYPE];

/*	981007	XL_NDC_DAILY_COUNTERS	DailyCounters; */
	XL_NDC_CENTRAL_COUNTERS	CentralCounters;
	XL_NDC_HW_CONFIG		HWConfig;
	XL_NDC_HW_FITNESS		HWFitness;
	XL_NDC_SUPPLIES			Supplies;
	XL_NDC_SENSORS			SensorStatus;
	XL_NDC_COUNTERS			SupplyCounters;

} XL_NDC_STATUS;
#pragma pack()
int main (int argc, char **argv)
{	
	fprintf (stdout, "size = %d \n", (int)sizeof (XL_NDC_STATUS));
	return 0;
}
