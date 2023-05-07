CC = gcc
RM = rm -f

SOURCE = $(wildcard *.c)
OBJECTFILES = $(SOURCE:.c=.o)

wavEditor: $(OBJECTFILES)
	$(CC) -o wavEditor $^
clean:
	$(RM)  *.o wavEditor 
