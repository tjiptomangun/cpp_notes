/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_PDN_GW_AllocationType_H_
#define	_PDN_GW_AllocationType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PDN_GW_AllocationType {
	PDN_GW_AllocationType_static	= 0,
	PDN_GW_AllocationType_dynamic	= 1
} e_PDN_GW_AllocationType;

/* PDN-GW-AllocationType */
typedef long	 PDN_GW_AllocationType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PDN_GW_AllocationType;
asn_struct_free_f PDN_GW_AllocationType_free;
asn_struct_print_f PDN_GW_AllocationType_print;
asn_constr_check_f PDN_GW_AllocationType_constraint;
ber_type_decoder_f PDN_GW_AllocationType_decode_ber;
der_type_encoder_f PDN_GW_AllocationType_encode_der;
xer_type_decoder_f PDN_GW_AllocationType_decode_xer;
xer_type_encoder_f PDN_GW_AllocationType_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _PDN_GW_AllocationType_H_ */
#include <asn_internal.h>
