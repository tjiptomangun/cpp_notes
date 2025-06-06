/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#include "GSM-SecurityContextData.h"

static asn_TYPE_member_t asn_MBR_GSM_SecurityContextData_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct GSM_SecurityContextData, kc),
		(ASN_TAG_CLASS_UNIVERSAL | (4 << 2)),
		0,
		&asn_DEF_Kc,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"kc"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct GSM_SecurityContextData, cksn),
		(ASN_TAG_CLASS_UNIVERSAL | (4 << 2)),
		0,
		&asn_DEF_Cksn,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"cksn"
		},
};
static const ber_tlv_tag_t asn_DEF_GSM_SecurityContextData_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_GSM_SecurityContextData_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 0, 0, 1 }, /* kc */
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 1, -1, 0 } /* cksn */
};
static asn_SEQUENCE_specifics_t asn_SPC_GSM_SecurityContextData_specs_1 = {
	sizeof(struct GSM_SecurityContextData),
	offsetof(struct GSM_SecurityContextData, _asn_ctx),
	asn_MAP_GSM_SecurityContextData_tag2el_1,
	2,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	1,	/* Start extensions */
	3	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_GSM_SecurityContextData = {
	"GSM-SecurityContextData",
	"GSM-SecurityContextData",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_GSM_SecurityContextData_tags_1,
	sizeof(asn_DEF_GSM_SecurityContextData_tags_1)
		/sizeof(asn_DEF_GSM_SecurityContextData_tags_1[0]), /* 1 */
	asn_DEF_GSM_SecurityContextData_tags_1,	/* Same as above */
	sizeof(asn_DEF_GSM_SecurityContextData_tags_1)
		/sizeof(asn_DEF_GSM_SecurityContextData_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_GSM_SecurityContextData_1,
	2,	/* Elements count */
	&asn_SPC_GSM_SecurityContextData_specs_1	/* Additional specs */
};

