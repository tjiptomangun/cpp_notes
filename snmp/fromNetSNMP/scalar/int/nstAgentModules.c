/*
 * Note: this file originally auto-generated by mib2c
 * using mib2c.int_watch.conf
 * and merged with one 
 * using mib2c.scalar.conf
*/

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "nstAgentModules.h"

/*
 * The variables we want to tie the relevant OIDs to.
 * The agent will handle all GET and (if applicable) SET requests
 * to these variables automatically, changing the values as needed.
 */

long    nstAgentModuleObject = 0;  /* XXX: set default value */
long    nstAgentSubagentObject = 0;  /* XXX: set default value */
long    nstAgentPluginObject = 0;  /* XXX: set default value */

/*
 * Our initialization routine, called automatically by the agent 
 * (Note that the function name must match init_FILENAME()) 
 */
const oid nstAgentModuleObject_oid[] = { 1,3,6,1,4,1,8072,2,4,1,1,1 };
const oid nstAgentSubagentObject_oid[] = { 1,3,6,1,4,1,8072,2,4,1,1,2 };
const oid nstAgentPluginObject_oid[] = { 1,3,6,1,4,1,8072,2,4,1,1,3 };

int handle_nstAgentModuleObject(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests);

int handle_nstAgentSubagentObject(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests);
int handle_nstAgentPluginObject(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests);
void
init_nstAgentModules(void)
{
  netsnmp_handler_registration *reg;

  static netsnmp_watcher_info nstAgentModuleObject_winfo;

  static netsnmp_watcher_info nstAgentSubagentObject_winfo;

  static netsnmp_watcher_info nstAgentPluginObject_winfo;

  /*
   * a debugging statement.  Run the agent with -DnstAgentModules to see
   * the output of this debugging statement. 
   */
  DEBUGMSGTL(("nstAgentModules", "Initializing the nstAgentModules module\n"));


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
    DEBUGMSGTL(("nstAgentModules",
                "Initializing nstAgentModuleObject scalar integer.  Default value = %d\n",
                nstAgentModuleObject));
    reg = netsnmp_create_handler_registration(
             "nstAgentModuleObject", handle_nstAgentModuleObject,
              nstAgentModuleObject_oid, OID_LENGTH(nstAgentModuleObject_oid),
              HANDLER_CAN_RWRITE);
    netsnmp_init_watcher_info(&nstAgentModuleObject_winfo, &nstAgentModuleObject, sizeof(long),
			      ASN_INTEGER, WATCHER_FIXED_SIZE);
if (netsnmp_register_watched_scalar( reg, &nstAgentModuleObject_winfo ) < 0 ) {
        snmp_log( LOG_ERR, "Failed to register watched nstAgentModuleObject" );
    }

    DEBUGMSGTL(("nstAgentModules",
                "Initializing nstAgentSubagentObject scalar integer.  Default value = %d\n",
                nstAgentSubagentObject));
    reg = netsnmp_create_handler_registration(
             "nstAgentSubagentObject", handle_nstAgentSubagentObject,
              nstAgentSubagentObject_oid, OID_LENGTH(nstAgentSubagentObject_oid),
              HANDLER_CAN_RWRITE);
    netsnmp_init_watcher_info(&nstAgentSubagentObject_winfo, &nstAgentSubagentObject, sizeof(long),
			      ASN_INTEGER, WATCHER_FIXED_SIZE);
if (netsnmp_register_watched_scalar( reg, &nstAgentSubagentObject_winfo ) < 0 ) {
        snmp_log( LOG_ERR, "Failed to register watched nstAgentSubagentObject" );
    }

    DEBUGMSGTL(("nstAgentModules",
                "Initializing nstAgentPluginObject scalar integer.  Default value = %d\n",
                nstAgentPluginObject));
    reg = netsnmp_create_handler_registration(
             "nstAgentPluginObject", handle_nstAgentPluginObject,
              nstAgentPluginObject_oid, OID_LENGTH(nstAgentPluginObject_oid),
              HANDLER_CAN_RWRITE);
    netsnmp_init_watcher_info(&nstAgentPluginObject_winfo, &nstAgentPluginObject, sizeof(long),
			      ASN_INTEGER, WATCHER_FIXED_SIZE);
if (netsnmp_register_watched_scalar( reg, &nstAgentPluginObject_winfo ) < 0 ) {
        snmp_log( LOG_ERR, "Failed to register watched nstAgentPluginObject" );
    }


  DEBUGMSGTL(("nstAgentModules",
              "Done initalizing nstAgentModules module\n"));
}

int
handle_nstAgentModuleObject(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    int ret;
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, 
            /* XXX: a pointer to the scalar's data */
            (void*) &nstAgentModuleObject, 
            /* XXX: the length of the data in bytes */
            sizeof(long));
            break;

        /*
         * SET REQUEST
         *
         * multiple states in the transaction.  See:
         * http://www.net-snmp.org/tutorial-5/toolkit/mib_module/set-actions.jpg
         */
        case MODE_SET_RESERVE1:
                /* or you could use netsnmp_check_vb_type_and_size instead */
            ret = netsnmp_check_vb_type(requests->requestvb, ASN_INTEGER);
            if ( ret != SNMP_ERR_NOERROR ) {
                netsnmp_set_request_error(reqinfo, requests, ret );
            }
            break;

        case MODE_SET_RESERVE2:
            /* XXX malloc "undo" storage buffer */

            //if (/* XXX if malloc, or whatever, failed: */) {
            //    netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            //}
            break;

        case MODE_SET_FREE:
            /* XXX: free resources allocated in RESERVE1 and/or
               RESERVE2.  Something failed somewhere, and the states
               below won't be called. */
            break;

        case MODE_SET_ACTION:
            /* XXX: perform the value change here */
            //if (/* XXX: error? */) {
            //    netsnmp_set_request_error(reqinfo, requests, /* some error */);
            //}
            break;

        case MODE_SET_COMMIT:
            /* XXX: delete temporary storage */
            //if (/* XXX: error? */) {
                /* try _really_really_ hard to never get to this point */
            //    netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
            //}
            break;

        case MODE_SET_UNDO:
            /* XXX: UNDO and return to previous value for the object */
            //if (/* XXX: error? */) {
                /* try _really_really_ hard to never get to this point */
            //    netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_UNDOFAILED);
            //}
            break;

        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nstAgentModuleObject\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
int
handle_nstAgentSubagentObject(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    int ret;
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                     /* XXX: a pointer to the scalar's data */
                                    (void *) &nstAgentSubagentObject,
                                     /* XXX: the length of the data in bytes */
                                     sizeof(long));
            break;

        /*
         * SET REQUEST
         *
         * multiple states in the transaction.  See:
         * http://www.net-snmp.org/tutorial-5/toolkit/mib_module/set-actions.jpg
         */
        case MODE_SET_RESERVE1:
                /* or you could use netsnmp_check_vb_type_and_size instead */
            ret = netsnmp_check_vb_type(requests->requestvb, ASN_INTEGER);
            if ( ret != SNMP_ERR_NOERROR ) {
                netsnmp_set_request_error(reqinfo, requests, ret );
            }
            break;

        case MODE_SET_RESERVE2:
            /* XXX malloc "undo" storage buffer */
            //if (/* XXX if malloc, or whatever, failed: */) {
            //    netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            //}
            break;

        case MODE_SET_FREE:
            /* XXX: free resources allocated in RESERVE1 and/or
               RESERVE2.  Something failed somewhere, and the states
               below won't be called. */
            break;

        case MODE_SET_ACTION:
            /* XXX: perform the value change here */
            //if (/* XXX: error? */) {
            //    netsnmp_set_request_error(reqinfo, requests, /* some error */);
            //}
            break;

        case MODE_SET_COMMIT:
            /* XXX: delete temporary storage */
            //if (/* XXX: error? */) {
                /* try _really_really_ hard to never get to this point */
            //    netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
            //}
            break;

        case MODE_SET_UNDO:
            /* XXX: UNDO and return to previous value for the object */
            //if (/* XXX: error? */) {
                /* try _really_really_ hard to never get to this point */
            //    netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_UNDOFAILED);
            //}
            break;

        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nstAgentSubagentObject\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
int
handle_nstAgentPluginObject(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info   *reqinfo,
                          netsnmp_request_info         *requests)
{
    int ret;
    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */
    
    switch(reqinfo->mode) {

        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                      (void *)&nstAgentPluginObject
                                     /* XXX: a pointer to the scalar's data */,
                                     sizeof(long)
                                     /* XXX: the length of the data in bytes */);
            break;

        /*
         * SET REQUEST
         *
         * multiple states in the transaction.  See:
         * http://www.net-snmp.org/tutorial-5/toolkit/mib_module/set-actions.jpg
         */
        case MODE_SET_RESERVE1:
                /* or you could use netsnmp_check_vb_type_and_size instead */
            ret = netsnmp_check_vb_type(requests->requestvb, ASN_INTEGER);
            if ( ret != SNMP_ERR_NOERROR ) {
                netsnmp_set_request_error(reqinfo, requests, ret );
            }
            break;

        case MODE_SET_RESERVE2:
            /* XXX malloc "undo" storage buffer */
            //if (/* XXX if malloc, or whatever, failed: */) {
            //    netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_RESOURCEUNAVAILABLE);
            //}
            break;

        case MODE_SET_FREE:
            /* XXX: free resources allocated in RESERVE1 and/or
               RESERVE2.  Something failed somewhere, and the states
               below won't be called. */
            break;

        case MODE_SET_ACTION:
            /* XXX: perform the value change here */
            //if (/* XXX: error? */) {
            //    netsnmp_set_request_error(reqinfo, requests, /* some error */);
            //}
            break;

        case MODE_SET_COMMIT:
            /* XXX: delete temporary storage */
            //if (/* XXX: error? */) {
                /* try _really_really_ hard to never get to this point */
            //    netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
            //}
            break;

        case MODE_SET_UNDO:
            /* XXX: UNDO and return to previous value for the object */
            //if (/* XXX: error? */) {
                /* try _really_really_ hard to never get to this point */
            //    netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_UNDOFAILED);
            //}
            break;

        default:
            /* we should never get here, so this is a really bad error */
            snmp_log(LOG_ERR, "unknown mode (%d) in handle_nstAgentPluginObject\n", reqinfo->mode );
            return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
void
deinit_nstAgentModules(void)
{
    unregister_mib(nstAgentModuleObject_oid,OID_LENGTH(nstAgentModuleObject_oid));
    unregister_mib(nstAgentSubagentObject_oid,OID_LENGTH(nstAgentSubagentObject_oid));
    unregister_mib(nstAgentPluginObject_oid,OID_LENGTH(nstAgentPluginObject_oid));
}
