OUT_DIR = out
CC = gcc
CFLAGS = -lm


ising_1d_test: src/1d_ising.c src/utils.c
	$(CC) -o $(OUT_DIR)/$@ $? $(CFLAGS)

ising_2d_test: src/2d_ising.c src/utils.c
	$(CC) -o $(OUT_DIR)/$@ $? $(CFLAGS)

ising_3d_test: src/3d_ising.c src/utils.c
	$(CC) -o $(OUT_DIR0/$@ $? $(CFLAGS)

ising: src/*.c
	$(CC) -o $(OUT_DIR)/$@ $? $(CFLAGS)
