/*
 * Note: this file originally auto-generated by mib2c
 * using mib2c.int_watch.conf
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "nstAgentModuleObject.h"

/*
 * The variables we want to tie the relevant OIDs to.
 * The agent will handle all GET and (if applicable) SET requests
 * to these variables automatically, changing the values as needed.
 */

long    nstAgentModuleObject = 42;  /* XXX: set default value */

/*
 * Our initialization routine, called automatically by the agent 
 * (Note that the function name must match init_FILENAME()) 
 */
void
init_nstAgentModuleObject(void)
{
  netsnmp_handler_registration *reg;

    const oid nstAgentModuleObject_oid[] = { 1,3,6,1,4,1,8072,2,4,1,1,1 };
  static netsnmp_watcher_info nstAgentModuleObject_winfo;

  /*
   * a debugging statement.  Run the agent with -DnstAgentModuleObject to see
   * the output of this debugging statement. 
   */
  DEBUGMSGTL(("nstAgentModuleObject", "Initializing the nstAgentModuleObject module\n"));


    /*
     * Register scalar watchers for each of the MIB objects.
     * The ASN type and RO/RW status are taken from the MIB definition,
     * but can be adjusted if needed.
     *
     * In most circumstances, the scalar watcher will handle all
     * of the necessary processing.  But the NULL parameter in the
     * netsnmp_create_handler_registration() call can be used to
     * supply a user-provided handler if necessary.
     *
     * This approach can also be used to handle Counter64, string-
     * and OID-based watched scalars (although variable-sized writeable
     * objects will need some more specialised initialisation).
     */
    DEBUGMSGTL(("nstAgentModuleObject",
                "Initializing nstAgentModuleObject scalar integer.  Default value = %d\n",
                nstAgentModuleObject));
    reg = netsnmp_create_handler_registration(
             "nstAgentModuleObject", NULL,
              nstAgentModuleObject_oid, OID_LENGTH(nstAgentModuleObject_oid),
              HANDLER_CAN_RWRITE);
    netsnmp_init_watcher_info(&nstAgentModuleObject_winfo, &nstAgentModuleObject, sizeof(long),
			      ASN_INTEGER, WATCHER_FIXED_SIZE);
if (netsnmp_register_watched_scalar( reg, &nstAgentModuleObject_winfo ) < 0 ) {
        snmp_log( LOG_ERR, "Failed to register watched nstAgentModuleObject" );
    }


  DEBUGMSGTL(("nstAgentModuleObject",
              "Done initalizing nstAgentModuleObject module\n"));
}
