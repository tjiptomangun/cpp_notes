/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_UserCSGInformation_H_
#define	_UserCSGInformation_H_


#include <asn_application.h>

/* Including external dependencies */
#include "CSG-Id.h"
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionContainer;

/* UserCSGInformation */
typedef struct UserCSGInformation {
	CSG_Id_t	 csg_Id;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	OCTET_STRING_t	*accessMode	/* OPTIONAL */;
	OCTET_STRING_t	*cmi	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} UserCSGInformation_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UserCSGInformation;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionContainer.h"

#endif	/* _UserCSGInformation_H_ */
#include <asn_internal.h>
