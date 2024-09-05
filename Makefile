CC = gcc
CFLAGS = -g -W -Wextra

OBJFILES = stools.o mySA.o
TARGET = mySA

all: $(TARGET)
	 $(MAKE) clean

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean:
	rm -f $(OBJFILES)