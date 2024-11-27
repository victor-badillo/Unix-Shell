EXECUTABLE=p3
CFLAGS= -Wall -g

p3: p3.o auxFunctions.o head_list.o memory_list.o back_list.o
	gcc $(CFLAGS) -o $(EXECUTABLE) p3.o head_list.o auxFunctions.o memory_list.o back_list.o
	
p3.o: p3.c headers.h
	gcc $(CFLAGS) -c p3.c

auxFunctions.o: auxFunctions.c headers.h
	gcc $(CFLAGS) -c auxFunctions.c
     
head_list.o: head_list.c head_list.h
	gcc $(CFLAGS) -c head_list.c
	
memory_list.o: memory_list.c memory_list.h
	gcc $(CFLAGS) -c memory_list.c
	
back_list.o: back_list.c back_list.h
	gcc $(CFLAGS) -c back_list.c

clean:
	rm *.o p3
