/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_VBSDataList_H_
#define	_VBSDataList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct VoiceBroadcastData;

/* VBSDataList */
typedef struct VBSDataList {
	A_SEQUENCE_OF(struct VoiceBroadcastData) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} VBSDataList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_VBSDataList;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "VoiceBroadcastData.h"

#endif	/* _VBSDataList_H_ */
#include <asn_internal.h>
