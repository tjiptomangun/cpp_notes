/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_Ext_SS_InfoFor_CSE_H_
#define	_Ext_SS_InfoFor_CSE_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Ext-ForwardingInfoFor-CSE.h"
#include "Ext-CallBarringInfoFor-CSE.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Ext_SS_InfoFor_CSE_PR {
	Ext_SS_InfoFor_CSE_PR_NOTHING,	/* No components present */
	Ext_SS_InfoFor_CSE_PR_forwardingInfoFor_CSE,
	Ext_SS_InfoFor_CSE_PR_callBarringInfoFor_CSE
} Ext_SS_InfoFor_CSE_PR;

/* Ext-SS-InfoFor-CSE */
typedef struct Ext_SS_InfoFor_CSE {
	Ext_SS_InfoFor_CSE_PR present;
	union Ext_SS_InfoFor_CSE_u {
		Ext_ForwardingInfoFor_CSE_t	 forwardingInfoFor_CSE;
		Ext_CallBarringInfoFor_CSE_t	 callBarringInfoFor_CSE;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Ext_SS_InfoFor_CSE_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Ext_SS_InfoFor_CSE;

#ifdef __cplusplus
}
#endif

#endif	/* _Ext_SS_InfoFor_CSE_H_ */
#include <asn_internal.h>
