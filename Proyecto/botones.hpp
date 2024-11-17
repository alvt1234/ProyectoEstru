#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> //para las imagenes png
class botones {
public:
    SDL_Texture * start;
    SDL_Rect startRect;
    bool startClickeado = false;
    int estadoCarro = 0;  // 0 para primer carro, 1 para segundo carro
    SDL_Texture* carro = NULL;  // Textura del carro
    SDL_Rect carroRect;  // Rectángulo para el carro

    botones(SDL_Renderer* renderer);
    void dibujarBotones(SDL_Renderer* renderer);
    void actualizarBotones(int mouseX, int mouseY, bool clicIzquierdo, SDL_Renderer* renderer);
    void cambiarCarro(SDL_Renderer* renderer);  // Función para cambiar el carro
};

botones::botones(SDL_Renderer* renderer) {
    startRect = {1700, 300, 200, 300}; // Posición y tamaño del botón

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
      //  tempSurface = SDL_LoadBMP("/home/allison/Documents/GitHub/ProyectoEstru/Proyecto/img/caromp.bmp");
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
    if (carro) {
        SDL_RenderCopy(renderer, carro, NULL, &carroRect);
    }
}


void botones::actualizarBotones(int mouseX, int mouseY, bool clicIzquierdo, SDL_Renderer* renderer) {
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
}