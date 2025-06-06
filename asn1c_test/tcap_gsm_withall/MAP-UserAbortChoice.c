/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-DialogueInformation"
 * 	found in "MAP-DialogueInformation.asn"
 * 	`asn1c -pdu=all`
 */

#include "MAP-UserAbortChoice.h"

static asn_TYPE_member_t asn_MBR_MAP_UserAbortChoice_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct MAP_UserAbortChoice, choice.userSpecificReason),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"userSpecificReason"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MAP_UserAbortChoice, choice.userResourceLimitation),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"userResourceLimitation"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MAP_UserAbortChoice, choice.resourceUnavailable),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ResourceUnavailableReason,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"resourceUnavailable"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct MAP_UserAbortChoice, choice.applicationProcedureCancellation),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProcedureCancellationReason,
		0,	/* Defer constraints checking to the member type */
		0,	/* PER is not compiled, use -gen-PER */
		0,
		"applicationProcedureCancellation"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_MAP_UserAbortChoice_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* userSpecificReason */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* userResourceLimitation */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* resourceUnavailable */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 } /* applicationProcedureCancellation */
};
static asn_CHOICE_specifics_t asn_SPC_MAP_UserAbortChoice_specs_1 = {
	sizeof(struct MAP_UserAbortChoice),
	offsetof(struct MAP_UserAbortChoice, _asn_ctx),
	offsetof(struct MAP_UserAbortChoice, present),
	sizeof(((struct MAP_UserAbortChoice *)0)->present),
	asn_MAP_MAP_UserAbortChoice_tag2el_1,
	4,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_MAP_UserAbortChoice = {
	"MAP-UserAbortChoice",
	"MAP-UserAbortChoice",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	0, 0,	/* No PER support, use "-gen-PER" to enable */
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	0,	/* No PER visible constraints */
	asn_MBR_MAP_UserAbortChoice_1,
	4,	/* Elements count */
	&asn_SPC_MAP_UserAbortChoice_specs_1	/* Additional specs */
};

