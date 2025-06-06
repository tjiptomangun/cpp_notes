/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#include "NoteMM-EventArg.h"

static asn_TYPE_member_t asn_MBR_NoteMM_EventArg_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct NoteMM_EventArg, serviceKey),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_ServiceKey,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"serviceKey"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct NoteMM_EventArg, eventMet),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MM_Code,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"eventMet"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct NoteMM_EventArg, imsi),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IMSI,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"imsi"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct NoteMM_EventArg, msisdn),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ISDN_AddressString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"msisdn"
		},
	{ ATF_POINTER, 5, offsetof(struct NoteMM_EventArg, locationInformation),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LocationInformation,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"locationInformation"
		},
	{ ATF_POINTER, 4, offsetof(struct NoteMM_EventArg, supportedCAMELPhases),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SupportedCamelPhases,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"supportedCAMELPhases"
		},
	{ ATF_POINTER, 3, offsetof(struct NoteMM_EventArg, extensionContainer),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ExtensionContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"extensionContainer"
		},
	{ ATF_POINTER, 2, offsetof(struct NoteMM_EventArg, locationInformationGPRS),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LocationInformationGPRS,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"locationInformationGPRS"
		},
	{ ATF_POINTER, 1, offsetof(struct NoteMM_EventArg, offeredCamel4Functionalities),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_OfferedCamel4Functionalities,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"offeredCamel4Functionalities"
		},
};
static const ber_tlv_tag_t asn_DEF_NoteMM_EventArg_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_NoteMM_EventArg_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 0, 0, 0 }, /* serviceKey */
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 1, 0, 0 }, /* eventMet */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 2, 0, 0 }, /* imsi */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 3, 0, 0 }, /* msisdn */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 4, 0, 0 }, /* locationInformation */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* supportedCAMELPhases */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* extensionContainer */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* locationInformationGPRS */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 } /* offeredCamel4Functionalities */
};
static asn_SEQUENCE_specifics_t asn_SPC_NoteMM_EventArg_specs_1 = {
	sizeof(struct NoteMM_EventArg),
	offsetof(struct NoteMM_EventArg, _asn_ctx),
	asn_MAP_NoteMM_EventArg_tag2el_1,
	9,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	6,	/* Start extensions */
	10	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_NoteMM_EventArg = {
	"NoteMM-EventArg",
	"NoteMM-EventArg",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_NoteMM_EventArg_tags_1,
	sizeof(asn_DEF_NoteMM_EventArg_tags_1)
		/sizeof(asn_DEF_NoteMM_EventArg_tags_1[0]), /* 1 */
	asn_DEF_NoteMM_EventArg_tags_1,	/* Same as above */
	sizeof(asn_DEF_NoteMM_EventArg_tags_1)
		/sizeof(asn_DEF_NoteMM_EventArg_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_NoteMM_EventArg_1,
	9,	/* Elements count */
	&asn_SPC_NoteMM_EventArg_specs_1	/* Additional specs */
};

