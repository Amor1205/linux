libmymath.a : ./test_lib/sub.o ./test_lib/add.o
	ar -rc $@ $^
%.o : ./test_lib%.cc
	g++ -c $<
.PHONY:clean
clean:
	rm -f *.o libmymath.a
.PHONY:output
output:
	mkdir output
	cp -fr ./test_lib/*.h output
	cp libmymath.a output
