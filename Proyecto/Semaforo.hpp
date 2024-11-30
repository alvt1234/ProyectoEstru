#ifndef SEMAFORO_HPP
#define SEMAFORO_HPP

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

// Declaración anticipada de Carro
class Carro;

class Semaforo {
public:
    enum Estado { ROJO, AMARILLO, VERDE };
    Estado estadoActual;
    SDL_Rect semaforoRect;
    Uint32 tiempoInicio;
    Uint32 duracionRojo = 7000;
    Uint32 duracionAmarillo = 2000;
    Uint32 duracionVerde = 4000;
    int radioCarros = 75;

    Semaforo(int x, int y) {
        semaforoRect = {x, y, 20, 30};
        estadoActual = ROJO;
        tiempoInicio = SDL_GetTicks();
    }

    bool hayCarrosCercanos(const std::vector<Carro>& carros) const;
    void actualizarEstado(const std::vector<Carro>& carros);

    void dibujar(SDL_Renderer* renderer) const {
        switch (estadoActual) {
        case ROJO:
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            break;
        case AMARILLO:
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            break;
        case VERDE:
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            break;
        }
        SDL_RenderFillRect(renderer, &semaforoRect);
    }
};

#endif
