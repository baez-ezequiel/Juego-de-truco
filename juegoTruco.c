#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void repartir() {
    int carta, palo, numero;

    for (int i = 0; i < 3; i++) {
        do {
            palo = rand() % 4; 
            numero = rand() % 10;
            carta = palo * 10 + numero;
        } while (!unica(carta));
        
        mano_computadora[i] = carta;
    }
    
    printf("\nMano del jugador:\n");
    for (int i = 0; i < 3; i++) {
        do {
            palo = rand() % 4;
            numero = rand() % 10;
            carta = palo * 10 + numero;
        } while (!unica(carta));
        
        mano_jugador[i] = carta;
        printf("Carta %d: %d de %s\n", i + 1, numeros[numero], palos[palo]);
    }
}

void juego() {
    int punto_computadora = 0, punto_jugador = 0; // Puntos acumulados para ganar el juego
    int punto_mano_jugador, punto_mano_computadora; // Puntos en cada mano
    int carta_computadora, carta_jugador; // Carta de cada jugador
    int carta_tirada; // Carta que tira el jugador
    int bandera_empate = 0;

    while (punto_computadora < 15 && punto_jugador < 15) {
        // Reiniciamos los puntos de la mano
        punto_mano_jugador = 0;
        punto_mano_computadora = 0;

        //! Primera mano
        // El jugador tira su carta
        printf("\nQue carta desea tirar (1, 2 o 3): ");
        scanf("%d", &carta_tirada);
        carta_tirada = validar_opcion(carta_tirada);
        carta_tirada = evitar_doble_carta_jugador(carta_tirada);
        carta_jugador = mano_jugador[carta_tirada];

        // Mostrar carta del jugador
        printf("Carta tirada por el jugador: ");
        conocer_carta(carta_jugador);

        // Computadora responde al jugador
        int indice_carta_computadora = evitar_doble_carta_computadora();
        carta_computadora = mano_computadora[indice_carta_computadora];

        // Mostrar carta de la computadora
        printf("Carta tirada por la computadora: ");
        conocer_carta(carta_computadora);


        // Definir ganador de la mano
        if (valor_baraja[carta_computadora] < valor_baraja[carta_jugador]) {
            printf("\nLa computadora gana esta mano\n");
            punto_mano_computadora++;
        } else if (valor_baraja[carta_jugador] < valor_baraja[carta_computadora]) {
            printf("\nEl jugador gana esta mano\n");
            punto_mano_jugador++;
        } else {
            printf("\nEmpate en esta mano\n");
        }

        // Cambiar valores para no repetir cartas
        mano_computadora[indice_carta_computadora] = -1;
        mano_jugador[carta_tirada] = -1;

        //! Esta es la segunda mano
        //TODO: Si el jugador gana la primera mano, vuelve a tirar
        if (punto_mano_jugador > punto_mano_computadora) {
            // El jugador elige su carta
            printf("\nQue carta desea tirar (1, 2 o 3): ");
            scanf("%d", &carta_tirada);
            carta_tirada = validar_opcion(carta_tirada);
            carta_tirada = evitar_doble_carta_jugador(carta_tirada);
            carta_jugador = mano_jugador[carta_tirada];

            // Mostrar carta del jugador
            printf("Carta tirada por el jugador: ");
            conocer_carta(carta_jugador);
                
            // Luego de tirar el jugador, es el turno de la computadora
            int indice_carta_computadora = evitar_doble_carta_computadora();
            carta_computadora = mano_computadora[indice_carta_computadora];

            // Mostrar carta de la computadora
            printf("Carta tirada por la computadora: ");
            conocer_carta(carta_computadora);

            // Definir al ganador de la mano
            if (valor_baraja[carta_computadora] < valor_baraja[carta_jugador]) {
                printf("\nLa computadora gana esta mano\n");
                punto_mano_computadora++;
            } else if (valor_baraja[carta_jugador] < valor_baraja[carta_computadora]) {
                printf("\nEl jugador gana esta mano\n");
                punto_mano_jugador++;
            } else {
                printf("\nEmpate en esta mano\n");
            }
        } //TODO: La computadora gana la primera mano
        else if (punto_mano_computadora > punto_mano_jugador){
            // Como gano la computadora, vuelve a tirar
            int indice_carta_computadora = evitar_doble_carta_computadora();
            carta_computadora = mano_computadora[indice_carta_computadora];

            // Mostrar carta de la computadora
            printf("\nCarta tirada por la computadora: ");
            conocer_carta(carta_computadora);

            // El jugador elige su carta
            printf("\nQue carta desea tirar (1, 2 o 3): ");
            scanf("%d", &carta_tirada);
            carta_tirada = validar_opcion(carta_tirada);
            carta_tirada = evitar_doble_carta_jugador(carta_tirada);
            carta_jugador = mano_jugador[carta_tirada];

            // Mostrar carta del jugador
            printf("Carta tirada por el jugador: ");
            conocer_carta(carta_jugador);
                
            // Definir al ganador de la mano
            if (valor_baraja[carta_computadora] < valor_baraja[carta_jugador]) {
                printf("\nLa computadora gana esta mano\n");
                punto_mano_computadora++;
            } else if (valor_baraja[carta_jugador] < valor_baraja[carta_computadora]) {
                printf("\nEl jugador gana esta mano\n");
                punto_mano_jugador++;
            } else {
                printf("\nEmpate en esta mano\n");
            }
        }// Si hay un empate en segunda ronda
        else {
            // El jugador elige su carta
            printf("\nQue carta desea tirar (1, 2 o 3): ");
            scanf("%d", &carta_tirada);
            carta_tirada = validar_opcion(carta_tirada);
            carta_tirada = evitar_doble_carta_jugador(carta_tirada);
            carta_jugador = mano_jugador[carta_tirada];

            // Mostrar carta del jugador
            printf("Carta tirada por el jugador: ");
            conocer_carta(carta_jugador);
                
            // Luego de tirar el jugador, es el turno de la computadora
            int indice_carta_computadora = evitar_doble_carta_computadora();
            carta_computadora = mano_computadora[indice_carta_computadora];

            // Mostrar carta de la computadora
            printf("Carta tirada por la computadora: ");
            conocer_carta(carta_computadora);

            // Definir al ganador de la mano
            if (valor_baraja[carta_computadora] < valor_baraja[carta_jugador]) {
                printf("\nLa computadora gana esta mano\n");
                punto_mano_computadora += 3;
            } else if (valor_baraja[carta_jugador] < valor_baraja[carta_computadora]) {
                printf("\nEl jugador gana esta mano\n");
                punto_mano_jugador += 3;
            } else {
                printf("\nEmpate en esta mano\n");
            }
        }

        // Cambiar valores para no repetir cartas
        mano_computadora[indice_carta_computadora] = -1;
        mano_jugador[carta_tirada] = -1; 

        //! Esta es la tercera mano, corregir que hay empate en la primera y segunda, se decide en esta
        //TODO: Si el jugador gana la segunda mano, vuelve a tirar
        if (punto_mano_jugador > punto_mano_computadora) {
            // El jugador elige su carta
            printf("\nQue carta desea tirar (1, 2 o 3): ");
            scanf("%d", &carta_tirada);
            carta_tirada = validar_opcion(carta_tirada);
            carta_tirada = evitar_doble_carta_jugador(carta_tirada);
            carta_jugador = mano_jugador[carta_tirada];

            // Mostrar carta del jugador
            printf("Carta tirada por el jugador: ");
            conocer_carta(carta_jugador);
                
            // Luego de tirar el jugador, es el turno de la computadora
            int indice_carta_computadora = evitar_doble_carta_computadora();
            carta_computadora = mano_computadora[indice_carta_computadora];

            // Mostrar carta de la computadora
            printf("Carta tirada por la computadora: ");
            conocer_carta(carta_computadora);

            // Definir al ganador de la mano
            if (valor_baraja[carta_computadora] < valor_baraja[carta_jugador]) {
                printf("\nLa computadora gana esta mano\n");
                punto_mano_computadora++;
            } else if (valor_baraja[carta_jugador] < valor_baraja[carta_computadora]) {
                printf("\nEl jugador gana esta mano\n");
                punto_mano_jugador++;
            } else {
                printf("\nEmpate en esta mano\n");
            }
        } //TODO: La computadora gana la segunda mano
        else if (punto_mano_computadora > punto_mano_jugador){
            // Como gano la computadora, vuelve a tirar
            int indice_carta_computadora = evitar_doble_carta_computadora();
            carta_computadora = mano_computadora[indice_carta_computadora];

            // Mostrar carta de la computadora
            printf("Carta tirada por la computadora: ");
            conocer_carta(carta_computadora);

            // El jugador elige su carta
            printf("\nQue carta desea tirar (1, 2 o 3): ");
            scanf("%d", &carta_tirada);
            carta_tirada = validar_opcion(carta_tirada);
            carta_tirada = evitar_doble_carta_jugador(carta_tirada);
            carta_jugador = mano_jugador[carta_tirada];

            // Mostrar carta del jugador
            printf("Carta tirada por el jugador: ");
            conocer_carta(carta_jugador);
                
            // Definir al ganador de la mano
            if (valor_baraja[carta_computadora] < valor_baraja[carta_jugador]) {
                printf("\nLa computadora gana esta mano\n");
                punto_mano_computadora++;
            } else if (valor_baraja[carta_jugador] < valor_baraja[carta_computadora]) {
                printf("\nEl jugador gana esta mano\n");
                punto_mano_jugador++;
            } else {
                printf("\nEmpate en esta mano\n");
            }
        }

    // Fuera de las manos 
    // Determinar el ganador de la ronda
    if (punto_mano_computadora > punto_mano_jugador) {
        punto_computadora++;
        printf("\nLa computadora gana la ronda\n");
    } else if (punto_mano_jugador > punto_mano_computadora) {
        punto_jugador++;
        printf("\nEl jugador gana la ronda\n");
    } else {
        printf("\nEmpate en la ronda\n");
    }

    // Mostrar puntos acumulados
    printf("\n***** Fin de la ronda *****\n");
    printf("Puntos del jugador: %d\n", punto_jugador);
    printf("Puntos de la computadora: %d\n", punto_computadora);

    reiniciar_baraja();
    printf("\n-------------------------");
    printf("\n------ Nueva Ronda ------\n");
    printf("-------------------------\n");

    repartir();
    }
    // Resultado final
    if (punto_jugador >= 15) {
        printf("\n¡El jugador ha ganado la partida!\n");
    } else {
        printf("\nLa computadora ha ganado la partida.\n");
    }
}


int unica(int carta) {
    if (baraja[carta] == 1) {
        return 0;
    } else {
        baraja[carta] = 1;
        return 1;
    }
}

int conocer_carta(int carta) {
    int palo = carta / 10;
    int numero = carta % 10;
    printf("%d de %s\n", numeros[numero], palos[palo]);
    return 0;
}

int validar_opcion(int carta_tirada) {
    while (carta_tirada < 1 || carta_tirada > 3) {
        printf("\nElija una opcion valida entre 1 y 3: ");
        scanf("%d", &carta_tirada);
    }
    return carta_tirada - 1;
}

int evitar_doble_carta_jugador(int carta) {
    while (mano_jugador[carta] == -1) {
        printf("La carta ya fue usada, elija otra: ");
        scanf("%d", &carta);
        carta = validar_opcion(carta);
    }
    return carta;
}

int evitar_doble_carta_computadora() {
    int carta;
    do {
        carta = rand() % 3;
    } while (mano_computadora[carta] == -1);
    return carta;
}

void reiniciar_baraja() {
    for (int i = 0; i < 40; i++) {
        baraja[i] = 0;
    }
}