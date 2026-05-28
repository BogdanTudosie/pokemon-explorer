//
// Created by Bogdan Tudosie on 28.5.2026.
//

#ifndef POKEMONEXPLORER_JSON_PARSEABLE_H
#define POKEMONEXPLORER_JSON_PARSEABLE_H

#include <concepts>
#include <nlohmann/json.hpp>

template<typename Derived>
class JsonParseable {
public:
    virtual ~JsonParseable() = default;
};

template<typename T>
concept Parseable = std::derived_from<T, JsonParseable<T>> &&
                    requires(const nlohmann::json& j) {
{ T::fromJson(j) } -> std::same_as<T>;
};

#endif //POKEMONEXPLORER_JSON_PARSEABLE_H