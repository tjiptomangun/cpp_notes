//http://www.net-snmp.org/wiki/index.php/TUT:Simple_Application
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#define DEMO_USE_SNMP_VERSION_3
#ifdef DEMO_USE_SNMP_VERSION_3
#include "net-snmp/library/transform_oids.h"
//const char *our_v3_passphrase = "k5hm1dt321";
const char *our_v3_passphrase = "kschmidt123";
const char *our_v3_privKey = "k5hm1dt321";
#endif
int main(int argc, char **argv) {
	struct snmp_session session, *ss;
	struct snmp_pdu *pdu;
	struct snmp_pdu *response;

	oid anOID[MAX_OID_LEN];
	size_t anOID_len = MAX_OID_LEN;
	struct variable_list *vars;
	int status;

	/** Initialize the SNMP library 
	 *
	 */
	init_snmp("snmpapp");

	/**
	 * Initialize a "session" that defines who we're going to talk to 
	 **/	
	snmp_sess_init(&session);
	session.peername = "localhost";

#ifdef DEMO_USE_SNMP_VERSION_3
	/**
	 * Use SNMPv3 to talk to the experimentak server
	 */
	
	/** Set the SNMP version number*/
	session.version = SNMP_VERSION_3;

	/** set the SNMP user name */
	session.securityName = strdup("kschmidt");
	session.securityNameLen = strlen(session.securityName);

	/** set the security level to authenticated, but not encrypted*/
	session.securityLevel = SNMP_SEC_LEVEL_AUTHNOPRIV;

	/** set the authentication method to MD5 */
	session.securityAuthProto = usmHMACMD5AuthProtocol;
	session.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
	session.securityAuthKeyLen =  USM_AUTH_KU_LEN;
	if (session.securityAuthProto == NULL) {
            /*
             * get .conf set default
             */
		const oid      *def = get_default_authtype(&session.securityAuthProtoLen);
		session.securityAuthProto =
		snmp_duplicate_objid(def, session.securityAuthProtoLen);
        }
	if (session.securityAuthProto == NULL) {
		session.securityAuthProto =
		snmp_duplicate_objid(SNMP_DEFAULT_AUTH_PROTO,
		SNMP_DEFAULT_AUTH_PROTOLEN);
		session.securityAuthProtoLen = SNMP_DEFAULT_AUTH_PROTOLEN;
        }


	/** set authentication key to MD5 hashed version of our passphrase "The Net-SNMP Demo Password"
        which will be at leat 8 characters long*/
	if(generate_Ku(session.securityAuthProto, session.securityAuthProtoLen, 
		(u_char *) our_v3_passphrase,
		strlen(our_v3_passphrase),
		session.securityAuthKey,
		&session.securityAuthKeyLen) != SNMPERR_SUCCESS) {
			snmp_perror(argv[0]);
			snmp_log(LOG_ERR, "Error generating Ku from authentication pass phrase. \n");
			exit(1);

	}
	session.securityPrivKeyLen = USM_PRIV_KU_LEN;
	if (session.securityPrivProto == NULL) {
            /*
             * get .conf set default
             */
		const oid      *def =
			get_default_privtype(&session.securityPrivProtoLen);
		session.securityPrivProto =
		snmp_duplicate_objid(def, session.securityPrivProtoLen);
        }
	if (session.securityPrivProto == NULL) {
		session.securityPrivProto =
		snmp_duplicate_objid(SNMP_DEFAULT_PRIV_PROTO,
		SNMP_DEFAULT_PRIV_PROTOLEN);
		session.securityPrivProtoLen = SNMP_DEFAULT_PRIV_PROTOLEN;
        }
        if (generate_Ku(session.securityAuthProto,
                        session.securityAuthProtoLen,
                        (u_char *) our_v3_privKey, strlen(our_v3_privKey),
                        session.securityPrivKey,
                        &session.securityPrivKeyLen) != SNMPERR_SUCCESS) {
            snmp_perror(argv[0]);
            fprintf(stderr,
                    "Error generating a key (Ku) from the supplied privacy pass phrase. \n");
            
            exit(1);
        }



#else /* We'll use the insecure but simpler SNMPv1*/
		/** set the snmp version number */
		session.version = SNMP_VERSION_1;

		/** set the snmpv1 community name used for authentication*/
		session.community = "k5hm1dt321";
		session.community_len = strlen(session.community);
#endif 

	/** windows32 specific initialization (is a noop on unix) */
	SOCK_STARTUP;

	/** Open the session*/
	ss = snmp_open(&session);		/*establish the session*/

	if (!ss) {
		snmp_perror("ack");
		snmp_log(LOG_ERR, "something horrible happened !!\n");
		exit(2);
	}

	/**
	 * Create the PDU for the data for our request
	 * 1) We're going to GET the system.sysDescr.0.node
	 */
	pdu = snmp_pdu_create(SNMP_MSG_GET);

	/**
	 * So, let's fill it with our requested oid. Let's get the system.sysDescr.0 
	 * variable for this example. There are numerous ways you could create the oid 
	 * in question. You could put in the numerical unsigned integer values yourself 
	 * into the anOID array we created above, or you could use one of the following 
	 * function calls to do it. We recommend the first one (get_node), 
	 * as it is the most powerful and accepts more types of OIDs. 
	 */
	read_objid(".1.3.6.1.2.1.1.1.0", anOID, &anOID_len);
	// if (!snmp_parse_oid(sOID, anOID, &anOID_len)) {
	// 	snmp_perror(sOID);
	// 	snmp_log(LOG_ERR, "parsing oid");
	// 	exit(3);
	// } else if(snmp_add_var(pdu, anOID, anOID_len, 's',"Whatever System We have")) {
	// 	snmp_perror("add var");
	// 	snmp_log(LOG_ERR, "add var");
	// 	exit(4);
	// }
	
#if OTHER_METHODS
	get_node("sysDescr.0", anOID, &anOID_len);
	read_objid("system.sysDescr.0", anOID, &anOID_len);
#endif 

	/**
	 * So we add this oid, with a NULL value to the PDU using the following statement: 
	 * (all oids should be paired with a NULL value for outgoing requests for 
	 * information. For an SNMP-SET pdu, we'd put in the value we wanted to set the oid to). 
	 */
	 snmp_add_null_var(pdu, anOID, anOID_len);

	/**
	 * Finally, we can send out the request using the session pointer and the pdu 
	 * and we get back a status and the response, which is stored in the newly malloced 
	 * response pdu pointer. 
	 */
	
	/**
	 * Send the request out
	 */
	status = snmp_synch_response(ss, pdu, &response);

	/**
	 * Process the response
	 */
	if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
		/**
	     * SUCCESS: print the result variables
		 */
		for (vars = response->variables; vars; vars = vars->next_variable) {
			int count = 1;
			if (vars->type == ASN_OCTET_STR) {
				char *sp = malloc(1 + vars->val_len);
				memcpy(sp, vars->val.string, vars->val_len);
				sp[vars->val_len] = '\0';
				printf("value #%d is a string: %s\n", count++, sp);
				free(sp); 
			} else {
				printf("value #%d is NOT a string! Ack!\n", count++);
			}
		}
	} else {
		/**
		 * FAILURE: print what went wrong!
		 */
		if ( status == STAT_SUCCESS ) {
			fprintf(stderr, "Error in packet\nReason: %s\n", snmp_errstring(response->errstat));
		} else {
			snmp_sess_perror("snmpget", ss);
		}
	}

	/**
	 * Clean up:
	 * 1) free the response
	 * 2) close the sesson
	 */
	if (response) {
		snmp_free_pdu(response);
	}
	snmp_close(ss);

	/**
	 * windows32 specific cleanup (is a noop on unix);
	 */
	SOCK_CLEANUP;
	
}
