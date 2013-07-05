warmup2: warmup2.c my402list.o warmup2.c my402list.h 
	gcc -lpthread -Wall -lm -o warmup2 -g warmup2.c my402list.o 

my402list.o: my402list.c my402list.h
	gcc -g -c -Wall my402list.c
	
clean:
	rm -f *.o warmup2