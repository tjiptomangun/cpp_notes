/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#include "DeleteSubscriberDataArg.h"

static asn_TYPE_member_t asn_MBR_DeleteSubscriberDataArg_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct DeleteSubscriberDataArg, imsi),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IMSI,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"imsi"
		},
	{ ATF_POINTER, 19, offsetof(struct DeleteSubscriberDataArg, basicServiceList),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BasicServiceList,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"basicServiceList"
		},
	{ ATF_POINTER, 18, offsetof(struct DeleteSubscriberDataArg, ss_List),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SS_List,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"ss-List"
		},
	{ ATF_POINTER, 17, offsetof(struct DeleteSubscriberDataArg, roamingRestrictionDueToUnsupportedFeature),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"roamingRestrictionDueToUnsupportedFeature"
		},
	{ ATF_POINTER, 16, offsetof(struct DeleteSubscriberDataArg, regionalSubscriptionIdentifier),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ZoneCode,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"regionalSubscriptionIdentifier"
		},
	{ ATF_POINTER, 15, offsetof(struct DeleteSubscriberDataArg, vbsGroupIndication),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"vbsGroupIndication"
		},
	{ ATF_POINTER, 14, offsetof(struct DeleteSubscriberDataArg, vgcsGroupIndication),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"vgcsGroupIndication"
		},
	{ ATF_POINTER, 13, offsetof(struct DeleteSubscriberDataArg, camelSubscriptionInfoWithdraw),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"camelSubscriptionInfoWithdraw"
		},
	{ ATF_POINTER, 12, offsetof(struct DeleteSubscriberDataArg, extensionContainer),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ExtensionContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"extensionContainer"
		},
	{ ATF_POINTER, 11, offsetof(struct DeleteSubscriberDataArg, gprsSubscriptionDataWithdraw),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_GPRSSubscriptionDataWithdraw,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"gprsSubscriptionDataWithdraw"
		},
	{ ATF_POINTER, 10, offsetof(struct DeleteSubscriberDataArg, roamingRestrictedInSgsnDueToUnsuppportedFeature),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"roamingRestrictedInSgsnDueToUnsuppportedFeature"
		},
	{ ATF_POINTER, 9, offsetof(struct DeleteSubscriberDataArg, lsaInformationWithdraw),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_LSAInformationWithdraw,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"lsaInformationWithdraw"
		},
	{ ATF_POINTER, 8, offsetof(struct DeleteSubscriberDataArg, gmlc_ListWithdraw),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"gmlc-ListWithdraw"
		},
	{ ATF_POINTER, 7, offsetof(struct DeleteSubscriberDataArg, istInformationWithdraw),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"istInformationWithdraw"
		},
	{ ATF_POINTER, 6, offsetof(struct DeleteSubscriberDataArg, specificCSI_Withdraw),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SpecificCSI_Withdraw,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"specificCSI-Withdraw"
		},
	{ ATF_POINTER, 5, offsetof(struct DeleteSubscriberDataArg, chargingCharacteristicsWithdraw),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"chargingCharacteristicsWithdraw"
		},
	{ ATF_POINTER, 4, offsetof(struct DeleteSubscriberDataArg, stn_srWithdraw),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"stn-srWithdraw"
		},
	{ ATF_POINTER, 3, offsetof(struct DeleteSubscriberDataArg, epsSubscriptionDataWithdraw),
		(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_EPS_SubscriptionDataWithdraw,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"epsSubscriptionDataWithdraw"
		},
	{ ATF_POINTER, 2, offsetof(struct DeleteSubscriberDataArg, apn_oi_replacementWithdraw),
		(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"apn-oi-replacementWithdraw"
		},
	{ ATF_POINTER, 1, offsetof(struct DeleteSubscriberDataArg, csg_SubscriptionDeleted),
		(ASN_TAG_CLASS_CONTEXT | (20 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"csg-SubscriptionDeleted"
		},
};
static const ber_tlv_tag_t asn_DEF_DeleteSubscriberDataArg_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_DeleteSubscriberDataArg_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* imsi */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* basicServiceList */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* ss-List */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 3, 0, 0 }, /* roamingRestrictionDueToUnsupportedFeature */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 4, 0, 0 }, /* regionalSubscriptionIdentifier */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 8, 0, 0 }, /* extensionContainer */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 5, 0, 0 }, /* vbsGroupIndication */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 6, 0, 0 }, /* vgcsGroupIndication */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 7, 0, 0 }, /* camelSubscriptionInfoWithdraw */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 9, 0, 0 }, /* gprsSubscriptionDataWithdraw */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 10, 0, 0 }, /* roamingRestrictedInSgsnDueToUnsuppportedFeature */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 11, 0, 0 }, /* lsaInformationWithdraw */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 12, 0, 0 }, /* gmlc-ListWithdraw */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 13, 0, 0 }, /* istInformationWithdraw */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 14, 0, 0 }, /* specificCSI-Withdraw */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 15, 0, 0 }, /* chargingCharacteristicsWithdraw */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 16, 0, 0 }, /* stn-srWithdraw */
    { (ASN_TAG_CLASS_CONTEXT | (18 << 2)), 17, 0, 0 }, /* epsSubscriptionDataWithdraw */
    { (ASN_TAG_CLASS_CONTEXT | (19 << 2)), 18, 0, 0 }, /* apn-oi-replacementWithdraw */
    { (ASN_TAG_CLASS_CONTEXT | (20 << 2)), 19, 0, 0 } /* csg-SubscriptionDeleted */
};
static asn_SEQUENCE_specifics_t asn_SPC_DeleteSubscriberDataArg_specs_1 = {
	sizeof(struct DeleteSubscriberDataArg),
	offsetof(struct DeleteSubscriberDataArg, _asn_ctx),
	asn_MAP_DeleteSubscriberDataArg_tag2el_1,
	20,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	8,	/* Start extensions */
	21	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_DeleteSubscriberDataArg = {
	"DeleteSubscriberDataArg",
	"DeleteSubscriberDataArg",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_DeleteSubscriberDataArg_tags_1,
	sizeof(asn_DEF_DeleteSubscriberDataArg_tags_1)
		/sizeof(asn_DEF_DeleteSubscriberDataArg_tags_1[0]), /* 1 */
	asn_DEF_DeleteSubscriberDataArg_tags_1,	/* Same as above */
	sizeof(asn_DEF_DeleteSubscriberDataArg_tags_1)
		/sizeof(asn_DEF_DeleteSubscriberDataArg_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_DeleteSubscriberDataArg_1,
	20,	/* Elements count */
	&asn_SPC_DeleteSubscriberDataArg_specs_1	/* Additional specs */
};

