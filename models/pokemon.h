#ifndef _H_POKEMON_H_
#define _H_POKEMON_H_

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <optional>
#include "move.h"
#include "ability.h"
#include "json-parseable.h"

struct Stat {
    std::string name;
    int base_stat;
};

struct Pokemon: public JsonParseable<Pokemon> {
    int id;
    std::string name;
    int baseExperience;
    int height;
    int weight;
    std::vector<std::string> types;
    std::vector<Stat> stats;
    std::vector<Move> moves;
    std::vector<Ability> abilities;

    // Serialization to JSON
    static nlohmann::json toJson(const Pokemon& pokemon) {
        nlohmann::json j;
        j["id"] = pokemon.id;
        j["name"] = pokemon.name;
        j["base_experience"] = pokemon.baseExperience;
        j["height"] = pokemon.height;
        j["weight"] = pokemon.weight;
        j["types"] = pokemon.types;
        j["stats"] = nlohmann::json::array();
        for (const auto& stat : pokemon.stats) {
            nlohmann::json s;
            s["name"] = stat.name;
            s["base_stat"] = stat.base_stat;
            j["stats"].push_back(s);
        }

        j["abilities"] = nlohmann::json::array();
        for (const auto& ability : pokemon.abilities) {
            nlohmann::json s;
            s["name"]      = ability.name;
            s["is_hidden"] = ability.isHidden;
            j["abilities"].push_back(s);
        }

        j["moves"] = nlohmann::json::array();
        for (const auto& move : pokemon.moves) {
            nlohmann::json s;
            s["id"]           = move.id;
            s["name"]         = move.name;
            s["effect_chance"] = move.effectChance;
            s["pp"]           = move.ppCost;
            s["priority"]     = move.priority;
            s["power"]        = move.power;
            j["moves"].push_back(s);
        }
        return j;
    }

    static Pokemon fromJson(const nlohmann::json& j) {
        Pokemon pokemon;
        pokemon.id = j.at("id").get<int>();
        pokemon.name = j.at("name").get<std::string>();
        pokemon.baseExperience = j.at("base_experience").get<int>();
        pokemon.height = j.at("height").get<int>();
        pokemon.weight = j.at("weight").get<int>();
        
        for (const auto& t : j.at("types")) {
            pokemon.types.push_back(t.at("type").at("name").get<std::string>());
        }

        for (const auto& s : j.at("stats")) {
            Stat stat;
            stat.name = s.at("stat").at("name").get<std::string>();
            stat.base_stat = s.at("base_stat").get<int>();
            pokemon.stats.push_back(stat);
        }

        for (const auto& a : j.at("abilities")) {
            Ability ability;
            ability.name = a.at("name").get<std::string>();
            ability.isHidden = a.at("isHidden").get<bool>();
            pokemon.abilities.push_back(Ability::fromJson(a));
        }

        for (const auto& m: j.at("moves")) {
            Move move;
            move.id = m.at("id").get<int>();
            move.name = m.at("name").get<std::string>();
           move.effectChance = m.at("effect_chance").is_null() ? std::nullopt 
                                                               : std::optional<int>(m.at("effect_chance").get<int>());
            move.ppCost = m.at("pp").get<int>();
            move.priority = m.at("priority").get<int>();
            move.power = m.at("power").get<int>();
            pokemon.moves.push_back(Move::fromJson(m));
        }
        return pokemon;
    }

    void print(std::ostream& os) const {
        os << "ID: " << id << " | " << name 
           << " | Height: " << height 
           << " | Weight: " << weight << "\n";
        os << "Types: ";
        for (const auto& t : types) os << t << " ";
        os << "\n";
        os << "Stats:\n";
        for (const auto& s : stats) {
            os << "  " << s.name << ": " << s.base_stat << "\n";
        }

        os << "Abilities:\n";
        for (const auto& a : abilities) {
            os << "  " << a.name << (a.isHidden ? " (Hidden)" : "") << "\n";
        }

        os << "Moves:\n";
        for (const auto& m : moves) {
            os << "  " << m.name << " (Power: " << m.power 
               << ", PP: " << m.ppCost << ", Priority: " << m.priority 
               << ", Effect Chance: " << (m.effectChance.has_value() ? std::to_string(m.effectChance.value()) : "N/A") << ")\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Pokemon& p) {
        p.print(os);
        return os;
    }

    int getId() const { return id; }

    static std::vector<std::string> parseMoveNames(const nlohmann::json& j) {
        std::vector<std::string> names;
        int count = 0;
        for (const auto& m: j.at("moves")) {
            if (count++ >= 5) break;
            names.push_back(m.at("move").at("name").get<std::string>());

        }
        return names;
    }
};

#endif // _H_POKEMON_H_