/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-SS-DataTypes"
 * 	found in "MAP-SS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_RegisterCC_EntryRes_H_
#define	_RegisterCC_EntryRes_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct CCBS_Feature;

/* RegisterCC-EntryRes */
typedef struct RegisterCC_EntryRes {
	struct CCBS_Feature	*ccbs_Feature	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RegisterCC_EntryRes_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RegisterCC_EntryRes;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "CCBS-Feature.h"

#endif	/* _RegisterCC_EntryRes_H_ */
#include <asn_internal.h>
