/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-ER-DataTypes"
 * 	found in "MAP-ER-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_IllegalSubscriberParam_H_
#define	_IllegalSubscriberParam_H_


#include <asn_application.h>

/* Including external dependencies */
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionContainer;

/* IllegalSubscriberParam */
typedef struct IllegalSubscriberParam {
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} IllegalSubscriberParam_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IllegalSubscriberParam;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionContainer.h"

#endif	/* _IllegalSubscriberParam_H_ */
#include <asn_internal.h>
