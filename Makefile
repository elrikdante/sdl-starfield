test:
	make build;
	make run;
build:
	gcc -c main.c;
	gcc main.o -o main -I/usr/local/include/SDL -D_GNU_SOURCE=1 -D_THREAD_SAFE -L/usr/local/lib -lSDLmain -lSDL -Wl,-framework,Cocoa;
run:
	./main;

clean:
	-rm main;
	-rm main.o;
