/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DummyMAP"
 * 	found in "GSMMAP.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_ForwardSM_Arg_H_
#define	_ForwardSM_Arg_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SM-RP-DAold.h"
#include "SM-RP-OAold.h"
#include "SignalInfo.h"
#include <NULL.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ForwardSM-Arg */
typedef struct ForwardSM_Arg {
	SM_RP_DAold_t	 sm_RP_DA;
	SM_RP_OAold_t	 sm_RP_OA;
	SignalInfo_t	 sm_RP_UI;
	NULL_t	*moreMessagesToSend	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ForwardSM_Arg_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ForwardSM_Arg;

#ifdef __cplusplus
}
#endif

#endif	/* _ForwardSM_Arg_H_ */
#include <asn_internal.h>
