cgitest: cgi.c
	gcc -Wall -DUNIT_TEST -I . -ggdb3 cgi.c -o cgitest
	gcc -Wall -DPERF_TEST -I . -ggdb3 cgi.c -o cgiperf

clean:
	rm cgitest
	rm cgiperf
