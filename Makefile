sources = a.c 			##
objects = $(sources:.c=.o)
dfile   = $(sources:.c=.d) 	#生成的.d文件
links   = -lpthread	 	##动静态链接库
exe     = exe_a 		##生成可执行文件
warringcc = -Wall -Wextra -Wshadow -pedantic

all: main
main: $(objects)
	gcc $^ -o $@ $(links)
	cp $@ $(exe)
	@chmod +x $(exe)

include $(dfile)
%.d: %.c
	@set -e; rm -rf $@; \
	gcc -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -rf $@.$$$$



#语法检查
lck:
	gcc -c $(sources) $(warringcc)
	-@rm -f $(objects)

.PHONY: clean
clean:
	-rm -f main
	-rm -f $(exe)
	-rm -f $(objects)
	-rm -f $(dfile)

