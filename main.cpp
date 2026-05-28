#include "http/http_client.h"
#include "pokemon-service/pokemon-service.h"
#include "repository/repository.h"
#include "pool/thread-pool.h"
#include <iostream>

int main() {
    HttpClient client;
    ThreadPool threadPool(4);
    PokemonService service(client, threadPool);
    Repository<Pokemon> repo;

    auto futures = service.fetchMultiple({"pikachu", "ditto", "eevee", "charizard", "mewtwo", "bulbasaur", "squirtle", "jigglypuff", "gengar", "snorlax"});

    for (auto& f : futures) {
        Pokemon p = f.get();
        repo.add(std::make_shared<Pokemon>(p));
        std::cout << p << "\n";
    }

    return 0;
}