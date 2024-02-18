default: touchtyping

%: %.c
	clang -Dtest_$@ -std=c11 -lncurses -Wall -pedantic -g $@.c -o $@ \
		-fsanitize=undefined -fsanitize=address

