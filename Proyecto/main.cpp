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
std::vector<Semaforo> semaforos;

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


void actualizarSemaforos() {
    for (auto& semaforo : semaforos) {
        semaforo.actualizarEstado();
    }
}

void dibujarSemaforos(SDL_Renderer* renderer) {
    for (const auto& semaforo : semaforos) {
        semaforo.dibujar(renderer);
    }
}

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

   void dibujarCarro() {
    SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255); // Color morado
    SDL_RenderFillRect(renderer, &carRect);
}


    // Función para dibujar el mapa (calles, edificios, etc.)
    void dibujarMapa(botones& boton) {
        SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255); // Azul claro
        calles();
        actualizarSemaforos();  // Actualiza el estado de los semáforos
        dibujarSemaforos(renderer);  // Dibuja los semáforos
        if (boton.startClickeado) {
            dibujarCarro();
            //grafo.dibujar();
        }

        boton.dibujarBotones(renderer);
        
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
        
        semaforos.push_back(Semaforo(210, 270)); //no tocar

        dibujarCalleRecta(0, 400 - anchoCalleEstrecha / 2, 670, anchoCalleEstrecha, true); //horizonta pedro
        dibujarCalleRecta(650 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 400, false); //calle #3 vertical arriba bajada
        semaforos.push_back(Semaforo(690, 390));  // no tocra

        dibujarCalleRecta(0, 100 - anchoCalleEstrecha / 2, 647, anchoCalleEstrecha, true);//calle #2 horizontal curacao
        dibujarCalleRecta(400 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 450, false); //calle #2 vertical Rosi
        //----
        dibujarCalleRecta(770 - anchoCalleEstrecha / 2, 0, anchoCalleEstrecha, 400, false);  // Calle principal3 estrecha
        dibujarCalleRecta(800, 750 - anchoCalleEstrecha / 20, 650, anchoCalleEstrecha, true); //calle alli
        dibujarCalleRecta(1350 - anchoCalleAncha / 150, 0, anchoCalleAncha, 1920, false); // vertical doble segunda
        dibujarCalleRecta(920 - anchoCalleAncha /5,0, anchoCalleEstrecha,1100, false); //calle vertical palo alto
        dibujarCalleRecta(0, 2 - anchoCalleEstrecha/ 50, 1350, anchoCalleEstrecha, true); //calle horizontal arriba principal
        dibujarCalleRecta(1100 - anchoCalleAncha /5,500, anchoCalleEstrecha,250, false); //calle barrial
        dibujarCalleRecta(1100 - anchoCalleAncha /5,150, anchoCalleEstrecha,350, false); //calle Guamilito
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
        dibujarCalleRecta(770 - anchoCalleAncha / 2, 0, anchoCalleAncha, 1100, false);  // Calle doble carril vertical */
        // Semáforos 
       
}

    
void inicializarSemaforos(){
 semaforos.push_back(Semaforo(425, 120)); // Intersección Curacao y Rosi
        semaforos.push_back(Semaforo(425, 320)); // Intersección Rosi y Principal 3
        semaforos.push_back(Semaforo(870, 445)); // Intersección Principal 3 y Sofia
        semaforos.push_back(Semaforo(875, 788)); // Intersección Alli y Roma
        //semaforos.push_back(Semaforo(1350, 620)); // Intersección Doble carril segunda y San Marcos
        semaforos.push_back(Semaforo(945, 43)); // Intersección Horizontal arriba principal y Principal 3
        semaforos.push_back(Semaforo(1120, 545)); // Intersección Barrial y Guamilito
        semaforos.push_back(Semaforo(1330, 445)); //no se cual era :()
        semaforos.push_back(Semaforo(605, 640)); // Intersección Comercio y San Marcos
        //semaforos.push_back(Semaforo(0, 620)); // Intersección Comercio y San Marcos
        semaforos.push_back(Semaforo(325, 740)); // Intersección Morazan y San Marcos
        semaforos.push_back(Semaforo(120, 840)); // Intersección Constantinopla y Rosario
        //semaforos.push_back(Semaforo(170, 820)); // Intersección Rosario y Real
        semaforos.push_back(Semaforo(330, 920)); // Intersección Real y Comercio
        semaforos.push_back(Semaforo(1200, 920)); // Intersección Roma y Principal 3*/
}





int escalaX(int x) {
    return (x - 100) * 1280 / (300 - 100);  
}

int escalaY(int y) {
    return (y - 65) * 720 / (300 - 100);  
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
void detectarInterseccionYDecidir(Carro& carro, const Grafo& grafo) {
    for (const auto& nodo : grafo.getNodos()) {
        // Detecta si el carro está cerca de un nodo
        if (std::abs(carro.rect.x - nodo.x) < 5 && std::abs(carro.rect.y - nodo.y) < 5) {
            carro.girarHaciaArista(grafo);  // Decidir nueva dirección aleatoria
            break;
        }
    }
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

    // Lista de coordenadas de los nodos (x, y)
    std::vector<std::pair<int, int>> nodos = {
        {0, 5}, {0, 28}, {380, 5}, {400, 28}, {630, 28}, {650, 5},
        {630, 85}, {650, 105}, {400, 85}, {380, 105}, {0, 85}, {0, 105},
        {630, 235}, {650, 255}, {400, 235}, {380, 255}, {165, 235}, {185, 255},
        {165, 385}, {185, 405}, {0, 385}, {0, 405}, {630, 385}, {650, 405},
        {400, 385}, {380, 405}, {0, 455}, {0, 475}, {185, 455}, {165, 475},
        {380, 455}, {400, 475}, {725, 455}, {745, 475}, {725, 255}, {745, 235},
        {725, 5}, {745, 28}, {770, 455}, {790, 475}, {770, 255}, {790, 235},
        {790, 5}, {770, 28}, {770, 515}, {790, 535}, {400, 515}, {380, 535},
        {185, 535}, {165, 515}, {295, 605}, {315, 625}, {295, 805}, {315, 825},
        {180, 805}, {160, 825}, {0, 805}, {0, 825}, {160, 1000}, {180, 1000},
        {295, 910}, {315, 890}, {295, 1000}, {315, 1000}, {660, 605}, {640, 625},
        {640, 805}, {660, 825}, {640, 910}, {660, 890}, {640, 1000}, {660, 1000},
        {725, 805}, {745, 825}, {725, 890}, {745, 910}, {770, 775}, {790, 755},
        {770, 805}, {790, 825}, {770, 890}, {790, 910}, {900, 515}, {920, 535},
        {790, 135}, {770, 155}, {900, 135}, {920, 155}, {900, 5}, {920, 28},
        {900, 755}, {920, 775}, {920, 890}, {900, 910}, {1350, 28}, {1370, 5},
        {1350, 135}, {1370, 155}, {1085, 135}, {1105, 155}, {1105, 515}, {1085, 535},
        {1270, 515}, {1250, 535}, {1370, 515}, {1350, 535}, {1085, 755}, {1105, 775},
        {1250, 755}, {1270, 775}, {1350, 755}, {1370, 775}, {900, 1000}, {920, 1000},
        {1085, 890}, {1105, 910}, {1370, 890}, {1350, 910}, {1400, 890}, {1420, 910},
        {1400, 755}, {1420, 775}, {1400, 515}, {1420, 535}, {1400, 135}, {1420, 155}, 
        {1400, 5}, {1420, 5}, {0,535}, {0,515}
    };

    // Agregar los nodos al grafo
    for (size_t i = 0; i < nodos.size(); ++i) {
        grafo.agregarNodo(nodos[i].first, nodos[i].second, colorNodo);
    }

    // Lista de aristas (pares de nodos conectados)
    std::vector<std::pair<int, int>> aristas = {
       {124, 126}, {125, 127}, {122, 124}, {123, 125}, {124, 96}, {125, 97},
        {104, 122}, {105, 123}, {120, 122}, {121, 123}, {120, 110}, {121, 111},
        {118, 120}, {119, 121}, {116, 118}, {117, 119}, {110, 117}, {111, 116},
        {92, 114}, {93, 115}, {115, 117}, {114, 116}, {113, 92}, {112, 93},
        {110, 108}, {111, 109}, {105, 110}, {104, 111}, {103, 108}, {102, 109},
        {108, 106}, {109, 107}, {106, 90}, {107, 91}, {100, 107}, {101, 106},
        {102, 104}, {103, 105}, {101, 103}, {100, 102}, {97, 104}, {96, 105},
        {99, 100}, {98, 101}, {82, 100}, {83, 101}, {98, 86}, {99, 87},
        {96, 98}, {97, 99}, {94, 96}, {95, 97}, {88, 95}, {89, 94}, {90, 82},
        {91, 83}, {91, 76}, {90, 77}, {93, 90}, {92, 91}, {82, 86}, {83, 87},
        {43, 89}, {42, 88}, {87, 85}, {86, 84}, {86, 88}, {87, 89}, {78, 72},
        {79, 73}, {78, 80}, {79, 81}, {74, 80}, {75, 81}, {44, 76}, {45, 77},
        {76, 78}, {77, 79}, {69, 74}, {68, 75}, {72, 66}, {73, 67}, {72, 32},
        {73, 33}, {74, 72}, {75, 73}, {68, 70}, {69, 71}, {66, 68}, {67, 69},
        {60, 68}, {61, 69}, {65, 66}, {64, 67}, {66, 52}, {67, 53}, {50, 64},
        {51, 65}, {60, 62}, {61, 63}, {53, 61}, {52, 60}, {50, 52}, {51, 53},
        {53, 55}, {52, 54}, {54, 56}, {55, 57}, {59, 54}, {58, 55}, {0, 1},
        {1, 3}, {3, 4}, {2, 5}, {4, 6}, {5, 7}, {6, 8}, {7, 9}, {8, 3}, {9, 2},
        {8, 10}, {9, 11}, {6, 12}, {7, 13}, {12, 14}, {13, 15}, {14, 8}, {15, 9},
        {14, 16}, {15, 17}, {16, 18}, {17, 19}, {18, 20}, {19, 21}, {12, 22},
        {13, 23}, {22, 24}, {23, 25}, {24, 14}, {25, 15}, {24, 18}, {25, 19},
        {26, 28}, {27, 29}, {18, 29}, {19, 28}, {30, 25}, {31, 24}, {28, 30},
        {29, 31}, {30, 32}, {31, 33}, {32, 34}, {33, 35}, {35, 12}, {34, 13},
        {34, 36}, {35, 37}, {5, 36}, {4, 37}, {40, 38}, {41, 39}, {43, 85},
        {42, 84}, {85, 40}, {84, 41}, {37, 43}, {36, 42}, {38, 44}, {44, 46},
        {45, 47}, {46, 31}, {47, 30}, {46, 49}, {47, 48}, {28, 48}, {29, 49},
        {38, 44}, {39, 45}, {44, 82}, {45, 83}, {49, 129}, {48,128}
    };
    // Agregar las aristas al grafo
    for (const auto& arista : aristas) {
        grafo.agregarArista(arista.first, arista.second);
    }
}



int main(int argc, char *args[]) {
    iniciar();
    crearPantalla();
    Grafo grafo;
    inicializarGrafo(grafo);
    inicializarSemaforos();
    inicializarCarros();  // Inicializa varios carros
    botones boton(renderer);
    bool corriendo = true;
    SDL_Event e;

    while (corriendo) {
        eventos(e, corriendo, boton, renderer);

        SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255); // Azul cornflower

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
