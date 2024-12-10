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
/*void Grafo::agregarArista(int indice1, int indice2, float peso) {
    if (indice1 >= 0 && indice1 < nodos.size() && indice2 >= 0 && indice2 < nodos.size()) {
        aristas.push_back({indice1, indice2, peso});
    } else {
        std::cerr << "Índices de nodos inválidos para la arista";
    }
}*/
void Grafo::agregarArista(int x1, int y1, int x2, int y2) {
    Nodo nodo1 = {x1, y1, {255, 255, 255, 255}};  // Color blanco por defecto
    Nodo nodo2 = {x2, y2, {255, 255, 255, 255}};
    aristas.push_back({nodo1, nodo2});
}

// Agrega una arista entre dos nodos (por índice)
void Grafo::agregarArista(int indice1, int indice2) {
    aristas.push_back({nodos.at(indice1), nodos.at(indice2)});
}
const std::vector<std::pair<Nodo, Nodo>>& Grafo::getAristas() const {
    return aristas;
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
/*const std::vector<Arista>& Grafo::getAristas() const {
    return aristas;
}*/

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
          SDL_RenderDrawLine(renderer, arista.first.x, arista.first.y, arista.second.x, arista.second.y);
        /*const Nodo& nodo1 = nodos[arista.nodo1];
        const Nodo& nodo2 = nodos[arista.nodo2];
        SDL_RenderDrawLine(renderer, nodo1.x, nodo1.y, nodo2.x, nodo2.y);*/
    }
}

std::vector<int> Grafo::dijkstra(int inicio, int fin) const {
    std::vector<float> dist(nodos.size(), std::numeric_limits<float>::infinity());
    std::vector<int> prev(nodos.size(), -1);
    std::vector<bool> visitado(nodos.size(), false);

    dist[inicio] = 0;

    for (size_t i = 0; i < nodos.size(); ++i) {
        int nodoActual = -1;

        // Buscar el nodo no visitado con la menor distancia
        for (size_t j = 0; j < nodos.size(); ++j) {
            if (!visitado[j] && (nodoActual == -1 || dist[j] < dist[nodoActual])) {
                nodoActual = j;
            }
        }

        if (dist[nodoActual] == std::numeric_limits<float>::infinity()) break;

        visitado[nodoActual] = true;

        // Actualizar las distancias de los nodos adyacentes
        for (const auto& arista : aristas) {
            int vecino = -1;

            if (arista.first.x == nodos[nodoActual].x && arista.first.y == nodos[nodoActual].y) {
                vecino = std::distance(nodos.begin(), std::find(nodos.begin(), nodos.end(), arista.second));
            } else if (arista.second.x == nodos[nodoActual].x && arista.second.y == nodos[nodoActual].y) {
                vecino = std::distance(nodos.begin(), std::find(nodos.begin(), nodos.end(), arista.first));
            }

            if (vecino != -1) {
                float peso = std::hypot(arista.first.x - arista.second.x, arista.first.y - arista.second.y);
                if (dist[nodoActual] + peso < dist[vecino]) {
                    dist[vecino] = dist[nodoActual] + peso;
                    prev[vecino] = nodoActual;
                }
            }
        }
    }

    // Reconstruir el camino más corto
    std::vector<int> ruta;
    for (int at = fin; at != -1; at = prev[at]) {
        ruta.push_back(at);
    }

    std::reverse(ruta.begin(), ruta.end());
    if (ruta.size() > 1 && ruta.front() == inicio) {
        return ruta;
    } else {
        return {}; // No hay ruta
    }
}


void configurarRuta(Carro& carro, Grafo& grafo, int nodoInicioID, int nodoDestinoID) {
}