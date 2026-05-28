# pokemon-explorer

A small C++20 program that fetches Pokémon data concurrently from [PokéAPI](https://pokeapi.co), parses it, and stores it in a thread-safe in-memory repository.

The project is intentionally compact but exercises a handful of modern C++ features end-to-end: concepts, templates, futures, a custom thread pool, RAII over libcurl, and JSON deserialization with `nlohmann/json`.

## What it does

`main.cpp` requests ten Pokémon (`pikachu`, `ditto`, `eevee`, `charizard`, `mewtwo`, `bulbasaur`, `squirtle`, `jigglypuff`, `gengar`, `snorlax`) in parallel. For each Pokémon, it also fetches the first five of its moves. Results are pushed into a generic `Repository<Pokemon>` and printed to stdout.

## Architecture

```
┌──────────────────┐
│      main        │
└────────┬─────────┘
         │
         ▼
┌──────────────────┐     submit(task)    ┌──────────────────┐
│  PokemonService  │ ──────────────────▶ │    ThreadPool    │
└────────┬─────────┘                     └──────────────────┘
         │ fetch(url)
         ▼
┌──────────────────┐    HTTPS (libcurl)
│    HttpClient    │ ───────────────────▶  pokeapi.co
└──────────────────┘
         │ JSON
         ▼
┌──────────────────┐
│ Pokemon / Move / │   (nlohmann::json → models)
│     Ability      │
└────────┬─────────┘
         │
         ▼
┌──────────────────┐
│ Repository<T>    │   thread-safe, `Identifiable` concept
└──────────────────┘
```

### Components

| Component | Files | Responsibility |
|---|---|---|
| `HttpClient` | `http/` | Thin libcurl wrapper exposing `fetch(url) -> std::string`. |
| `ThreadPool` | `pool/` | Fixed-size worker pool with a templated `submit()` returning `std::future`. |
| `PokemonService` | `pokemon-service/` | Orchestrates HTTP + parsing; `fetchMultiple` fans out via the pool. |
| `Pokemon` / `Move` / `Ability` / `Stat` | `models/` | Plain data with `fromJson` / `toJson`. |
| `JsonParseable<T>` | `models/json-parseable.h` | CRTP base + `Parseable` concept for compile-time enforcement of `fromJson`. |
| `Repository<T>` | `repository/` | Generic, mutex-guarded store constrained by the `Identifiable` concept (`T::getId() -> int`). |

A more detailed class diagram lives in [`diagram.md`](diagram.md).

## Requirements

- C++20 compiler (Clang or GCC)
- CMake ≥ 3.15
- [libcurl](https://curl.se/libcurl/)
- [nlohmann/json](https://github.com/nlohmann/json)

### macOS

```sh
brew install cmake curl nlohmann-json
```

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Run

```sh
./build/pokemon_explorer
```

The binary makes live HTTPS calls to `pokeapi.co` and prints one block per Pokémon, e.g.:

```
ID: 25 | pikachu | Height: 4 | Weight: 60
Types: electric
Stats:
  hp: 35
  attack: 55
  ...
Abilities:
  static
  lightning-rod (Hidden)
Moves:
  mega-punch (Power: 80, PP: 20, Priority: 0, Effect Chance: N/A)
  ...
```

## Layout

```
.
├── CMakeLists.txt
├── main.cpp
├── http/              # HttpClient (libcurl)
├── pool/              # ThreadPool
├── pokemon-service/   # PokemonService
├── models/            # Pokemon, Move, Ability, JsonParseable
├── repository/        # Generic Repository<T>
└── diagram.md         # Mermaid class diagram
```
