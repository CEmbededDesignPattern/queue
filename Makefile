# macro

# build tools
CC := gcc
CFLAGS := -g
LDFLAGS :=

# source file
srcFiles = $(wildcard *.c)
objFiles = $(patsubst %.c, %.o, $(srcFiles))

# target
TARGET := main


#####################################################
# 生成可执行文件
all:$(TARGET)

# 可执行文件的生成依赖于.o文件
$(TARGET):$(objFiles)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $^

# .o文件依赖于.c文件
%.o:%.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) $<

.PHONY:clean all

clean:
	rm -f *.o
	rm -f $(TARGET)
