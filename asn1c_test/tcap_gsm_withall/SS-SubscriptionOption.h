/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-SS-DataTypes"
 * 	found in "MAP-SS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_SS_SubscriptionOption_H_
#define	_SS_SubscriptionOption_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CliRestrictionOption.h"
#include "OverrideCategory.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SS_SubscriptionOption_PR {
	SS_SubscriptionOption_PR_NOTHING,	/* No components present */
	SS_SubscriptionOption_PR_cliRestrictionOption,
	SS_SubscriptionOption_PR_overrideCategory
} SS_SubscriptionOption_PR;

/* SS-SubscriptionOption */
typedef struct SS_SubscriptionOption {
	SS_SubscriptionOption_PR present;
	union SS_SubscriptionOption_u {
		CliRestrictionOption_t	 cliRestrictionOption;
		OverrideCategory_t	 overrideCategory;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SS_SubscriptionOption_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SS_SubscriptionOption;

#ifdef __cplusplus
}
#endif

#endif	/* _SS_SubscriptionOption_H_ */
#include <asn_internal.h>
