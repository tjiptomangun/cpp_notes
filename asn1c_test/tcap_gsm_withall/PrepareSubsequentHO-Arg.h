/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_PrepareSubsequentHO_Arg_H_
#define	_PrepareSubsequentHO_Arg_H_


#include <asn_application.h>

/* Including external dependencies */
#include "GlobalCellId.h"
#include "ISDN-AddressString.h"
#include "RNCId.h"
#include "RAB-Id.h"
#include "GERAN-Classmark.h"
#include <NULL.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct AccessNetworkSignalInfo;
struct ExtensionContainer;

/* PrepareSubsequentHO-Arg */
typedef struct PrepareSubsequentHO_Arg {
	GlobalCellId_t	*targetCellId	/* OPTIONAL */;
	ISDN_AddressString_t	 targetMSC_Number;
	RNCId_t	*targetRNCId	/* OPTIONAL */;
	struct AccessNetworkSignalInfo	*an_APDU	/* OPTIONAL */;
	RAB_Id_t	*selectedRab_Id	/* OPTIONAL */;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	GERAN_Classmark_t	*geran_classmark	/* OPTIONAL */;
	NULL_t	*rab_ConfigurationIndicator	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PrepareSubsequentHO_Arg_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PrepareSubsequentHO_Arg;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "AccessNetworkSignalInfo.h"
#include "ExtensionContainer.h"

#endif	/* _PrepareSubsequentHO_Arg_H_ */
#include <asn_internal.h>
