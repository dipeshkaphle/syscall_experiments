no-event:
	gcc -O3 -o p1 _p1.c
	gcc -O3 -o p2 _p2.c
event:
	gcc -O3 -o p1 p1.c
	gcc -O3 -o p2 p2.c
clean:
	rm p1 p2
