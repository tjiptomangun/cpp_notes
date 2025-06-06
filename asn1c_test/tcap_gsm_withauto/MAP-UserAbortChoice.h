/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-DialogueInformation"
 * 	found in "MAP-DialogueInformation.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_MAP_UserAbortChoice_H_
#define	_MAP_UserAbortChoice_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NULL.h>
#include "ResourceUnavailableReason.h"
#include "ProcedureCancellationReason.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MAP_UserAbortChoice_PR {
	MAP_UserAbortChoice_PR_NOTHING,	/* No components present */
	MAP_UserAbortChoice_PR_userSpecificReason,
	MAP_UserAbortChoice_PR_userResourceLimitation,
	MAP_UserAbortChoice_PR_resourceUnavailable,
	MAP_UserAbortChoice_PR_applicationProcedureCancellation
} MAP_UserAbortChoice_PR;

/* MAP-UserAbortChoice */
typedef struct MAP_UserAbortChoice {
	MAP_UserAbortChoice_PR present;
	union MAP_UserAbortChoice_u {
		NULL_t	 userSpecificReason;
		NULL_t	 userResourceLimitation;
		ResourceUnavailableReason_t	 resourceUnavailable;
		ProcedureCancellationReason_t	 applicationProcedureCancellation;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MAP_UserAbortChoice_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MAP_UserAbortChoice;

#ifdef __cplusplus
}
#endif

#endif	/* _MAP_UserAbortChoice_H_ */
#include <asn_internal.h>
