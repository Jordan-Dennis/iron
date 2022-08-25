# build.sh 
# --------
# Contains useful abreviations for working on the project. 
# An attempt at a more wholesome and involved experience 
# that simplifies commonly used functionality.


load_config() {
    if [[ -f ".config.toml" ]]; then
        let line_number=0
        if [[ ! -z "$block" ]]; then
            unset block
        fi 
        declare -l block
        if [[ ! -z "$extras" ]]; then 
            unset extras
        fi 
        declare -g extras
        if [[ ! -z "$executable" ]]; then 
            unset executable
        fi
        declare -g executable
        if [[ ! -z "$viewable" ]]; then 
            unset viewable
        fi
        declare -g viewable
        if [[ ! -z "$publishable" ]]; then 
            unset publishable 
        fi
        declare -g publishable
        if [[ ! -z "$readables" ]]; then 
            unset readables
        fi
        declare -g readables
        while read line; do
            let line_number++
            if [[ $line == "[executable]" ]]; then
                let target=line_number+1
                executable="$(sed -n "${target}p" .config.toml)"
            elif [[ $line == "[viewable]" ]]; then
                let target=line_number+1
                viewable="$(sed -n "${target}p" .config.toml)"
            elif [[ $line == "[publishable]" ]]; then
                let target=line_number+1
                publishable="$(sed -n "${target}p" .config.toml)"
            else
                if [[ $line == "[extras]" ]]; then
                    block="extras"
                elif [[ $line == "[readables]" ]]; then 
                    block="readables"
                elif [[ $block == "extras" ]]; then 
                    extras+=( "$line" )
                    if [[ -z "${line/ //}" ]]; then
                        block="null"
                    fi
                elif [[ $block == "readables" ]]; then
                    readables+=( "$line" )
                    if [[ -z "${line/ //}" ]]; then
                        block="null"
                    fi
                fi 
            fi
        done < .config.toml
    else
        echo -e "${RED}error:${NORMAL} The config file has not been "
        echo -e "found. Aborting!"
    fi
    if [[ -z "$executable" ]] ||
        [[ -z "$viewable" ]] ||
        [[ -z "$publishable" ]]; then  
        echo "${RED}error:${NORMAL} Configuration not read correctly"
    fi 
    return 
}


#load_config

RED="\033[[0;31m"
NORMAL="\033[[0m"
GREEN="\033[[0;32m"
BLUE="\033[[0;36m"


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
    OPTIND=1
    while getopts "ho" option; do
        case $option in
            h) print_build_help
               OPTIND=1
               return 0
               ;;
            o) gcc -O3 -lm -o -ftree-loop-vectorize -ftree-loop-optimize ${executable} ${COMPILABLE}
               OPTIND=1
               return 0
               ;;
        esac
    done
    gcc -o ${executable} ${COMPILABLE} -lm
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
    OPTIND=1
    while getopts "hl" option; do
        case $option in
            h) print_run_help
               OPTIND=1
               return 0
               ;;
            l) build 
               ${executable}
               OPTIND=1
               return
               ;;
        esac
    done
    file=out/ising
    if [[ -e "$file" ]]; then
        out/ising $1
    else
        echo "The code has not been compiled. Use instead:"
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
    OPTIND=1
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
        vim ${viewable}
    elif [[ $1 == "publishable" ]] || [[ $1 == "p" ]]; then
        vim "pub/${publishable}"
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
    OPTIND=1
    while getopts "h" option; do
        case $option in
            h) print_clean_help
               OPTIND=1
               return 0
               ;;
        esac
    done
    if [[ -f "${executable}" ]]; then
        rm ${executable}
    fi
    if [[ -f "${READABLE}" ]]; then
        rm ${READABLE}
    fi
    for extra in "${extras[[@]]}"; do
        if [[ -f "${extra}" ]]; then
            rm ${extra}
        fi
    done
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
    OPTIND=1
    while getopts "hl" option; do
        case $option in
            h) print_peak_help
               OPTIND=1
               return 0
               ;;
            l) cat ${readables[[@]]}
               OPTIND=1
               return 0
               ;;
        esac
    done
    for readable in ${readables[[@]]}; do
        echo -e "${BLUE}${readable}${NORMAL}"
        head ${readable}
    done
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
    echo -e "${RED}-v${NORMAL} : Open the result in the pdf viewer"
}
plot() {
    OPTIND=1
    while getopts "hv" option; do
        case $option in
            h) print_plot_help
               OPTIND=1
               return 0
               ;;
            v) gnuplot ${viewable}
               evince pub/q1.pdf
               OPTIND=1
               return 0
               ;;
        esac
    done
    gnuplot ${viewable}
}


print_publish_help() {
    echo -e "${BLUE}publish${NORMAL}"
    echo -e "-------"
    echo -e "Generate the .pdf file associated with the final output"
    echo 
    echo -e "${GREEN}parameters${NORMAL}"
    echo -e "----------"
    echo -e "${RED}-h${NORMAL} : Display this help message and exit."
    echo -e "${RED}-l${NORMAL} : Run latex in quiet mode to supress output."
}
publish() {
    cd pub
    OPTIND=1
    while getopts "hq" option; do
        case ${option} in
            h) print_publish_help
               OPTIND=1
               return 0
               ;;
            q) pdftex ${publishable} > /dev/null/
               OPTIND=1
               return 0
               ;;
        esac
    done 
    pdftex ${publishable} 
    cd ..
}


print_execute_help() {
    echo -e "${BLUE}execute${NORMAL}"
    echo -e "-------"
    echo -e "Compile, build, run, plot and generate the report."
    echo
    echo -e "${GREEN}parameters${NORMAL}"
    echo -e "----------"
    echo -e "${RED}-h${NORMAL} : Display this help message then exit."
}
execute() {
    if [[ $1 == "-h" ]]; then
        print_execute_help
    fi
    build -o
    run 
    plot
    publish

    if [[ $1 == "clean" ]]; then
        clean
    fi
}


help() {
    echo -e "${BLUE}help${NORMAL}"
    echo -e "----"
    echo -e "The useful functions available in this directory are:"
    echo -e "${RED}execute${NORMAL} ${BLUE}(clean)${NORMAL}"
    echo -e "${RED}run${NORMAL} ${BLUE}(-h -l)${NORMAL}"
    echo -e "${RED}publish${NORMAL} ${BLUE}(-h -q)${NORMAL}"
    echo -e "${RED}build${NORMAL} ${BLUE}(-h -o)${NORMAL}"
    echo -e "${RED}clean${NORMAL} ${BLUE}(-h)${NORMAL}"
    echo -e "${RED}peak${NORMAL} ${BLUE}(-h -f)${NORMAL}"
    echo -e "${RED}edit${NORMAL} ${BLUE}(compilable viewable publishable)${NORMAL}"
}


print_todo_help() {
    echo -e "${BLUE}todo${NORMAL}"
    echo -e "----"
    echo -e "Draft and get reminders for yourself"
    echo
    echo -e "${GREEN}parameters${NORMAL}"
    echo -e "----------"
    echo -e "${RED}-h${NORMAL}: Print this message then exit"
    echo -e "${RED}add${NORMAL}: Add a new todo to the list"
    echo -e "${RED}remove${NORMAL}: Remove a todo from the list"
    echo -e "${RED}complete${NORMAL}: Complete a todo"
}
_check_todo() {
    if [[ -f .todo.yml ]]; then 
        return 
    else
        echo "${GREEN}todos:${NORMAL}" > .todo.yml
    fi
}
_add_todo() {
    for i in "$@"; do
        if [[ "$i" == "--title" ]]; then
            printf "    ${BLUE}title${NORMAL}: " >> .todo.yml
        elif [[ $i == "--description" ]]; then 
            title="false"
            printf "\n" >> .todo.yml
            printf "    ${BLUE}description${NORMAL}: " >> .todo.yml
        else 
            printf "$i " >> .todo.yml
        fi
    done
    printf "\n" >> .todo.yml
}
_remove_todo() {
    mv .todo.yml .save.yml
    target=$(printf "${BLUE}title${NORMAL}: $1\n")
    line_number=0
    while read line; do
        let line_number++
        if [[ "$target" == "$line" ]]; then
            let target_line_number=$line_number+1
        elif [[ "$target_line_number" == "$line_number" ]]; then
            true
        else
            if [[ "$line_number" == "1" ]]; then
                _check_todo 
            else
                printf "    $line\n" >> .todo.yml
            fi 
        fi
    done < .save.yml   
    rm .save.yml
}
todo() {
    OPTIND=1
    while getopts "h" option; do
        case ${option} in
            h) print_todo_help
               OPTIND=1
               return
               ;;
        esac
    done
    _check_todo
    if [[ $1 == "add" ]]; then 
        _add_todo $2 $3 $4 $5
    elif [[ $1 == "remove" ]]; then 
        _remove_todo $2 
    elif [[ $1 == "view" ]]; then 
        echo -e "$(cat .todo.yml)"
    elif [[ $1 == "clear" ]]; then 
        rm .todo.yml
    fi    
}    
    


if [[ $1 == "execute" ]]; then 
    execute 
fi 
