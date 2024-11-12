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


void repartir(); // Funcion que reparte las cartas al jugador y a la computadora
void juego(); // Simula el juego de truco
void reiniciar_baraja(); // Permite que siempre esten disponibles las 40 cartas de la baraja
void mostrar_carta(int carta); // Muestra el numero y palo de la carta si le pasamos su indice en la baraja

int unica(int carta); // Evita que se repitan las cartas en las manos
int evitar_doble_carta_jugador(int carta); // Evita que el jugador tire dos veces la misma carta
int evitar_doble_carta_computadora(); // Evita que la computadora tire dos veces la misma carta
int turno_jugador(); // Simula el turno del jugador y le permite tirar la carta
int turno_computadora(); // Simula el turno de la computadora y le permite tirar la carta
int determinar_ganador_mano(int carta_jugador, int carta_computadora); // Determina quien gana la mano

//! Funciones y variables relacionadas a cantar truco
int bandera_truco = 0; // Sirve para saber si se canto truco en algun momento de la partida
int bandera_truco_compu; // Sirve para saber si la computadora acepta o rechaza el truco
int bandera_truco_jugador; // Sirve para saber si el jugador acepta o rechaza el truco

int validar_canto(int cantar); // Se valida el numero que ingresa el jugador para que este en el rango
void cantar_truco_jugador(); // Preguntamos al jugador si quiere cantar truco
void cantar_truco_computadora(); // Preguntamos a la computadora si quiere cantar truco
int respuesta_truco_compu(); // Preguntamos a la computadora si quiere responder al truco
int respuesta_truco_jugador(); // El jugador debe responder al canto de la computadora
int respuesta_retruco_jugador(); // El jugador responde dependiendo de la respuesta de la compu
int respuesta_retruco_compu();
int respuesta_valeCuatro_compu();
int respuesta_valeCuatro_jugador();
int puntos_truco();

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
            carta = palo * 10 + numero; // Calcula el indice de la carta en la baraja
        } while (!unica(carta));  // Verifica que la carta sea unica
        mano_computadora[i] = carta;
    }

    // Reparte la mano del jugador y muestra las cartas
    printf("\nMano del jugador:\n");
    for (int i = 0; i < 3; i++) {
        do {
            palo = rand() % 4; // Genera un palo aleatorio
            numero = rand() % 10; // Genera un numero aleatorio
            carta = palo * 10 + numero; // Calcula el indice de la carta en la baraja
        } while (!unica(carta)); // Verifica que la carta sea unica
        mano_jugador[i] = carta;
        printf("Carta %d: %d de %s\n", i + 1, numeros[numero], palos[palo]); // Muestra la carta del jugador en la pantalla
    }
}

//! Ejecuta el flujo del juego de Truco hasta que alguno de los jugadores gane
void juego() {
    int puntos_computadora = 0, puntos_jugador = 0;

    while (puntos_computadora < 15 && puntos_jugador < 15) {
        int punto_mano_jugador = 0, punto_mano_computadora = 0; // Inicializa los puntos de la mano
        int turno = 1;  // Variable que determina quien empieza cada ronda

        for (int ronda = 0; ronda < 3; ronda++) { // Se pueden jugar hasta 3 manos en una ronda
            int carta_jugador, carta_computadora, resultado;
            
            cantar_truco_jugador(); // El jugador decide si canta truco o no
            
            if (turno == 1) { // Si el turno es del jugador
                if (bandera_truco == 1) { // Significa que se canto truco
                    respuesta_truco_compu(); // La compu debe responder
                    bandera_truco = -1; // Evita que se cante truco en todas las manos

                    if (bandera_truco_compu == 0) { // La compu rechaza el truco y termina la ronda
                        puntos_jugador++;
                        break;
                    } else if (bandera_truco_compu == 2) { // Si la compu canta retruco
                        respuesta_retruco_jugador(); // El jugador decide si acepta el retruco o no

                        if (bandera_truco_jugador == 0) { // El jugador se nego al retruco
                            puntos_computadora += 2;
                            break;
                        } else if (bandera_truco_jugador == 2){
                            respuesta_valeCuatro_compu();

                            if (bandera_truco_compu == 0){ // La compu se nego al vale 4
                            puntos_jugador += 3;
                                break;
                            }
                        }
                    } 
                }
                // Despues que se resuelve el truco, el jugador juega una carta
                carta_jugador = turno_jugador();
                carta_computadora = turno_computadora();

            } else { // Turno de la computadora
                carta_computadora = turno_computadora();
                carta_jugador = turno_jugador();
            }

            // Se determina el resultado de la ronda
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
            puntos_jugador = puntos_truco();
            printf("\n* El jugador gana la ronda\n");
        } else if (punto_mano_computadora > punto_mano_jugador) {
            puntos_computadora = puntos_truco();
            printf("\n* La computadora gana la ronda\n");
        } else if (punto_mano_jugador == punto_mano_computadora && bandera_truco == 1) {
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

        // Se reinicia el valor de las banderas
        bandera_truco = 0; 
        bandera_truco_compu = 0; 
        bandera_truco_jugador = 0;
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
    int respuesta;
    if (valor_baraja[carta_computadora] > valor_baraja[carta_jugador]) {
        printf("\n- El jugador gana esta mano\n");
        respuesta = 1;
    } else if (valor_baraja[carta_jugador] > valor_baraja[carta_computadora]) {
        printf("\n- La computadora gana esta mano\n");
        respuesta = -1;
    } else {
        printf("\n- Empate en esta mano\n");
        respuesta = 0;
    }
    return respuesta;
}

// Reinicia la baraja para permitir nuevas cartas en la siguiente ronda
void reiniciar_baraja() {
    for (int i = 0; i < 40; i++) {
        baraja[i] = 0;
    }
}

int validar_canto(int cantar) {
    while (cantar < 1 || cantar > 2) {
        printf("Ingresa una opcion valida (1 o 2): ");
        scanf("%d", &cantar);
    }
    return cantar;
}

//* Se canta truco
void cantar_truco_jugador(){
    int cantar_truco;
    if (bandera_truco == 0) { // No se debe haber cantado truco antes
        printf("\nQueres cantar truco? (1: No, 2: Si): ");
        scanf("%d", &cantar_truco);
        cantar_truco = validar_canto(cantar_truco);

        if (cantar_truco == 2) {
            printf("\n- player: TRUCO\n");
            bandera_truco = 1;
        }
    } 
}

void cantar_truco_computadora() {
    int cantar_truco;
    cantar_truco = rand() % 2; // 0: no quire, 1: quiere

    // La compu solo canta si no se ha cantado truco antes
    if (cantar_truco == 1 && bandera_truco == 0) {
        printf("\n- computadora: TRUCO\n");
        bandera_truco = 1;
    }
}

//* Se responde al truco
int respuesta_truco_compu(){ 
    int respuesta_compu;

    if (bandera_truco == 1) { // El jugador canta truco
        respuesta_compu = rand() % 3 + 1; // La compu elige una carta random para responder

        if (respuesta_compu == 1) {
            printf("\n- compu: NO QUIERO\n");
            bandera_truco_compu = 0; 
        } else if (respuesta_compu == 2) {
            printf("\n- compu: QUIERO\n");
            bandera_truco_compu = 1;
        } else {
            printf("\n- compu: RETRUCO?\n");
            bandera_truco_compu = 2;
        }
    }
    return respuesta_compu;
}

int respuesta_truco_jugador(){
    int respuesta_jugador;

    if (bandera_truco == 1) { // Se canto truco
        printf("\nRespuestas (1: No quiero, 2: Quiero, 3: Retruco): "); // Responder al truco de la compu
        scanf("%d", &respuesta_jugador);

        if (respuesta_jugador == 1) {
            printf("\n- player: NO QUIERO\n");
            bandera_truco_jugador = 0;
        } else if (respuesta_jugador == 2) {
            printf("\n- player: QUIERO\n");
            bandera_truco = 2; // Indica que estamos en el truco, se juegan por 2 puntos
            bandera_truco_jugador = 1;
        } else if (respuesta_jugador == 3) {
            printf("\n- player: RETRUCO!\n");
            bandera_truco_jugador = 2;
        }
    }
    return 0; 
}

//* Se responde al retruco
int respuesta_retruco_jugador() {
    int responder_retruco;
    if (bandera_truco_compu == 2) {
        printf("\nRespuestas: (1: No quiero, 2: Quiero, 3: Vale cuatro): ");
        scanf("%d", &responder_retruco);

        if (responder_retruco == 1) {
            printf("\n- player: NO QUIERO\n");
            bandera_truco_jugador = 0;
        } else if (responder_retruco == 2) {
            printf("\n- player: QUIERO\n");
            bandera_truco_jugador = 1;
            bandera_truco = 3; // Indica que estamos en retruco, se juegan por 3 puntos
        } else if (responder_retruco == 3) {
            printf("\n- player: VALE CUATRO\n");
            bandera_truco_jugador = 2;
        }
    }
    return responder_retruco;
}

int respuesta_retruco_compu() {
    int responder_retruco;
    responder_retruco = rand() % 3+1;
    if (bandera_truco_jugador == 2) {
        if (responder_retruco == 1) {
            printf("\n- compu: NO QUIERO\n");
            bandera_truco_compu = 0;
        } else if (responder_retruco == 2) {
            printf("\n- compu: QUIERO\n");
            bandera_truco_compu = 1;
            bandera_truco = 3; // Indica que estamos en retruco, se juegan por 3 puntos
        } else if (responder_retruco == 3) {
            printf("\n- compu: VALE CUATRO\n");
            bandera_truco_compu = 3;
        }
    }
    return responder_retruco;
}

//* Se responde al vale 4
int respuesta_valeCuatro_compu(){
    int responder_valeCuatro;

    responder_valeCuatro = rand() % 2+1;
    if (bandera_truco_jugador == 2) {
        if (responder_valeCuatro == 1) {
            printf("\n- compu: NO QUIERO\n");
            bandera_truco_compu = 0;
        } else {
            printf("\n- compu: QUIERO\n");
            bandera_truco_compu = 1;
            bandera_truco = 4; // Indica que estamos en vale cuatro, se juegan por 4 puntos
        }
    }
    return responder_valeCuatro;    
}

int respuesta_valeCuatro_jugador() {
    int responder_valeCuatro;

    if (bandera_truco_compu == 2) {
        printf("\nRespuestas: (1: No quiero, 2: Quiero, 3: Vale cuatro): ");
        scanf("%d", &responder_valeCuatro);

        if (responder_valeCuatro == 1) {
            printf("\n- player: NO QUIERO\n");
            bandera_truco_jugador = 0;
        } else if (responder_valeCuatro == 2) {
            printf("\n- player: QUIERO\n");
            bandera_truco_jugador = 1;
            bandera_truco = 3; // Indica que estamos en retruco, se juegan por 3 puntos
        } else if (responder_valeCuatro == 3) {
            printf("\n- player: VALE CUATRO\n");
            bandera_truco_jugador = 2;
        }
    }
    return responder_valeCuatro;
}

int puntos_truco() {
    int puntos;
    switch (bandera_truco) {
    case 1: // Estan en truco
        puntos = 1;
        break;
    case 2:
        puntos = 2;
        break;
    case 3:
        puntos = 3;
        break;
    case 4:
        puntos = 4;
        break;
    }
    return puntos;
}