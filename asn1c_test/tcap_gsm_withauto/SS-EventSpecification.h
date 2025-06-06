/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-SS-DataTypes"
 * 	found in "MAP-SS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_SS_EventSpecification_H_
#define	_SS_EventSpecification_H_


#include <asn_application.h>

/* Including external dependencies */
#include "AddressString.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SS-EventSpecification */
typedef struct SS_EventSpecification {
	A_SEQUENCE_OF(AddressString_t) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SS_EventSpecification_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SS_EventSpecification;

#ifdef __cplusplus
}
#endif

#endif	/* _SS_EventSpecification_H_ */
#include <asn_internal.h>
