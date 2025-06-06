/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-CH-DataTypes"
 * 	found in "MAP-CH-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_StatusReportArg_H_
#define	_StatusReportArg_H_


#include <asn_application.h>

/* Including external dependencies */
#include "IMSI.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct EventReportData;
struct CallReportData;
struct ExtensionContainer;

/* StatusReportArg */
typedef struct StatusReportArg {
	IMSI_t	 imsi;
	struct EventReportData	*eventReportData	/* OPTIONAL */;
	struct CallReportData	*callReportdata	/* OPTIONAL */;
	struct ExtensionContainer	*extensionContainer	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} StatusReportArg_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_StatusReportArg;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "EventReportData.h"
#include "CallReportData.h"
#include "ExtensionContainer.h"

#endif	/* _StatusReportArg_H_ */
#include <asn_internal.h>
