/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#include "ModificationRequestFor-CSG.h"

static asn_TYPE_member_t asn_MBR_ModificationRequestFor_CSG_1[] = {
	{ ATF_POINTER, 2, offsetof(struct ModificationRequestFor_CSG, modifyNotificationToCSE),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ModificationInstruction,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"modifyNotificationToCSE"
		},
	{ ATF_POINTER, 1, offsetof(struct ModificationRequestFor_CSG, extensionContainer),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ExtensionContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"extensionContainer"
		},
};
static const ber_tlv_tag_t asn_DEF_ModificationRequestFor_CSG_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ModificationRequestFor_CSG_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* modifyNotificationToCSE */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* extensionContainer */
};
static asn_SEQUENCE_specifics_t asn_SPC_ModificationRequestFor_CSG_specs_1 = {
	sizeof(struct ModificationRequestFor_CSG),
	offsetof(struct ModificationRequestFor_CSG, _asn_ctx),
	asn_MAP_ModificationRequestFor_CSG_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	1,	/* Start extensions */
	3	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_ModificationRequestFor_CSG = {
	"ModificationRequestFor-CSG",
	"ModificationRequestFor-CSG",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_ModificationRequestFor_CSG_tags_1,
	sizeof(asn_DEF_ModificationRequestFor_CSG_tags_1)
		/sizeof(asn_DEF_ModificationRequestFor_CSG_tags_1[0]), /* 1 */
	asn_DEF_ModificationRequestFor_CSG_tags_1,	/* Same as above */
	sizeof(asn_DEF_ModificationRequestFor_CSG_tags_1)
		/sizeof(asn_DEF_ModificationRequestFor_CSG_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_ModificationRequestFor_CSG_1,
	2,	/* Elements count */
	&asn_SPC_ModificationRequestFor_CSG_specs_1	/* Additional specs */
};

