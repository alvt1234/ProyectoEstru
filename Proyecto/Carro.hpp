#ifndef BCD25E07_30ED_4976_B6F1_C318CD369A0A
#define BCD25E07_30ED_4976_B6F1_C318CD369A0A
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> // Para las imágenes PNG
#include <random>  // Para la generación de números aleatorios
/*
class carro {
public:
    SDL_Texture * texturaCarro;
    SDL_Rect rectCarro;
    int estadoCarro;  // 0 para primer carro, 1 para segundo carro

    carro(SDL_Renderer* renderer);
    void cambiarCarro(SDL_Renderer* renderer);
    void dibujarCarro(SDL_Renderer* renderer);
};

carro::carro(SDL_Renderer* renderer) {
    // Inicializa el estado y la posición del carro
    estadoCarro = 0;  // Empieza con el primer carro
    rectCarro = {200, 200, 100, 100};  // Posición inicial del carro

    // Carga la primera imagen del carro
    SDL_Surface* tempSurface = SDL_LoadBMP("/home/anareyes/Documentos/Proyecto/img/caromp.bmp");
    if (!tempSurface) {
        fprintf(stderr, "Error al cargar la imagen del carro: %s\n", SDL_GetError());
        exit(1);
    }

    texturaCarro = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (!texturaCarro) {
        fprintf(stderr, "Error al crear la textura del carro: %s\n", SDL_GetError());
        exit(1);
    }
}

void carro::cambiarCarro(SDL_Renderer* renderer) {
    // Cambia el estado del carro (alternar entre 0 y 1)
    estadoCarro = (estadoCarro + 1) % 2;

    // Liberar la textura anterior del carro
    if (texturaCarro) {
        SDL_DestroyTexture(texturaCarro);
        texturaCarro = NULL;
    }

    // Cargar la nueva imagen según el estado
    SDL_Surface* tempSurface = NULL;
    if (estadoCarro == 0) {
        tempSurface = SDL_LoadBMP("/home/anareyes/Documentos/Proyecto/img/caromp.bmp");
    } else {
        tempSurface = SDL_LoadBMP("/home/anareyes/Documentos/Proyecto/img/carro2.bmp");
    }

    if (!tempSurface) {
        fprintf(stderr, "Error al cargar la imagen del carro: %s\n", SDL_GetError());
        exit(1);
    }

    // Crear la textura con la nueva imagen
    texturaCarro = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (!texturaCarro) {
        fprintf(stderr, "Error al crear la textura del carro: %s\n", SDL_GetError());
        exit(1);
    }

    // Establecer la posición del carro
    rectCarro = {400, 200, 100, 100};
}

void carro::dibujarCarro(SDL_Renderer* renderer) {
    // Dibuja el carro en la pantalla
    SDL_RenderCopy(renderer, texturaCarro, NULL, &rectCarro);
}

*/
/*class Carro {
public:
    SDL_Rect rect; // Representa la posición y tamaño del auto

    Carro(int x, int y, int w, int h) {
        rect.x = x;  // Posición inicial en x
        rect.y = y;  // Posición inicial en y
        rect.w = w;  // Ancho del auto
        rect.h = h;  // Alto del auto
    }

    void mover(int dx, int dy) {
        rect.x += dx;
        rect.y += dy;
    }

    const SDL_Rect& getRect() const {
    return rect;
}

};*/


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
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Rojo para el carro
        SDL_RenderFillRect(renderer, &rect);
    }
};

#endif /* BCD25E07_30ED_4976_B6F1_C318CD369A0A */
