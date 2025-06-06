/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_CheckIMEI_Res_H_
#define	_CheckIMEI_Res_H_


#include <asn_application.h>

/* Including external dependencies */
#include "EquipmentStatus.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct UESBI_Iu;
struct ExtensionContainer;

/* CheckIMEI-Res */
typedef struct CheckIMEI_Res {
	EquipmentStatus_t	*equipmentStatus	/* OPTIONAL */;
	struct UESBI_Iu	*bmuef	/* OPTIONAL */;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CheckIMEI_Res_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CheckIMEI_Res;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "UESBI-Iu.h"
#include "ExtensionContainer.h"

#endif	/* _CheckIMEI_Res_H_ */
#include <asn_internal.h>
