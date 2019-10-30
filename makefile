all: bwtencode bwtsearch

bwtencode: bwtencode.c encode.o 
	gcc bwtencode.c encode.o -o bwtencode

encode.o: encode.c
	gcc -c encode.c


bwtsearch: bwtsearch.c bitmap.o search.o
	gcc bwtsearch.c bitmap.o search.o -o bwtsearch

search.o: search.c
	gcc -c search.c

bitmap.o: bitmap.c
	gcc -c bitmap.c

clean:
	rm *.o test
