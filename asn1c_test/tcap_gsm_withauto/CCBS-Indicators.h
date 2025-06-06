/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-CH-DataTypes"
 * 	found in "MAP-CH-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_CCBS_Indicators_H_
#define	_CCBS_Indicators_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionContainer;

/* CCBS-Indicators */
typedef struct CCBS_Indicators {
	NULL_t	*ccbs_Possible	/* OPTIONAL */;
	NULL_t	*keepCCBS_CallIndicator	/* OPTIONAL */;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CCBS_Indicators_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CCBS_Indicators;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionContainer.h"

#endif	/* _CCBS_Indicators_H_ */
#include <asn_internal.h>
