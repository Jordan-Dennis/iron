# build.sh 
# --------
# Contains useful abreviations for working on the project. 
# An attempt at a more wholesome and involved experience 
# that simplifies commonly used functionality.


COMPILABLE=src/ising.c
EXECUTABLE=out/ising
PUBLISHABLE=pub/report.tex
READABLE=out/data.txt
VIEWABLE=pub/figures.gnu


RED="\033[0;31m"
NORMAL="\033[0m"
GREEN="\033[0;32m"
BLUE="\033[0;36m"


print_build_help() {
    echo -e "${BLUE}build${NORMAL}"
    echo -e "-----"
    echo -e "Compile the c code of the simulation in src/ising.c to" 
    echo -e "machine code in the out directory as ising."
    echo
    echo -e "${GREEN}parameters${NORMAL}"
    echo -e "----------"
    echo -e "${RED}-h${NORMAL} : Print this helpful message and then exit"
    echo -e "${RED}-o${NORMAL} : Pass additional optimisation flags to gcc"
}
build() {
    while getopts ":ho:" option; do
        case $option in
            h) print_build_help
               OPTIND=1
               return 0
               ;;
            o) gcc -O3 -lm -o -ftree-loop-vectorize -ftree-loop-optimize ${EXECUTABLE} ${COMPILABLE}
               OPTIND=1
               return 0
               ;;
        esac
    done
    gcc -o ${EXECUTABLE} ${COMPILABLE} -lm
}


print_run_help() {
    echo -e "${BLUE}run${NORMAL}"
    echo -e "---"
    echo -e "Excecute the executable created by build. These two will"
    echo -e "nearly always be run together."
    echo 
    echo -e "${GREEN}parameters${NORMAL}"
    echo -e "----------"
    echo -e "${RED}-h${NORMAL} : Print this error message and then exit."
    echo -e "${RED}-l${NORMAL} : Recompile the code and run by invoking build."
}
# TODO: Allow the additional optimisation flag to get passed into 
# the -l command which would further compile the code. 
run() {
    while getopts ":hl:" option; do
        case $option in
            h) print_run_help
               OPTIND=1
               return 0
               ;;
            l) build 
               ${EXECUTABLE}
               OPTIND=1
               ;;
        esac
    done
    file=out/ising
    if [ -e "$file" ]; then
        out/ising
    else
        echo "The code has not been compile. Use instead:"
        echo "run -l or build; run."
    fi
}


print_edit_help() {
    echo -e "${BLUE}edit${NORMAL}"
    echo -e "----"
    echo -e "A convinience function for editing the source code"
    echo -e "file for this project."
    echo 
    echo -e "${GREEN}parameters${NORMAL}"
    echo -e "----------"
    echo -e "${RED}-h${NORMAL} : Display this help message and exit"
}
edit() {
    while getopts ":h:" option; do
        case $option in
            h) print_edit_help
               OPTIND=1
               return 0
               ;;
        esac
    done

    if [[ $1 == "compilable" ]] || [[ $1 == "c" ]]; then
        vim ${COMPILABLE}
    elif [[ $1 == "viewable" ]] || [[ $1 == "v" ]]; then
        vim ${VIEWABLE}
    fi
}


print_clean_help() {
    echo -e "${BLUE}clean${NORMAL}"
    echo -e "-----"
    echo -e "A convinience funtion for deleting the old executable"
    echo -e "and save file."
    echo
    echo -e "${GREEN}parameters${NORMAL}"
    echo -e "----------"
    echo -e "${RED}-h${NORMAL} : Display this help message then exit."
}
clean() {
    rm ${READABLE}
    rm ${EXECUTABLE}
}


print_peak_help() {
    echo -e "${BLUE}peak${NORMAL}"
    echo -e "----"
    echo -e "View the top of the data file for the spin data file to"
    echo -e "debug that it has run."
    echo
    echo -e "${GREEN}parameters${NORMAL}"
    echo -e "----------"
    echo -e "${RED}-h${NORMAL} : Display this help message then exit"
    echo -e "${RED}-l${NORMAL} : Display the full data set"
}
peak() {
    while getopts ":hl:" option; do
        case $option in
            h) print_peak_help
               OPTIND=1
               return 0
               ;;
            l) cat ${READABLE}
               OPTIND=1
               return 0
               ;;
        esac
    done
    head ${READABLE}
}


print_plot_help() {
    echo -e "${BLUE}plot${NORMAL}"
    echo -e "----"
    echo -e "Plot the data from the previous run. A gnuplot-qt window"
    echo -e "should open containing the figure."
    echo
    echo -e "${GREEN}parameters${NORMAL}"
    echo -e "----------"
    echo -e "${RED}-h${NORMAL} : Display this message and exit"
}
plot() {
    while getopts ":h:" option; do
        case $option in
            h) print_plot_help
               OPTIND=1
               return 0
               ;;
        esac
    done
    gnuplot ${VIEWABLE}
}


print_publish_help() {}
publish() {
    while getopts ":hq:" option; do
        case $option in
            h) ;;
            q) ;;
        esac
    done 
    pdflatex ${PUBLISHABLE} >> ~/dev/null
}


print_execute_help() {}
execute() {
    build -o
    run 
    plot
    publish

    if [[ $1 == "clean" ]]; then
        clean
    fi
}
