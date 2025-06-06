/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-OM-DataTypes"
 * 	found in "MAP-OM-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_TraceDepthList_H_
#define	_TraceDepthList_H_


#include <asn_application.h>

/* Including external dependencies */
#include "TraceDepth.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TraceDepthList */
typedef struct TraceDepthList {
	TraceDepth_t	*msc_s_TraceDepth	/* OPTIONAL */;
	TraceDepth_t	*mgw_TraceDepth	/* OPTIONAL */;
	TraceDepth_t	*sgsn_TraceDepth	/* OPTIONAL */;
	TraceDepth_t	*ggsn_TraceDepth	/* OPTIONAL */;
	TraceDepth_t	*rnc_TraceDepth	/* OPTIONAL */;
	TraceDepth_t	*bmsc_TraceDepth	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	TraceDepth_t	*mme_TraceDepth	/* OPTIONAL */;
	TraceDepth_t	*sgw_TraceDepth	/* OPTIONAL */;
	TraceDepth_t	*pgw_TraceDepth	/* OPTIONAL */;
	TraceDepth_t	*eNB_TraceDepth	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TraceDepthList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TraceDepthList;

#ifdef __cplusplus
}
#endif

#endif	/* _TraceDepthList_H_ */
#include <asn_internal.h>
