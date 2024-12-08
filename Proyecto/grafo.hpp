#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <SDL2/SDL.h>
#include <limits>
#include <algorithm>

class Carro;  // Declaración adelantada


struct Nodo {
    int x, y;          // Coordenadas del nodo
    SDL_Color color;   // Color del nodo (opcional, para visualización)
};
struct Arista {
    int nodo1;  
    int nodo2;  
    float peso; 
};
class Grafo {
private:
    std::vector<Nodo> nodos;                           // Lista de nodos (intersecciones)
    std::vector<Arista> aristas;     
    

public:
    Grafo();                                           // Constructor

    void agregarNodo(int x, int y, SDL_Color color);   // Agregar un nodo
    void agregarArista(int indice1, int indice2, float peso = 1.0f); // Agregar una arista entre nodos (por coordenadas)
    

    const Nodo& getNodo(size_t indice) const;          // Obtener un nodo por índice
    size_t getCantidadNodos() const;                  // Obtener la cantidad de nodos
    const std::vector<Nodo>& getNodos() const;         // Obtener todos los nodos
    const std::vector<Arista>& getAristas() const;     // Obtener todas las aristas
    std::vector<int> dijkstra(int inicio, int fin) const;
    void configurarRuta(Carro& carro, int nodoInicioID, int nodoDestinoID);

    void dibujar(SDL_Renderer* renderer) const;        // Dibujar nodos y aristas
};

#endif