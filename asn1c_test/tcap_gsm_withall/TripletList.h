/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_TripletList_H_
#define	_TripletList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct AuthenticationTriplet;

/* TripletList */
typedef struct TripletList {
	A_SEQUENCE_OF(struct AuthenticationTriplet) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TripletList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TripletList;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "AuthenticationTriplet.h"

#endif	/* _TripletList_H_ */
#include <asn_internal.h>
