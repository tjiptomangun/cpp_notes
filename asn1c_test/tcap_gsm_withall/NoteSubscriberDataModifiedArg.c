/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#include "NoteSubscriberDataModifiedArg.h"

static asn_TYPE_member_t asn_MBR_NoteSubscriberDataModifiedArg_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct NoteSubscriberDataModifiedArg, imsi),
		(ASN_TAG_CLASS_UNIVERSAL | (4 << 2)),
		0,
		&asn_DEF_IMSI,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"imsi"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct NoteSubscriberDataModifiedArg, msisdn),
		(ASN_TAG_CLASS_UNIVERSAL | (4 << 2)),
		0,
		&asn_DEF_ISDN_AddressString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"msisdn"
		},
	{ ATF_POINTER, 8, offsetof(struct NoteSubscriberDataModifiedArg, forwardingInfoFor_CSE),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ext_ForwardingInfoFor_CSE,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"forwardingInfoFor-CSE"
		},
	{ ATF_POINTER, 7, offsetof(struct NoteSubscriberDataModifiedArg, callBarringInfoFor_CSE),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Ext_CallBarringInfoFor_CSE,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"callBarringInfoFor-CSE"
		},
	{ ATF_POINTER, 6, offsetof(struct NoteSubscriberDataModifiedArg, odb_Info),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ODB_Info,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"odb-Info"
		},
	{ ATF_POINTER, 5, offsetof(struct NoteSubscriberDataModifiedArg, camel_SubscriptionInfo),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CAMEL_SubscriptionInfo,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"camel-SubscriptionInfo"
		},
	{ ATF_POINTER, 4, offsetof(struct NoteSubscriberDataModifiedArg, allInformationSent),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"allInformationSent"
		},
	{ ATF_POINTER, 3, offsetof(struct NoteSubscriberDataModifiedArg, extensionContainer),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_ExtensionContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"extensionContainer"
		},
	{ ATF_POINTER, 2, offsetof(struct NoteSubscriberDataModifiedArg, ue_reachable),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ServingNode,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"ue-reachable"
		},
	{ ATF_POINTER, 1, offsetof(struct NoteSubscriberDataModifiedArg, csg_SubscriptionDataList),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CSG_SubscriptionDataList,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"csg-SubscriptionDataList"
		},
};
static const ber_tlv_tag_t asn_DEF_NoteSubscriberDataModifiedArg_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_NoteSubscriberDataModifiedArg_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 0, 0, 1 }, /* imsi */
    { (ASN_TAG_CLASS_UNIVERSAL | (4 << 2)), 1, -1, 0 }, /* msisdn */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 7, 0, 0 }, /* extensionContainer */
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 2, 0, 0 }, /* forwardingInfoFor-CSE */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 3, 0, 0 }, /* callBarringInfoFor-CSE */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 4, 0, 0 }, /* odb-Info */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 5, 0, 0 }, /* camel-SubscriptionInfo */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 6, 0, 0 }, /* allInformationSent */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 8, 0, 0 }, /* ue-reachable */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 9, 0, 0 } /* csg-SubscriptionDataList */
};
static asn_SEQUENCE_specifics_t asn_SPC_NoteSubscriberDataModifiedArg_specs_1 = {
	sizeof(struct NoteSubscriberDataModifiedArg),
	offsetof(struct NoteSubscriberDataModifiedArg, _asn_ctx),
	asn_MAP_NoteSubscriberDataModifiedArg_tag2el_1,
	10,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	7,	/* Start extensions */
	11	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_NoteSubscriberDataModifiedArg = {
	"NoteSubscriberDataModifiedArg",
	"NoteSubscriberDataModifiedArg",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_NoteSubscriberDataModifiedArg_tags_1,
	sizeof(asn_DEF_NoteSubscriberDataModifiedArg_tags_1)
		/sizeof(asn_DEF_NoteSubscriberDataModifiedArg_tags_1[0]), /* 1 */
	asn_DEF_NoteSubscriberDataModifiedArg_tags_1,	/* Same as above */
	sizeof(asn_DEF_NoteSubscriberDataModifiedArg_tags_1)
		/sizeof(asn_DEF_NoteSubscriberDataModifiedArg_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_NoteSubscriberDataModifiedArg_1,
	10,	/* Elements count */
	&asn_SPC_NoteSubscriberDataModifiedArg_specs_1	/* Additional specs */
};

