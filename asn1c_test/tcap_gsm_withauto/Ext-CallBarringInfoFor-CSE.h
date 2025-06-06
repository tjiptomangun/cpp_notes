/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_Ext_CallBarringInfoFor_CSE_H_
#define	_Ext_CallBarringInfoFor_CSE_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SS-Code.h"
#include "Ext-CallBarFeatureList.h"
#include "Password.h"
#include "WrongPasswordAttemptsCounter.h"
#include <NULL.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionContainer;

/* Ext-CallBarringInfoFor-CSE */
typedef struct Ext_CallBarringInfoFor_CSE {
	SS_Code_t	 ss_Code;
	Ext_CallBarFeatureList_t	 callBarringFeatureList;
	Password_t	*password	/* OPTIONAL */;
	WrongPasswordAttemptsCounter_t	*wrongPasswordAttemptsCounter	/* OPTIONAL */;
	NULL_t	*notificationToCSE	/* OPTIONAL */;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Ext_CallBarringInfoFor_CSE_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Ext_CallBarringInfoFor_CSE;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionContainer.h"

#endif	/* _Ext_CallBarringInfoFor_CSE_H_ */
#include <asn_internal.h>
