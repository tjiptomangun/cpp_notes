#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main (void)
{
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_PAIR);
    int rc = zmq_bind (responder, "tcp://*:5555");
    assert (rc == 0);

    while (1) {
		char errbuff [200] = {0};
        char buffer [1000] = {0};
        rc = zmq_recv (responder, buffer, 1000, 0);
		if (rc < 0) {
			if (errno != EAGAIN) {
				strerror_r(errno, errbuff, sizeof(errbuff)); 
				fprintf(stderr,  "%s \n", errbuff);
				break;
			}
		}
        printf ("Received %s\n", buffer);
        usleep (1000000);          //  Do some 'work'
        rc = zmq_send (responder, "World", 5, 0);
		if (rc < 0) {
			if (errno != EAGAIN) {
				strerror_r(errno, errbuff, sizeof(errbuff)); 
				fprintf(stderr,  "%s \n", errbuff);
				break;
			}
		}
    }
    return 0;
}

