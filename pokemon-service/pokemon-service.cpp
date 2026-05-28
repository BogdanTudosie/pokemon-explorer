#include <nlohmann/json.hpp>
#include "pokemon-service.h"

Pokemon PokemonService::fetchPokemon(const std::string& name) {
    std::string url = "https://pokeapi.co/api/v2/pokemon/" + name;
    nlohmann::json raw = nlohmann::json::parse(httpClient.fetch(url));

    Pokemon pokemon;
    pokemon.id             = raw.at("id").get<int>();
    pokemon.name           = raw.at("name").get<std::string>();
    pokemon.baseExperience = raw.at("base_experience").get<int>();
    pokemon.height         = raw.at("height").get<int>();
    pokemon.weight         = raw.at("weight").get<int>();

    for (const auto& t : raw.at("types")) {
        pokemon.types.push_back(t.at("type").at("name").get<std::string>());
    }

    for (const auto& s : raw.at("stats")) {
        Stat stat;
        stat.name      = s.at("stat").at("name").get<std::string>();
        stat.base_stat = s.at("base_stat").get<int>();
        pokemon.stats.push_back(stat);
    }

    for (const auto& a : raw.at("abilities")) {
        pokemon.abilities.push_back(Ability::fromJson(a));
    }

    std::vector<std::string> moveNames = Pokemon::parseMoveNames(raw);
    for (const auto& moveName : moveNames) {
        pokemon.moves.push_back(fetchMove(moveName));
    }

    return pokemon;
}

std::vector<std::future<Pokemon>> PokemonService::fetchMultiple(const std::vector<std::string>& names) {
    std::vector<std::future<Pokemon>> futures;
    for (const auto& name : names) {
        futures.push_back(threadPool.submit([this, name]() -> Pokemon {
            return fetchPokemon(name);
        }));
    }
    return futures;
}

Move PokemonService::fetchMove(const std::string& moveName) {
    std::string url = "https://pokeapi.co/api/v2/move/" + moveName;
    return Move::fromJson(nlohmann::json::parse(httpClient.fetch(url)));
}
