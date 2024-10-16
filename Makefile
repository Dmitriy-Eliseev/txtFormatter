CC = gcc
#CC = clang
#CC = tcc

all:
	$(CC) txtfmt.c help.c tag_handler.c tags.c tags_lib.c tinyexpr.c -lm -O3 -o txtfmt 
