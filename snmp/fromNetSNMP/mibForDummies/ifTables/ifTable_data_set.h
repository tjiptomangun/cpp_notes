/*
 * Note: this file originally auto-generated by mib2c
 * using mfd-data-set.m2c
 */
#ifndef IFTABLE_DATA_SET_H
#define IFTABLE_DATA_SET_H

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************************
 * SET function declarations
 */

/* *********************************************************************
 * SET Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table ifTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * IF-MIB::ifTable is subid 2 of interfaces.
 * Its status is Current.
 * OID: .1.3.6.1.2.1.2.2, length: 8
*/


int ifTable_undo_setup( ifTable_rowreq_ctx *rowreq_ctx);
int ifTable_undo_cleanup( ifTable_rowreq_ctx *rowreq_ctx);
int ifTable_undo( ifTable_rowreq_ctx *rowreq_ctx);
int ifTable_commit( ifTable_rowreq_ctx *rowreq_ctx);
int ifTable_undo_commit( ifTable_rowreq_ctx *rowreq_ctx);


int ifDescr_check_value( ifTable_rowreq_ctx *rowreq_ctx, char *ifDescr_val_ptr,  size_t ifDescr_val_ptr_len);
int ifDescr_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifDescr_set( ifTable_rowreq_ctx *rowreq_ctx, char *ifDescr_val_ptr,  size_t ifDescr_val_ptr_len );
int ifDescr_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifType_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifType_val);
int ifType_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifType_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifType_val );
int ifType_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifMtu_check_value( ifTable_rowreq_ctx *rowreq_ctx, long ifMtu_val);
int ifMtu_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifMtu_set( ifTable_rowreq_ctx *rowreq_ctx, long ifMtu_val );
int ifMtu_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifSpeed_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifSpeed_val);
int ifSpeed_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifSpeed_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifSpeed_val );
int ifSpeed_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifPhysAddress_check_value( ifTable_rowreq_ctx *rowreq_ctx, char *ifPhysAddress_val_ptr,  size_t ifPhysAddress_val_ptr_len);
int ifPhysAddress_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifPhysAddress_set( ifTable_rowreq_ctx *rowreq_ctx, char *ifPhysAddress_val_ptr,  size_t ifPhysAddress_val_ptr_len );
int ifPhysAddress_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifAdminStatus_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifAdminStatus_val);
int ifAdminStatus_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifAdminStatus_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifAdminStatus_val );
int ifAdminStatus_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifOperStatus_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOperStatus_val);
int ifOperStatus_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifOperStatus_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOperStatus_val );
int ifOperStatus_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifLastChange_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifLastChange_val);
int ifLastChange_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifLastChange_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifLastChange_val );
int ifLastChange_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifInOctets_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifInOctets_val);
int ifInOctets_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifInOctets_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifInOctets_val );
int ifInOctets_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifInUcastPkts_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifInUcastPkts_val);
int ifInUcastPkts_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifInUcastPkts_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifInUcastPkts_val );
int ifInUcastPkts_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifInNUcastPkts_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifInNUcastPkts_val);
int ifInNUcastPkts_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifInNUcastPkts_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifInNUcastPkts_val );
int ifInNUcastPkts_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifInDiscards_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifInDiscards_val);
int ifInDiscards_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifInDiscards_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifInDiscards_val );
int ifInDiscards_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifInErrors_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifInErrors_val);
int ifInErrors_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifInErrors_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifInErrors_val );
int ifInErrors_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifInUnknownProtos_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifInUnknownProtos_val);
int ifInUnknownProtos_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifInUnknownProtos_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifInUnknownProtos_val );
int ifInUnknownProtos_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifOutOctets_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOutOctets_val);
int ifOutOctets_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifOutOctets_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOutOctets_val );
int ifOutOctets_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifOutUcastPkts_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOutUcastPkts_val);
int ifOutUcastPkts_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifOutUcastPkts_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOutUcastPkts_val );
int ifOutUcastPkts_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifOutNUcastPkts_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOutNUcastPkts_val);
int ifOutNUcastPkts_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifOutNUcastPkts_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOutNUcastPkts_val );
int ifOutNUcastPkts_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifOutDiscards_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOutDiscards_val);
int ifOutDiscards_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifOutDiscards_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOutDiscards_val );
int ifOutDiscards_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifOutErrors_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOutErrors_val);
int ifOutErrors_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifOutErrors_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOutErrors_val );
int ifOutErrors_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifOutQLen_check_value( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOutQLen_val);
int ifOutQLen_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifOutQLen_set( ifTable_rowreq_ctx *rowreq_ctx, u_long ifOutQLen_val );
int ifOutQLen_undo( ifTable_rowreq_ctx *rowreq_ctx );

int ifSpecific_check_value( ifTable_rowreq_ctx *rowreq_ctx, oid *ifSpecific_val_ptr,  size_t ifSpecific_val_ptr_len);
int ifSpecific_undo_setup( ifTable_rowreq_ctx *rowreq_ctx );
int ifSpecific_set( ifTable_rowreq_ctx *rowreq_ctx, oid *ifSpecific_val_ptr,  size_t ifSpecific_val_ptr_len );
int ifSpecific_undo( ifTable_rowreq_ctx *rowreq_ctx );


int ifTable_check_dependencies(ifTable_rowreq_ctx *ctx);


#ifdef __cplusplus
}
#endif

#endif /* IFTABLE_DATA_SET_H */
