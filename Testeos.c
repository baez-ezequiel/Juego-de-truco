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
int bandera_truco = 0; // Sirve para saber si se canta truco y analizar las posibles respuestas
int bandera_puntos = 0; // Sirve para saber cuantos puntos dar cuando se responde No quiero al truco

int validar_canto(int cantar); // Se valida el numero que ingresa el jugador para que este en el rango
int puntos_truco(); // Dependiendo del estado de truco asigna los puntos
void cantar_truco_jugador(); // Preguntamos al jugador si quiere cantar truco
void respuesta_truco_compu();
void respuesta_retruco_jugador();
void respuesta_valeCuatro_compu();
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
    int puntos_computadora = 0, puntos_jugador = 0, rondas_jugadas = 1;

    while (puntos_computadora < 15 && puntos_jugador < 15) {
        int punto_mano_jugador = 0, punto_mano_computadora = 0; // Inicializa los puntos de la mano
        int turno = 1;  // Variable que determina quien empieza cada ronda

        for (int ronda = 0; ronda < 3; ronda++) { // Se pueden jugar hasta 3 manos en una ronda
            int carta_jugador, carta_computadora, resultado;
            
            printf("\nMano %d:", ronda + 1);
            if (turno == 1) { // Si el turno es del jugador
            //! Primero se evalua la decision del jugador para cantar truco y en base a eso se realizan las condiciones
                if (bandera_truco == 0) { // Si todavia no se canta truco
                    cantar_truco_jugador(); // El jugador elige si canta truco

                    if (bandera_truco == 2) { // Si el jugador canta truco
                        respuesta_truco_compu(); // La computadora responde
                        if (bandera_truco == 1) { // Si la computadora no quiere truco se termina la mano
                            puntos_jugador++;
                            break;
                        } else if (bandera_truco == 3){ // La compu responde con retruco
                            respuesta_retruco_jugador(); // El jugador responde
                            if (bandera_truco == 1) { // Si no quiere retruco
                                puntos_computadora += 2;
                                break;
                            } else if (bandera_truco == 4){ // Si quiere vale 4
                                respuesta_valeCuatro_compu(); // La compu responde
                                if (bandera_truco == 1) {
                                    puntos_jugador += 3;
                                    break;
                                }
                            }
                        }   
                    }

                }

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
            puntos_jugador += puntos_truco();
            printf("\n* El jugador gana la ronda\n");
        } else if (punto_mano_computadora > punto_mano_jugador) {
            puntos_computadora += puntos_truco();
            printf("\n* La computadora gana la ronda\n");
        } else if (punto_mano_jugador == punto_mano_computadora && bandera_truco != 1) {
            printf("\n* Empate en la ronda\n");
        }

        // Muestra los puntos acumulados y reinicia la baraja para la siguiente ronda
        printf("\n***** Fin de la ronda *****\n");
        printf("Puntos del jugador: %d\n", puntos_jugador);
        printf("Puntos de la computadora: %d\n", puntos_computadora);

        // Analiza si ya se han llegado a los 15 puntos
        if (puntos_jugador >= 15) {
            printf("\nEl jugador ha ganado la partida\n");
            break;
        } else if (puntos_computadora >= 15) {
            printf("\nLa computadora ha ganado la partida.\n");
            break;
        }

        reiniciar_baraja();
        printf("\n-------------------------");
        printf("\n-----Ronda actual:%d -----\n",rondas_jugadas+1);
        printf("-------------------------\n");

        // Reiniciar valores de variables
        bandera_truco = 0;
        bandera_puntos = 0;
        rondas_jugadas++;

            // Muestra el resultado final del juego
        
        repartir();
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
    printf("\nCarta de la computadora: ");
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

//! Cuando el jugador canta truco
void cantar_truco_jugador(){
    int cantar_truco;
        printf("\nQueres cantar truco? (1: No, 2: Si): ");
        scanf("%d", &cantar_truco);
        cantar_truco = validar_canto(cantar_truco);

        if (cantar_truco == 2) {
            printf("\n- player: TRUCO\n");
            bandera_truco = 2; // Se ha cantado truco
        } 
}

void respuesta_truco_compu() {
    int respuesta_compu;

    respuesta_compu = rand() % 3;

    if (respuesta_compu == 0) {
        printf("\n- computadora: NO QUIERO\n");
        bandera_truco = 1; // Se termina el juego y el jugador gana 1 punto
    } else if (respuesta_compu == 1) {
        printf("\n- computadora: QUIERO\n");
        bandera_truco = 2; // Se sigue jugando pero ahora por 2 puntos
        bandera_puntos = 2;
    } else {
        printf("\n- computadora: QUIERO RETRUCO\n");
        bandera_truco = 3; // Se sigue jugando pero ahora por 3 puntos
        bandera_puntos = 3;
    } 
}

void respuesta_retruco_jugador() {
    int responder_retruco;
    printf("\nRespuestas: (1: No quiero, 2: Quiero, 3: Vale cuatro): ");
    scanf("%d", &responder_retruco);

    if (responder_retruco == 1) {
        printf("\n- player: NO QUIERO\n");
        bandera_truco = 1;
    } else if (responder_retruco == 2) {
        printf("\n- player: QUIERO\n");
        bandera_puntos = 3;
    } else if (responder_retruco == 3) {
        printf("\n- player: VALE CUATRO\n");
        bandera_truco = 4;
    }
}

void respuesta_valeCuatro_compu(){
    int responder_valeCuatro;

    responder_valeCuatro = rand() % 2;
        if (responder_valeCuatro == 0) {
            printf("\n- compu: NO QUIERO\n");
            bandera_truco = 1;
        } else {
            printf("\n- compu: QUIERO\n");
            bandera_truco = 4;
            bandera_puntos = 4;
        }  
}

//! Cuando la compu canta truco

// Se asigna el punto dependiendo de en que instancia del truco se este
int puntos_truco() {
    int puntos;
    switch (bandera_puntos) {
    case 0:
        puntos = 1;
        break;
    case 2: // Se dijo quiero al truco
        puntos = 2;
        break;
    case 3: // Se dijo quiero al retruco
        puntos = 3;
        break;
    case 4: // Se dijo quiero al vale cuatro
        puntos = 4;
        break;
    }
    return puntos;
}
