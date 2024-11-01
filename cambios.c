#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Se inicializan los datos
const char *palos[4] = {"Oro", "Copas", "Bastos", "Espadas"};
int numeros[] = {1, 2, 3, 4, 5, 6, 7, 10, 11, 12}; // Numeros de cartas
int baraja[40] = {0};
int mano_computadora[3] = {0};
int mano_jugador[3] = {0};

// Se define el valor de las cartas esto en base al archivo README.txt
int valor_baraja[40] = {7,6,5,14,13,12,4,10,9,8,7,6,5,14,13,12,11,10,9,8,2,6,5,14,13,12,11,10,9,8,1,6,5,14,13,12,3,10,9,8};

// Funciones que se utilizan en el programa
void repartir();
void juego();
void reiniciar_baraja();
int unica(int carta);
int conocer_carta(int carta);
int validar_opcion(int carta_tirada);
int evitar_doble_carta_jugador(int carta);
int evitar_doble_carta_computadora();

// Funcion principal
int main() {
    srand(time(0));

    printf("\nBienvenido al juego de truco\n");

    repartir();
    
    juego();

    return 0;
}

// Reparte las cartas a cada jugador y los guarda en memoria
void repartir() {
    int carta, palo, numero;

    for (int i = 0; i < 3; i++) {
        do {
            palo = rand() % 4;  // Elegir el palo aleatoriamente (entre 0 y 3)
            numero = rand() % 10;  // Elegir el numero aleatoriamente (entre 0 y 9)
            carta = palo * 10 + numero; // Generar la carta unica en la baraja
        } while (!unica(carta));
        
        mano_computadora[i] = carta;
    }
    
    printf("\nMano del jugador:\n");
    for (int i = 0; i < 3; i++) {
        do {
            palo = rand() % 4;  // Elegir el palo aleatoriamente (entre 0 y 3)
            numero = rand() % 10;  // Elegir el numero aleatoriamente (entre 0 y 9)
            carta = palo * 10 + numero; // Generar la carta unica en la baraja
        } while (!unica(carta));
        
        mano_jugador[i] = carta;
        printf("Carta %d: %d de %s\n", i + 1, numeros[numero], palos[palo]);
    }
}

// En esta funcion se juega al juego Truco y se define al ganador del juego
void juego() {
    // Se saca de la mano del jugador, la carta que se va a jugar en ese momento
    int tirar_carta, carta_jugador, carta_computadora;
    // Son puntos parciales para describir al ganador de la ronda
    int punto_jugador = 0, punto_computadora = 0;
    // Son los puntos definitivos para que uno gane
    int punto_definitivo_jugador = 0, punto_definitivo_computadora = 0;
    // Bandera para saber si hubo empate en la primera ronda
    int bandera_empate = 0;

    //  Es el ciclo para que el juego se juegue hasta que uno de los jugadores llegue a 15 puntos
    while (punto_definitivo_jugador  < 15 && punto_definitivo_computadora < 15) {    
        // En este ciclo se juegan las cartas y se define al ganador de la mano
        for (int i = 0; i < 3; i++) {
            // Evita que la computadora tire la misma carta mas de una vez
            int indice_carta_computadora = evitar_doble_carta_computadora();
            carta_computadora = mano_computadora[indice_carta_computadora];

            // El jugador tira su carta elegida
            printf("\nQue carta desea tirar (1, 2 o 3): ");
            scanf("%d", &tirar_carta);
            tirar_carta = validar_opcion(tirar_carta);
            tirar_carta = evitar_doble_carta_jugador(tirar_carta);
            carta_jugador = mano_jugador[tirar_carta];

            printf("Carta tirada por el jugador: ");
            conocer_carta(carta_jugador);

            printf("Carta tirada por la computadora: ");
            conocer_carta(carta_computadora);

            // Se define a la mano ganadora o si hubo empate
            if (valor_baraja[carta_computadora] > valor_baraja[carta_jugador]) {
                printf("\nLa carta del jugador gana\n");
                punto_jugador += 1;
            } else if (valor_baraja[carta_jugador] > valor_baraja[carta_computadora]){
                printf("\nLa carta de la computadora gana\n");
                punto_computadora += 1;
            } else {
                printf("\nEmpate\n");
                bandera_empate = 1;
            }

            // Cambiar los valores para no tirar la misma carta dos veces
            mano_computadora[indice_carta_computadora] = -1;
            mano_jugador[tirar_carta] = -1;

            // Se define al ganador de la ronda
            if (punto_computadora > punto_jugador) {
                punto_definitivo_computadora += 1;
            } else if (punto_jugador > punto_computadora) {
                punto_definitivo_jugador += 1;
            }

            // Se reinicia el valor de los datos para volver a usarlos
            punto_jugador = 0;
            punto_computadora = 0;
        }

        // Se muestra en pantalla los puntos de cada jugador
        printf("\n***** Fin de la mano *****\n");
        printf("Puntos del jugador: %d\n", punto_definitivo_jugador);
        printf("Puntos de la computadora: %d\n", punto_definitivo_computadora);
        
        reiniciar_baraja();
        repartir();
    }   
}  

// Se evita que se repitan las cartas de la baraja
int unica(int carta) {
    if (baraja[carta] == 1) {
        return 0;
    } else {
        baraja[carta] = 1;
        return 1;
    }
}

// Se muestra en pantalla la carta que se tiro
int conocer_carta(int carta) {
    int palo = carta / 10; 
    int numero = carta % 10;
    printf("%d de %s\n", numeros[numero], palos[palo]);
    return 0;
}

// Prevee que solo se elija una de las 3 cartas de la mano del jugador
int validar_opcion(int carta_tirada) {
    while (carta_tirada < 1 || carta_tirada > 3) {
        printf("\nElija una opcion valida entre 1 y 3: ");
        scanf("%d", &carta_tirada);
    }
    return carta_tirada - 1;
}

// Evita que el jugador tire 2 veces la misma carta de la mano
int evitar_doble_carta_jugador(int carta) {
    while (mano_jugador[carta] == -1) {
        printf("La carta ya fue usada, elija otra: ");
        scanf("%d", &carta);
        carta = validar_opcion(carta);
    }
    return carta;
}

// Evita que el computadora tire 2 veces la misma carta de la mano
int evitar_doble_carta_computadora() {
    int carta;
    do {
        carta = rand() % 3;
    } while (mano_computadora[carta] == -1);
    return carta;
}

//* crear funcion para cantar truco, retruco, vale cuatro

//* crear funcion para cantar envido, real envido y falta envido

