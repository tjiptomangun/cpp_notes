/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-CH-DataTypes"
 * 	found in "MAP-CH-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_UnavailabilityCause_H_
#define	_UnavailabilityCause_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum UnavailabilityCause {
	UnavailabilityCause_bearerServiceNotProvisioned	= 1,
	UnavailabilityCause_teleserviceNotProvisioned	= 2,
	UnavailabilityCause_absentSubscriber	= 3,
	UnavailabilityCause_busySubscriber	= 4,
	UnavailabilityCause_callBarred	= 5,
	UnavailabilityCause_cug_Reject	= 6
	/*
	 * Enumeration is extensible
	 */
} e_UnavailabilityCause;

/* UnavailabilityCause */
typedef long	 UnavailabilityCause_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_UnavailabilityCause;
asn_struct_free_f UnavailabilityCause_free;
asn_struct_print_f UnavailabilityCause_print;
asn_constr_check_f UnavailabilityCause_constraint;
ber_type_decoder_f UnavailabilityCause_decode_ber;
der_type_encoder_f UnavailabilityCause_encode_der;
xer_type_decoder_f UnavailabilityCause_decode_xer;
xer_type_encoder_f UnavailabilityCause_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _UnavailabilityCause_H_ */
#include <asn_internal.h>
