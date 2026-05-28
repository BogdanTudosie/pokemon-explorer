classDiagram
    class HttpClient {
        +fetch(url: string) string
    }

    class Pokemon {
        +int id
        +string name
        +int baseExperience
        +int height
        +int weight
        +vector~string~ types
        +vector~Stat~ stats
        +fromJson(json: string)$ Pokemon
        -parse(j: json) void
    }

    class Stat {
        +string name
        +int baseStat
    }

    class PokemonService {
        -HttpClient client
        +fetchPokemon(name: string) future~Pokemon~
        +fetchMultiple(names: vector~string~) vector~Pokemon~
    }

    class Repository~T~ {
        -vector~shared_ptr~T~~ items
        -mutex mtx
        +add(item: shared_ptr~T~) void
        +findById(id: int) shared_ptr~T~
        +getAll() vector~shared_ptr~T~~
        +filter(pred: Predicate) vector~shared_ptr~T~~
        +removeById(id: int) void
        +operator[](index: int) shared_ptr~T~
    }

    class Identifiable {
        <<concept>>
        +getId() int
    }

    Pokemon "1" --* "many" Stat : contains
    PokemonService --> HttpClient : uses
    PokemonService --> Pokemon : creates
    Repository~T~ --> Identifiable : requires
    PokemonService --> Repository~Pokemon~ : stores into