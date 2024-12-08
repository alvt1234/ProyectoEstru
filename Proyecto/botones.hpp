#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> //para las imagenes png
#include <iostream>
#include "Carro.hpp"
#include "Semaforo.hpp"
#include "grafo.hpp"
extern bool enPausALosCARROS;

class botones {
public:

    //bool enPausALosCARROS = false;
    //botones de calles para ruta
    
    SDL_Texture * botonRuta1;
    SDL_Texture * botonRuta2;
    SDL_Texture * botonRuta3;
    SDL_Texture * botonRuta4;
    SDL_Texture * botonRuta5;
    SDL_Rect botonRuta1Rect;
    SDL_Rect botonRuta2Rect;
    SDL_Rect botonRuta3Rect;
    SDL_Rect botonRuta4Rect;
    SDL_Rect botonRuta5Rect;
    bool botonRutaClickead = false;
    bool botonRuta2Clickeado = false;
    bool botonRuta3Clickeado = false;
    bool botonRuta4Clickeado = false;
    bool botonRuta5Clickeado = false;
// demas botones
//texturas
    SDL_Texture * start;
    SDL_Texture * lluvia;
    SDL_Texture * niebla;
    SDL_Texture * agregar;
    SDL_Texture * ruta;
    SDL_Texture * atras;
    //rectangulos
    SDL_Rect nieblaRect;
    SDL_Rect startRect;
    SDL_Rect lluviaRect;
    SDL_Rect rutaRect;
    SDL_Rect agregarRect;
    SDL_Rect atrasRect;

    bool atrasClickeado = false;
    bool agregarClickeado = false;
    bool lluviaClickeado = false;
    bool nieblaClickeado = false;
    bool startClickeado = false;
    bool rutaClickeado = false;
    bool mostrarBotonesRuta = false; 
    bool carroCreado = false; 
    bool puntoInicioSeleccionado = false;
    bool puntoDestinoSeleccionado = false;
    
    

    int estadoCarro = 0;  // 0 para primer carro, 1 para segundo carro
    SDL_Texture* carro = NULL;  // Textura del carro
    SDL_Rect carroRect;  // Rectángulo para el carro
    Uint8 fondoR = 100, fondoG = 149, fondoB = 237; 
    bool lloviendo = false;
    bool neblina = false;
    int puntoInicio = -1;  
    int puntoDestino = -1; 
    Grafo grafo;
    int seleccionarNodoDestino(int mouseX, int mouseY);

    botones(SDL_Renderer* renderer);
    void dibujarBotones(SDL_Renderer* renderer);
    void actualizarBotones(int mouseX, int mouseY, bool clicIzquierdo, SDL_Renderer* renderer, std::vector<Carro>& carros);
    void cambiarCarro(SDL_Renderer* renderer);  // Función para cambiar el carro
};

botones::botones(SDL_Renderer* renderer) {
    startRect = {1700, 300, 200, 300}; // Posición y tamaño del botón
    lluviaRect = {1800, 120, 70, 100}; // Posición y tamaño del botón
    nieblaRect = {1800, 220, 70, 100}; // Posición y tamaño del botón
    agregarRect = {1800, 800, 80, 80}; // Posición y tamaño del botón
    rutaRect = {1800, 700, 80,80}; // posicion y tamaño 
    botonRuta1Rect = {0, 450 , 40 , 40}; 
    botonRuta2Rect = {770, 0 , 40 , 40}; 
    botonRuta3Rect = {720, 980 , 40 , 40}; 
    botonRuta4Rect = {900, 0 , 40 , 40}; 
    botonRuta5Rect = {1400, 980 , 40 , 40}; 
    atrasRect = {1800, 900, 80, 80}; 

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

    //AGREGAR
    SDL_Surface* tempSurfaceAgregar = SDL_LoadBMP("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/agregar.bmp");
    if (!tempSurfaceAgregar) {
        fprintf(stderr, "Error al cargar la imagen del botón agregar: %s\n", SDL_GetError());
        exit(1);
    }

    //AGREGAR
    agregar = SDL_CreateTextureFromSurface(renderer, tempSurfaceAgregar);
    SDL_FreeSurface(tempSurfaceAgregar);
    if (!agregar) {
        fprintf(stderr, "Error al crear la textura del botón agregar: %s\n", SDL_GetError());
        exit(1);
    }

    //RUTA
    SDL_Surface* tempSurfaceRuta = SDL_LoadBMP("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/ruta.bmp");
    if(!tempSurfaceRuta){
        fprintf(stderr, "ERROR AL cargar el boton de la ruta: %s\n", SDL_GetError());
        exit(1);    
    }

    //RUTA

    ruta = SDL_CreateTextureFromSurface(renderer, tempSurfaceRuta);
    SDL_FreeSurface(tempSurfaceRuta);
    if(!ruta){
        fprintf(stderr, "error al crear la textura del boton ruta: %s\n", SDL_GetError());
        exit(1);
    }

    //BOTONES PARA LA RUTA
    //boton1
    SDL_Surface * tempSurfaceBotonRuta1 = SDL_LoadBMP("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/numero1.bmp");
    if(!tempSurfaceBotonRuta1){
        fprintf(stderr, "ERROR AL cargar el boton de la ruta: %s\n", SDL_GetError());
        exit(1);    
    }

    botonRuta1 = SDL_CreateTextureFromSurface(renderer, tempSurfaceBotonRuta1);
    SDL_FreeSurface(tempSurfaceBotonRuta1);
    if(!botonRuta1){
        fprintf(stderr, "error al crear la textura del boton ruta: %s\n", SDL_GetError());
        exit(1);
    }
    //boton2

     SDL_Surface * tempSurfaceBotonRuta2 = SDL_LoadBMP("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/numero2.bmp");
    if(!tempSurfaceBotonRuta2){
        fprintf(stderr, "ERROR AL cargar el boton de la ruta: %s\n", SDL_GetError());
        exit(1);    
    }
    botonRuta2 = SDL_CreateTextureFromSurface(renderer, tempSurfaceBotonRuta2);
    SDL_FreeSurface(tempSurfaceBotonRuta2);
    if(!botonRuta2){
        fprintf(stderr, "error al crear la textura del boton ruta: %s\n", SDL_GetError());
        exit(1);
    }

    //boton3
     SDL_Surface * tempSurfaceBotonRuta3 = SDL_LoadBMP("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/numero3.bmp");
    if(!tempSurfaceBotonRuta3){
        fprintf(stderr, "ERROR AL cargar el boton de la ruta: %s\n", SDL_GetError());
        exit(1);    
    }
    botonRuta3 = SDL_CreateTextureFromSurface(renderer, tempSurfaceBotonRuta3);
    SDL_FreeSurface(tempSurfaceBotonRuta3);
    if(!botonRuta3){
        fprintf(stderr, "error al crear la textura del boton ruta: %s\n", SDL_GetError());
        exit(1);
    }

    //boton4
    SDL_Surface * tempSurfaceBotonRuta4 = SDL_LoadBMP("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/numero4.bmp");
    if(!tempSurfaceBotonRuta4){
        fprintf(stderr, "ERROR AL cargar el boton de la ruta: %s\n", SDL_GetError());
        exit(1);    
    }
    botonRuta4 = SDL_CreateTextureFromSurface(renderer, tempSurfaceBotonRuta4);
    SDL_FreeSurface(tempSurfaceBotonRuta4);
    if(!botonRuta4){
        fprintf(stderr, "error al crear la textura del boton ruta: %s\n", SDL_GetError());
        exit(1);
    }

    //boton5
      SDL_Surface * tempSurfaceBotonRuta5 = SDL_LoadBMP("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/numero5.bmp");
    if(!tempSurfaceBotonRuta5){
        fprintf(stderr, "ERROR AL cargar el boton de la ruta: %s\n", SDL_GetError());
        exit(1);    
    }
    botonRuta5 = SDL_CreateTextureFromSurface(renderer, tempSurfaceBotonRuta5);
    SDL_FreeSurface(tempSurfaceBotonRuta5);
    if(!botonRuta5){
        fprintf(stderr, "error al crear la textura del boton ruta: %s\n", SDL_GetError());
        exit(1);
    }

    //atras cargar
    SDL_Surface * tempSurfaceAtras = SDL_LoadBMP("/home/anareyes/Documentos/GitHub/ProyectoEstru/Proyecto/img/atras.bmp");
    if(!tempSurfaceAtras){
        fprintf(stderr, "ERROR AL cargar el boton de la ruta: %s\n", SDL_GetError());
        exit(1);    
    }
    //atras textura
    atras = SDL_CreateTextureFromSurface(renderer, tempSurfaceAtras);
    SDL_FreeSurface(tempSurfaceAtras);
    if(!atras){
        fprintf(stderr, "error al crear la textura del boton ruta: %s\n", SDL_GetError());
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

//HACERLO SIEMPRE PARA QUE SE MIREN
void botones::dibujarBotones(SDL_Renderer* renderer) 
{
    if (mostrarBotonesRuta) {
        // Dibujar botones de ruta
        SDL_RenderCopy(renderer, botonRuta1, NULL, &botonRuta1Rect);
        SDL_RenderCopy(renderer, botonRuta2, NULL, &botonRuta2Rect);
        SDL_RenderCopy(renderer, botonRuta3, NULL, &botonRuta3Rect);
        SDL_RenderCopy(renderer, botonRuta4, NULL, &botonRuta4Rect);
        SDL_RenderCopy(renderer, botonRuta5, NULL, &botonRuta5Rect);
        SDL_RenderCopy(renderer, atras, NULL, &atrasRect);
    } else {
        // Dibujar los demás botones
        SDL_RenderCopy(renderer, start, NULL, &startRect);
        SDL_RenderCopy(renderer, lluvia, NULL, &lluviaRect);
        SDL_RenderCopy(renderer, niebla, NULL, &nieblaRect);
        SDL_RenderCopy(renderer, agregar, NULL, &agregarRect);
        SDL_RenderCopy(renderer, ruta, NULL, &rutaRect);
        SDL_RenderCopy(renderer, atras, NULL, &atrasRect);
    }

    if (carro) {
        SDL_RenderCopy(renderer, carro, NULL, &carroRect);
    }

}
    int botones::seleccionarNodoDestino(int mouseX, int mouseY) {
    
    // BotónRuta1
    if (mouseX > botonRuta1Rect.x && mouseX < (botonRuta1Rect.x + botonRuta1Rect.w) &&
        mouseY > botonRuta1Rect.y && mouseY < (botonRuta1Rect.y + botonRuta1Rect.h)) {
        return 44; //{0, 455}
    }

    // BotónRuta2
    if (mouseX > botonRuta2Rect.x && mouseX < (botonRuta2Rect.x + botonRuta2Rect.w) &&
        mouseY > botonRuta2Rect.y && mouseY < (botonRuta2Rect.y + botonRuta2Rect.h)) {
        return 50; //{725, 5}
    }

    // Botón Ruta3
    if (mouseX > botonRuta3Rect.x && mouseX < (botonRuta3Rect.x + botonRuta3Rect.w) &&
        mouseY > botonRuta3Rect.y && mouseY < (botonRuta3Rect.y + botonRuta3Rect.h)) {
        return 1; //{725, 890}
    }

    // BotónRuta4
    if (mouseX > botonRuta4Rect.x && mouseX < (botonRuta4Rect.x + botonRuta4Rect.w) &&
        mouseY > botonRuta4Rect.y && mouseY < (botonRuta4Rect.y + botonRuta4Rect.h)) {
        return 1; // {900, 5}
    }

    // BotónRuta5
    if (mouseX > botonRuta5Rect.x && mouseX < (botonRuta5Rect.x + botonRuta5Rect.w) &&
        mouseY > botonRuta5Rect.y && mouseY < (botonRuta5Rect.y + botonRuta5Rect.h)) {
        return 1; //{1430, 910}
    }

    return -1;  
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
    // Botón de agregar
    if (mouseX > agregarRect.x && mouseX < (agregarRect.x + agregarRect.w) &&
    mouseY > agregarRect.y && mouseY < (agregarRect.y + agregarRect.h)) {
        if (clicIzquierdo) {
        agregarClickeado = true;
        std::cout << "Botón de agregar clickeado" << std::endl;
        SDL_Color color = {255, 165, 0, 255}; 
        carros.push_back(Carro(770 - 100 / 2, 0, 30, 50, 'V', 2, false, color));
        }
    } else {
    agregarClickeado = false;
    }

    //BOTON RUTA

    if (mouseX > rutaRect.x && mouseX < (rutaRect.x + rutaRect.w) &&
    mouseY > rutaRect.y && mouseY < (rutaRect.y + rutaRect.h)) {
    if (clicIzquierdo) {
        rutaClickeado = true;
        mostrarBotonesRuta = !mostrarBotonesRuta; 
        enPausALosCARROS = !enPausALosCARROS;
        std::cout << "Botón de ruta clickeado" << std::endl;
    }
    } else 
    {
    rutaClickeado = false;
    }

    //botones de ruta1,2,...
    if (mouseX > botonRuta1Rect.x && mouseX < botonRuta1Rect.x + botonRuta1Rect.w &&
        mouseY > botonRuta1Rect.y && mouseY < botonRuta1Rect.y + botonRuta1Rect.h) {
            if (clicIzquierdo) {
                if (!carroCreado) {
                    puntoInicio = 26;  // Nodo de inicio específico
                    std::cout << "Punto de inicio seleccionado: Nodo {0, 455}" << std::endl;

                    // Crear y agregar el carro
                    SDL_Color color = {255, 0, 255}; // Color del carro
                    carros.push_back(Carro(0, 455, 30, 50, 'V', 2, false, color));
                    std::cout << "Carro agregado en Nodo {0, 455}" << std::endl;
                    carroCreado = true;  // Marcar carro como creado
                } else if (!puntoDestinoSeleccionado) {
                    // Selección de punto destino basado en el botón presionado
                    puntoDestino = seleccionarNodoDestino(mouseX, mouseY);
                    puntoDestinoSeleccionado = true;
                    std::cout << "Punto de destino seleccionado: Nodo " << puntoDestino << std::endl;
                }
            }
        }

        // Botón Ruta 2
        if (mouseX > botonRuta2Rect.x && mouseX < (botonRuta2Rect.x + botonRuta2Rect.w) &&
            mouseY > botonRuta2Rect.y && mouseY < (botonRuta2Rect.y + botonRuta2Rect.h)) {
            if (clicIzquierdo) {
                if (!carroCreado) {
                    puntoInicio = 32;  // Nodo de inicio específico
                    std::cout << "Punto de inicio seleccionado: Nodo {725, 5}" << std::endl;

                    // Crear y agregar el carro
                    SDL_Color color = {255, 0, 255}; // Color del carro
                    carros.push_back(Carro(725, 5, 30, 50, 'V', 2, false, color));
                    std::cout << "Carro agregado en Nodo {725, 5}" << std::endl;
                    carroCreado = true;
                } else if (!puntoDestinoSeleccionado) {
                    puntoDestino = seleccionarNodoDestino(mouseX, mouseY);
                    puntoDestinoSeleccionado = true;
                    std::cout << "Punto de destino seleccionado: Nodo " << puntoDestino << std::endl;
                }
            }
        }

        // Botón Ruta 3
        if (mouseX > botonRuta3Rect.x && mouseX < (botonRuta3Rect.x + botonRuta3Rect.w) &&
            mouseY > botonRuta3Rect.y && mouseY < (botonRuta3Rect.y + botonRuta3Rect.h)) {
            if (clicIzquierdo) {
                if (!carroCreado) {
                    puntoInicio = 50;  // Nodo de inicio específico
                    std::cout << "Punto de inicio seleccionado: Nodo {725, 890}" << std::endl;

                    // Crear y agregar el carro
                    SDL_Color color = {255, 0, 255}; // Color del carro
                    carros.push_back(Carro(725, 890, 30, 50, 'V', 2, false, color));
                    std::cout << "Carro agregado en Nodo {725, 890}" << std::endl;
                    carroCreado = true;
                } else if (!puntoDestinoSeleccionado) {
                    puntoDestino = seleccionarNodoDestino(mouseX, mouseY);
                    puntoDestinoSeleccionado = true;
                    std::cout << "Punto de destino seleccionado: Nodo " << puntoDestino << std::endl;
                }
            }
        }

// Botón Ruta 4
if (mouseX > botonRuta4Rect.x && mouseX < (botonRuta4Rect.x + botonRuta4Rect.w) &&
    mouseY > botonRuta4Rect.y && mouseY < (botonRuta4Rect.y + botonRuta4Rect.h)) {
    if (clicIzquierdo) {
        if (!carroCreado) {
            puntoInicio = 1;
            std::cout << "Punto de inicio seleccionado: Ruta 4" << std::endl;

            // Crear y agregar un nuevo carro en el punto de inicio
            SDL_Color color = {255, 0, 255}; // Color del carro
            carros.push_back(Carro(900, 5, 30, 50, 'V', 2, false, color));  // Coordenadas de inicio
            std::cout << "Carro agregado en Ruta 4" << std::endl;
            carroCreado = true;  // Marcar que el carro ha sido creado
        } else if (!puntoDestinoSeleccionado) {
            puntoDestino = 1; 
           puntoDestino = seleccionarNodoDestino(mouseX, mouseY);
                    puntoDestinoSeleccionado = true;
                    std::cout << "Punto de destino seleccionado: Nodo " << puntoDestino << std::endl;
        }
    }
}

// Botón Ruta 5
if (mouseX > botonRuta5Rect.x && mouseX < (botonRuta5Rect.x + botonRuta5Rect.w) &&
    mouseY > botonRuta5Rect.y && mouseY < (botonRuta5Rect.y + botonRuta5Rect.h)) {
    if (clicIzquierdo) {
        if (!carroCreado) {
            puntoInicio = 1; // Ruta 2 como punto de inicio
            std::cout << "Punto de inicio seleccionado: Ruta 5" << std::endl;
            SDL_Color color = {255, 0, 255}; // Color del carro
            carros.push_back(Carro(1430, 90, 30, 50, 'V', 2, false, color));  // Coordenadas de inicio
            std::cout << "Carro agregado en Ruta 5" << std::endl;
            carroCreado = true;  // Marcar que el carro ha sido creado
        } else if (!puntoDestinoSeleccionado) {
        puntoDestino = seleccionarNodoDestino(mouseX, mouseY);
        puntoDestinoSeleccionado = true;
    }
}


        if (puntoInicioSeleccionado && puntoDestinoSeleccionado) {
    std::vector<int> ruta = grafo.dijkstra(puntoInicio, puntoDestino);

    if (!ruta.empty()) {
        std::cout << "Ruta encontrada: ";
        for (int nodo : ruta) {
            std::cout << nodo << " ";
        }
        std::cout << std::endl;

        // Asignar ruta al carro
        for (auto& carro : carros) {
            carro.establecerRuta(ruta);  // Establecer la ruta para el carro
        }
    } else {
        std::cout << "No se encontró una ruta válida." << std::endl;
    }

    // Resetear después de asignar la ruta
    puntoInicio = -1;
    puntoDestino = -1;
    puntoInicioSeleccionado = false;
    puntoDestinoSeleccionado = false;
                                        }
} 



//boton atras
if (mouseX > atrasRect.x && mouseX < (atrasRect.x + atrasRect.w) &&
    mouseY > atrasRect.y && mouseY < (atrasRect.y + atrasRect.h)) {
    if (clicIzquierdo) {
        atrasClickeado = true;
        mostrarBotonesRuta = !mostrarBotonesRuta; 
        enPausALosCARROS = !enPausALosCARROS;
    }
} else {
    atrasClickeado = false;
}


}

