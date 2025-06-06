/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "MAP-CommonDataTypes"
 * 	found in "MAP-CommonDataTypes.asn"
 * 	`asn1c -pdu=auto`
 */

#ifndef	_AccessNetworkProtocolId_H_
#define	_AccessNetworkProtocolId_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum AccessNetworkProtocolId {
	AccessNetworkProtocolId_ts3G_48006	= 1,
	AccessNetworkProtocolId_ts3G_25413	= 2
	/*
	 * Enumeration is extensible
	 */
} e_AccessNetworkProtocolId;

/* AccessNetworkProtocolId */
typedef long	 AccessNetworkProtocolId_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AccessNetworkProtocolId;
asn_struct_free_f AccessNetworkProtocolId_free;
asn_struct_print_f AccessNetworkProtocolId_print;
asn_constr_check_f AccessNetworkProtocolId_constraint;
ber_type_decoder_f AccessNetworkProtocolId_decode_ber;
der_type_encoder_f AccessNetworkProtocolId_encode_der;
xer_type_decoder_f AccessNetworkProtocolId_decode_xer;
xer_type_encoder_f AccessNetworkProtocolId_encode_xer;

#ifdef __cplusplus
}
#endif

#endif	/* _AccessNetworkProtocolId_H_ */
#include <asn_internal.h>
