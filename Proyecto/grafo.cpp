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

        if (dist[u] == std::numeric_limits<float>::infinity())
            break;

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

    if (ruta.size() > 1 && ruta.front() == inicio)
        return ruta;
    return {}; // No hay ruta
                } //aqui termina dijikstra


void configurarRuta(Carro& carro, Grafo& grafo, int nodoInicioID, int nodoDestinoID) 
{
    // Calcular la ruta más corta con Dijkstra
    std::vector<int> ruta = grafo.dijkstra(nodoInicioID, nodoDestinoID);

    if (ruta.empty()) {
        std::cerr << "No se encontró una ruta válida.\n";
        return;
    }

    // Pasar la ruta calculada al carro
    carro.establecerRuta(ruta);
}
