#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

class Semaforo {
public:
    enum Estado { ROJO, AMARILLO, VERDE };
    Estado estadoActual;
    SDL_Rect semaforoRect;
    Uint32 tiempoInicio;
    Uint32 duracionRojo = 5000;  // Duración en milisegundos
    Uint32 duracionAmarillo = 2000;
    Uint32 duracionVerde = 4000;

    Semaforo(int x, int y) {
        semaforoRect = {x, y, 20, 60};
        estadoActual = ROJO;
        tiempoInicio = SDL_GetTicks();
    }

    void actualizarEstado() {
    Uint32 tiempoActual = SDL_GetTicks();
    float tiempoTranscurrido = tiempoActual - tiempoInicio;
    std::cout << "Estado: " << estadoActual << ", Tiempo transcurrido: " << tiempoTranscurrido << " ms" << std::endl;

    switch (estadoActual) {
    case ROJO:
        if (tiempoTranscurrido >= duracionRojo) {
            estadoActual = VERDE;
            tiempoInicio = tiempoActual;  // Actualiza el tiempo de inicio
            std::cout << "Cambio a VERDE" << std::endl;
        }
        break;
    case VERDE:
        if (tiempoTranscurrido >= duracionVerde) {
            estadoActual = AMARILLO;
            tiempoInicio = tiempoActual;  // Actualiza el tiempo de inicio
            std::cout << "Cambio a AMARILLO" << std::endl;
        }
        break;
    case AMARILLO:
        if (tiempoTranscurrido >= duracionAmarillo) {
            estadoActual = ROJO;
            tiempoInicio = tiempoActual;  // Actualiza el tiempo de inicio
            std::cout << "Cambio a ROJO" << std::endl;
        }
        break;
    }
}
void dibujar(SDL_Renderer* renderer) const {
    switch (estadoActual) {
    case ROJO:
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Rojo
        break;
    case AMARILLO:
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Amarillo
        break;
    case VERDE:
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Verde
        break;
    }
    SDL_RenderFillRect(renderer, &semaforoRect);
}
};