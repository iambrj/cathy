cathy: scanner.l parser.y cathy.h cathy.c
	bison -d parser.y && \
	flex -oscanner.lex.c scanner.l && \
	gcc -g -o $@ parser.tab.c scanner.lex.c cathy.c -lm -lfl
clean:
	rm -f parser.tab.c parser.tab.h scanner.lex.c
