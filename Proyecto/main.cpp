#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "botones.hpp"
#include "grafo.hpp"
#include <cmath>
// Crear variables globales
SDL_Surface * screen;  // espacio en memoria de la pantalla
SDL_Surface * imagen;  // espacio en memoria de la imagen
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* carTexture = NULL;  // Textura para el carro
SDL_Rect carRect = {10, 100, 90, 70};  // Posición inicial y tamaño del carro

Grafo grafo;  // Grafo que representa las intersecciones y las calles

// Funciones auxiliares
void calles();
void edificios();
void dibujarMapa(botones& boton);



void dibujarCirculo(SDL_Renderer* renderer, int centroX, int centroY, int radio)
 {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Color de la rotonda (gris oscuro)
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

    // Dibujar calles y edificios
    calles();
    //edificios();

    // Dibujar nodos (intersecciones)
    

    // Si el botón de inicio está presionado, dibujamos el carro
    if (boton.startClickeado) {
        dibujarCarro();
        grafo.dibujar();
    }

    boton.dibujarBotones(renderer);

    SDL_RenderPresent(renderer);
}


/*void dibujarMediaCurva(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radio) {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);  // Color gris oscuro para la curva
    int cx = (x1 + x2) / 2; // Centro en x de la curva
    int cy = (y1 + y2) / 2; // Centro en y de la curva

    for (double t = 0; t < M_PI / 2; t += 0.01) {
        int x = cx + radio * cos(t);
        int y = cy - radio * sin(t);
        SDL_RenderDrawPoint(renderer, x, y);
    }
}
*/

void calles() {
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);  // Color gris oscuro
    int anchoCalle = 40;  
    int grosorLinea = 2;  
    int espacio = 80;     
    int anchoMapa = 1500; 

    // Dibujar calles horizontales
    for (int y = espacio; y < 1080; y += espacio + anchoCalle) {
        SDL_Rect calle = {0, y, anchoMapa, anchoCalle};
        SDL_RenderFillRect(renderer, &calle);
        
        // Línea amarilla en el centro de la calle
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect linea = {0, y + anchoCalle / 2 - grosorLinea / 2, anchoMapa, grosorLinea};
        SDL_RenderFillRect(renderer, &linea);

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    }

    // Dibujar calles verticales
    for (int x = espacio; x < anchoMapa; x += espacio + anchoCalle) {
        SDL_Rect calle = {x, 0, anchoCalle, 1080};
        SDL_RenderFillRect(renderer, &calle);
        
        // Línea amarilla en el centro de la calle
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_Rect linea = {x + anchoCalle / 2 - grosorLinea / 2, 0, grosorLinea, 1080};
        SDL_RenderFillRect(renderer, &linea);

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    }

    
    dibujarCirculo(renderer, 200, 400, 100);  //primero: coordenada x, segundo cordenada: y, tercero: radio del circulo

    // Dibujar otras rotondas
    dibujarCirculo(renderer, 1000, 800, 100);  
    dibujarCirculo(renderer, 1000, 200, 100); 

   
   
}


// Función para dibujar los edificios
/*void edificios() {
    SDL_SetRenderDrawColor(renderer, 100, 110, 130, 255);  // Color gris

    SDL_Rect edificio1 = { 80, 20, 60, 80 };
    SDL_RenderFillRect(renderer, &edificio1);

    SDL_Rect edificio2 = { 1040, 20, 60, 80 };
    SDL_RenderFillRect(renderer, &edificio2);

    SDL_Rect edificio3 = { 320, 380, 60, 100 };
    SDL_RenderFillRect(renderer, &edificio3);

    SDL_Rect edificio4 = { 840, 450, 60, 100 };
    SDL_RenderFillRect(renderer, &edificio4);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Color blanco

    int ventana_ancho = 15, ventana_alto = 15, espacio_horizontal = 5, espacio_vertical = 5;

    // Dibujar ventanas en edificios
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            SDL_Rect ventana = { edificio1.x + 10 + j * (ventana_ancho + espacio_horizontal),
                                 edificio1.y + 10 + i * (ventana_alto + espacio_vertical), 
                                 ventana_ancho, ventana_alto };
            SDL_RenderFillRect(renderer, &ventana);
        }
    }
}*/

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

int main(int argc, char * args[]) {
    iniciar();
    crearPantalla();
    ponerCarro();
    inicializarGrafo();

    botones boton(renderer);
    bool corriendo = true;
    SDL_Event e;

    while (corriendo) {
        eventos(e, corriendo, boton, renderer);

        // Mover el carro automáticamente
        moverCarro();

        // Dibujar la interfaz completa
        dibujarMapa(boton);
    }

    SDL_Delay(5000); // Esperar 5 segundos
    SDL_FreeSurface(imagen);  // Liberar memoria para la imagen
    SDL_Quit();
    return 0;
}