# 
#

build() {
    gcc -o out/ising src/ising.c -lm
}


run() {
    out/ising
}


edit() {
    vim src/ising.c
}
