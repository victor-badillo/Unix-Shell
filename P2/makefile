EXECUTABLE=p2
CFLAGS= -Wall -g

p2: p2.o auxFunctions.o head_list.o memory_list.o
	gcc $(CFLAGS) -o $(EXECUTABLE) p2.o head_list.o auxFunctions.o memory_list.o
	
p2.o: p2.c headers.h
	gcc $(CFLAGS) -c p2.c

auxFunctions.o: auxFunctions.c headers.h
	gcc $(CFLAGS) -c auxFunctions.c
     
head_list.o: head_list.c head_list.h
	gcc $(CFLAGS) -c head_list.c
	
memory_list.o: memory_list.c memory_list.h
	gcc $(CFLAGS) -c memory_list.c
     
clean:
	rm *.o p2
