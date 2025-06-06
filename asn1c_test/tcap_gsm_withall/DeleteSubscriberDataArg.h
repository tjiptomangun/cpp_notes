/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_DeleteSubscriberDataArg_H_
#define	_DeleteSubscriberDataArg_H_


#include <asn_application.h>

/* Including external dependencies */
#include "IMSI.h"
#include <NULL.h>
#include "ZoneCode.h"
#include "SpecificCSI-Withdraw.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct BasicServiceList;
struct SS_List;
struct ExtensionContainer;
struct GPRSSubscriptionDataWithdraw;
struct LSAInformationWithdraw;
struct EPS_SubscriptionDataWithdraw;

/* DeleteSubscriberDataArg */
typedef struct DeleteSubscriberDataArg {
	IMSI_t	 imsi;
	struct BasicServiceList	*basicServiceList	/* OPTIONAL */;
	struct SS_List	*ss_List	/* OPTIONAL */;
	NULL_t	*roamingRestrictionDueToUnsupportedFeature	/* OPTIONAL */;
	ZoneCode_t	*regionalSubscriptionIdentifier	/* OPTIONAL */;
	NULL_t	*vbsGroupIndication	/* OPTIONAL */;
	NULL_t	*vgcsGroupIndication	/* OPTIONAL */;
	NULL_t	*camelSubscriptionInfoWithdraw	/* OPTIONAL */;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct GPRSSubscriptionDataWithdraw	*gprsSubscriptionDataWithdraw	/* OPTIONAL */;
	NULL_t	*roamingRestrictedInSgsnDueToUnsuppportedFeature	/* OPTIONAL */;
	struct LSAInformationWithdraw	*lsaInformationWithdraw	/* OPTIONAL */;
	NULL_t	*gmlc_ListWithdraw	/* OPTIONAL */;
	NULL_t	*istInformationWithdraw	/* OPTIONAL */;
	SpecificCSI_Withdraw_t	*specificCSI_Withdraw	/* OPTIONAL */;
	NULL_t	*chargingCharacteristicsWithdraw	/* OPTIONAL */;
	NULL_t	*stn_srWithdraw	/* OPTIONAL */;
	struct EPS_SubscriptionDataWithdraw	*epsSubscriptionDataWithdraw	/* OPTIONAL */;
	NULL_t	*apn_oi_replacementWithdraw	/* OPTIONAL */;
	NULL_t	*csg_SubscriptionDeleted	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DeleteSubscriberDataArg_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DeleteSubscriberDataArg;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "BasicServiceList.h"
#include "SS-List.h"
#include "ExtensionContainer.h"
#include "GPRSSubscriptionDataWithdraw.h"
#include "LSAInformationWithdraw.h"
#include "EPS-SubscriptionDataWithdraw.h"

#endif	/* _DeleteSubscriberDataArg_H_ */
#include <asn_internal.h>
