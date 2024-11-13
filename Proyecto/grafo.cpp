#include "grafo.hpp"
#include <iostream>


Grafo::Grafo() {
}

// Agrega un nodo al grafo
void Grafo::agregarNodo(int x, int y, SDL_Color color) {
    Nodo nuevoNodo = {x, y, color};
    nodos.push_back(nuevoNodo);  
}

// Agrega una arista entre dos nodos
void Grafo::agregarArista(int x1, int y1, int x2, int y2) {
    Nodo nodo1 = {x1, y1, {255, 255, 255, 255}};  
    Nodo nodo2 = {x2, y2, {255, 255, 255, 255}};  
    
    // Agrega la arista como un par de nodos
    aristas.push_back({nodo1, nodo2});
}


void Grafo::dibujar() {
    std::cout << "Dibujando nodos:" << std::endl;
    for (const auto& nodo : nodos) {
        std::cout << "Nodo en (" << nodo.x << ", " << nodo.y << ") con color (" 
                  << (int)nodo.color.r << ", " << (int)nodo.color.g << ", " 
                  << (int)nodo.color.b << ")" << std::endl;
    }
    
    // Dibuja todas las aristas
    std::cout << "Dibujando aristas:" << std::endl;
    for (const auto& arista : aristas) {
        std::cout << "Arista de (" << arista.first.x << ", " << arista.first.y << ") "
                  << "a (" << arista.second.x << ", " << arista.second.y << ")" << std::endl;
    }
}
