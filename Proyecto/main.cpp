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
        dibujarCalleRecta(0, 500 - anchoCalleAncha / 2, 740, anchoCalleAncha, true);  // calle dobele Horizontal
        dibujarCalleRecta(820, 505 , 630, anchoCalleEstrecha, true);  // Calle perito

        //primer cuadrante
        dibujarCalleRecta(200, 250 - anchoCalleEstrecha / 2, 550, anchoCalleEstrecha, true); // calle acamparito Horizontal
        dibujarCalleRecta(175 - anchoCalleEstrecha / 5, 230, anchoCalleEstrecha, 220, false);//calle #1 vertical Villa
        dibujarCalleRecta(0, 400 - anchoCalleEstrecha / 2, 670, anchoCalleEstrecha, true); //horizonta pedro
        dibujarCalleRecta(650 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 400, false); //calle #3 vertical arriba bajada
        dibujarCalleRecta(0, 100 - anchoCalleEstrecha / 2, 647, anchoCalleEstrecha, true);//calle #2 horizontal curacao
        dibujarCalleRecta(400 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 450, false); //calle #2 vertical Rosi
        //----
        dibujarCalleRecta(770 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 400, false);  // Calle principal3 estrecha
        dibujarCalleRecta(800, 750 - anchoCalleEstrecha / 20, 650, anchoCalleEstrecha, true); //calle alli
        dibujarCalleRecta(1350 - anchoCalleAncha / 150, 0, anchoCalleAncha, 1920, false); // vertical doble segunda
        dibujarCalleRecta(920 - anchoCalleAncha /5,0, anchoCalleEstrecha,1100, false); //calle vertical palo alto
        dibujarCalleRecta(0, 2 - anchoCalleEstrecha/ 50, 1350, anchoCalleEstrecha, true); //calle horizontal arriba principal
        dibujarCalleRecta(1100 - anchoCalleAncha /5,500, anchoCalleEstrecha,250, false); //calle barrial
        dibujarCalleRecta(1100 - anchoCalleAncha /5,150, anchoCalleEstrecha,350, false); //calle Guamilito**/
        dibujarCalleRecta(770, 150 - anchoCalleEstrecha / 2, 580, anchoCalleEstrecha, true); //calle horizontal sofia
        dibujarCalleRecta(940, 900 - anchoCalleEstrecha / 2, 410, anchoCalleEstrecha, true); //calle roma
        dibujarCalleRecta(1250 - anchoCalleAncha /5,530, anchoCalleEstrecha,700, false);//calle horizona
        //tercer cuadrante
        
        dibujarCalleRecta(650 - anchoCalleEstrecha / 2, 600, anchoCalleEstrecha, 480, false); //calle vertical comercio
        dibujarCalleRecta(0, 620 - anchoCalleEstrecha / 2, 650, anchoCalleEstrecha, true);  //calle horizontal abajo san marcos
        dibujarCalleRecta(300 - anchoCalleEstrecha / 2, 620, anchoCalleEstrecha, 460, false);//calle vertical morazan
        
        dibujarCalleRecta(0, 820 - anchoCalleEstrecha / 2, 720, anchoCalleEstrecha, true);  //calle horizontal constantinopla

        dibujarCalleRecta(170 - anchoCalleEstrecha / 2, 820, anchoCalleEstrecha, 300, false);//calle vertical rosario
        dibujarCalleRecta(300, 900- anchoCalleEstrecha / 2, 450, anchoCalleEstrecha, true); //calle horizontal real

        // Dibujar calles verticales (una ancha, una estrecha) PRINCIPALES tmb
        //MODIFICAR*/
        dibujarCalleRecta(770 - anchoCalleAncha / 2, 0, anchoCalleAncha, 1100, false);  // Calle doble carril vertical */
      
    // Dibujar la rotonda
    /*SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    dibujarCirculo(renderer, 770, 500, 200);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);//circulo negro mas peque
    dibujarCirculo(renderer, 770, 500, 198);
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    dibujarCirculo(renderer, 770, 500, 150);
    SDL_SetRenderDrawColor(renderer, 60, 179, 113, 255);  //circulo verde de fondo
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    dibujarCirculo(renderer, 770, 500, 148);
    SDL_SetRenderDrawColor(renderer, 60, 179, 113, 255);
    dibujarCirculo(renderer, 770, 500, 100);*/
 
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
    grafo.agregarNodo(0, 5, colorNodo);  // Nodo 0
    grafo.agregarNodo(0, 28, colorNodo);  // Nodo 1
    grafo.agregarNodo(380, 5, colorNodo);    // Nodo 2
    grafo.agregarNodo(400, 28, colorNodo);    // Nodo 3
    grafo.agregarNodo(630, 28, colorNodo);  // Nodo 4
    grafo.agregarNodo(650, 5, colorNodo);  // Nodo 5
    grafo.agregarNodo(630, 85, colorNodo); // Nodo 6
    grafo.agregarNodo(650, 105, colorNodo); // Nodo 7
    grafo.agregarNodo(400, 85, colorNodo); // Nodo 8
    grafo.agregarNodo(380, 105, colorNodo); // Nodo 9
    grafo.agregarNodo(0, 85, colorNodo); // Nodo 10
    grafo.agregarNodo(0, 105, colorNodo); // Nodo 11
    grafo.agregarNodo(630, 235, colorNodo); // Nodo 12
    grafo.agregarNodo(650, 255, colorNodo); // Nodo 13
    grafo.agregarNodo(400, 235, colorNodo); // Nodo 14
    grafo.agregarNodo(380, 255, colorNodo); // Nodo 15
    grafo.agregarNodo(165, 235, colorNodo);  // Nodo 16
    grafo.agregarNodo(185, 255, colorNodo);  // Nodo 17
    grafo.agregarNodo(165, 385, colorNodo);  // Nodo 18
    grafo.agregarNodo(185, 405, colorNodo);  // Nodo 19
    grafo.agregarNodo(0, 385, colorNodo);  // Nodo 20
    grafo.agregarNodo(0, 405, colorNodo);  // Nodo 21
    grafo.agregarNodo(630, 385, colorNodo);  // Nodo 22
    grafo.agregarNodo(650, 405, colorNodo);  // Nodo 23
    grafo.agregarNodo(400, 385, colorNodo);  // Nodo 24
    grafo.agregarNodo(380, 405, colorNodo);  // Nodo 25
    grafo.agregarNodo(0, 455, colorNodo);  // Nodo 26
    grafo.agregarNodo(0, 475, colorNodo);  // Nodo 27
    grafo.agregarNodo(185, 455, colorNodo);  // Nodo 28
    grafo.agregarNodo(165, 475, colorNodo);  // Nodo 29
    grafo.agregarNodo(380, 455, colorNodo);  // Nodo 30
    grafo.agregarNodo(400, 475, colorNodo);  // Nodo 31
    grafo.agregarNodo(725, 455, colorNodo);  // Nodo 32
    grafo.agregarNodo(745, 475, colorNodo);  // Nodo 33
    grafo.agregarNodo(725, 255, colorNodo);  // Nodo 34
    grafo.agregarNodo(745, 235, colorNodo);  // Nodo 35
    grafo.agregarNodo(725, 5, colorNodo);  // Nodo 36
    grafo.agregarNodo(745, 28, colorNodo);  // Nodo 37
    grafo.agregarNodo(770, 455, colorNodo);  // Nodo 38
    grafo.agregarNodo(790, 475, colorNodo);  // Nodo 39
    grafo.agregarNodo(770, 255, colorNodo);  // Nodo 40
    grafo.agregarNodo(790, 235, colorNodo);  // Nodo 41
    grafo.agregarNodo(790, 5, colorNodo);  // Nodo 42
    grafo.agregarNodo(770, 28, colorNodo);  // Nodo 43
    grafo.agregarNodo(770, 515, colorNodo);  // Nodo 44
    grafo.agregarNodo(790, 535, colorNodo);  // Nodo 45
    grafo.agregarNodo(400, 515, colorNodo);  // Nodo 46
    grafo.agregarNodo(380, 535, colorNodo);  // Nodo 47
    grafo.agregarNodo(185, 535, colorNodo);  // Nodo 48
    grafo.agregarNodo(165, 515, colorNodo);  // Nodo 49
    grafo.agregarNodo(290, 605, colorNodo);  // Nodo 50
    grafo.agregarNodo(310, 625, colorNodo);  // Nodo 51
    grafo.agregarNodo(290, 805, colorNodo);  // Nodo 52
    grafo.agregarNodo(310, 825, colorNodo);  // Nodo 53
    grafo.agregarNodo(180, 805, colorNodo);  // Nodo 54
    grafo.agregarNodo(160, 825, colorNodo);  // Nodo 55
    grafo.agregarNodo(0, 805, colorNodo);  // Nodo 56
    grafo.agregarNodo(0, 825, colorNodo);  // Nodo 57
    grafo.agregarNodo(160, 1000, colorNodo);  // Nodo 58
    grafo.agregarNodo(180, 1000, colorNodo);  // Nodo 59
    grafo.agregarNodo(290, 910, colorNodo);  // Nodo 60
    grafo.agregarNodo(310, 890, colorNodo);  // Nodo 61
    grafo.agregarNodo(290, 1000, colorNodo);  // Nodo 62
    grafo.agregarNodo(310, 1000, colorNodo);  // Nodo 63
    grafo.agregarNodo(660, 605, colorNodo);  // Nodo 64
    grafo.agregarNodo(640, 625, colorNodo);  // Nodo 65
    grafo.agregarNodo(640, 805, colorNodo);  // Nodo 66
    grafo.agregarNodo(660, 825, colorNodo);  // Nodo 67
    grafo.agregarNodo(640, 910, colorNodo);  // Nodo 68
    grafo.agregarNodo(660, 890, colorNodo);  // Nodo 69
    grafo.agregarNodo(640, 1000, colorNodo);  // Nodo 70
    grafo.agregarNodo(660, 1000, colorNodo);  // Nodo 71
    grafo.agregarNodo(725, 805, colorNodo);  // Nodo 72
    grafo.agregarNodo(745, 825, colorNodo);  // Nodo 73
    grafo.agregarNodo(725, 890, colorNodo);  // Nodo 74
    grafo.agregarNodo(745, 910, colorNodo);  // Nodo 75

    grafo.agregarNodo(770, 775, colorNodo);  // Nodo 76
    grafo.agregarNodo(790, 755, colorNodo);  // Nodo 77

    grafo.agregarNodo(770, 805, colorNodo);  // Nodo 78
    grafo.agregarNodo(790, 825, colorNodo);  // Nodo 79
    grafo.agregarNodo(770, 890, colorNodo);  // Nodo 80
    grafo.agregarNodo(790, 910, colorNodo);  // Nodo 81
    grafo.agregarNodo(900, 515, colorNodo);  // Nodo 82
    grafo.agregarNodo(920, 535, colorNodo);  // Nodo 83
    grafo.agregarNodo(790, 135, colorNodo);  // Nodo 84
    grafo.agregarNodo(770, 155, colorNodo);  // Nodo 85
    grafo.agregarNodo(900, 135, colorNodo);  // Nodo 86
    grafo.agregarNodo(920, 155, colorNodo);  // Nodo 87
    grafo.agregarNodo(900, 5, colorNodo);  // Nodo 88
    grafo.agregarNodo(920, 28, colorNodo);  // Nodo 89
    grafo.agregarNodo(900, 755, colorNodo);  // Nodo 90
    grafo.agregarNodo(920, 775, colorNodo);  // Nodo 91
    grafo.agregarNodo(920, 890, colorNodo);  // Nodo 92
    grafo.agregarNodo(900, 910, colorNodo);  // Nodo 93
    grafo.agregarNodo(1350, 28, colorNodo);  // Nodo 94
    grafo.agregarNodo(1370, 5, colorNodo);  // Nodo 95
    grafo.agregarNodo(1350, 135, colorNodo);  // Nodo 96
    grafo.agregarNodo(1370, 155, colorNodo);  // Nodo 97
    grafo.agregarNodo(1085, 135, colorNodo);  // Nodo 98
    grafo.agregarNodo(1105, 155, colorNodo);  // Nodo 99
    grafo.agregarNodo(1105, 515, colorNodo);  // Nodo 100
    grafo.agregarNodo(1085, 535, colorNodo);  // Nodo 101
    grafo.agregarNodo(1270, 515, colorNodo);  // Nodo 102
    grafo.agregarNodo(1250, 535, colorNodo);  // Nodo 103
    grafo.agregarNodo(1370, 515, colorNodo);  // Nodo 104
    grafo.agregarNodo(1350, 535, colorNodo);  // Nodo 105
    grafo.agregarNodo(1085, 755, colorNodo);  // Nodo 106
    grafo.agregarNodo(1105, 775, colorNodo);  // Nodo 107
    grafo.agregarNodo(1250, 755, colorNodo);  // Nodo 108
    grafo.agregarNodo(1270, 775, colorNodo);  // Nodo 109
    grafo.agregarNodo(1350, 755, colorNodo);  // Nodo 110
    grafo.agregarNodo(1370, 775, colorNodo);  // Nodo 111
    grafo.agregarNodo(900, 1000, colorNodo);  // Nodo 112
    grafo.agregarNodo(920, 1000, colorNodo);  // Nodo 113
    grafo.agregarNodo(1085, 890, colorNodo);  // Nodo 114
    grafo.agregarNodo(1105, 910, colorNodo);  // Nodo 115
    grafo.agregarNodo(1370, 890, colorNodo);  // Nodo 116
    grafo.agregarNodo(1350, 910, colorNodo);  // Nodo 117
    grafo.agregarNodo(1400, 890, colorNodo);  // Nodo 118
    grafo.agregarNodo(1420, 910, colorNodo);  // Nodo 119
    grafo.agregarNodo(1400, 755, colorNodo);  // Nodo 120
    grafo.agregarNodo(1420, 775, colorNodo);  // Nodo 121
    grafo.agregarNodo(1400, 515, colorNodo);  // Nodo 122
    grafo.agregarNodo(1420, 535, colorNodo);  // Nodo 123
    grafo.agregarNodo(1400, 135, colorNodo);  // Nodo 124
    grafo.agregarNodo(1420, 155, colorNodo);  // Nodo 125
     grafo.agregarNodo(1400, 5, colorNodo);  // Nodo 124
    grafo.agregarNodo(1420, 5, colorNodo);  // Nodo 125

    grafo.agregarArista(124, 126);
    grafo.agregarArista(125, 127); 

    grafo.agregarArista(122, 124);
    grafo.agregarArista(123, 125); 

    grafo.agregarArista(124, 96);
    grafo.agregarArista(125, 97); 

     grafo.agregarArista(104, 122);
    grafo.agregarArista(105, 123); 


    grafo.agregarArista(120, 122);
    grafo.agregarArista(121, 123); 

    grafo.agregarArista(120, 110);
    grafo.agregarArista(121, 111); 

    grafo.agregarArista(118, 120);
    grafo.agregarArista(119, 121); 

    grafo.agregarArista(116, 118);
    grafo.agregarArista(117, 119); 

    grafo.agregarArista(110, 117);
    grafo.agregarArista(111, 116); 
     grafo.agregarArista(92, 114);
    grafo.agregarArista(93, 115); 
     grafo.agregarArista(115, 117);
    grafo.agregarArista(114, 116); 
    grafo.agregarArista(113, 92);
    grafo.agregarArista(112, 93); 
    grafo.agregarArista(110, 108);
    grafo.agregarArista(111, 109); 
    grafo.agregarArista(105, 110);
    grafo.agregarArista(104, 111); 
    grafo.agregarArista(103, 108);
    grafo.agregarArista(102, 109); 
    grafo.agregarArista(108, 106);
    grafo.agregarArista(109, 107); 
    grafo.agregarArista(106, 90);
    grafo.agregarArista(107, 91); 
    grafo.agregarArista(100, 107);
    grafo.agregarArista(101, 106); 
    grafo.agregarArista(102, 104);
    grafo.agregarArista(103, 105); 
    grafo.agregarArista(101, 103);
    grafo.agregarArista(100, 102); 
    grafo.agregarArista(97, 104);
    grafo.agregarArista(96, 105); 
    grafo.agregarArista(99, 100);
    grafo.agregarArista(98, 101); 
    grafo.agregarArista(82, 100);
    grafo.agregarArista(83, 101); 
    grafo.agregarArista(98, 86);
    grafo.agregarArista(99, 87); 
    grafo.agregarArista(96, 98);
    grafo.agregarArista(97, 99); 
    grafo.agregarArista(94, 96);
    grafo.agregarArista(95, 97); 
    grafo.agregarArista(88, 95);
    grafo.agregarArista(89, 94); 
    grafo.agregarArista(90, 82);
    grafo.agregarArista(91, 83); 
    grafo.agregarArista(91, 76);
    grafo.agregarArista(90, 77); 
    grafo.agregarArista(93, 90);
    grafo.agregarArista(92, 91); 
    grafo.agregarArista(82, 86);
    grafo.agregarArista(83, 87); 
    grafo.agregarArista(43, 89); 
    grafo.agregarArista(42, 88);
    grafo.agregarArista(87, 85); 
    grafo.agregarArista(86, 84);
    grafo.agregarArista(86, 88); 
    grafo.agregarArista(87, 89); 
    grafo.agregarArista(78, 72); 
    grafo.agregarArista(79, 73);
    grafo.agregarArista(78, 80); 
    grafo.agregarArista(79, 81);
    grafo.agregarArista(74, 80); 
    grafo.agregarArista(75, 81);  
    grafo.agregarArista(44, 76); 
    grafo.agregarArista(45, 77);
    grafo.agregarArista(76, 78); 
    grafo.agregarArista(77, 79);  
    grafo.agregarArista(69, 74); 
    grafo.agregarArista(68, 75);
    grafo.agregarArista(72, 66); 
    grafo.agregarArista(73, 67);  
    grafo.agregarArista(72, 32); 
    grafo.agregarArista(73, 33);  
    grafo.agregarArista(74, 72); 
    grafo.agregarArista(75, 73);  
    grafo.agregarArista(68, 70); 
    grafo.agregarArista(69, 71);  
    grafo.agregarArista(66, 68); 
    grafo.agregarArista(67, 69);    
    grafo.agregarArista(60, 68); 
    grafo.agregarArista(61, 69);
    grafo.agregarArista(65, 66); 
    grafo.agregarArista(64, 67);    
    grafo.agregarArista(66, 52); 
    grafo.agregarArista(67, 53);
    grafo.agregarArista(50, 64); 
    grafo.agregarArista(51, 65);    
    grafo.agregarArista(60, 62); 
    grafo.agregarArista(61, 63);
    grafo.agregarArista(53, 61); 
    grafo.agregarArista(52, 60);
    grafo.agregarArista(50, 52); 
    grafo.agregarArista(51, 53);
    grafo.agregarArista(53, 55); 
    grafo.agregarArista(52, 54);
    grafo.agregarArista(54, 56); 
    grafo.agregarArista(55, 57);
    grafo.agregarArista(59, 54); 
    grafo.agregarArista(58, 55);
    grafo.agregarArista(0, 1); 
    grafo.agregarArista(1, 3);
    grafo.agregarArista(3, 4);
    grafo.agregarArista(2, 5);
    grafo.agregarArista(4, 6);
    grafo.agregarArista(5, 7);
    grafo.agregarArista(6, 8);
    grafo.agregarArista(7, 9);
    grafo.agregarArista(8, 3);
    grafo.agregarArista(9, 2);
    grafo.agregarArista(8, 10);
    grafo.agregarArista(9, 11);
    grafo.agregarArista(6, 12);
    grafo.agregarArista(7, 13);
    grafo.agregarArista(12, 14);
    grafo.agregarArista(13, 15);
    grafo.agregarArista(14, 8);
    grafo.agregarArista(15, 9);
    grafo.agregarArista(14, 16);
    grafo.agregarArista(15, 17);
    grafo.agregarArista(16, 18);
    grafo.agregarArista(17, 19);
    grafo.agregarArista(18, 20);
    grafo.agregarArista(19, 21);
    grafo.agregarArista(12, 22);
    grafo.agregarArista(13, 23);
    grafo.agregarArista(22, 24);
    grafo.agregarArista(23, 25);
    grafo.agregarArista(24, 14);
    grafo.agregarArista(25, 15);
    grafo.agregarArista(24, 18);
    grafo.agregarArista(25, 19);
    grafo.agregarArista(26, 28);
    grafo.agregarArista(27, 29);
    grafo.agregarArista(18, 29);
    grafo.agregarArista(19, 28);
    grafo.agregarArista(30, 25);
    grafo.agregarArista(31, 24);
    grafo.agregarArista(28, 30);
    grafo.agregarArista(29, 31);
    grafo.agregarArista(30, 32);
    grafo.agregarArista(31, 33);
    grafo.agregarArista(32, 34);
    grafo.agregarArista(33, 35);
    grafo.agregarArista(35, 12);
    grafo.agregarArista(34, 13);
    grafo.agregarArista(34, 36);
    grafo.agregarArista(35, 37);
    grafo.agregarArista(5, 36);
    grafo.agregarArista(4, 37);
    grafo.agregarArista(40, 38);
    grafo.agregarArista(41, 39);
    grafo.agregarArista(43, 85);
    grafo.agregarArista(42, 84);
    grafo.agregarArista(85, 40);
    grafo.agregarArista(84, 41);
    grafo.agregarArista(37, 43);
    grafo.agregarArista(36, 42);
    grafo.agregarArista(38, 44);
     grafo.agregarArista(44, 46);
    grafo.agregarArista(45, 47);
    grafo.agregarArista(46, 31);
    grafo.agregarArista(47, 30);
     grafo.agregarArista(46, 49);
    grafo.agregarArista(47, 48);
    grafo.agregarArista(28, 48);
    grafo.agregarArista(29, 49);
     grafo.agregarArista(38, 44);
    grafo.agregarArista(39, 45);
    grafo.agregarArista(44, 82);
    grafo.agregarArista(45, 83);
   

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
