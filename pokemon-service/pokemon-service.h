#ifndef _H_POKEMON_SERVICE_H_
#define _H_POKEMON_SERVICE_H_

#include <string>
#include <vector>
#include <future>
#include "models/pokemon.h"
#include "http/http_client.h"
#include "pool/thread-pool.h"

class PokemonService {
public:
    explicit PokemonService(HttpClient& httpClient, ThreadPool& threadPool)
        : httpClient(httpClient), threadPool(threadPool) {}

    // Fetches a single Pokemon + cascades into its first 5 moves
    Pokemon fetchPokemon(const std::string& name);

    // Fetches multiple Pokemon concurrently via the thread pool
    std::vector<std::future<Pokemon>> fetchMultiple(const std::vector<std::string>& names);

private:
    HttpClient& httpClient;
    ThreadPool& threadPool;

    Move fetchMove(const std::string& moveName);
};

#endif // _H_POKEMON_SERVICE_H_
