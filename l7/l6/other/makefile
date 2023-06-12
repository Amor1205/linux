mytest: test.cc
	g++ -o $@ $^ -I./lib -L./lib -lmymath
# 只是告知了编译器，编译好了后与编译器无关
# 运行程序时是加载器帮你识别库文件
# 需要在运行时告知系统 库在哪 有几个解决办法
# 1.
# LD_LIBRARY_PATH : 指明程序启动之后，库的搜索路径，默认在系统里没有配置
# 可以在lib下 export LD_LIBRARY_PATH=pwd的路径
# 环境变量在登陆就不见了，所以可以添加到以下路径中：vim ~/.bash_profile  <or>  vim ~/.bash_rc
# 不建议
# 2.
# ldconfig 
# lib所在路径拷贝到/etc/ld.so.conf 中新建的.conf后缀文件中 执行ldconfig
.PHONY:clean
clean:
	rm -f mytest








# test:test.cc
# 	g++ -o $@ $^ -I./lib -L./lib -lmymath
# .PHONY:clean
# clean:
# 	rm -f log.txt test
# 	rm -fr ./.ccls-cache
