/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#include "InsertSubscriberDataArg.h"

static asn_TYPE_member_t asn_MBR_InsertSubscriberDataArg_1[] = {
	{ ATF_POINTER, 32, offsetof(struct InsertSubscriberDataArg, imsi),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IMSI,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"imsi"
		},
	{ ATF_POINTER, 31, offsetof(struct InsertSubscriberDataArg, msisdn),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_ISDN_AddressString,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"msisdn"
		},
	{ ATF_POINTER, 30, offsetof(struct InsertSubscriberDataArg, category),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_MsCategory,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"category"
		},
	{ ATF_POINTER, 29, offsetof(struct InsertSubscriberDataArg, subscriberStatus),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_SubscriberStatus,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"subscriberStatus"
		},
	{ ATF_POINTER, 28, offsetof(struct InsertSubscriberDataArg, bearerServiceList),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_BearerServiceList,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"bearerServiceList"
		},
	{ ATF_POINTER, 27, offsetof(struct InsertSubscriberDataArg, teleserviceList),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_TeleserviceList,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"teleserviceList"
		},
	{ ATF_POINTER, 26, offsetof(struct InsertSubscriberDataArg, provisionedSS),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_Ext_SS_InfoList,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"provisionedSS"
		},
	{ ATF_POINTER, 25, offsetof(struct InsertSubscriberDataArg, odb_Data),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_ODB_Data,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"odb-Data"
		},
	{ ATF_POINTER, 24, offsetof(struct InsertSubscriberDataArg, roamingRestrictionDueToUnsupportedFeature),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"roamingRestrictionDueToUnsupportedFeature"
		},
	{ ATF_POINTER, 23, offsetof(struct InsertSubscriberDataArg, regionalSubscriptionData),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_ZoneCodeList,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"regionalSubscriptionData"
		},
	{ ATF_POINTER, 22, offsetof(struct InsertSubscriberDataArg, vbsSubscriptionData),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_VBSDataList,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"vbsSubscriptionData"
		},
	{ ATF_POINTER, 21, offsetof(struct InsertSubscriberDataArg, vgcsSubscriptionData),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_VGCSDataList,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"vgcsSubscriptionData"
		},
	{ ATF_POINTER, 20, offsetof(struct InsertSubscriberDataArg, vlrCamelSubscriptionInfo),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_VlrCamelSubscriptionInfo,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"vlrCamelSubscriptionInfo"
		},
	{ ATF_POINTER, 19, offsetof(struct InsertSubscriberDataArg, extensionContainer),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ExtensionContainer,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"extensionContainer"
		},
	{ ATF_POINTER, 18, offsetof(struct InsertSubscriberDataArg, naea_PreferredCI),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NAEA_PreferredCI,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"naea-PreferredCI"
		},
	{ ATF_POINTER, 17, offsetof(struct InsertSubscriberDataArg, gprsSubscriptionData),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_GPRSSubscriptionData,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"gprsSubscriptionData"
		},
	{ ATF_POINTER, 16, offsetof(struct InsertSubscriberDataArg, roamingRestrictedInSgsnDueToUnsupportedFeature),
		(ASN_TAG_CLASS_CONTEXT | (23 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"roamingRestrictedInSgsnDueToUnsupportedFeature"
		},
	{ ATF_POINTER, 15, offsetof(struct InsertSubscriberDataArg, networkAccessMode),
		(ASN_TAG_CLASS_CONTEXT | (24 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NetworkAccessMode,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"networkAccessMode"
		},
	{ ATF_POINTER, 14, offsetof(struct InsertSubscriberDataArg, lsaInformation),
		(ASN_TAG_CLASS_CONTEXT | (25 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LSAInformation,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"lsaInformation"
		},
	{ ATF_POINTER, 13, offsetof(struct InsertSubscriberDataArg, lmu_Indicator),
		(ASN_TAG_CLASS_CONTEXT | (21 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"lmu-Indicator"
		},
	{ ATF_POINTER, 12, offsetof(struct InsertSubscriberDataArg, lcsInformation),
		(ASN_TAG_CLASS_CONTEXT | (22 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LCSInformation,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"lcsInformation"
		},
	{ ATF_POINTER, 11, offsetof(struct InsertSubscriberDataArg, istAlertTimer),
		(ASN_TAG_CLASS_CONTEXT | (26 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IST_AlertTimerValue,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"istAlertTimer"
		},
	{ ATF_POINTER, 10, offsetof(struct InsertSubscriberDataArg, superChargerSupportedInHLR),
		(ASN_TAG_CLASS_CONTEXT | (27 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AgeIndicator,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"superChargerSupportedInHLR"
		},
	{ ATF_POINTER, 9, offsetof(struct InsertSubscriberDataArg, mc_SS_Info),
		(ASN_TAG_CLASS_CONTEXT | (28 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MC_SS_Info,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"mc-SS-Info"
		},
	{ ATF_POINTER, 8, offsetof(struct InsertSubscriberDataArg, cs_AllocationRetentionPriority),
		(ASN_TAG_CLASS_CONTEXT | (29 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CS_AllocationRetentionPriority,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"cs-AllocationRetentionPriority"
		},
	{ ATF_POINTER, 7, offsetof(struct InsertSubscriberDataArg, sgsn_CAMEL_SubscriptionInfo),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SGSN_CAMEL_SubscriptionInfo,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"sgsn-CAMEL-SubscriptionInfo"
		},
	{ ATF_POINTER, 6, offsetof(struct InsertSubscriberDataArg, chargingCharacteristics),
		(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ChargingCharacteristics,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"chargingCharacteristics"
		},
	{ ATF_POINTER, 5, offsetof(struct InsertSubscriberDataArg, accessRestrictionData),
		(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AccessRestrictionData,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"accessRestrictionData"
		},
	{ ATF_POINTER, 4, offsetof(struct InsertSubscriberDataArg, ics_Indicator),
		(ASN_TAG_CLASS_CONTEXT | (20 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"ics-Indicator"
		},
	{ ATF_POINTER, 3, offsetof(struct InsertSubscriberDataArg, eps_SubscriptionData),
		(ASN_TAG_CLASS_CONTEXT | (31 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_EPS_SubscriptionData,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"eps-SubscriptionData"
		},
	{ ATF_POINTER, 2, offsetof(struct InsertSubscriberDataArg, csg_SubscriptionDataList),
		(ASN_TAG_CLASS_CONTEXT | (32 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CSG_SubscriptionDataList,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"csg-SubscriptionDataList"
		},
	{ ATF_POINTER, 1, offsetof(struct InsertSubscriberDataArg, ue_ReachabilityRequestIndicator),
		(ASN_TAG_CLASS_CONTEXT | (33 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"ue-ReachabilityRequestIndicator"
		},
};
static const ber_tlv_tag_t asn_DEF_InsertSubscriberDataArg_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_InsertSubscriberDataArg_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* imsi */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* msisdn */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* category */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* subscriberStatus */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* bearerServiceList */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 5, 0, 0 }, /* teleserviceList */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 6, 0, 0 }, /* provisionedSS */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 7, 0, 0 }, /* odb-Data */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 8, 0, 0 }, /* roamingRestrictionDueToUnsupportedFeature */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 9, 0, 0 }, /* regionalSubscriptionData */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 10, 0, 0 }, /* vbsSubscriptionData */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 11, 0, 0 }, /* vgcsSubscriptionData */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 12, 0, 0 }, /* vlrCamelSubscriptionInfo */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 13, 0, 0 }, /* extensionContainer */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 14, 0, 0 }, /* naea-PreferredCI */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 15, 0, 0 }, /* gprsSubscriptionData */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 25, 0, 0 }, /* sgsn-CAMEL-SubscriptionInfo */
    { (ASN_TAG_CLASS_CONTEXT | (18 << 2)), 26, 0, 0 }, /* chargingCharacteristics */
    { (ASN_TAG_CLASS_CONTEXT | (19 << 2)), 27, 0, 0 }, /* accessRestrictionData */
    { (ASN_TAG_CLASS_CONTEXT | (20 << 2)), 28, 0, 0 }, /* ics-Indicator */
    { (ASN_TAG_CLASS_CONTEXT | (21 << 2)), 19, 0, 0 }, /* lmu-Indicator */
    { (ASN_TAG_CLASS_CONTEXT | (22 << 2)), 20, 0, 0 }, /* lcsInformation */
    { (ASN_TAG_CLASS_CONTEXT | (23 << 2)), 16, 0, 0 }, /* roamingRestrictedInSgsnDueToUnsupportedFeature */
    { (ASN_TAG_CLASS_CONTEXT | (24 << 2)), 17, 0, 0 }, /* networkAccessMode */
    { (ASN_TAG_CLASS_CONTEXT | (25 << 2)), 18, 0, 0 }, /* lsaInformation */
    { (ASN_TAG_CLASS_CONTEXT | (26 << 2)), 21, 0, 0 }, /* istAlertTimer */
    { (ASN_TAG_CLASS_CONTEXT | (27 << 2)), 22, 0, 0 }, /* superChargerSupportedInHLR */
    { (ASN_TAG_CLASS_CONTEXT | (28 << 2)), 23, 0, 0 }, /* mc-SS-Info */
    { (ASN_TAG_CLASS_CONTEXT | (29 << 2)), 24, 0, 0 }, /* cs-AllocationRetentionPriority */
    { (ASN_TAG_CLASS_CONTEXT | (31 << 2)), 29, 0, 0 }, /* eps-SubscriptionData */
    { (ASN_TAG_CLASS_CONTEXT | (32 << 2)), 30, 0, 0 }, /* csg-SubscriptionDataList */
    { (ASN_TAG_CLASS_CONTEXT | (33 << 2)), 31, 0, 0 } /* ue-ReachabilityRequestIndicator */
};
static asn_SEQUENCE_specifics_t asn_SPC_InsertSubscriberDataArg_specs_1 = {
	sizeof(struct InsertSubscriberDataArg),
	offsetof(struct InsertSubscriberDataArg, _asn_ctx),
	asn_MAP_InsertSubscriberDataArg_tag2el_1,
	32,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	13,	/* Start extensions */
	33	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_InsertSubscriberDataArg = {
	"InsertSubscriberDataArg",
	"InsertSubscriberDataArg",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_InsertSubscriberDataArg_tags_1,
	sizeof(asn_DEF_InsertSubscriberDataArg_tags_1)
		/sizeof(asn_DEF_InsertSubscriberDataArg_tags_1[0]), /* 1 */
	asn_DEF_InsertSubscriberDataArg_tags_1,	/* Same as above */
	sizeof(asn_DEF_InsertSubscriberDataArg_tags_1)
		/sizeof(asn_DEF_InsertSubscriberDataArg_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_InsertSubscriberDataArg_1,
	32,	/* Elements count */
	&asn_SPC_InsertSubscriberDataArg_specs_1	/* Additional specs */
};

