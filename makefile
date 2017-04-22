organizer: main.o easyOrg.o musicOrg.o vidOrg.o
	gcc main.o easyOrg.o musicOrg.o vidOrg.o -o organizer
main.o: main.c
	gcc -c main.c
easyOrg.o: easyOrg.c
	gcc -c easyOrg.c
musicOrg.o: musicOrg.c
	gcc -c musicOrg.c
vidOrg.o: vidOrg.c
	gcc -c vidOrg.c
token.o: token.c
	gcc -c token.c
clean:
	rm *.o
destroy:
	rm -rf documents movies music other pictures shows
