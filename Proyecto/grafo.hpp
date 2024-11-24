#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <SDL2/SDL.h>

struct Nodo {
    int x, y;          // Coordenadas del nodo
    SDL_Color color;   // Color del nodo (opcional, para visualización)
};

class Grafo {
private:
    std::vector<Nodo> nodos;                           // Lista de nodos (intersecciones)
    std::vector<std::pair<Nodo, Nodo>> aristas;     
       // Lista de aristas (calles)

public:
    Grafo();                                           // Constructor

    void agregarNodo(int x, int y, SDL_Color color);   // Agregar un nodo
    void agregarArista(int x1, int y1, int x2, int y2); // Agregar una arista entre nodos (por coordenadas)
    void agregarArista(int indice1, int indice2);      // Agregar una arista entre nodos (por índice)

    const Nodo& getNodo(size_t indice) const;          // Obtener un nodo por índice
    size_t getCantidadNodos() const;                  // Obtener la cantidad de nodos
    const std::vector<Nodo>& getNodos() const;         // Obtener todos los nodos
    const std::vector<std::pair<Nodo, Nodo>>& getAristas() const;

    void dibujar(SDL_Renderer* renderer) const;        // Dibujar nodos y aristas
};

#endif
