/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#include "UpdateLocationRes.h"

static asn_TYPE_member_t asn_MBR_UpdateLocationRes_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct UpdateLocationRes, hlr_Number),
		(ASN_TAG_CLASS_UNIVERSAL | (4 << 2)),
		0,
		&asn_DEF_ISDN_AddressString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"hlr-Number"
		},
	{ ATF_POINTER, 3, offsetof(struct UpdateLocationRes, extensionContainer),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ExtensionContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"extensionContainer"
		},
	{ ATF_POINTER, 2, offsetof(struct UpdateLocationRes, add_Capability),
		(ASN_TAG_CLASS_UNIVERSAL | (5 << 2)),
		0,
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"add-Capability"
		},
	{ ATF_POINTER, 1, offsetof(struct UpdateLocationRes, pagingArea_Capability),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"pagingArea-Capability"
		},
};
static const ber_tlv_tag_t asn_DEF_UpdateLocationRes_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_UpdateLocationRes_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 0, 0, 0 }, /* hlr-Number */
    { (ASN_TAG_CLASS_UNIVERSAL | (5 << 2)), 2, 0, 0 }, /* add-Capability */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 1, 0, 0 }, /* extensionContainer */
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 3, 0, 0 } /* pagingArea-Capability */
};
static asn_SEQUENCE_specifics_t asn_SPC_UpdateLocationRes_specs_1 = {
	sizeof(struct UpdateLocationRes),
	offsetof(struct UpdateLocationRes, _asn_ctx),
	asn_MAP_UpdateLocationRes_tag2el_1,
	4,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	1,	/* Start extensions */
	5	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_UpdateLocationRes = {
	"UpdateLocationRes",
	"UpdateLocationRes",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_UpdateLocationRes_tags_1,
	sizeof(asn_DEF_UpdateLocationRes_tags_1)
		/sizeof(asn_DEF_UpdateLocationRes_tags_1[0]), /* 1 */
	asn_DEF_UpdateLocationRes_tags_1,	/* Same as above */
	sizeof(asn_DEF_UpdateLocationRes_tags_1)
		/sizeof(asn_DEF_UpdateLocationRes_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_UpdateLocationRes_1,
	4,	/* Elements count */
	&asn_SPC_UpdateLocationRes_specs_1	/* Additional specs */
};

