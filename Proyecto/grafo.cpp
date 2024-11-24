#include "grafo.hpp"
#include <iostream>

Grafo::Grafo() {
}

// Agrega un nodo al grafo
void Grafo::agregarNodo(int x, int y, SDL_Color color) {
    Nodo nuevoNodo = {x, y, color};
    nodos.push_back(nuevoNodo);
}

// Agrega una arista entre dos nodos (por coordenadas)
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

// Dibujar nodos y aristas en la ventana
void Grafo::dibujar(SDL_Renderer* renderer) const {
    // Dibujar nodos como círculos
    for (const auto& nodo : nodos) {
        SDL_SetRenderDrawColor(renderer, nodo.color.r, nodo.color.g, nodo.color.b, nodo.color.a);
        SDL_Rect rect = {nodo.x - 5, nodo.y - 5, 10, 10};  // Nodo de tamaño 10x10
        SDL_RenderFillRect(renderer, &rect);
    }

    // Dibujar aristas como líneas
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Blanco para las aristas
    for (const auto& arista : aristas) {
        SDL_RenderDrawLine(renderer, arista.first.x, arista.first.y, arista.second.x, arista.second.y);
    }
}

