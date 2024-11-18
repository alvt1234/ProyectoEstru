#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "botones.hpp"
#include "grafo.hpp"
#include <cmath>
#include <vector>
#include "Semaforo.hpp" 
#include "Carro.hpp"
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
//void edificios();
void dibujarMapa(botones& boton);

void cargarImg() {
    imagen = IMG_Load("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/map.bmp");
    //imagen = IMG_Load("/home/allison/Documents/GitHub/ProyectoEstru/Proyecto/img/map.bmp");
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

void dibujarCirculo(SDL_Renderer* renderer, int centroX, int centroY, int radio)
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
    /*void dibujarCarro() {
        SDL_RenderCopy(renderer, carTexture, NULL, &carRect);
    }*/
   void dibujarCarro() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Color rojo para el carro
    SDL_RenderFillRect(renderer, &carRect);
}


    // Función para dibujar el mapa (calles, edificios, etc.)
    void dibujarMapa(botones& boton) {
        SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255); // Azul claro
        //SDL_RenderClear(renderer);

        // Dibujar el mapa cargado (imagen de fondo)
        //cargarImg();
        // Dibujar calles y edificios (si es necesario)
        calles();

        // Dibujar el carro si está activo
        if (boton.startClickeado) {
            dibujarCarro();
            grafo.dibujar();
        }

        boton.dibujarBotones(renderer);
        //SDL_RenderPresent(renderer);
    }



    void calles() {
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);  // Color gris oscuro
        int anchoCalleAncha = 100;   // Calle ancha
        int anchoCalleEstrecha = 40; // Calle estrecha
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

        //primer cuadrante
        dibujarCalleRecta(200, 250 - anchoCalleEstrecha / 2, 550, anchoCalleEstrecha, true); // calle acostada en L
        dibujarCalleRecta(180 - anchoCalleEstrecha / 5, 230, anchoCalleEstrecha, 240, false);
        dibujarCalleRecta(0, 400 - anchoCalleEstrecha / 2, 700, anchoCalleEstrecha, true); //horizontak
        dibujarCalleRecta(650 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 400, false); //vrtical
        dibujarCalleRecta(0, 100 - anchoCalleEstrecha / 2, 650, anchoCalleEstrecha, true);
        dibujarCalleRecta(400 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 450, false); 
        //----
        dibujarCalleRecta(800, 750 - anchoCalleEstrecha / 20, 650, anchoCalleEstrecha, true); //estrecha abajo
        dibujarCalleRecta(1350 - anchoCalleAncha / 150, 0, anchoCalleAncha, 1920, false); //derecha alta
        dibujarCalleRecta(0, 2 - anchoCalleEstrecha/ 50, 1350, anchoCalleEstrecha, true); //calle horizontal arriba
        dibujarCalleRecta(950 - anchoCalleAncha /2, 600, anchoCalleEstrecha,580, false); 
        dibujarCalleRecta(1100 - anchoCalleAncha /5,500, anchoCalleEstrecha,250, false); 
        dibujarCalleRecta(1100 - anchoCalleAncha /5,150, anchoCalleEstrecha,350, false); //calle morada derecha vertical --segundo cuadrante
        dibujarCalleRecta(750, 150 - anchoCalleEstrecha / 2, 600, anchoCalleEstrecha, true); //calle recta arriba de morada --segundo cuadrante
        dibujarCalleRecta(900 - anchoCalleAncha /5,10, anchoCalleEstrecha,550, false); //cuarti
        dibujarCalleRecta(900, 900 - anchoCalleEstrecha / 2, 450, anchoCalleEstrecha, true); //cuarto
        dibujarCalleRecta(1250 - anchoCalleAncha /5,500, anchoCalleEstrecha,700, false);//cuarto  cuadrante
        //tercer cuadrante
        dibujarCalleRecta(650 - anchoCalleEstrecha / 2, 600, anchoCalleEstrecha, 480, false); //calle vertical a la izquierda de la princiap 4
        dibujarCalleRecta(0, 620 - anchoCalleEstrecha / 2, 650, anchoCalleEstrecha, true);  //calle horizontal abajo de la principal 1
        dibujarCalleRecta(300 - anchoCalleEstrecha / 2, 600, anchoCalleEstrecha, 480, false);//segunda ertical a la izquierda de la principal 4
        dibujarCalleRecta(170 - anchoCalleEstrecha / 2, 800, anchoCalleEstrecha, 300, false);//segunda ertical a la izquierda de la principal 4
        dibujarCalleRecta(0, 820 - anchoCalleEstrecha / 2, 650, anchoCalleEstrecha, true);  //tercer cuadrante
        dibujarCalleRecta(300, 900- anchoCalleEstrecha / 2, 450, anchoCalleEstrecha, true);

        // Dibujar calles verticales (una ancha, una estrecha) PRINCIPALES tmb
        dibujarCalleRecta(770 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 400, false);  // Calle principal3 estrecha
        dibujarCalleRecta(770 - anchoCalleAncha / 2, 600, anchoCalleAncha, 480, false);  // Calle principal 4 ancha
      
    // Dibujar la rotonda
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    dibujarCirculo(renderer, 770, 500, 200);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);//circulo negro mas peque
    dibujarCirculo(renderer, 770, 500, 198);
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    dibujarCirculo(renderer, 770, 500, 150);
    SDL_SetRenderDrawColor(renderer, 60, 179, 113, 255);  //circulo verde de fondo
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    dibujarCirculo(renderer, 770, 500, 148);
    SDL_SetRenderDrawColor(renderer, 60, 179, 113, 255);
    dibujarCirculo(renderer, 770, 500, 100);
 
    }






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

/*int main(int argc, char *args[]) {
    iniciar();
    crearPantalla();
    ponerCarro();
    //cargarImg();  // Cargar la imagen del mapa
    inicializarGrafo();
    
    botones boton(renderer);
    bool corriendo = true;
    SDL_Event e;
    Carro carro(100, 100, 20, 10);  // Crear un carro con posición inicial

    while (corriendo) {
        // Procesar eventos
        eventos(e, corriendo, boton, renderer);
        // Dibujar el mapa
        dibujarMapa(boton);  // Asegúrate de que esta función dibuja el mapa en cada fotograma
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fondo blanco
        SDL_RenderClear(renderer);

        // Actualizar y dibujar el auto
        carro.mover(1, 0); // Mover el carro hacia la derecha
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Color rojo para el auto
        SDL_RenderFillRect(renderer, &carro.getRect());

        // Actualizar pantalla
        SDL_RenderPresent(renderer);
        // Controlar el framerate
        SDL_Delay(16); // Aproximadamente 60 FPS
    }

    // Limpiar recursos al salir del bucle
    SDL_FreeSurface(imagen);  // Liberar memoria para la imagen
    SDL_Quit();
    return 0;
}*/
int main(int argc, char *args[]) {
    iniciar();
    crearPantalla();
    inicializarGrafo();
    botones boton(renderer);
    bool corriendo = true;
    SDL_Event e;
    Carro carro(100, 100, 20, 10);  // Crear un carro con posición inicial

    while (corriendo) {
        // Procesar eventos
         eventos(e, corriendo, boton, renderer);
     SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255); // Color de fondo del mapa
        SDL_RenderClear(renderer);
        dibujarMapa(boton);
        // Limpiar la pantalla al inicio de cada iteración
        carRect.x += 1;
         dibujarCarro();
           SDL_RenderPresent(renderer);

        // Controlar el framerate
        SDL_Delay(16); // Aproximadamente 60 FPS
    }

    SDL_Quit();
    return 0;
}
