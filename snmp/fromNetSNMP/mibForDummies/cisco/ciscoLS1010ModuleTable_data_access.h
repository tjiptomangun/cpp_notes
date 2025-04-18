/*
 * Note: this file originally auto-generated by mib2c
 * using mfd-data-access.m2c
 */
#ifndef CISCOLS1010MODULETABLE_DATA_ACCESS_H
#define CISCOLS1010MODULETABLE_DATA_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif


/* *********************************************************************
 * function declarations
 */

/* *********************************************************************
 * Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table ciscoLS1010ModuleTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * CISCO-RHINO-MIB::ciscoLS1010ModuleTable is subid 1 of ciscoLS1010ModuleGroup.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.9.5.11.1.2.1, length: 12
*/


    int ciscoLS1010ModuleTable_init_data(ciscoLS1010ModuleTable_registration * ciscoLS1010ModuleTable_reg);


void ciscoLS1010ModuleTable_container_init(netsnmp_container **container_ptr_ptr);
void ciscoLS1010ModuleTable_container_shutdown(netsnmp_container *container_ptr);

int ciscoLS1010ModuleTable_container_load(netsnmp_container *container);
void ciscoLS1010ModuleTable_container_free(netsnmp_container *container);

    /*
    ***************************************************
    ***             START EXAMPLE CODE              ***
    ***---------------------------------------------***/
/* *********************************************************************
 * Since we have no idea how you really access your data, we'll go with
 * a worst case example: a flat text file.
 */
#define MAX_LINE_SIZE 256
    /*
    ***---------------------------------------------***
    ***              END  EXAMPLE CODE              ***
    ***************************************************/
    int ciscoLS1010ModuleTable_row_prep( ciscoLS1010ModuleTable_rowreq_ctx *rowreq_ctx);



#ifdef __cplusplus
}
#endif

#endif /* CISCOLS1010MODULETABLE_DATA_ACCESS_H */
