#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "botones.hpp"
#include "grafo.hpp"
#include <cmath>
#include <vector>
#include "Semaforo.hpp" 
// Crear variables globales
SDL_Surface * screen;  // espacio en memoria de la pantalla
SDL_Surface * imagen;  // espacio en memoria de la imagen
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* carTexture = NULL;  // Textura para el carro
SDL_Rect carRect = {10, 100, 90, 70};  // Posición inicial y tamaño del carro

Grafo grafo;  // Grafo que representa las intersecciones y las calles

// Funciones auxiliares
//void calles();
//void edificios();
void dibujarMapa(botones& boton);

void cargarImg() {
    imagen = IMG_Load("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/map.bmp");
    if (imagen == NULL) {
        fprintf(stderr, "No se pudo cargar la imagen: %s\n", SDL_GetError());
        exit(1);
    }

    // Crear una textura desde la superficie cargada
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imagen);
    SDL_FreeSurface(imagen);  // Liberar la superficie
    if (texture == NULL) {
        fprintf(stderr, "No se pudo crear la textura: %s\n", SDL_GetError());
        exit(1);
    }

    // Renderizar la textura en la pantalla
    SDL_Rect destino = {0, 0, 1550, 1080};  // Área donde se dibujará la imagen
    SDL_RenderCopy(renderer, texture, NULL, &destino);

    SDL_DestroyTexture(texture);  // Liberar la textura después de usarla
}

//std::vector<Semaforo> semaforos;

/*void inicializarSemaforos() { // hace las calles y despues pongo los semaforos 

    std::vector<std::pair<int, int>> posicionesPrincipales = {
        {100, 100}, {100, 300}, {100, 500},
        {300, 100}, {300, 300}, {300, 500},
        {500, 100}, {500, 300}, {500, 500}
    };

    for (const auto& pos : posicionesPrincipales) {
        semaforos.emplace_back(pos.first, pos.second);
    }
}
*/
/*void actualizarSemaforos() {
    for (auto& semaforo : semaforos) {
        semaforo.actualizarEstado();
    }
}

void dibujarSemaforos(SDL_Renderer* renderer) {
    for (const auto& semaforo : semaforos) {
        semaforo.dibujar(renderer);
    }
}
*/

/*void dibujarCirculo(SDL_Renderer* renderer, int centroX, int centroY, int radio)
 {
   // SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Color de la rotonda (gris oscuro)
    for (int w = 0; w < radio * 2; w++) {
        for (int h = 0; h < radio * 2; h++) {
            int dx = radio - w; // distancia horizontal al centro
            int dy = radio - h; // distancia vertical al centro
            if ((dx * dx + dy * dy) <= (radio * radio)) {
                SDL_RenderDrawPoint(renderer, centroX + dx, centroY + dy);
            }
        }
    }

  
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Color de las líneas amarillas
    for (int i = 0; i < 360; i += 45) {
        int x = centroX + (radio - 5) * cos(i * M_PI / 180);
        int y = centroY + (radio - 5) * sin(i * M_PI / 180);
        SDL_RenderDrawPoint(renderer, x, y);
    }
}
*/




// Función para manejar los eventos
void eventos(SDL_Event& e, bool& corriendo, botones& boton, SDL_Renderer* renderer) 
{
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            corriendo = false;
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            boton.actualizarBotones(mouseX, mouseY, e.button.button == SDL_BUTTON_LEFT, renderer);
        }
    }
}

// Función para iniciar SDL
void iniciar()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "No se pudo inicializar SDL: %s\n", SDL_GetError());
        exit(1);
    }
}

// Función para crear la ventana
void crearPantalla()
{
    window = SDL_CreateWindow("Ventana SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "No se pudo crear la ventana: %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "No se pudo crear el renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

// Función para cargar la imagen del carro
void ponerCarro() {
    SDL_Surface* carSurface = SDL_LoadBMP("/home/anareyes/Documentos/Proyecto/img/caromp.bmp");
    //SDL_Surface* carSurface = SDL_LoadBMP("/home/allison/Documents/GitHub/ProyectoEstru/Proyecto/img/caromp.bmp");
    if (!carSurface) {
        fprintf(stderr, "Error al cargar la imagen del carro: %s\n", SDL_GetError());
        exit(1);
    }

    carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
    SDL_FreeSurface(carSurface);
    if (!carTexture) {
        fprintf(stderr, "Error al crear la textura del carro: %s\n", SDL_GetError());
        exit(1);
    }
}

// Función para dibujar el carro
void dibujarCarro() {
    SDL_RenderCopy(renderer, carTexture, NULL, &carRect);
}

// Función para dibujar el mapa (calles, edificios, etc.)
void dibujarMapa(botones& boton) {
    SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255); // Azul claro
    SDL_RenderClear(renderer);

    // Dibujar el mapa cargado (imagen de fondo)
    cargarImg();
    // Dibujar calles y edificios (si es necesario)
    // calles();

    // Dibujar el carro si está activo
    if (boton.startClickeado) {
        dibujarCarro();
        grafo.dibujar();
    }

    boton.dibujarBotones(renderer);
    SDL_RenderPresent(renderer);
}



/*void calles() {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);  // Color gris oscuro
    int anchoCalleAncha = 120;   // Calle ancha
    int anchoCalleEstrecha = 60; // Calle estrecha
    int grosorLinea = 4;         // Grosor de la línea amarilla
    int radioRotonda = 200;      // Radio de la rotonda
    int centroRotondaX = 770, centroRotondaY = 500;  // Centro de la rotonda

    // Dibujar calles rectas
    auto dibujarCalleRecta = [&](int x, int y, int w, int h, bool horizontal) {
        SDL_Rect calle = {x, y, w, h};
        SDL_RenderFillRect(renderer, &calle);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Amarillo para la línea
        if (horizontal) {
            for (int i = 1; i < grosorLinea; i++) {
                SDL_RenderDrawLine(renderer, x, y + h / 2 - i, x + w, y + h / 2 - i);  // Línea amarilla
            }
        } else {
            for (int i = 1; i < grosorLinea; i++) {
                SDL_RenderDrawLine(renderer, x + w / 2 - i, y, x + w / 2 - i, y + h);  // Línea amarilla
            }
        }
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);  // Regresar a color gris
    };
   ///principales horizontales
    dibujarCalleRecta(0, 500 - anchoCalleAncha / 2, 700, anchoCalleAncha, true);  // calle principal 1
    dibujarCalleRecta(850, 500 - anchoCalleEstrecha / 2, 600, anchoCalleEstrecha, true);  // Calleprincipal 2 estrecha

    //probar
    dibujarCalleRecta(200, 250 - anchoCalleEstrecha / 2, 600, anchoCalleEstrecha, true);
    dibujarCalleRecta(180 - anchoCalleEstrecha / 2, 220, anchoCalleEstrecha, 250, false);
    dibujarCalleRecta(800, 750 - anchoCalleEstrecha / 20, 650, anchoCalleEstrecha, true); //estrecha abajo
    dibujarCalleRecta(1350 - anchoCalleAncha / 150, 0, anchoCalleAncha, 1920, false); //derecha alta
    dibujarCalleRecta(0, 2 - anchoCalleEstrecha/ 50, 1350, anchoCalleEstrecha, true);



  


    // Dibujar calles verticales (una ancha, una estrecha)
    dibujarCalleRecta(770 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 400, false);  // Calle principal3 estrecha
    dibujarCalleRecta(770 - anchoCalleAncha / 2, 600, anchoCalleAncha, 480, false);  // Calle principal 4 ancha
    

    // Dibujar la rotonda
    dibujarCirculo(renderer, centroRotondaX, centroRotondaY, radioRotonda);
}

*/




int escalaX(int x) {
    return (x - 100) * 1280 / (300 - 100);  
}

int escalaY(int y) {
    return (y - 65) * 720 / (300 - 100);  
}
// Inicializar el grafo con nodos (intersecciones) y aristas (calles)
void inicializarGrafo() {
    // Crear nodos (intersecciones) con las coordenadas escaladas
    grafo.agregarNodo(escalaX(0), escalaY(100), {255, 0, 0, 255});  
    grafo.agregarNodo(escalaX(100), escalaY(100), {0, 255, 0, 255});  
    grafo.agregarNodo(escalaX(200), escalaY(100), {0, 0, 255, 255});  
    grafo.agregarNodo(escalaX(300), escalaY(100), {255, 255, 0, 255});  

    // Crear aristas (calles) entre los nodos (ya con las coordenadas escaladas)
    grafo.agregarArista(escalaX(0), escalaY(100), escalaX(100), escalaY(100));  // Calle entre nodo rojo y verde
    grafo.agregarArista(escalaX(100), escalaY(100), escalaX(200), escalaY(200));  // Calle entre nodo verde y azul
    grafo.agregarArista(escalaX(200), escalaY(200), escalaX(300), escalaY(100));  // Calle entre nodo azul y amarillo
}


void moverCarro() {
    static int nodoActual = 0;  // Nodo actual en el que está el carro
    static int nodoSiguiente = 1;  // Nodo al que nos movemos

    // Obtener las coordenadas escaladas del nodo actual y el siguiente
    int xActual = grafo.getNodo(nodoActual).x;
    int yActual = grafo.getNodo(nodoActual).y;
    int xSiguiente = grafo.getNodo(nodoSiguiente).x;
    int ySiguiente = grafo.getNodo(nodoSiguiente).y;

    // Mover el carro hacia el siguiente nodo
 if (carRect.x < xSiguiente) {
        carRect.x += 1;  // Mover a la derecha
    } else if (carRect.x > xSiguiente) {
        carRect.x -= 1;  // Mover a la izquierda
    }

    // Mover el carro en el eje 
    if (carRect.y < ySiguiente) {
        carRect.y += 2;  // Mover hacia abajo
    } else if (carRect.y > ySiguiente) {
        carRect.y -= 2;  // Mover hacia arriba
    }

    // Si el carro llega al nodo siguiente (tanto en X como en Y)
    if (carRect.x == xSiguiente && carRect.y == ySiguiente) {
        nodoActual = nodoSiguiente;
        nodoSiguiente = (nodoSiguiente + 1) % grafo.getCantidadNodos();  
    }
   
}
/*void cargarVariables(){
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "No se pudo inicializar SDL: %s\n", SDL_GetError());
        exit(1);
    }

    screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
    if (screen == NULL) {
        fprintf(stderr, "No se pudo crear la ventana: %s\n", SDL_GetError());
        exit(1);
    }

    imagen = SDL_LoadBMP("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/map.bmp");
    if (imagen == NULL){
        fprintf(stderr, "No se pudo cargar la imagen: %s\n", SDL_GetError());
        exit(1);
    }
}

void ponerImagen() {
    float aspectRatio = (float)imagen->w / imagen->h;
    int newWidth = screen->w;
    int newHeight = screen->h;

    if (aspectRatio > 1) {
        newHeight = screen->w / aspectRatio;
    } else {
        newWidth = screen->h * aspectRatio;
    }

    SDL_Rect destRect = { (screen->w - newWidth) / 2, (screen->h - newHeight) / 2, newWidth, newHeight };

    if (SDL_BlitScaled(imagen, NULL, screen, &destRect) < 0) {
        fprintf(stderr, "Error al blitear la imagen: %s\n", SDL_GetError());
    }

    SDL_Flip(screen);  // Para actualizar la pantalla
}
*/
int main(int argc, char * args[]) {
    iniciar();
    crearPantalla();
    ponerCarro();
    cargarImg();
   //cargarVariables();
   // ponerImagen();
    inicializarGrafo();
  //inicializarSemaforos(); 

    botones boton(renderer);
    bool corriendo = true;
    SDL_Event e;

    while (corriendo) {
        eventos(e, corriendo, boton, renderer);
        
        // Actualizar estado de los semáforos
        //actualizarSemaforos();

        // Dibujar todo
        dibujarMapa(boton);
    }


    SDL_Delay(5000); // Esperar 5 segundos
    SDL_FreeSurface(imagen);  // Liberar memoria para la imagen
    SDL_Quit();
    return 0;
}