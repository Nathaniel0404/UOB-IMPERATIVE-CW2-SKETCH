default: test

test: sketch.c test.c
	clang -DTESTING -std=c11 -Wall -pedantic -g sketch.c test.c -o test -fsanitize=undefined `pkg-config --libs --cflags sdl2`

sketch: sketch.c
	clang -std=c11 -Wall -pedantic -g sketch.c displayfull.c -o sketch -fsanitize=undefined `pkg-config --libs --cflags sdl2`

%: %.c
	clang -Dtest_$@ -std=c11 -Wall -pedantic -g $@.c -o $@ \
	    -fsanitize=undefined -fsanitize=address
