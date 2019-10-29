cgitest: cgi.c
	gcc -Wall -DCGI_UNIT_TEST -I . -ggdb3 cgi.c -o cgitest
	gcc -Wall -DCGI_PERF_TEST -I . -ggdb3 cgi.c -o cgiperf

clean:
	rm cgitest
	rm cgiperf
