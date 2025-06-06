/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DummyMAP"
 * 	found in "GSMMAP.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_ErrorCode_H_
#define	_ErrorCode_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <OBJECT_IDENTIFIER.h>
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ErrorCode_PR {
	ErrorCode_PR_NOTHING,	/* No components present */
	ErrorCode_PR_localValue,
	ErrorCode_PR_globalValue
} ErrorCode_PR;

/* ErrorCode */
typedef struct ErrorCode {
	ErrorCode_PR present;
	union ErrorCode_u {
		long	 localValue;
		OBJECT_IDENTIFIER_t	 globalValue;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ErrorCode_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ErrorCode;

#ifdef __cplusplus
}
#endif

#endif	/* _ErrorCode_H_ */
#include <asn_internal.h>
