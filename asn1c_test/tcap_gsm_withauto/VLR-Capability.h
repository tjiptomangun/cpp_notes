/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_VLR_Capability_H_
#define	_VLR_Capability_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SupportedCamelPhases.h"
#include <NULL.h>
#include "IST-SupportIndicator.h"
#include "SupportedLCS-CapabilitySets.h"
#include "OfferedCamel4CSIs.h"
#include "SupportedRAT-Types.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionContainer;
struct SuperChargerInfo;

/* VLR-Capability */
typedef struct VLR_Capability {
	SupportedCamelPhases_t	*supportedCamelPhases	/* OPTIONAL */;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	NULL_t	*solsaSupportIndicator	/* OPTIONAL */;
	IST_SupportIndicator_t	*istSupportIndicator	/* OPTIONAL */;
	struct SuperChargerInfo	*superChargerSupportedInServingNetworkEntity	/* OPTIONAL */;
	NULL_t	*longFTN_Supported	/* OPTIONAL */;
	SupportedLCS_CapabilitySets_t	*supportedLCS_CapabilitySets	/* OPTIONAL */;
	OfferedCamel4CSIs_t	*offeredCamel4CSIs	/* OPTIONAL */;
	SupportedRAT_Types_t	*supportedRAT_TypesIndicator	/* OPTIONAL */;
	NULL_t	*longGroupID_Supported	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} VLR_Capability_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_VLR_Capability;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionContainer.h"
#include "SuperChargerInfo.h"

#endif	/* _VLR_Capability_H_ */
#include <asn_internal.h>
