default: touchtyping

%: %.c
	clang -Dtest_$@ -std=c2x -lncurses -Wall -pedantic -g $@.c -o $@ \
		-fsanitize=undefined -fsanitize=address
