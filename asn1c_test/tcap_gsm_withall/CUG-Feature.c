/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#include "CUG-Feature.h"

static asn_TYPE_member_t asn_MBR_CUG_Feature_1[] = {
	{ ATF_POINTER, 2, offsetof(struct CUG_Feature, basicService),
		-1 /* Ambiguous tag (CHOICE?) */,
		0,
		&asn_DEF_Ext_BasicServiceCode,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"basicService"
		},
	{ ATF_POINTER, 1, offsetof(struct CUG_Feature, preferentialCUG_Indicator),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_CUG_Index,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"preferentialCUG-Indicator"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct CUG_Feature, interCUG_Restrictions),
		(ASN_TAG_CLASS_UNIVERSAL | (4 << 2)),
		0,
		&asn_DEF_InterCUG_Restrictions,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"interCUG-Restrictions"
		},
	{ ATF_POINTER, 1, offsetof(struct CUG_Feature, extensionContainer),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ExtensionContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"extensionContainer"
		},
};
static const ber_tlv_tag_t asn_DEF_CUG_Feature_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_CUG_Feature_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 1, 0, 0 }, /* preferentialCUG-Indicator */
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 2, 0, 0 }, /* interCUG-Restrictions */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 3, 0, 0 }, /* extensionContainer */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 0, 0, 0 }, /* ext-BearerService */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 0, 0, 0 } /* ext-Teleservice */
};
static asn_SEQUENCE_specifics_t asn_SPC_CUG_Feature_specs_1 = {
	sizeof(struct CUG_Feature),
	offsetof(struct CUG_Feature, _asn_ctx),
	asn_MAP_CUG_Feature_tag2el_1,
	5,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	3,	/* Start extensions */
	5	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_CUG_Feature = {
	"CUG-Feature",
	"CUG-Feature",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_CUG_Feature_tags_1,
	sizeof(asn_DEF_CUG_Feature_tags_1)
		/sizeof(asn_DEF_CUG_Feature_tags_1[0]), /* 1 */
	asn_DEF_CUG_Feature_tags_1,	/* Same as above */
	sizeof(asn_DEF_CUG_Feature_tags_1)
		/sizeof(asn_DEF_CUG_Feature_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_CUG_Feature_1,
	4,	/* Elements count */
	&asn_SPC_CUG_Feature_specs_1	/* Additional specs */
};

