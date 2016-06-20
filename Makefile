all:
	gcc test.c config.c -o test
clean:
	-rm test
