udp: udp.c udp.h main.c
	gcc -o udp udp.c main.c
clean:	
	rm udp
