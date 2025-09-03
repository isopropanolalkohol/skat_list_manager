//
// Created by joshua on 10.04.25.
//

#ifndef SKAT_H
#define SKAT_H
#include <iostream>
#include <vector>

#define M_EINFACH "Einfach"
#define M_SCHNEIDER "Schneider"
#define M_SCHWARZ "Schwarz"

#define M_H_EINFACH "Hand Einfach"
#define M_H_SCHNEIDER "Hand Schneider"
#define M_H_SCHWARZ "Hand Schwarz"
#define M_N_H "Hand"
#define M_H_SCHNEIDER_ANGESAGT "Hand Schneider angesagt"
#define M_H_SCHNEIDER_ANGESAGT_SCHWARZ "Hand Schneider angesagt, schwarz"
#define M_H_SCHWARZ_ANGESAGT "Hand Schwarz angesagt"
#define M_OFFEN "Offen"


enum GameType
{
    SCHELLEN, ROT, GRUEN, EICHEL, GRAND, ZERO, ZERO_HAND, ZERO_OUVERT, ZERO_HAND_OUVERT, RAMSCH, UNSET
};

struct Modifier
{
    std::string name;
    int multiplier;
};
extern std::vector<Modifier> modifiers;

struct GameEntry
{
    GameType type;
    int peaks{};
    Modifier modifier;
    int user_id{};
    bool won{};
};

struct TableGameEntry
{
    std::string playerName;
    std::string GameType;
    int peaks;
    std::string ModifierName;
    int value;
    bool hasWon;
    std::string dateTime;
    int id;
};

int return_base_value(GameType type);

int calculate_game_value(GameEntry entry);

std::string return_type_name(GameType type);

#endif //SKAT_H
