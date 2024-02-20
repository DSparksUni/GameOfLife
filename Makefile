SDIR = src
ODIR = obj
IDIR = include

vpath %.c $(SDIR)

CC = gcc
CFLAGS = -I $(IDIR) -Wall -Wno-missing-braces -Wextra -pedantic -std=c11 \
					-D DEBUG
TARGET = uni.exe

__OBJS = main.o board.o
OBJS = $(patsubst %.o,$(ODIR)/%.o,$(__OBJS))

run: all
	./$(TARGET)

all: $(TARGET)
	echo Build Complete!

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm $(TARGET) $(OBJS)
