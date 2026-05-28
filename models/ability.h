//
// Created by Bogdan Tudosie on 28.5.2026.
//

#ifndef POKEMONEXPLORER_ABILITY_H
#define POKEMONEXPLORER_ABILITY_H

#include <string>
#include <nlohmann/json.hpp>
#include "json-parseable.h"

class Ability: public JsonParseable<Ability> {
public:
    std::string name;
    bool isHidden;

    static Ability fromJson(const nlohmann::json& j) {
        Ability ability;
        ability.name     = j.at("ability").at("name").get<std::string>();
        ability.isHidden = j.at("is_hidden").get<bool>();
        return ability;
    }
};

#endif //POKEMONEXPLORER_ABILITY_H
