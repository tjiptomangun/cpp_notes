/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-SS-DataTypes"
 * 	found in "MAP-SS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_CCBS_FeatureList_H_
#define	_CCBS_FeatureList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct CCBS_Feature;

/* CCBS-FeatureList */
typedef struct CCBS_FeatureList {
	A_SEQUENCE_OF(struct CCBS_Feature) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CCBS_FeatureList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CCBS_FeatureList;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "CCBS-Feature.h"

#endif	/* _CCBS_FeatureList_H_ */
#include <asn_internal.h>
