/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-MS-DataTypes"
 * 	found in "MAP-MS-DataTypes.asn"
 * 	`asn1c -pdu=all`
 */

#ifndef	_AccessType_H_
#define	_AccessType_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum AccessType {
	AccessType_call	= 0,
	AccessType_emergencyCall	= 1,
	AccessType_locationUpdating	= 2,
	AccessType_supplementaryService	= 3,
	AccessType_shortMessage	= 4,
	AccessType_gprsAttach	= 5,
	AccessType_routingAreaUpdating	= 6,
	AccessType_serviceRequest	= 7,
	AccessType_pdpContextActivation	= 8,
	AccessType_pdpContextDeactivation	= 9,
	/*
	 * Enumeration is extensible
	 */
	AccessType_gprsDetach	= 10
} e_AccessType;

/* AccessType */
typedef long	 AccessType_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AccessType;
asn_struct_free_f AccessType_free;
asn_struct_print_f AccessType_print;
asn_constr_check_f AccessType_constraint;
ber_type_decoder_f AccessType_decode_ber;
der_type_encoder_f AccessType_encode_der;
xer_type_decoder_f AccessType_decode_xer;
xer_type_encoder_f AccessType_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _AccessType_H_ */
#include <asn_internal.h>
