CC=gcc -Wall -ggdb3  -std=c11 -Werror -pedantic -pthread
OPTS=-Dftr_unittest
future_test: future.c future_test.c
	$(CC) $(OPTS) future.c future_test.c -o future_test
clean:
	rm -f *.o future future_test
