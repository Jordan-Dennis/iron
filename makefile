SRC_DIR = src
OUT_DIR = out
CC = gcc
CFLAGS = -lm


ising: $(SRC_DIR)/*c
	$(CC) -o $(OUT_DIR)/$@ $? $(CFLAGS)
