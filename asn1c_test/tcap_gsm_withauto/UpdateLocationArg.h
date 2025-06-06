/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_UpdateLocationArg_H_
#define	_UpdateLocationArg_H_


#include <asn_application.h>

/* Including external dependencies */
#include "IMSI.h"
#include "ISDN-AddressString.h"
#include "LMSI.h"
#include <NULL.h>
#include "GSN-Address.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionContainer;
struct VLR_Capability;
struct ADD_Info;
struct PagingArea;

/* UpdateLocationArg */
typedef struct UpdateLocationArg {
	IMSI_t	 imsi;
	ISDN_AddressString_t	 msc_Number;
	ISDN_AddressString_t	 vlr_Number;
	LMSI_t	*lmsi	/* OPTIONAL */;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct VLR_Capability	*vlr_Capability	/* OPTIONAL */;
	NULL_t	*informPreviousNetworkEntity	/* OPTIONAL */;
	NULL_t	*cs_LCS_NotSupportedByUE	/* OPTIONAL */;
	GSN_Address_t	*v_gmlc_Address	/* OPTIONAL */;
	struct ADD_Info	*add_info	/* OPTIONAL */;
	struct PagingArea	*pagingArea	/* OPTIONAL */;
	NULL_t	*skipSubscriberDataUpdate	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UpdateLocationArg_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UpdateLocationArg;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionContainer.h"
#include "VLR-Capability.h"
#include "ADD-Info.h"
#include "PagingArea.h"

#endif	/* _UpdateLocationArg_H_ */
#include <asn_internal.h>
