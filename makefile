OUT_DIR = out
CC = gcc
CFLAGS = -lm

ising_1d_test: src/1d_ising.c src/utils.c src/tests/1d_ising.c src/toml.c
	$(CC) -o $(OUT_DIR)/$@ $? $(CFLAGS)

ising_2d_test: src/2d_ising.c src/utils.c src/tests/2d_ising.c src/toml.c
	$(CC) -o $(OUT_DIR)/$@ $? $(CFLAGS)

external_magnetic_field: src/external_field.c src/utils.c
	$(CC) -o $(OUT_DIR)/$@ $? $(CFLAGS)

toml_test: src/toml.c src/tests/toml.c
	$(CC) -o $(OUT_DIR)/$@ $? $(CFLAGS)
    

ising: src/*.c
	$(CC) -o $(OUT_DIR)/$@ $? $(CFLAGS)
