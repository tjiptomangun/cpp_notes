/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-CommonDataTypes"
 * 	found in "MAP-CommonDataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_NAEA_PreferredCI_H_
#define	_NAEA_PreferredCI_H_


#include <asn_application.h>

/* Including external dependencies */
#include "NAEA-CIC.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionContainer;

/* NAEA-PreferredCI */
typedef struct NAEA_PreferredCI {
	NAEA_CIC_t	 naea_PreferredCIC;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} NAEA_PreferredCI_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_NAEA_PreferredCI;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionContainer.h"

#endif	/* _NAEA_PreferredCI_H_ */
#include <asn_internal.h>
