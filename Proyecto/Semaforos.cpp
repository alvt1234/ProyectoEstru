#include "Semaforo.hpp"
#include "Carro.hpp"

bool Semaforo::hayCarrosCercanos(const std::vector<Carro>& carros) const {
    for (const auto& carro : carros) {
        if (std::abs(carro.rect.x - semaforoRect.x) < radioCarros &&
            std::abs(carro.rect.y - semaforoRect.y) < radioCarros) {
            return true;
        }
    }
    return false;
}

void Semaforo::actualizarEstado(const std::vector<Carro>& carros) {
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
            if (!hayCarrosCercanos(carros)) {
                estadoActual = ROJO;
                tiempoInicio = tiempoActual;
            }
        }
        break;
    }
}
