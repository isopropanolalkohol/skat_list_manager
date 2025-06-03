//
// Created by joshua on 10.04.25.
//
#include "skat.h"
#include <iostream>
#include <vector>



std::vector<Modifier> modifiers
{
    Modifier{M_EINFACH, 1},
    Modifier{M_SCHNEIDER, 2},
    Modifier{M_SCHWARZ, 3},
    Modifier{M_H_EINFACH, 2},
    Modifier{M_H_SCHNEIDER, 3},
    Modifier{M_H_SCHWARZ, 4},
    Modifier{M_N_H, 1},
    Modifier{M_H_SCHNEIDER_ANGESAGT, 4},
    Modifier{M_H_SCHNEIDER_ANGESAGT_SCHWARZ, 5},
    Modifier{M_H_SCHWARZ_ANGESAGT, 6},
    Modifier{M_OFFEN, 7}
};

int return_base_value(GameType type)
{
    switch (type)
    {
        case SCHELLEN:
            return 9;
        case ROT:
            return 10;
        case GRUEN:
            return 11;
        case EICHEL:
            return 12;
        case GRAND:
            return 24;
        case ZERO:
            return 23;
        case ZERO_HAND:
            return 35;
        case ZERO_OUVERT:
            return 46;
        case ZERO_HAND_OUVERT:
            return 59;
        case RAMSCH:
            return 20;
        default:
            return 0;
    }
}


int calculate_game_value(GameEntry entry)
{
    return (entry.peaks + entry.modifier.multiplier)*return_base_value(entry.type);
}

std::string return_type_name(GameType type)
{
    switch (type)
    {
        case SCHELLEN:
            return "Schellen";
        case ROT:
            return "Rot";
        case GRUEN:
            return "Gruen";
        case EICHEL:
            return "Eichel";
        case GRAND:
            return "Grand";
        case ZERO:
            return "Null";
        case ZERO_HAND:
            return "Null";
        case ZERO_OUVERT:
            return "Null ouvert";
        case ZERO_HAND_OUVERT:
            return "Null ouvert";
        case RAMSCH:
            return "Ramsch";
    }
}
