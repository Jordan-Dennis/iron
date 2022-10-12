plot() {
    python src/plots/question_1.py first_and_last 
    python src/plots/question_1.py physical_parameters 
    python src/plots/question_1.py magnetisation 
    python src/plots/question_2.py physical_parameters 
    python src/plots/question_2.py magnetisation 
    python src/plots/question_2.py heating_and_cooling 
}


run() {
    echo -e "\033[31mRunning 1D simulations:\033[37m"
    out/ising 1d first_and_last configs/first_and_last_ising_1d.toml
    echo -e "\t - First and last!"
    out/ising 1d physical_parameters configs/physical_parameters_ising_1d.toml
    echo -e "\t - Physical Parameters!"
    out/ising 1d magnetisation configs/magnetisation_ising_1d.toml
    echo -e "\t - Magnetisations!"
    echo -e "\033[31mRunning 2D simulations:\033[37m"
    out/ising 2d first_and_last configs/first_and_last_ising_2d.toml
    python src/plots/question_2.py first_and_last pub/data/first_and_last_ising_2d.csv
    echo -e "\t - First and last!"
    out/ising 2d physical_parameters configs/physical_parameters_ising_2d.toml
    echo -e "\t - Physical Parameters!"
    out/ising 2d magnetisation configs/magnetisation_ising_2d.toml
    echo -e "\t - Magnetisations!"
    out/ising 2d heating_and_cooling configs/heating_and_cooling_ising_2d.toml
    echo -e "\t - Heating and Cooling!"
}


schedule() {
    run 
    plot
}
