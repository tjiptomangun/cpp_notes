/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_DefaultCallHandling_H_
#define	_DefaultCallHandling_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DefaultCallHandling {
	DefaultCallHandling_continueCall	= 0,
	DefaultCallHandling_releaseCall	= 1
	/*
	 * Enumeration is extensible
	 */
} e_DefaultCallHandling;

/* DefaultCallHandling */
typedef long	 DefaultCallHandling_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DefaultCallHandling;
asn_struct_free_f DefaultCallHandling_free;
asn_struct_print_f DefaultCallHandling_print;
asn_constr_check_f DefaultCallHandling_constraint;
ber_type_decoder_f DefaultCallHandling_decode_ber;
der_type_encoder_f DefaultCallHandling_encode_der;
xer_type_decoder_f DefaultCallHandling_decode_xer;
xer_type_encoder_f DefaultCallHandling_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _DefaultCallHandling_H_ */
#include <asn_internal.h>
