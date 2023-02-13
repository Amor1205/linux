# 形成动态链接的共享库
libmymath.so: sub.o add.o
	g++ -shared -o $@ $^
# 与位置无关 ： -fPIC
# 库文件可以在内存的任地址加载，而且不影响与其他程序的关联性
%.o : ./test_lib/%.cc
	g++ -fPIC -c $<
.PHONY:clean
clean:
	rm -fr lib
	rm -f libmymath.so *.o
.PHONY:lib
lib:
	mkdir lib
	cp ./test_lib/*.h lib
	cp libmymath.so lib










# libmymath.a : ./test_lib/sub.o ./test_lib/add.o
# 	ar -rc $@ $^
# %.o : ./test_lib%.cc
# 	g++ -c $<
# .PHONY:clean
# clean:
# 	rm -f /test_lib/*.o libmymath.a 
# 	rm -rf output
# .PHONY:output
# output:
# 	mkdir output
# 	cp -fr ./test_lib/*.h output
# 	cp libmymath.a output


