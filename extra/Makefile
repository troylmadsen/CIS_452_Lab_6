filename = sample
objects = sample.o
static_libs = libsem_tk.a
var = 10000000

sample: $(objects) $(static_libs)
	clang -o $(filename) -Wall $(objects) $(static_libs)

sample.o: sample.c
	clang -c -o sample.o -Wall sample.c
libsem_tk.a: sem_tk.h
	clang -c -o libsem_tk.o sem_tk.c
	ar rcs libsem_tk.a libsem_tk.o sem_tk.h

.PHONY: clean run debug
clean:
	-rm $(filename) $(objects) $(static_libs)

run: $(filename)
	./$(filename) $(var)

debug:
	clang -c -o sample.o -Wall -g sample.c
	clang -o ${filename} -Wall -g $(objects) $(static_libs)
