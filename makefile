OUT_DIR = out
CC = gcc
CFLAGS = -lm -O3

external_magnetic_field: src/external_field.c src/utils.c
	$(CC) -o $(OUT_DIR)/$@ $? $(CFLAGS)

ising: src/1d_ising.c src/2d_ising.c src/toml.c src/main.c src/utils.c
	$(CC) -o $(OUT_DIR)/$@ $? $(CFLAGS)
