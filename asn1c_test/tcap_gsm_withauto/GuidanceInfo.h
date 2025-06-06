/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-SS-DataTypes"
 * 	found in "MAP-SS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_GuidanceInfo_H_
#define	_GuidanceInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum GuidanceInfo {
	GuidanceInfo_enterPW	= 0,
	GuidanceInfo_enterNewPW	= 1,
	GuidanceInfo_enterNewPW_Again	= 2
} e_GuidanceInfo;

/* GuidanceInfo */
typedef long	 GuidanceInfo_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_GuidanceInfo;
asn_struct_free_f GuidanceInfo_free;
asn_struct_print_f GuidanceInfo_print;
asn_constr_check_f GuidanceInfo_constraint;
ber_type_decoder_f GuidanceInfo_decode_ber;
der_type_encoder_f GuidanceInfo_encode_der;
xer_type_decoder_f GuidanceInfo_decode_xer;
xer_type_encoder_f GuidanceInfo_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _GuidanceInfo_H_ */
#include <asn_internal.h>
