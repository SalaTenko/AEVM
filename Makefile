all: bin/app

bin/app: build/main.o build/myTerm.o build/mysimple.o build/myBigChars.o build/myReadkey.o build/myConsole.o
	gcc -Wall build/main.o build/myTerm.o build/mysimple.o build/myBigChars.o build/myReadkey.o build/myConsole.o -o bin/app

build/main.o: src/main.c
	gcc -Wall -I include -c src/main.c -o build/main.o

build/myTerm.o: src/myTerm.c
	gcc -Wall -I include -c src/myTerm.c -o build/myTerm.o

build/mysimple.o: src/mysimple.c
	gcc -Wall -I include -c src/mysimple.c -o build/mysimple.o

build/myBigChars.o: src/myBigChars.c
	gcc -Wall -I include -c src/myBigChars.c -o build/myBigChars.o

build/myReadkey.o: src/myReadkey.c
	gcc -Wall -I include -c src/myReadkey.c -o build/myReadkey.o

build/myConsole.o: src/myConsole.c
	gcc -Wall -I include -c src/myConsole.c -o build/myConsole.o


clear:
	rm -rf bin/* build/* 
