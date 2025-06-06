/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_CancellationType_H_
#define	_CancellationType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CancellationType {
	CancellationType_updateProcedure	= 0,
	CancellationType_subscriptionWithdraw	= 1,
	/*
	 * Enumeration is extensible
	 */
	CancellationType_initialAttachProcedure	= 2
} e_CancellationType;

/* CancellationType */
typedef long	 CancellationType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CancellationType;
asn_struct_free_f CancellationType_free;
asn_struct_print_f CancellationType_print;
asn_constr_check_f CancellationType_constraint;
ber_type_decoder_f CancellationType_decode_ber;
der_type_encoder_f CancellationType_encode_der;
xer_type_decoder_f CancellationType_decode_xer;
xer_type_encoder_f CancellationType_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _CancellationType_H_ */
#include <asn_internal.h>
