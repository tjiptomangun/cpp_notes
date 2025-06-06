/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_Used_RAT_Type_H_
#define	_Used_RAT_Type_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Used_RAT_Type {
	Used_RAT_Type_utran	= 0,
	Used_RAT_Type_geran	= 1,
	Used_RAT_Type_gan	= 2,
	Used_RAT_Type_i_hspa_evolution	= 3,
	Used_RAT_Type_e_utran	= 4
	/*
	 * Enumeration is extensible
	 */
} e_Used_RAT_Type;

/* Used-RAT-Type */
typedef long	 Used_RAT_Type_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Used_RAT_Type;
asn_struct_free_f Used_RAT_Type_free;
asn_struct_print_f Used_RAT_Type_print;
asn_constr_check_f Used_RAT_Type_constraint;
ber_type_decoder_f Used_RAT_Type_decode_ber;
der_type_encoder_f Used_RAT_Type_encode_der;
xer_type_decoder_f Used_RAT_Type_decode_xer;
xer_type_encoder_f Used_RAT_Type_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _Used_RAT_Type_H_ */
#include <asn_internal.h>
