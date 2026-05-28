//
// Created by Bogdan Tudosie on 28.5.2026.
//

#ifndef POKEMONEXPLORER_MOVE_H
#define POKEMONEXPLORER_MOVE_H

#include <string>
#include <nlohmann/json.hpp>
#include <optional>
#include "json-parseable.h"

class Move : public JsonParseable<Move> {
public:
    int id;
    std::string name;
    std::optional<int> effectChance;
    int ppCost;
    int priority;
    int power;

    static Move fromJson(const nlohmann::json& j) {
        Move move;
        move.id       = j.at("id").get<int>();
        move.name     = j.at("name").get<std::string>();
        move.ppCost   = j.at("pp").get<int>();
        move.priority = j.at("priority").get<int>();

        // power and effect_chance are null for status moves
        move.power        = j.at("power").is_null() ? 0 : j.at("power").get<int>();
        move.effectChance = j.at("effect_chance").is_null() ? std::nullopt : std::optional<int>(j.at("effect_chance").get<int>());

        return move;
    }
};

#endif //POKEMONEXPLORER_MOVE_H
