#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define los palos y numeros de las cartas, y las variables para las manos de jugador y computadora

const char *palos[4] = {"Oro", "Copas", "Bastos", "Espadas"};
int numeros[] = {1, 2, 3, 4, 5, 6, 7, 10, 11, 12}; // Numeros de cartas
int baraja[40] = {0};  // Indica si una carta ha sido usada o no
int mano_computadora[3] = {0}; // Cartas de la computadora
int mano_jugador[3] = {0}; // Cartas del jugador

// Se define el valor de las cartas según las reglas del truco
int valor_baraja[40] = {7,6,5,14,13,12,4,10,9,8,7,6,5,14,13,12,11,10,9,8,2,6,5,14,13,12,11,10,9,8,1,6,5,14,13,12,3,10,9,8};

// Prototipos de las funciones principales del programa
void repartir();
void juego();
void reiniciar_baraja();
int unica(int carta);
void mostrar_carta(int carta);
int evitar_doble_carta_jugador(int carta);
int evitar_doble_carta_computadora();
int turno_jugador();
int turno_computadora();
int determinar_ganador_mano(int carta_jugador, int carta_computadora);

// Función principal
int main() {
    srand(time(0)); // Inicializa la semilla de aleatoriedad
    printf("\nBienvenido al juego de Truco\n");
    repartir(); // Llama a la funcion para repartir cartas
    juego(); // Inicia el juego
    return 0;
}

// Genera la mano del jugador y de la computadora
void repartir() {
    int carta, palo, numero;

    // Reparte la mano de la computadora
    for (int i = 0; i < 3; i++) {
        do {
            palo = rand() % 4;  // Genera un palo aleatorio
            numero = rand() % 10;  // Genera un numero aleatorio
            carta = palo * 10 + numero;
        } while (!unica(carta));  // Verifica que la carta sea unica
        mano_computadora[i] = carta;
    }

    // Reparte la mano del jugador y muestra las cartas
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

// Ejecuta el flujo del juego de Truco hasta que alguno de los jugadores gane
void juego() {
    int puntos_computadora = 0, puntos_jugador = 0;

    while (puntos_computadora < 15 && puntos_jugador < 15) {
        int punto_mano_jugador = 0, punto_mano_computadora = 0;
        int turno = 1;  // Variable que determina quien empieza cada ronda

        for (int ronda = 0; ronda < 3; ronda++) {
            int carta_jugador, carta_computadora, resultado;

            if (turno == 1) {
                carta_jugador = turno_jugador();
                carta_computadora = turno_computadora();
            } else {
                carta_computadora = turno_computadora();
                carta_jugador = turno_jugador();
            }

            resultado = determinar_ganador_mano(carta_jugador, carta_computadora);

            // Actualiza los puntos de la mano y determina quien tira primero
            if (resultado == 1) {
                punto_mano_jugador++;
                turno = 1;
            } else if (resultado == -1) {
                punto_mano_computadora++;
                turno = -1;
            }

            // Si un jugador tiene dos puntos se termina automaticamente
            if (punto_mano_jugador == 2 || punto_mano_computadora == 2) {
                break;
            }
        }

        // Actualiza los puntos totales de la ronda
        if (punto_mano_jugador > punto_mano_computadora) {
            puntos_jugador++;
            printf("\n* El jugador gana la ronda\n");
        } else if (punto_mano_computadora > punto_mano_jugador) {
            puntos_computadora++;
            printf("\n* La computadora gana la ronda\n");
        } else {
            printf("\n* Empate en la ronda\n");
        }

        // Muestra los puntos acumulados y reinicia la baraja para la siguiente ronda
        printf("\n***** Fin de la ronda *****\n");
        printf("Puntos del jugador: %d\n", puntos_jugador);
        printf("Puntos de la computadora: %d\n", puntos_computadora);

        reiniciar_baraja();
        printf("\n-------------------------");
        printf("\n------ Nueva Ronda ------\n");
        printf("-------------------------\n");

        repartir();
    }

    // Muestra el resultado final del juego
    if (puntos_jugador >= 15) {
        printf("\n¡El jugador ha ganado la partida!\n");
    } else {
        printf("\nLa computadora ha ganado la partida.\n");
    }
}

// Verifica si una carta es unica en la baraja y la marca como usada
int unica(int carta) {
    if (baraja[carta] == 1) {
        return 0;
    }
    baraja[carta] = 1;
    
    return 1;
}

// Muestra una carta en formato "numero de palo"
void mostrar_carta(int carta) {
    int palo = carta / 10;
    int numero = carta % 10;
    printf("%d de %s\n", numeros[numero], palos[palo]);
}

// Verifica que el jugador no tire una carta que ya ha sido usada
int evitar_doble_carta_jugador(int carta) {
    while (mano_jugador[carta] == -1) {
        printf("La carta ya fue usada, elija otra: ");
        scanf("%d", &carta);
        carta = carta - 1;
    }
    return carta;
}

// Genera una carta aleatoria para la computadora, verificando que no se repita
int evitar_doble_carta_computadora() {
    int carta;
    do {
        carta = rand() % 3;
    } while (mano_computadora[carta] == -1);
    return carta;
}

// Realiza el turno del jugador, mostrando y marcando la carta seleccionada como usada
int turno_jugador() {
    // El jugador elige una carta de su mano para jugar
    int carta_tirada;
    printf("\nElija una carta (1, 2 o 3): ");
    scanf("%d", &carta_tirada);
    carta_tirada = carta_tirada - 1;
    carta_tirada = evitar_doble_carta_jugador(carta_tirada);

    // Elige la carta y se muestra en pantalla
    int carta_jugador = mano_jugador[carta_tirada];
    mano_jugador[carta_tirada] = -1;
    printf("Carta del jugador: ");
    mostrar_carta(carta_jugador);

    return carta_jugador;
}

// Realiza el turno de la computadora, mostrando y marcando la carta seleccionada como usada
int turno_computadora() {
    int carta_computadora = evitar_doble_carta_computadora();
    int carta = mano_computadora[carta_computadora];
    mano_computadora[carta_computadora] = -1;
    printf("Carta de la computadora: ");
    mostrar_carta(carta);

    return carta;
}

// Determina el ganador de una mano comparando el valor de las cartas
int determinar_ganador_mano(int carta_jugador, int carta_computadora) {
    if (valor_baraja[carta_computadora] > valor_baraja[carta_jugador]) {
        printf("\n- El jugador gana esta mano\n");
        return 1;
    } else if (valor_baraja[carta_jugador] > valor_baraja[carta_computadora]) {
        printf("\n- La computadora gana esta mano\n");
        return -1;
    } else {
        printf("\n- Empate en esta mano\n");
        return 0;
    }
}

// Reinicia la baraja para permitir nuevas cartas en la siguiente ronda
void reiniciar_baraja() {
    for (int i = 0; i < 40; i++) {
        baraja[i] = 0;
    }
}
