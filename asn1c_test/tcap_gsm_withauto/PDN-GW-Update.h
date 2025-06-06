/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_PDN_GW_Update_H_
#define	_PDN_GW_Update_H_


#include <asn_application.h>

/* Including external dependencies */
#include "APN.h"
#include "ContextId.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct PDN_GW_Identity;
struct ExtensionContainer;

/* PDN-GW-Update */
typedef struct PDN_GW_Update {
	APN_t	*apn	/* OPTIONAL */;
	struct PDN_GW_Identity	*pdn_gw_Identity	/* OPTIONAL */;
	ContextId_t	*contextId	/* OPTIONAL */;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PDN_GW_Update_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PDN_GW_Update;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "PDN-GW-Identity.h"
#include "ExtensionContainer.h"

#endif	/* _PDN_GW_Update_H_ */
#include <asn_internal.h>
