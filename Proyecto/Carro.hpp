#ifndef BCD25E07_30ED_4976_B6F1_C318CD369A0A
#define BCD25E07_30ED_4976_B6F1_C318CD369A0A
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> // Para las imágenes PNG
#include <random>  // Para la generación de números aleatorios


class Carro {
public:
    SDL_Rect rect;
    int velocidad;
    char direccion;  // 'H' para horizontal, 'V' para vertical

    Carro(int x, int y, int ancho, int alto, char dir = 'H', int vel = 5)
        : velocidad(vel), direccion(dir) {
        rect = {x, y, ancho, alto};
    }

    void mover() {
        if (direccion == 'H') {
            rect.x += velocidad;  // Mover horizontalmente
        } else if (direccion == 'V') {
            rect.y += velocidad;  // Mover verticalmente
        }
    }

    void girarHaciaArista(const Grafo& grafo) {
    // Buscar el nodo más cercano
    for (const auto& nodo : grafo.getNodos()) {
        if (std::abs(rect.x - nodo.x) < 5 && std::abs(rect.y - nodo.y) < 5) {
            // Encontrar todas las aristas conectadas a este nodo
            std::vector<std::pair<Nodo, Nodo>> conexiones;
            for (const auto& arista : grafo.getAristas()) {
                if (arista.first.x == nodo.x && arista.first.y == nodo.y) {
                    conexiones.push_back(arista);
                }
            }

            // Seleccionar una arista aleatoria para continuar
            if (!conexiones.empty()) {
                static std::random_device rd;
                static std::mt19937 gen(rd());
                std::uniform_int_distribution<> dist(0, conexiones.size() - 1);
                auto aristaSeleccionada = conexiones[dist(gen)];

                // Dirigir el carro hacia el nodo destino
                Nodo destino = aristaSeleccionada.second;
                if (destino.x > rect.x) {
                    direccion = 'H';
                    velocidad = std::abs(velocidad);
                } else if (destino.x < rect.x) {
                    direccion = 'H';
                    velocidad = -std::abs(velocidad);
                } else if (destino.y > rect.y) {
                    direccion = 'V';
                    velocidad = std::abs(velocidad);
                } else if (destino.y < rect.y) {
                    direccion = 'V';
                    velocidad = -std::abs(velocidad);
                }
            }
            break;
        }
    }
}


    void dibujar(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 20, 147, 255); // Color rosado intenso (fucsia)
        SDL_RenderFillRect(renderer, &rect);
    }
};

#endif /* BCD25E07_30ED_4976_B6F1_C318CD369A0A */
