/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-SS-DataTypes"
 * 	found in "MAP-SS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_CCBS_Feature_H_
#define	_CCBS_Feature_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CCBS-Index.h"
#include "ISDN-AddressString.h"
#include "ISDN-SubaddressString.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct BasicServiceCode;

/* CCBS-Feature */
typedef struct CCBS_Feature {
	CCBS_Index_t	*ccbs_Index	/* OPTIONAL */;
	ISDN_AddressString_t	*b_subscriberNumber	/* OPTIONAL */;
	ISDN_SubaddressString_t	*b_subscriberSubaddress	/* OPTIONAL */;
	struct BasicServiceCode	*basicServiceGroup	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CCBS_Feature_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CCBS_Feature;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "BasicServiceCode.h"

#endif	/* _CCBS_Feature_H_ */
#include <asn_internal.h>
