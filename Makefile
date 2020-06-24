all: face

clean:
	rm -f build/*.o
	rm -f face

face: build/main.o build/util.o build/cmdparse.o
	gcc -g -lreadline -o $@ $^

build/%.o: src/%.c
	gcc -g -Iinclude/ -c -o $@ $^

