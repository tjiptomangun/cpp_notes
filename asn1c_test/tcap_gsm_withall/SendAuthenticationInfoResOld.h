/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "DummyMAP"
 * 	found in "GSMMAP.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_SendAuthenticationInfoResOld_H_
#define	_SendAuthenticationInfoResOld_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include "MapRAND.h"
#include "MapSRES.h"
#include "MapKc.h"
#include <constr_SEQUENCE.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SendAuthenticationInfoResOld */
typedef struct SendAuthenticationInfoResOld {
	A_SEQUENCE_OF(struct Member {
		MapRAND_t	 rand;
		MapSRES_t	 sres;
		MapKc_t	 kc;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} ) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SendAuthenticationInfoResOld_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SendAuthenticationInfoResOld;

#ifdef __cplusplus
}
#endif

#endif	/* _SendAuthenticationInfoResOld_H_ */
#include <asn_internal.h>
