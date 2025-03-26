#include<stdio.h>
#include<time.h>
#define XL_ATM_MAX_CASSETTE 4

typedef struct xl_atm_central_counters2
{
#pragma pack(1)
	time_t				LastBalancingTimestamp;
	unsigned short		nLastBusinessCycle;
	unsigned int		nLastStan;
	unsigned short		nUnused;				/*	HOLE  */
	char				cLastMsgCoordinationNo;
	unsigned short		nLastTrxSerialNo;
	unsigned short		LastNotesDispensedCassette[XL_ATM_MAX_CASSETTE];
	unsigned int		nAccumTrxCount;
	unsigned short		NotesInCassette[XL_ATM_MAX_CASSETTE];
	unsigned short		NotesDispensedCassette[XL_ATM_MAX_CASSETTE];
	// NH 20110309 Handle Cash Deposit Machine (CDM)
	unsigned short		nBNA;

} XL_ATM_CENTRAL_COUNTERS2;


typedef struct xl_atm_central_counters
{

	time_t				LastBalancingTimestamp;
	unsigned short		nLastBusinessCycle;
	unsigned int		nLastStan;
	unsigned short		nUnused;				/*	HOLE  */
	char				cLastMsgCoordinationNo;
	unsigned short      nLastTrxSerialNo;
	unsigned short      LastNotesDispensedCassette[XL_ATM_MAX_CASSETTE];
	unsigned int		nAccumTrxCount;
	unsigned short		NotesInCassette[XL_ATM_MAX_CASSETTE];
	unsigned short		NotesDispensedCassette[XL_ATM_MAX_CASSETTE];
	// NH 20110309 Handle Cash Deposit Machine (CDM)
	unsigned short		nBNA;

} XL_ATM_CENTRAL_COUNTERS;


typedef struct xl_ndc_status
{
	unsigned short			bFullDownload		:1;
	unsigned short			nUNUSED				:15;
	unsigned				nConfigVer;
	unsigned short			nLanguageIdx;
	time_t					nLastEchoTimestamp;
	time_t					nLastInactivityTimestamp;
	time_t					nLastKeyReplTimestamp;
	unsigned int			nNotesDenomination[4];



} XL_NDC_STATUS;

typedef struct xl_ndc_status2
{
	unsigned short			bFullDownload		:1;
	unsigned				nConfigVer;
	unsigned short			nLanguageIdx;
	time_t					nLastEchoTimestamp;
	time_t					nLastInactivityTimestamp;
	time_t					nLastKeyReplTimestamp;
	unsigned int			nNotesDenomination[4];



} XL_NDC_STATUS2;


int main(int argc, char *argv[])
{
   printf("1 sz =%d\n",sizeof(XL_ATM_CENTRAL_COUNTERS2));
   printf("2 sz =%d\n",sizeof(XL_ATM_CENTRAL_COUNTERS));
   printf("1 sz =%d\n",sizeof(XL_NDC_STATUS));
   printf("2 sz =%d\n",sizeof(XL_NDC_STATUS2));
}