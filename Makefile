all: file_search thread_search

file_search: lab4.c
	gcc -o file_search lab4.c

thread_search: thread_search.c
	gcc -o thread_search thread_search.c -lpthread

clean:
	rm -f file_search
	rm -f thread_search
