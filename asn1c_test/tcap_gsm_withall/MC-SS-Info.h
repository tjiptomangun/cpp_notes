/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-CommonDataTypes"
 * 	found in "MAP-CommonDataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_MC_SS_Info_H_
#define	_MC_SS_Info_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SS-Code.h"
#include "Ext-SS-Status.h"
#include "MaxMC-Bearers.h"
#include "MC-Bearers.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionContainer;

/* MC-SS-Info */
typedef struct MC_SS_Info {
	SS_Code_t	 ss_Code;
	Ext_SS_Status_t	 ss_Status;
	MaxMC_Bearers_t	 nbrSB;
	MC_Bearers_t	 nbrUser;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MC_SS_Info_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MC_SS_Info;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionContainer.h"

#endif	/* _MC_SS_Info_H_ */
#include <asn_internal.h>
