# build.sh 
# --------
# Contains useful abreviations for working on the project. 
# An attempt at a more wholesome and involved experience 
# that simplifies commonly used functionality.


# build
# -----
# Compile the c code of the simulation in src/ising.c to 
# machine code in the out directory as ising.
build() {
    gcc -o out/ising src/ising.c -lm
}


# run
# ---
# Excecute the executable created by build. These two will 
# nearly always be run together. 
# 
# parameters
# ----------
# 
run() {
    while getopts ":hl:" option; do
        case $option in
            h) echo TODO:;;
            l) build;;
        esac
    done
    out/ising
}


edit() {
    vim src/ising.c
}


clean() {
    rm out/*txt
}
