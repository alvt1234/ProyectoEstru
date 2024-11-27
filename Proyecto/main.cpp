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
std::vector<Carro> carros;
Grafo grafo;  // Grafo que representa las intersecciones y las calles

// Funciones auxiliares
void calles();
//void edificios();
void dibujarMapa(botones& boton);

void cargarImg() {
    //imagen = IMG_Load("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/map.bmp");
    imagen = IMG_Load("/home/allison/Documents/GitHub/ProyectoEstru/Proyecto/img/map.bmp");
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
    //SDL_Surface* carSurface = SDL_LoadBMP("/home/anareyes/Documentos/Proyecto/img/caromp.bmp");
    SDL_Surface* carSurface = SDL_LoadBMP("/home/allison/Documents/GitHub/ProyectoEstru/Proyecto/img/caromp.bmp");
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
            //grafo.dibujar();
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
        dibujarCalleRecta(175 - anchoCalleEstrecha / 5, 230, anchoCalleEstrecha, 220, false);//calle #1 vertical 
        dibujarCalleRecta(0, 400 - anchoCalleEstrecha / 2, 700, anchoCalleEstrecha, true); //horizontak
        dibujarCalleRecta(650 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 400, false); //calle #3 vertical arriba
        dibujarCalleRecta(0, 100 - anchoCalleEstrecha / 2, 647, anchoCalleEstrecha, true);//calle #2 horizontal 
        dibujarCalleRecta(400 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 450, false); //calle #2 vertical
        //----
        dibujarCalleRecta(770 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 400, false);  // Calle principal3 estrecha
        dibujarCalleRecta(800, 750 - anchoCalleEstrecha / 20, 650, anchoCalleEstrecha, true); //calle alli
        dibujarCalleRecta(1350 - anchoCalleAncha / 150, 0, anchoCalleAncha, 1920, false); //derecha alta
        dibujarCalleRecta(900 - anchoCalleAncha /5,0, anchoCalleEstrecha,550, false); //cuarti
        dibujarCalleRecta(0, 2 - anchoCalleEstrecha/ 50, 1350, anchoCalleEstrecha, true); //calle horizontal arriba
        dibujarCalleRecta(950 - anchoCalleAncha /2, 600, anchoCalleEstrecha,580, false); //calle samaritana
        dibujarCalleRecta(1100 - anchoCalleAncha /5,500, anchoCalleEstrecha,250, false); //calle barrial
        dibujarCalleRecta(1100 - anchoCalleAncha /5,150, anchoCalleEstrecha,350, false); //calle morada derecha vertical --segundo cuadrante
        dibujarCalleRecta(770, 150 - anchoCalleEstrecha / 2, 580, anchoCalleEstrecha, true); //calle recta arriba de morada --segundo cuadrante calle guamilito
        dibujarCalleRecta(940, 900 - anchoCalleEstrecha / 2, 410, anchoCalleEstrecha, true); //calle roma
        dibujarCalleRecta(1250 - anchoCalleAncha /5,500, anchoCalleEstrecha,700, false);//calle horizona*/
        //tercer cuadrante
        
        /*dibujarCalleRecta(650 - anchoCalleEstrecha / 2, 600, anchoCalleEstrecha, 480, false); //calle vertical a la izquierda de la princiap 4/*
        dibujarCalleRecta(0, 620 - anchoCalleEstrecha / 2, 650, anchoCalleEstrecha, true);  //calle horizontal abajo de la principal 1
        dibujarCalleRecta(300 - anchoCalleEstrecha / 2, 620, anchoCalleEstrecha, 460, false);//segunda ertical a la izquierda de la principal 4
        
        dibujarCalleRecta(0, 820 - anchoCalleEstrecha / 2, 645, anchoCalleEstrecha, true);  //tercer cuadrante

        dibujarCalleRecta(170 - anchoCalleEstrecha / 2, 820, anchoCalleEstrecha, 300, false);//segunda ertical a la izquierda de la principal 4
        dibujarCalleRecta(300, 900- anchoCalleEstrecha / 2, 450, anchoCalleEstrecha, true);

        // Dibujar calles verticales (una ancha, una estrecha) PRINCIPALES tmb
        
        dibujarCalleRecta(770 - anchoCalleAncha / 2, 600, anchoCalleAncha, 480, false);  // Calle principal 4 ancha*/
      
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
/*void inicializarGrafo() {
    // Crear nodos (intersecciones) con las coordenadas escaladas
    grafo.agregarNodo(escalaX(0), escalaY(100), {255, 0, 0, 255});  
    grafo.agregarNodo(escalaX(100), escalaY(100), {0, 255, 0, 255});  
    grafo.agregarNodo(escalaX(200), escalaY(100), {0, 0, 255, 255});  
    grafo.agregarNodo(escalaX(300), escalaY(100), {255, 255, 0, 255});  

    // Crear aristas (calles) entre los nodos (ya con las coordenadas escaladas)
    grafo.agregarArista(escalaX(0), escalaY(100), escalaX(100), escalaY(100));  // Calle entre nodo rojo y verde
    grafo.agregarArista(escalaX(100), escalaY(100), escalaX(200), escalaY(200));  // Calle entre nodo verde y azul
    grafo.agregarArista(escalaX(200), escalaY(200), escalaX(300), escalaY(100));  // Calle entre nodo azul y amarillo
}*/


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
void detectarInterseccionYDecidir(Carro& carro, const Grafo& grafo) {
    for (const auto& nodo : grafo.getNodos()) {
        // Detecta si el carro está cerca de un nodo
        if (std::abs(carro.rect.x - nodo.x) < 5 && std::abs(carro.rect.y - nodo.y) < 5) {
            carro.girarHaciaArista(grafo);  // Decidir nueva dirección aleatoria
            break;
        }
    }
}
void agregarNodosRotonda(Grafo& grafo) {
    SDL_Color colorNodo = {255, 255, 0, 255};  // Amarillo para los nodos de la rotonda
    int centroX = 770;  // Coordenada X del centro de la rotonda
    int centroY = 500;  // Coordenada Y del centro de la rotonda
    int radio = 150;    // Radio de la rotonda

    // Crear 8 nodos distribuidos uniformemente en la rotonda
    for (int i = 0; i < 8; ++i) {
        double angulo = i * (2 * M_PI / 8);  // Dividir el círculo en 8 partes
        int x = static_cast<int>(centroX + radio * cos(angulo));  // Coordenada X
        int y = static_cast<int>(centroY + radio * sin(angulo));  // Coordenada Y
        grafo.agregarNodo(x, y, colorNodo);  // Agregar el nodo al grafo
    }

    // Conectar los nodos de la rotonda en un círculo
    int inicioRotonda = grafo.getCantidadNodos() - 8;  // Índice del primer nodo de la rotonda
    for (int i = 0; i < 8; ++i) {
        grafo.agregarArista(inicioRotonda + i, inicioRotonda + (i + 1) % 8);
    }
}

void conectarRotondaConCalles(Grafo& grafo) {
    int inicioRotonda = grafo.getCantidadNodos() - 8;  // Índice del primer nodo de la rotonda

    // Conectar entradas/salidas de la rotonda con las calles
    grafo.agregarArista(54, inicioRotonda);  // Nodo B -> Entrada de la rotonda
    grafo.agregarArista(inicioRotonda + 4, 3);  // Salida de la rotonda -> Nodo D
}

void inicializarCarros() {
    // Ejemplo: Crear 3 carros con diferentes posiciones iniciales
  
    carros.push_back(Carro(0, 28, 20, 10));
    carros.push_back(Carro(30, 28, 20, 10));
    carros.push_back(Carro(60, 28, 20, 10));
    carros.push_back(Carro(90, 28, 20, 10));
    carros.push_back(Carro(0, 5, 20, 10));
    carros.push_back(Carro(30, 5, 20, 10));
    carros.push_back(Carro(60, 5, 20, 10));
    carros.push_back(Carro(90, 5, 20, 10));
    std::vector<Carro> carros = {
        Carro(100, 500, 50, 30, 'H', 2),  // Carro en nodo inicial
        Carro(200, 500, 50, 30, 'H', 2)   // Otro carro
    };
}

void actualizarCarros(Grafo& grafo) {
    for (auto& carro : carros) {
        detectarInterseccionYDecidir(carro, grafo);  // Verificar si necesita girar
        carro.mover();  // Mover el carro
        carro.dibujar(renderer);  // Dibujar el carro
    }
}

void inicializarGrafo(Grafo& grafo) {
    // Colores para los nodos (puedes cambiarlos si quieres visualizarlos diferentes)
    SDL_Color colorNodo = {255, 0, 0, 255};  // Rojo para las intersecciones

    // Agregar nodos (intersecciones)
    // Primer cuadrante carril
    
    grafo.agregarNodo(380, 5, colorNodo);    // Nodo A (calle principal 1)
    grafo.agregarNodo(650, 5, colorNodo);  // Nodo B (final calle principal 1)
    grafo.agregarNodo(770, 5, colorNodo);  // Nodo C (inicio calle principal 2)
    grafo.agregarNodo(925, 5, colorNodo); // Nodo D (final calle principal 2)
    grafo.agregarNodo(1370, 5, colorNodo); // Nodo E

    //segundo carril calle 1 hor
    grafo.agregarNodo(400, 28, colorNodo);    // Nodo F (calle principal 1)
    grafo.agregarNodo(630, 28, colorNodo);  // Nodo G (final calle principal 1)
    grafo.agregarNodo(750, 28, colorNodo);  // Nodo H (inicio calle principal 2)
    grafo.agregarNodo(905, 28, colorNodo); // Nodo I (final calle principal 2)
    grafo.agregarNodo(1350, 28, colorNodo); // Nodo J
    grafo.agregarNodo(630, 85, colorNodo); // Nodo k
    grafo.agregarNodo(650, 105, colorNodo); // Nodo L
    grafo.agregarNodo(400, 85, colorNodo); // Nodo M
    grafo.agregarNodo(380, 105, colorNodo); // Nodo N
    grafo.agregarNodo(0, 85, colorNodo);  // Nodo N`
    grafo.agregarNodo(0, 105, colorNodo);  // Nodo O
    grafo.agregarNodo(630, 235, colorNodo);  // Nodo P
    grafo.agregarNodo(650, 255, colorNodo);  // Nodo Q
    grafo.agregarNodo(400, 235, colorNodo);  // Nodo R
    grafo.agregarNodo(380, 255, colorNodo);  // Nodo S
    grafo.agregarNodo(165, 235, colorNodo);  // Nodo T
    grafo.agregarNodo(185, 255, colorNodo);  // Nodo U
    grafo.agregarNodo(165, 385, colorNodo);  // Nodo V
    grafo.agregarNodo(185, 405, colorNodo);  // Nodo W
    grafo.agregarNodo(0, 385, colorNodo);  // Nodo X
    grafo.agregarNodo(0, 405, colorNodo);  // Nodo Y
    grafo.agregarNodo(1350, 135, colorNodo);  // Nodo Z
    grafo.agregarNodo(1370, 155, colorNodo);  // Nodo 27
    grafo.agregarNodo(1080, 135, colorNodo);  // Nodo 28
    grafo.agregarNodo(1100, 155, colorNodo);  // Nodo 29
    grafo.agregarNodo(1080, 505, colorNodo);  // Nodo 30
    grafo.agregarNodo(1100, 485, colorNodo);  // Nodo 31
    grafo.agregarNodo(1350, 485, colorNodo);  // Nodo 32
    grafo.agregarNodo(1370, 505, colorNodo);  // Nodo 33
    grafo.agregarNodo(1350, 755, colorNodo);  // Nodo 34
    grafo.agregarNodo(1370, 775, colorNodo);  // Nodo 35
    grafo.agregarNodo(1350, 885, colorNodo);  // Nodo 36
    grafo.agregarNodo(1370, 905, colorNodo);  // Nodo 37
    grafo.agregarNodo(1350, 975, colorNodo);  // Nodo 38
    grafo.agregarNodo(1370, 995, colorNodo);  // Nodo 39
    grafo.agregarNodo(1080, 755, colorNodo);  // Nodo 40
    grafo.agregarNodo(1100, 775, colorNodo);  // Nodo 41
    grafo.agregarNodo(1230, 485, colorNodo);  // Nodo 42
    grafo.agregarNodo(1250, 505, colorNodo);  // Nodo 43
    grafo.agregarNodo(1230, 755, colorNodo);  // Nodo 44
    grafo.agregarNodo(1250, 775, colorNodo);  // Nodo 45
    grafo.agregarNodo(1230, 905, colorNodo);  // Nodo 46
    grafo.agregarNodo(1250, 885, colorNodo);  // Nodo 47
    grafo.agregarNodo(1230, 995, colorNodo);  // Nodo 48
    grafo.agregarNodo(1250, 975, colorNodo);  // Nodo 49
    grafo.agregarNodo(925, 885, colorNodo);  // Nodo 50
    grafo.agregarNodo(905, 905, colorNodo);  // Nodo 51
    grafo.agregarNodo(925, 755, colorNodo);  // Nodo 52
    grafo.agregarNodo(905, 775, colorNodo);  // Nodo 53
    grafo.agregarNodo(925, 600, colorNodo);  // Nodo 54
    grafo.agregarNodo(905, 590, colorNodo);  // Nodo 55
    grafo.agregarNodo(770, 255, colorNodo);  // Nodo 56
    grafo.agregarNodo(750, 235, colorNodo);  // Nodo 57
    grafo.agregarNodo(925, 135, colorNodo);  // Nodo 58
    grafo.agregarNodo(905, 155, colorNodo);  // Nodo 59
    grafo.agregarNodo(925, 385, colorNodo);  // Nodo 60
    grafo.agregarNodo(905, 405, colorNodo);  // Nodo 61
    grafo.agregarNodo(770, 135, colorNodo);  // Nodo 62
    grafo.agregarNodo(750, 155, colorNodo);  // Nodo 63
    grafo.agregarNodo(380, 385, colorNodo);  // Nodo 64
    grafo.agregarNodo(400, 405, colorNodo);  // Nodo 65
    grafo.agregarNodo(630, 385, colorNodo);  // Nodo 66
    grafo.agregarNodo(650, 405, colorNodo);  // Nodo 67
     //agregarNodosRotonda(grafo);
    // Aristas
    grafo.agregarArista(0, 1);  // A -> B 
    grafo.agregarArista(1, 2);  // B -> C 
    grafo.agregarArista(2, 3);  // C -> D 
    grafo.agregarArista(3, 4);  // D -> E 
    grafo.agregarArista(5, 6);  // F -> G 
    grafo.agregarArista(6, 7);  // G -> H 
    grafo.agregarArista(7, 8);  // H -> I 
    grafo.agregarArista(8, 9);  // I -> J 
    grafo.agregarArista(6, 10);  // G -> K
    grafo.agregarArista(1, 11); // B -> L 
    grafo.agregarArista(10, 12); // k -> M
    grafo.agregarArista(12, 5); // M -> F 
    grafo.agregarArista(11, 13); // L -> N
    grafo.agregarArista(13, 0); // N -> A
    grafo.agregarArista(12, 14); // M -> N`
    grafo.agregarArista(13, 15); // N -> O
    grafo.agregarArista(10, 16); // M -> N`
    grafo.agregarArista(11, 17); // N -> O
    grafo.agregarArista(16, 18); // M -> N`
    grafo.agregarArista(17, 19); // N -> O
    grafo.agregarArista(18, 12); // M -> N`
    grafo.agregarArista(19, 13); // N -> O
    grafo.agregarArista(18, 20); // M -> N`
    grafo.agregarArista(19, 21); // N -> O
    grafo.agregarArista(20, 22); // M -> N`
    grafo.agregarArista(21, 23); // N -> O
    grafo.agregarArista(22, 24); // M -> N`
    grafo.agregarArista(23, 25); // N -> O
    grafo.agregarArista(9, 26); // N -> O
    grafo.agregarArista(4, 27); // N -> O
    grafo.agregarArista(26, 28); // N -> O
    grafo.agregarArista(27, 29);
    grafo.agregarArista(28, 30); // N -> O
    grafo.agregarArista(29, 31);
    grafo.agregarArista(30, 43);
    grafo.agregarArista(31, 42);
    grafo.agregarArista(43, 33);
    grafo.agregarArista(42, 32);
    grafo.agregarArista(26, 32);
    grafo.agregarArista(27, 33);
    grafo.agregarArista(32, 34);
    grafo.agregarArista(33, 35);
    grafo.agregarArista(34, 36);
    grafo.agregarArista(35, 37);
    grafo.agregarArista(36, 38);
    grafo.agregarArista(37, 39);

    grafo.agregarArista(34, 44);
    grafo.agregarArista(35, 45);
    grafo.agregarArista(45, 41);
    grafo.agregarArista(44, 40);

    grafo.agregarArista(42, 44);
    grafo.agregarArista(43, 45);
    grafo.agregarArista(30, 40);
    grafo.agregarArista(31, 41);
    grafo.agregarArista(45, 47);
    grafo.agregarArista(44, 46);
    grafo.agregarArista(46, 37);
    grafo.agregarArista(47, 36);
    grafo.agregarArista(47, 49);
    grafo.agregarArista(46, 48);
    grafo.agregarArista(50, 47);
    grafo.agregarArista(51, 46);
    grafo.agregarArista(40, 52);
    grafo.agregarArista(41, 53);
    grafo.agregarArista(50, 52);
    grafo.agregarArista(51, 53);
    grafo.agregarArista(52, 54);
    grafo.agregarArista(53, 55);
    grafo.agregarArista(63, 57);
    grafo.agregarArista(7, 63);
    grafo.agregarArista(2, 62);
    grafo.agregarArista(62, 56);
    grafo.agregarArista(57, 16);
    grafo.agregarArista(56, 17);
    grafo.agregarArista(58, 3);
    grafo.agregarArista(59, 8);
    grafo.agregarArista(60, 58);
    grafo.agregarArista(61, 59);
    grafo.agregarArista(28, 58);
    grafo.agregarArista(29, 59);
    grafo.agregarArista(28, 58);
    grafo.agregarArista(29, 59);
    grafo.agregarArista(59, 63);
    grafo.agregarArista(58, 62);
    grafo.agregarArista(64, 22);
    grafo.agregarArista(65, 23);
    grafo.agregarArista(64, 19);
    grafo.agregarArista(65, 18);
    grafo.agregarArista(67, 65);
    grafo.agregarArista(66, 64);
    //grafo.agregarArista(67, 61);
    //grafo.agregarArista(66, 60);
    grafo.agregarArista(16, 66);
    grafo.agregarArista(17, 67);
    //grafo.agregarArista(54, 60);
    //grafo.agregarArista(55, 61);
    //grafo.agregarArista(54, 56);
    //grafo.agregarArista(55, 56);
     //grafo.agregarArista(56, 57);
       //conectarRotondaConCalles(grafo);
    
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


int main(int argc, char *args[]) {
    iniciar();
    crearPantalla();
    Grafo grafo;
    inicializarGrafo(grafo);
    inicializarCarros();  // Inicializa varios carros
    botones boton(renderer);
    bool corriendo = true;
    SDL_Event e;

    while (corriendo) {
        eventos(e, corriendo, boton, renderer);

        SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255); // Color de fondo del mapa
        SDL_RenderClear(renderer);
    grafo.dibujar(renderer);

        dibujarMapa(boton);  // Dibuja el mapa
        actualizarCarros(grafo);  // Mueve y dibuja todos los carros

        SDL_RenderPresent(renderer);
        SDL_Delay(16);  // Aproximadamente 60 FPS
    }

    SDL_Quit();
    return 0;
}
