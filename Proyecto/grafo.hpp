#include <vector>
#include <unordered_map>
#include <algorithm> // Para std::reverse
#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

/*struct Nodo {
    int x, y;  // Posición en el mapa
    SDL_Color color;
};

// Clase que representa el grafo
class Grafo {
public:
    std::vector<Nodo> nodos;  // Lista de nodos

    // Función para agregar un nodo al grafo
    void agregarNodo(int x, int y, SDL_Color color) {
        nodos.push_back({x, y, color});
    }

    // Función para dibujar el grafo en la ventana SDL
    void dibujarGrafo(SDL_Renderer* renderer) {
        for (const auto& nodo : nodos) {
            // Dibujar cada nodo (calle)
            SDL_SetRenderDrawColor(renderer, nodo.color.r, nodo.color.g, nodo.color.b, 255);
            SDL_RenderDrawPoint(renderer, nodo.x, nodo.y);  // Dibujar un punto como nodo
        }

        // Dibujar las aristas entre los nodos (aquí puedes conectar los nodos según tus rutas)
        for (size_t i = 0; i < nodos.size() - 1; ++i) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Color amarillo para las aristas
            SDL_RenderDrawLine(renderer, nodos[i].x, nodos[i].y, nodos[i + 1].x, nodos[i + 1].y);
        }
    }
};*/
#ifndef GRAFO_HPP
#define GRAFO_HPP


struct Nodo {
    int x, y;           
    SDL_Color color;   
};

class Grafo {
public:
    Grafo();                                    
    void agregarNodo(int x, int y, SDL_Color color);  
    void agregarArista(int x1, int y1, int x2, int y2);
    void dibujar();       
         Nodo getNodo(int index) {
        return nodos.at(index);
    }

    int getCantidadNodos() {
        return nodos.size();
    }
    
private:
    std::vector<Nodo> nodos;      
    std::vector<std::pair<Nodo, Nodo>> aristas;  
};

#endif // GRAFO_HPP
