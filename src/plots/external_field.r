library("tidyverse")
library("latex2exp")

field <- read_csv("pub/data/external_field.csv") %>% 
    mutate(magnetic_field = factor(magnetic_field)) %>%
    mutate(epsilon = factor(epsilon)) %>% 
    ggplot()
