/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_PrepareHO_Res_H_
#define	_PrepareHO_Res_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ISDN-AddressString.h"
#include "MulticallBearerInfo.h"
#include <NULL.h>
#include "Codec.h"
#include "AoIPCodec.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct RelocationNumberList;
struct AccessNetworkSignalInfo;
struct SelectedUMTS_Algorithms;
struct ChosenRadioResourceInformation;
struct ExtensionContainer;
struct CodecList;
struct AoIPCodecsList;

/* PrepareHO-Res */
typedef struct PrepareHO_Res {
	ISDN_AddressString_t	*handoverNumber	/* OPTIONAL */;
	struct RelocationNumberList	*relocationNumberList	/* OPTIONAL */;
	struct AccessNetworkSignalInfo	*an_APDU	/* OPTIONAL */;
	MulticallBearerInfo_t	*multicallBearerInfo	/* OPTIONAL */;
	NULL_t	*multipleBearerNotSupported	/* OPTIONAL */;
	struct SelectedUMTS_Algorithms	*selectedUMTS_Algorithms	/* OPTIONAL */;
	struct ChosenRadioResourceInformation	*chosenRadioResourceInformation	/* OPTIONAL */;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	Codec_t	*iuSelectedCodec	/* OPTIONAL */;
	struct CodecList	*iuAvailableCodecsList	/* OPTIONAL */;
	AoIPCodec_t	*aoipSelectedCodecTarget	/* OPTIONAL */;
	struct AoIPCodecsList	*aoipAvailableCodecsListMap	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PrepareHO_Res_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PrepareHO_Res;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "RelocationNumberList.h"
#include "AccessNetworkSignalInfo.h"
#include "SelectedUMTS-Algorithms.h"
#include "ChosenRadioResourceInformation.h"
#include "ExtensionContainer.h"
#include "CodecList.h"
#include "AoIPCodecsList.h"

#endif	/* _PrepareHO_Res_H_ */
#include <asn_internal.h>
