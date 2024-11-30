#ifndef BCD25E07_30ED_4976_B6F1_C318CD369A0A
#define BCD25E07_30ED_4976_B6F1_C318CD369A0A
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> // Para las imágenes PNG
#include <random>  // Para la generación de números aleatorios
#include "grafo.hpp"

class Carro {
public:
    SDL_Rect rect;
    int velocidad;
    char direccion; 
    bool esEmergencia; // 'H' para horizontal, 'V' para vertical
    Grafo grafo;
    SDL_Color color;
    
    Carro(int x, int y, int ancho, int alto, char dir = 'H', int vel = 5, bool emergencia = false, SDL_Color c = {169, 169, 169, 255})
        : velocidad(vel), direccion(dir), esEmergencia(emergencia), color(c) {
        rect = {x, y, ancho, alto};
    }

    void mover(const std::vector<Semaforo>& semaforos, const std::vector<Carro>& carros) {
        if (verificarSemaforo(semaforos)) {
            return; // Detener si el semáforo está en rojo
        }
    
        // Mover el carro según la dirección
        if (direccion == 'H') {
            rect.x += velocidad;
        } else if (direccion == 'V') {
            rect.y += velocidad;
        }

        // Verificar colisiones con otros carros
        for (const auto& otroCarro : carros) {
            if (&otroCarro != this && detectarColision(otroCarro)) {
                // Si hay colisión, revertir el movimiento
                if (direccion == 'H') {
                    rect.x -= velocidad;  // Detener movimiento horizontal
                } else if (direccion == 'V') {
                    rect.y -= velocidad;  // Detener movimiento vertical
                }
                break; // Salir del ciclo porque ya detectamos una colisión
            }
        }

        // Detectar y ajustar al nodo más cercano
        for (const auto& nodo : grafo.getNodos()) {
            if (detectarNodo(nodo)) {
                ajustarPosicionAlNodo(nodo);
                break;
            }
        }
    }

    void girarHaciaArista(const Grafo& grafo) {
        // Buscar el nodo más cercano
        for (const auto& nodo : grafo.getNodos()) {
            if (std::abs(rect.x - nodo.x) < 10 && std::abs(rect.y - nodo.y) < 10) {
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
                    if (std::abs(rect.x - destino.x) > 5) {
                        direccion = 'H';
                        velocidad = (destino.x > rect.x) ? std::abs(velocidad) : -std::abs(velocidad);
                    } else if (std::abs(rect.y - destino.y) > 5) {
                        direccion = 'V';
                        velocidad = (destino.y > rect.y) ? std::abs(velocidad) : -std::abs(velocidad);
                    }

                    std::cout << "Nodo actual: (" << rect.x << ", " << rect.y << ") -> Nodo destino: (" 
                        << destino.x << ", " << destino.y << ")" << std::endl;
                    std::cout << "Dirección: " << direccion << " Velocidad: " << velocidad << std::endl;
                    if (rect.x == destino.x && rect.y == destino.y) {
                        std::cout << "Vehículo alcanzó el nodo destino: (" << destino.x << ", " << destino.y << ")" << std::endl;
                    } else {
                        std::cout << "Vehículo en movimiento hacia: (" << destino.x << ", " << destino.y << ")" << std::endl;
                    }
                }
                break;
            }
        }
    }

    void ajustarPosicionAlNodo(Nodo nodo) {
        if (std::abs(rect.x - nodo.x) <= 5 && std::abs(rect.y - nodo.y) <= 5) {
            rect.x = nodo.x;
            rect.y = nodo.y;
            velocidad = 0; // Detener el movimiento temporalmente
        }
    }

    bool detectarNodo(const Nodo& nodo, int margen = 5) {
        return std::abs(rect.x - nodo.x) <= margen && std::abs(rect.y - nodo.y) <= margen;
    }

    void dibujar(SDL_Renderer* renderer) {
        if(esEmergencia){
            
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Color rojo puro
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            //SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255); // Gris metálico
        }
        SDL_RenderFillRect(renderer, &rect);
    }

    bool verificarSemaforo(const std::vector<Semaforo>& semaforos) {
        if (esEmergencia) {
            return false; // Los carros de emergencia no se detienen
        }

        for (const auto& semaforo : semaforos) {
            if (std::abs(rect.x - semaforo.semaforoRect.x) < 75 &&
                std::abs(rect.y - semaforo.semaforoRect.y) < 75) {
                return semaforo.estadoActual == Semaforo::ROJO;
            }
        }
        return false;
    }

    bool detectarColision(const Carro& otroCarro) const {
        // Verificar si los rectángulos del carro actual y el otro carro se cruzan
        return SDL_HasIntersection(&rect, &otroCarro.rect);
    }
};

#endif /* BCD25E07_30ED_4976_B6F1_C318CD369A0A */
