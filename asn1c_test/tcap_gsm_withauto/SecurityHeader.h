/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DummyMAP"
 * 	found in "GSMMAP.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_SecurityHeader_H_
#define	_SecurityHeader_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SecurityParametersIndex.h"
#include "OriginalComponentIdentifier.h"
#include "InitialisationVector.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SecurityHeader */
typedef struct SecurityHeader {
	SecurityParametersIndex_t	 securityParametersIndex;
	OriginalComponentIdentifier_t	 originalComponentIdentifier;
	InitialisationVector_t	*initialisationVector	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SecurityHeader_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SecurityHeader;

#ifdef __cplusplus
}
#endif

#endif	/* _SecurityHeader_H_ */
#include <asn_internal.h>
