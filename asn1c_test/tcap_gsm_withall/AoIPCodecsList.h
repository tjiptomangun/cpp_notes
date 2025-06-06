/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_AoIPCodecsList_H_
#define	_AoIPCodecsList_H_


#include <asn_application.h>

/* Including external dependencies */
#include "AoIPCodec.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ExtensionContainer;

/* AoIPCodecsList */
typedef struct AoIPCodecsList {
	AoIPCodec_t	 codec1;
	AoIPCodec_t	*codec2	/* OPTIONAL */;
	AoIPCodec_t	*codec3	/* OPTIONAL */;
	AoIPCodec_t	*codec4	/* OPTIONAL */;
	AoIPCodec_t	*codec5	/* OPTIONAL */;
	AoIPCodec_t	*codec6	/* OPTIONAL */;
	AoIPCodec_t	*codec7	/* OPTIONAL */;
	AoIPCodec_t	*codec8	/* OPTIONAL */;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} AoIPCodecsList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AoIPCodecsList;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ExtensionContainer.h"

#endif	/* _AoIPCodecsList_H_ */
#include <asn_internal.h>
