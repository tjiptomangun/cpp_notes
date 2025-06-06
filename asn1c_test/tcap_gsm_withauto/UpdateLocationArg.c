/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#include "UpdateLocationArg.h"

static asn_TYPE_member_t asn_MBR_UpdateLocationArg_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct UpdateLocationArg, imsi),
		(ASN_TAG_CLASS_UNIVERSAL | (4 << 2)),
		0,
		&asn_DEF_IMSI,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"imsi"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UpdateLocationArg, msc_Number),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ISDN_AddressString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"msc-Number"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UpdateLocationArg, vlr_Number),
		(ASN_TAG_CLASS_UNIVERSAL | (4 << 2)),
		0,
		&asn_DEF_ISDN_AddressString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"vlr-Number"
		},
	{ ATF_POINTER, 9, offsetof(struct UpdateLocationArg, lmsi),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LMSI,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"lmsi"
		},
	{ ATF_POINTER, 8, offsetof(struct UpdateLocationArg, extensionContainer),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ExtensionContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"extensionContainer"
		},
	{ ATF_POINTER, 7, offsetof(struct UpdateLocationArg, vlr_Capability),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_VLR_Capability,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"vlr-Capability"
		},
	{ ATF_POINTER, 6, offsetof(struct UpdateLocationArg, informPreviousNetworkEntity),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"informPreviousNetworkEntity"
		},
	{ ATF_POINTER, 5, offsetof(struct UpdateLocationArg, cs_LCS_NotSupportedByUE),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"cs-LCS-NotSupportedByUE"
		},
	{ ATF_POINTER, 4, offsetof(struct UpdateLocationArg, v_gmlc_Address),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GSN_Address,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"v-gmlc-Address"
		},
	{ ATF_POINTER, 3, offsetof(struct UpdateLocationArg, add_info),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ADD_Info,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"add-info"
		},
	{ ATF_POINTER, 2, offsetof(struct UpdateLocationArg, pagingArea),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PagingArea,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"pagingArea"
		},
	{ ATF_POINTER, 1, offsetof(struct UpdateLocationArg, skipSubscriberDataUpdate),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"skipSubscriberDataUpdate"
		},
};
static const ber_tlv_tag_t asn_DEF_UpdateLocationArg_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_UpdateLocationArg_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 0, 0, 1 }, /* imsi */
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 2, -1, 0 }, /* vlr-Number */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 4, 0, 0 }, /* extensionContainer */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* msc-Number */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 8, 0, 0 }, /* v-gmlc-Address */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 5, 0, 0 }, /* vlr-Capability */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 3, 0, 0 }, /* lmsi */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 6, 0, 0 }, /* informPreviousNetworkEntity */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 7, 0, 0 }, /* cs-LCS-NotSupportedByUE */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 9, 0, 0 }, /* add-info */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 10, 0, 0 }, /* pagingArea */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 11, 0, 0 } /* skipSubscriberDataUpdate */
};
static asn_SEQUENCE_specifics_t asn_SPC_UpdateLocationArg_specs_1 = {
	sizeof(struct UpdateLocationArg),
	offsetof(struct UpdateLocationArg, _asn_ctx),
	asn_MAP_UpdateLocationArg_tag2el_1,
	12,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	4,	/* Start extensions */
	13	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_UpdateLocationArg = {
	"UpdateLocationArg",
	"UpdateLocationArg",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_UpdateLocationArg_tags_1,
	sizeof(asn_DEF_UpdateLocationArg_tags_1)
		/sizeof(asn_DEF_UpdateLocationArg_tags_1[0]), /* 1 */
	asn_DEF_UpdateLocationArg_tags_1,	/* Same as above */
	sizeof(asn_DEF_UpdateLocationArg_tags_1)
		/sizeof(asn_DEF_UpdateLocationArg_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_UpdateLocationArg_1,
	12,	/* Elements count */
	&asn_SPC_UpdateLocationArg_specs_1	/* Additional specs */
};

