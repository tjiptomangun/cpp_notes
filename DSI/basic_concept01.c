/** 
 * DSI : Dialogic Signaling Interface ?
 */

/**
 * Base DSI Headers
 */
#include "system.h"
#include "msg.h"
#include "sysgct.h"

/**
 * DSI protocol headers
 */
#include "mtp_inc.h"

/**
 * MACROs for sending and receiving requests
 */
#define NO_RESPONSE			(0)
#define RESPONSE(mod_id)	(1 << ((mod_id) & 0x0f))
#define CONF(i)				((i) & ~REQUEST) //REQUEST defined in msg.h as 0x4000

#define EXAMPLE_MODULE_ID	(0x1)

int allocate_and_send_example(void) {
	MSG	*m;
	u8	*pptr;

	/**
	 * Allocate a MSG from the message protocol.
	 * In this example, a MTP3 Linkset Configuration Message
	 */
}
