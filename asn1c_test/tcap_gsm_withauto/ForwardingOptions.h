/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-SS-DataTypes"
 * 	found in "MAP-SS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_ForwardingOptions_H_
#define	_ForwardingOptions_H_


#include <asn_application.h>

/* Including external dependencies */
#include <OCTET_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ForwardingOptions */
typedef OCTET_STRING_t	 ForwardingOptions_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ForwardingOptions;
asn_struct_free_f ForwardingOptions_free;
asn_struct_print_f ForwardingOptions_print;
asn_constr_check_f ForwardingOptions_constraint;
ber_type_decoder_f ForwardingOptions_decode_ber;
der_type_encoder_f ForwardingOptions_encode_der;
xer_type_decoder_f ForwardingOptions_decode_xer;
xer_type_encoder_f ForwardingOptions_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _ForwardingOptions_H_ */
#include <asn_internal.h>
