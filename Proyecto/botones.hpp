#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> //para las imagenes png
#include <iostream>
#include "Carro.hpp"
#include "Semaforo.hpp"
class botones {
public:
    SDL_Texture * start;
    SDL_Rect startRect;
    SDL_Rect lluviaRect;
    SDL_Texture * lluvia;
    SDL_Rect nieblaRect;
    SDL_Texture * niebla;
    bool lluviaClickeado = false;
    bool nieblaClickeado = false;
    bool startClickeado = false;
    int estadoCarro = 0;  // 0 para primer carro, 1 para segundo carro
    SDL_Texture* carro = NULL;  // Textura del carro
    SDL_Rect carroRect;  // Rectángulo para el carro
    Uint8 fondoR = 100, fondoG = 149, fondoB = 237; 
    bool lloviendo = false;
    bool neblina = false;


    botones(SDL_Renderer* renderer);
    void dibujarBotones(SDL_Renderer* renderer);
    void actualizarBotones(int mouseX, int mouseY, bool clicIzquierdo, SDL_Renderer* renderer, std::vector<Carro>& carros);
    void cambiarCarro(SDL_Renderer* renderer);  // Función para cambiar el carro
};

botones::botones(SDL_Renderer* renderer) {
    startRect = {1700, 300, 200, 300}; // Posición y tamaño del botón
    lluviaRect = {1730, 50, 70, 100}; // Posición y tamaño del botón
    nieblaRect = {1550, 120, 70, 100}; // Posición y tamaño del botón

    // Cargar imagen BMP del botón
   //SDL_Surface* tempSurface = SDL_LoadBMP("/home/allison/Documents/GitHub/ProyectoEstru/Proyecto/img/start.bmp");
    SDL_Surface* tempSurface = SDL_LoadBMP("/home/anareyes/Documentos/Proyecto/img/start.bmp");
    if (!tempSurface) {
        fprintf(stderr, "Error al cargar la imagen del botón: %s\n", SDL_GetError());
        exit(1);
    }

    // Crear textura del botón
    start = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (!start) {
        fprintf(stderr, "Error al crear la textura del botón: %s\n", SDL_GetError());
        exit(1);
    }

    //BOTON LLUVIA
    SDL_Surface* tempSurfaceLluvia = SDL_LoadBMP("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/lluvia.bmp");
    if (!tempSurfaceLluvia) {
        fprintf(stderr, "Error al cargar la imagen del botón Lluvia: %s\n", SDL_GetError());
        exit(1);
    }

    //LLUVIA
    lluvia = SDL_CreateTextureFromSurface(renderer, tempSurfaceLluvia);
    SDL_FreeSurface(tempSurfaceLluvia);
    if (!lluvia) {
        fprintf(stderr, "Error al crear la textura del botón Lluvia: %s\n", SDL_GetError());
        exit(1);
    }

    //NIEBLA
    SDL_Surface* tempSurfaceNiebla = SDL_LoadBMP("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/niebla.bmp");
    if (!tempSurfaceNiebla) {
        fprintf(stderr, "Error al cargar la imagen del botón Niebla: %s\n", SDL_GetError());
        exit(1);
    }

    //NIEBLA
    niebla = SDL_CreateTextureFromSurface(renderer, tempSurfaceNiebla);
    SDL_FreeSurface(tempSurfaceNiebla);
    if (!niebla) {
        fprintf(stderr, "Error al crear la textura del botón Niebla: %s\n", SDL_GetError());
        exit(1);
    }
    // Inicializar la textura del carro (primer carro)
    carro = NULL;
    carroRect = {200, 200, 100, 100};  // Posición inicial del carro
}


void botones::cambiarCarro(SDL_Renderer* renderer) {
    // Cambiar el estado del carro (alternar entre 0 y 1)
    estadoCarro = (estadoCarro + 1) % 2;

    // Liberar la textura anterior del carro, si existe
    if (carro) {
        SDL_DestroyTexture(carro);
        carro = NULL;
    }

    SDL_Surface* tempSurface = NULL;
    if (estadoCarro == 0) {
        //tempSurface = SDL_LoadBMP("/home/allison/Documents/GitHub/ProyectoEstru/Proyecto/img/caromp.bmp");
      tempSurface = SDL_LoadBMP("/home/anareyes/Documentos/Proyecto/img/caromp.bmp");
    } else {
        //tempSurface = SDL_LoadBMP("/home/allison/Documents/GitHub/ProyectoEstru/Proyecto/img/carro2.bmp");
        tempSurface = SDL_LoadBMP("/home/anareyes/Documentos/Proyecto/img/carro2.bmp");
    }

    if (!tempSurface) {
        fprintf(stderr, "Error al cargar la imagen del carro: %s\n", SDL_GetError());
        exit(1);
    }

    // Crear la textura del carro con la nueva imagen
    carro = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    if (!carro) {
        fprintf(stderr, "Error al crear la textura del carro: %s\n", SDL_GetError());
        exit(1);
    }

    // Establecer la posición del carro (opcionalmente puedes cambiarla)
    carroRect = {400, 0, 50, 70};
}


void botones::dibujarBotones(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, start, NULL, &startRect);
    SDL_RenderCopy(renderer, lluvia, NULL, &lluviaRect);
    SDL_RenderCopy(renderer, niebla, NULL, &nieblaRect);

    if (carro) {
        SDL_RenderCopy(renderer, carro, NULL, &carroRect);
    }

}


void botones::actualizarBotones(int mouseX, int mouseY, bool clicIzquierdo, SDL_Renderer* renderer, std::vector<Carro>& carros) {
    // Verifica si el mouse está sobre el botón y si se hizo clic
    if (mouseX > startRect.x && mouseX < (startRect.x + startRect.w) &&
        mouseY > startRect.y && mouseY < (startRect.y + startRect.h)) {
        if (clicIzquierdo) {
            startClickeado = true;
            cambiarCarro(renderer);  // Cambia la imagen del carro
        }
    } else {
        startClickeado = false;
    }
   // Botón de lluvia
    if (mouseX > lluviaRect.x && mouseX < (lluviaRect.x + lluviaRect.w) &&
        mouseY > lluviaRect.y && mouseY < (lluviaRect.y + lluviaRect.h)) {
        if (clicIzquierdo) {
            lluviaClickeado = true;
            // Cambiar el color del fondo a gris azulado (ejemplo de lluvia)
            fondoR = 112;
            fondoG = 128;
            fondoB = 144;
            lloviendo = !lloviendo;
            std::cout<< "Botón de lluvia clickeado" << std::endl;
            for (auto& carro : carros) {
                carro.reducirVelocidadPorLluvia(lloviendo);
            }
        
        }
    } else {
        lluviaClickeado = false;
    }

    // Botón de niebla
    if (mouseX > nieblaRect.x && mouseX < (nieblaRect.x + nieblaRect.w) &&
        mouseY > nieblaRect.y && mouseY < (nieblaRect.y + nieblaRect.h)) {
        if (clicIzquierdo) {
            nieblaClickeado = true;
            // Cambiar el color del fondo a un tono más claro (ejemplo de niebla)
            fondoR = 169;
            fondoG = 169;
            fondoB = 169;
            neblina = !neblina;
            for(auto& carro : carros){
                carro.aumetarVelocidadPorNiebla(neblina);
        }
    } else {
        nieblaClickeado = false;
    }
}
}
