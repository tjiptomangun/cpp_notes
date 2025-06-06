/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-CommonDataTypes"
 * 	found in "MAP-CommonDataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_TMSI_H_
#define	_TMSI_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TMSI */
typedef OCTET_STRING_t	 TMSI_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TMSI;
asn_struct_free_f TMSI_free;
asn_struct_print_f TMSI_print;
asn_constr_check_f TMSI_constraint;
ber_type_decoder_f TMSI_decode_ber;
der_type_encoder_f TMSI_encode_der;
xer_type_decoder_f TMSI_decode_xer;
xer_type_encoder_f TMSI_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _TMSI_H_ */
#include <asn_internal.h>
