// meat_temps.h


#ifndef _MEAT_TEMPS_
#define _MEAT_TEMPS_

// includes
#include "main.h"

// typedef
typedef struct meat_temp_
{
    char meatType[8];
    char doneness[14];
    float temperatureF;
    float temperatureC;
} meat_temp;

// beef
meat_temp beefRare = {"Beef", "Rare", 125, 52};
meat_temp beefMedRare = {"Beef", "Medium Rare", 130, 54};
meat_temp beefMed = {"Beef", "Medium", 140, 60};
meat_temp beefMedWell = {"Beef", "Medium Well", 150, 65};
meat_temp beefWell = {"Beef", "Well Done", 160, 71};
meat_temp beefGrnd = {"Beef", "Ground", 160, 71};

// fish
meat_temp fishT_S = {"Fish", "Tuna/Swrdfish", 125, 52};
meat_temp fish = {"Fish", "N/a", 140, 60};

// lamb
meat_temp lambRare = {"Lamb", "Rare", 125, 52};
meat_temp lambMedRare = {"Lamb", "Medium Rare", 130, 54};
meat_temp lambMed = {"Lamb", "Medium", 140, 60};
meat_temp lambMedWell = {"Lamb", "Medium Well", 150, 65};
meat_temp lambWell = {"Lamb", "Well Done", 160, 71};
meat_temp lambChops = {"Lamb", "Chops", 160, 71};

// pork
meat_temp porkMed = {"Pork", "Medium", 145, 63};
meat_temp porkWell = {"Pork", "Well Done", 160, 71};
meat_temp porkOther = {"Pork", "N/a", 160, 71};

// poultry
meat_temp poultry = {"Poultry", "N/a", 165, 74};
meat_temp poultryGrnd = {"Poultry", "Ground", 165, 74};

// target meat selected
meat_temp targetMeat;

#endif