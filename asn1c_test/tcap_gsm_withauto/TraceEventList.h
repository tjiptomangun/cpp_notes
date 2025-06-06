/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-OM-DataTypes"
 * 	found in "MAP-OM-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_TraceEventList_H_
#define	_TraceEventList_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MSC-S-EventList.h"
#include "MGW-EventList.h"
#include "SGSN-EventList.h"
#include "GGSN-EventList.h"
#include "BMSC-EventList.h"
#include "MME-EventList.h"
#include "SGW-EventList.h"
#include "PGW-EventList.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TraceEventList */
typedef struct TraceEventList {
	MSC_S_EventList_t	*msc_s_List	/* OPTIONAL */;
	MGW_EventList_t	*mgw_List	/* OPTIONAL */;
	SGSN_EventList_t	*sgsn_List	/* OPTIONAL */;
	GGSN_EventList_t	*ggsn_List	/* OPTIONAL */;
	BMSC_EventList_t	*bmsc_List	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	MME_EventList_t	*mme_List	/* OPTIONAL */;
	SGW_EventList_t	*sgw_List	/* OPTIONAL */;
	PGW_EventList_t	*pgw_List	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TraceEventList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TraceEventList;

#ifdef __cplusplus
}
#endif

#endif	/* _TraceEventList_H_ */
#include <asn_internal.h>
