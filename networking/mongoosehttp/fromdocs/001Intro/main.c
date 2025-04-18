#include "mongoose.h"
// Mongoose event handler function, gets called by the mg_mgr_poll()
static void fn(struct mg_connection *conn, int ev, void *ev_data, void *fn_data) {
	if (ev == MG_EV_HTTP_MSG) {
		// The MG_EV_HTTP_MSG event means HTTP request. `hm` holds parsed request,
		// see https://mongoose.ws/documentation/#struct-mg_http_message
		struct mg_http_message *hm = (struct mg_http_message *) ev_data;

		// If the requested URI is "/api/hi", send a simple JSON response back
		if (mg_http_match_uri(hm, "api/hi")) {
			// Use mg_http_reply() API function to generate JSON response. 
			// It adds  Content-Length header automatically. In the response, we show
			// the requested URI and HTTP body:
			mg_http_reply(conn, 200, "",  "{%m:%m, %m:%m}\n",
				MG_ESC("uri"), mg_print_esc, hm->uri.len, hm->uri.ptr, 
				MG_ESC("body"), mg_print_esc, hm->body.len, hm->body.ptr);
		} else {
			// For all other URIs, serve static content from the current directory
				struct mg_http_serve_opts opts = {.root_dir = "."};
				mg_http_serve_dir(conn, hm, &opts);
		
		}
	}
}

int main () {
	struct mg_mgr mgr;			// Event manager
	mg_mgr_init(&mgr);			// Init manager
	mg_log_set(MG_LL_DEBUG);	// Set debug log level. Default is MG_LL_INFO

	mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL); // Setup Listener
	for(;;)mg_mgr_poll(&mgr, 1000);

	return 0;
}