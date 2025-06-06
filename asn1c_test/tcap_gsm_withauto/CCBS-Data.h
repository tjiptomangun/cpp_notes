/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-SS-DataTypes"
 * 	found in "MAP-SS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_CCBS_Data_H_
#define	_CCBS_Data_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CCBS-Feature.h"
#include "ISDN-AddressString.h"
#include "ServiceIndicator.h"
#include "ExternalSignalInfo.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CCBS-Data */
typedef struct CCBS_Data {
	CCBS_Feature_t	 ccbs_Feature;
	ISDN_AddressString_t	 translatedB_Number;
	ServiceIndicator_t	*serviceIndicator	/* OPTIONAL */;
	ExternalSignalInfo_t	 callInfo;
	ExternalSignalInfo_t	 networkSignalInfo;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CCBS_Data_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CCBS_Data;

#ifdef __cplusplus
}
#endif

#endif	/* _CCBS_Data_H_ */
#include <asn_internal.h>
