/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-ER-DataTypes"
 * 	found in "MAP-ER-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_SubBusyForMT_SMS_Param_H_
#define	_SubBusyForMT_SMS_Param_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionContainer;

/* SubBusyForMT-SMS-Param */
typedef struct SubBusyForMT_SMS_Param {
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	NULL_t	*gprsConnectionSuspended	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SubBusyForMT_SMS_Param_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SubBusyForMT_SMS_Param;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionContainer.h"

#endif	/* _SubBusyForMT_SMS_Param_H_ */
#include <asn_internal.h>
