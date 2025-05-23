/*
 * start be including the appropriate header files 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * Then, we declare the variables we want to be accessed 
 */
static int      example1 = 42;  /* default value */

/*
 * our initialization routine, automatically called by the agent 
 * (to get called, the function name must match init_FILENAME())
 */
void
init_scalar_int(void)
{
    /*
     * the OID we want to register our integer at.  This should be a
     * fully qualified instance.  In our case, it's a scalar at:
     * NET-SNMP-EXAMPLES-MIB::netSnmpExampleInteger.0 (note the
     * trailing 0 which is required for any instantiation of any
     * scalar object) 
     */
    oid             my_registration_oid[] =
        { 1, 3, 6, 1, 4, 1, 8072, 2, 1, 1, 0 };

    /*
     * a debugging statement.  Run the agent with -Dexample_scalar_int to see
     * the output of this debugging statement. 
     */
    DEBUGMSGTL(("example_scalar_int",
                "Initalizing example scalar int.  Default value = %d\n",
                example1));

    /*
     * the line below registers our "example1" variable above as
     * accessible and makes it writable.  A read only version of the
     * same registration would merely call
     * register_read_only_int_instance() instead.
     * 
     * If we wanted a callback when the value was retrieved or set
     * (even though the details of doing this are handled for you),
     * you could change the NULL pointer below to a valid handler
     * function. 
     */
    netsnmp_register_int_instance("my example int variable",
                                  my_registration_oid,
                                  OID_LENGTH(my_registration_oid),
                                  &example1, NULL);

    DEBUGMSGTL(("example_scalar_int",
                "Done initalizing example scalar int\n"));
}
