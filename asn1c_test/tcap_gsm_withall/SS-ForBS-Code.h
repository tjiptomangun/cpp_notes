/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-SS-DataTypes"
 * 	found in "MAP-SS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_SS_ForBS_Code_H_
#define	_SS_ForBS_Code_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SS-Code.h"
#include <NULL.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct BasicServiceCode;

/* SS-ForBS-Code */
typedef struct SS_ForBS_Code {
	SS_Code_t	 ss_Code;
	struct BasicServiceCode	*basicService	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	NULL_t	*longFTN_Supported	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SS_ForBS_Code_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SS_ForBS_Code;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "BasicServiceCode.h"

#endif	/* _SS_ForBS_Code_H_ */
#include <asn_internal.h>
