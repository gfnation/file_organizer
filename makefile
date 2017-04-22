organizer: main.o easyOrg.o musicOrg.o videoOrg.o
	gcc main.o easyOrg.o musicOrg.o videoOrg.o -o organizer
main.o: main.c
	gcc -c main.c
easyOrg.o: easyOrg.c
	gcc -c easyOrg.c
musicOrg.o: musicOrg.c
	gcc -c musicOrg.c
videoOrg.o: videoOrg.c
	gcc -c videoOrg.o
clean:
	rm *.o
destroy:
	rm -rf documents movies music other pictures shows
