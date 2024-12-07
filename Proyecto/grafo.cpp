#include "grafo.hpp"
#include <iostream>
#include "Carro.hpp"
#include <algorithm> //para el reverse
#include <limits>


Grafo::Grafo() {
}

// Agrega un nodo al grafo
void Grafo::agregarNodo(int x, int y, SDL_Color color) {
    Nodo nuevoNodo = {x, y, color};
    nodos.push_back(nuevoNodo);
}

//Agrega una arista entre dos nodos (por índice) con un peso
void Grafo::agregarArista(int indice1, int indice2, float peso) {
    if (indice1 >= 0 && indice1 < nodos.size() && indice2 >= 0 && indice2 < nodos.size()) {
        aristas.push_back({indice1, indice2, peso});
    } else {
        std::cerr << "Índices de nodos inválidos para la arista";
    }
}

// Obtener un nodo por índice
const Nodo& Grafo::getNodo(size_t indice) const {
    return nodos.at(indice);
}

// Obtener la cantidad de nodos
size_t Grafo::getCantidadNodos() const {
    return nodos.size();
}

// Obtener todos los nodos
const std::vector<Nodo>& Grafo::getNodos() const {
    return nodos;
}

// Obtener todas las aristas
const std::vector<Arista>& Grafo::getAristas() const {
    return aristas;
}

// Dibujar nodos y aristas en la ventana
void Grafo::dibujar(SDL_Renderer* renderer) const {
    // Dibujar nodos como círculos
    for (const auto& nodo : nodos) {
        SDL_SetRenderDrawColor(renderer, nodo.color.r, nodo.color.g, nodo.color.b, nodo.color.a);
        SDL_Rect rect = {nodo.x - 5, nodo.y - 5, 15, 15};  // Nodo de tamaño 15x15
        SDL_RenderFillRect(renderer, &rect);
    }

    // Dibujar aristas como líneas
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Blanco para las aristas
    for (const auto& arista : aristas) {
        const Nodo& nodo1 = nodos[arista.nodo1];
        const Nodo& nodo2 = nodos[arista.nodo2];
        SDL_RenderDrawLine(renderer, nodo1.x, nodo1.y, nodo2.x, nodo2.y);
    }
}

std::vector<int> Grafo::dijkstra(int inicio, int fin) const {
    // Validar índices inicial y final
    if (inicio < 0 || inicio >= nodos.size() || fin < 0 || fin >= nodos.size()) {
        std::cerr << "Error: Nodos de inicio o fin inválidos.\n";
        return {};
    }

    size_t n = nodos.size();
    std::vector<float> dist(n, std::numeric_limits<float>::infinity());
    std::vector<int> prev(n, -1);
    std::vector<bool> visitado(n, false);
    
    dist[inicio] = 0;

    for (size_t i = 0; i < n; ++i) {
        // Encuentra el nodo no visitado con la menor distancia
        int u = -1;
        for (size_t j = 0; j < n; ++j) {
            if (!visitado[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        // Si no se encontró un nodo válido, detener el bucle
        if (u == -1 || dist[u] == std::numeric_limits<float>::infinity()) {
            std::cerr << "Error: Nodo desconectado o sin ruta.\n";
            break;
        }

        visitado[u] = true;

        // Actualiza las distancias de los vecinos
        for (const auto& arista : aristas) {
            if (arista.nodo1 == u || arista.nodo2 == u) {
                int vecino = (arista.nodo1 == u) ? arista.nodo2 : arista.nodo1;
                float nuevaDist = dist[u] + arista.peso;

                if (nuevaDist < dist[vecino]) {
                    dist[vecino] = nuevaDist;
                    prev[vecino] = u;
                }
            }
        }
    }

    // Reconstruir el camino
    std::vector<int> ruta;
    for (int at = fin; at != -1; at = prev[at]) {
        ruta.push_back(at);
    }
    std::reverse(ruta.begin(), ruta.end());

    // Validar que la ruta es completa
    if (ruta.size() < 2 || ruta.front() != inicio || ruta.back() != fin) {
        std::cerr << "Error: No se pudo calcular una ruta válida entre " << inicio << " y " << fin << ".\n";
        return {};
    }

    // Validar que todos los nodos en la ruta están conectados
    for (size_t i = 1; i < ruta.size(); ++i) {
        bool conectados = false;
        for (const auto& arista : aristas) {
            if ((arista.nodo1 == ruta[i - 1] && arista.nodo2 == ruta[i]) ||
                (arista.nodo2 == ruta[i - 1] && arista.nodo1 == ruta[i])) {
                conectados = true;
                break;
            }
        }
        if (!conectados) {
            std::cerr << "Error: Ruta contiene nodos no conectados (" << ruta[i - 1] << " -> " << ruta[i] << ").\n";
            return {};
        }
    }

    return ruta; // Ruta válida
} //aqui termina dijikstra

void configurarRuta(Carro& carro, Grafo& grafo, int nodoInicioID, int nodoDestinoID) {
    if (nodoInicioID < 0 || nodoInicioID >= grafo.getCantidadNodos() || 
        nodoDestinoID < 0 || nodoDestinoID >= grafo.getCantidadNodos()) {
        std::cerr << "Índices de nodo inválidos.\n";
        return;
    }

    std::vector<int> ruta = grafo.dijkstra(nodoInicioID, nodoDestinoID);

    // Validar que la ruta conecta todos los nodos correctamente
    for (size_t i = 1; i < ruta.size(); ++i) {
        bool conectados = false;
        for (const auto& arista : grafo.getAristas()) {
            if ((arista.nodo1 == ruta[i - 1] && arista.nodo2 == ruta[i]) ||
                (arista.nodo2 == ruta[i - 1] && arista.nodo1 == ruta[i])) {
                conectados = true;
                break;
            }
        }
        if (!conectados) {
            std::cerr << "Ruta contiene nodos no conectados (" << ruta[i - 1] << " -> " << ruta[i] << ").\n";
            return;
        }
    }

    carro.establecerRuta(ruta);
}