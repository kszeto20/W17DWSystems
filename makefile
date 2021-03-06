all: write.o control.o
	gcc -o write write.o
	gcc -o control control.o

control.o: control.c control.h
	gcc -c control.c

write.o: write.c write.h
	gcc -c write.c

clean:
	-rm *.o
	-rm control
	-rm write
