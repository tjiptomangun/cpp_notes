LDFLAGS=-ggdb3 -Wall
LDDIR=-L src/lib
INCDIR=-I src/inc

all: wc onewordperline longestline_revised morethan blanks copies trimmer \
	entab detab value_of_EOF fold_line remove_comments check_csyntax

wc: wc.o 
	gcc -o target/wc target/wc.o

wc.o : src/bin/wc.c
	gcc -c src/bin/wc.c -o target/wc.o

onewordperline: onewordperline.o 
	gcc -o target/onewordperline target/onewordperline.o

onewordperline.o: src/bin/onewordperline.c
	gcc -c src/bin/onewordperline.c -o target/onewordperline.o

longestline_revised: longestline_revised.o 
	gcc -o target/longestline_revised target/longestline_revised.o

longestline_revised.o: src/bin/longestline_revised.c
	gcc -c src/bin/longestline_revised.c -o target/longestline_revised.o

morethan: morethan.o 
	gcc -o target/morethan target/morethan.o

morethan.o: src/bin/morethan.c
	gcc -c src/bin/morethan.c -o target/morethan.o

blanks: blanks.o 
	gcc -o target/blanks target/blanks.o

blanks.o: src/bin/blanks.c
	gcc -c src/bin/blanks.c -o target/blanks.o

copies: copies.o 
	gcc -o target/copies target/copies.o

copies.o: src/bin/copies.c
	gcc -c src/bin/copies.c -o target/copies.o

trimmer: trimmer.o 
	gcc -o target/trimmer target/trimmer.o

trimmer.o: src/bin/trimmer.c
	gcc -c src/bin/trimmer.c -o target/trimmer.o

entab: entab.o 
	gcc -o target/entab target/entab.o

entab.o: src/bin/entab.c
	gcc -c src/bin/entab.c -o target/entab.o

detab: detab.o 
	gcc -o target/detab target/detab.o

detab.o: src/bin/detab.c
	gcc -c src/bin/detab.c -o target/detab.o

value_of_EOF: value_of_EOF.o 
	gcc -o target/value_of_EOF target/value_of_EOF.o

value_of_EOF.o: src/bin/value_of_EOF.c
	gcc -c src/bin/value_of_EOF.c -o target/value_of_EOF.o

fold_line: fold_line.o 
	gcc $(LDFLAGS) -o target/fold_line target/fold_line.o

fold_line.o: src/bin/fold_line.c
	gcc $(LDFLAGS) -c src/bin/fold_line.c -o target/fold_line.o

remove_comments: remove_comments.o 
	gcc $(LDFLAGS) -o target/remove_comments target/remove_comments.o

remove_comments.o: src/bin/remove_comments.c
	gcc $(LDFLAGS) -c src/bin/remove_comments.c -o target/remove_comments.o

check_csyntax: check_csyntax.o 
	gcc $(LDFLAGS) -o target/check_csyntax target/check_csyntax.o

check_csyntax.o: src/bin/check_csyntax.c
	gcc $(LDFLAGS) -c src/bin/check_csyntax.c -o target/check_csyntax.o

clean:
	rm target/wc.o target/wc target/onewordperline.o target/onewordperline \
		target/longestline_revised.o target/longestline_revised\
		target/morethan.o target/morethan \
		target/blanks.o target/blanks \
		target/copies.o target/copies\
		target/trimmer.o target/trimmer \
		target/entab.o target/entab \
		target/detab.o target/detab \
		target/value_of_EOF.o target/value_of_EOF \
		target/fold_line.o target/fold_line \
		target/remove_comments.o target/remove_comments \
		target/check_csyntax.o target/check_csyntax 

.PHONY: wc wc.o onewordperline.o onewordperline \
	longestline_revised.o longestline_revised \
	morethan.o morethan blanks.o blanks copies.o copies \
	trimmer.o trimmer entab.o entab detab.o detab \
	value_of_EOF.o value_of_EOF \
	fold_line.o fold_line \
	remove_comments.o remove_comments \
	check_csyntax.o check_csyntax 
