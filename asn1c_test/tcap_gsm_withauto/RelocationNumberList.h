/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_RelocationNumberList_H_
#define	_RelocationNumberList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct RelocationNumber;

/* RelocationNumberList */
typedef struct RelocationNumberList {
	A_SEQUENCE_OF(struct RelocationNumber) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RelocationNumberList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RelocationNumberList;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "RelocationNumber.h"

#endif	/* _RelocationNumberList_H_ */
#include <asn_internal.h>
