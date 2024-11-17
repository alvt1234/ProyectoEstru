#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

class Semaforo {
public:
    enum Estado { ROJO, AMARILLO, VERDE };
    Estado estadoActual;
    SDL_Rect semaforoRect;
    Uint32 tiempoInicio;
    float duracionRojo = 5000;  // Duración en milisegundos
    float duracionAmarillo = 2000;
    float duracionVerde = 4000;

    Semaforo(int x, int y) {
        semaforoRect = {x, y, 20, 60};
        estadoActual = ROJO;
        tiempoInicio = SDL_GetTicks();
    }

    void actualizarEstado() {
        Uint32 tiempoActual = SDL_GetTicks();
        float tiempoTranscurrido = tiempoActual - tiempoInicio;
        
        switch (estadoActual) {
        case ROJO:
            if (tiempoTranscurrido >= duracionRojo) {
                estadoActual = VERDE;
                tiempoInicio = tiempoActual;
            }
            break;
        case VERDE:
            if (tiempoTranscurrido >= duracionVerde) {
                estadoActual = AMARILLO;
                tiempoInicio = tiempoActual;
            }
            break;
        case AMARILLO:
            if (tiempoTranscurrido >= duracionAmarillo) {
                estadoActual = ROJO;
                tiempoInicio = tiempoActual;
            }
            break;
        }
    }

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


