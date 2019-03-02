deskCal: scanner.l parser.y cathy.h
	bison -d parser.y
	flex -oscanner.lex.c scanner.l
	gcc -o cathy parser.tab.c scanner.lex.c cathy.c -lfl
