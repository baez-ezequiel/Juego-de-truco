#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char *palos[4] = {"Oro", "Copas", "Bastos", "Espadas"}; // Se definen los palos del juego
int numeros[] = {1, 2, 3, 4, 5, 6, 7, 10, 11, 12}; // Se definen los valores de las cartas
int baraja[40] = {0};  // Baraja de 40 cartas
int mano_computadora[3] = {0}; // Cartas de la computadora
int mano_jugador[3] = {0}; // Cartas del jugador

// Se define el valor de las cartas por orden de numeros y palos
int valor_baraja[40] = {7,6,5,14,13,12,4,10,9,8,7,6,5,14,13,12,11,10,9,8,2,6,5,14,13,12,11,10,9,8,1,6,5,14,13,12,3,10,9,8};

void repartir(); // Funcion que reparte las cartas al jugador y a la computadora
void juego(); // Simula el juego de truco
void reiniciar_baraja(); // Permite que siempre esten disponibles las 40 cartas de la baraja
void mostrar_carta(int carta); // Muestra en pantalla la carta jugada

int unica(int carta); // Evita que se repitan las cartas en las manos
int evitar_doble_carta_jugador(int carta); // Evita que el jugador tire dos veces la misma carta
int evitar_doble_carta_computadora(); // Evita que la computadora tire dos veces la misma carta
int turno_jugador(); // Simula el turno del jugador y le permite tirar la carta
int turno_computadora(); // Simula el turno de la computadora y le permite tirar la carta
int determinar_ganador_mano(int carta_jugador, int carta_computadora); // Determina quien gana la mano

// Validar opciones
int validar_DosOpciones(int opcion); 
int validar_TresOpciones(int opcion);
int validar_CuatroOpciones(int opcion);

//* Variables globales
int puntos_jugador = 0; // Puntos del jugador que permite que el juego funcione
int puntos_computadora = 0; // Puntos de la computadora para que el juego funcione

//* Funciones y variables relacionadas a cantar truco
int bandera_truco = 0; // Sirve para saber si se canta truco y analizar las posibles respuestas
int puntos_truco = 0; // Sirve para saber cuantos puntos dar dependiendo de la fase del truco

// Preguntamos al jugador si quiere cantar truco
void cantar_truco_jugador();
void cantar_truco_compu();

// Se definen todas las posibles respuestas del truco
void respuesta_truco_compu();
void respuesta_retruco_compu();
void respuesta_valeCuatro_compu();
void respuesta_truco_jugador();
void respuesta_retruco_jugador();
void respuesta_valeCuatro_jugador();

//* Funciones y variables relacionadas a cantar envido // Sirve para saber si se canta envido
int puntos_envido = 0; // Determinar cuantos puntos asignar al ganador del envido
int bandera_envido = 0; // Evita que se muestre en pantalla el envido cuando se responde no quiero al envido

// Se canta el envido
void cantar_envido_jugador();
void cantar_envido_compu();

// Se definen todas las posibles respuestas del envido
void respuesta_envido_jugador();
void respuesta_RealEnvido_jugador();
void respuesta_FaltaEnvido_jugador();
void respuesta_envido_compu();
void respuesta_RealEnvido_compu();
void respuesta_FaltaEnvido_compu();

// Compara la mano del jugador para saber de cuanto es el envido
void comparar_envido();

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

//* Ejecuta el flujo del juego de Truco hasta que alguno de los jugadores gane
void juego() {
    int rondas_jugadas = 1; // Ubica al usuario sobre la ronda que se esta jugando
    int turno = 1; // Inicia el jugador la primera mano

    while (puntos_computadora < 15 && puntos_jugador < 15) { // Ciclo para jugar el truco, se juega mientras nadie tenga 15 puntos o mas
        int punto_mano_jugador = 0, punto_mano_computadora = 0; // Inicializa los puntos de la mano

        for (int ronda = 0; ronda < 3; ronda++) { // Se pueden jugar hasta 3 manos en una ronda
            int carta_jugador, carta_computadora, resultado; // Conocer las cartas de los jugadores y el resultado
            
            // Muestra que numero de mano se esta jugando, para ubicar al usuario
            printf("\n--- Mano %d:", ronda + 1);

            if (turno == 1) { //* Turno del jugador
                // Si es la primera ronda y aun no se ha cantado envido, el jugador puede cantar envido
                if (ronda == 0 && puntos_envido == 0) {
                    cantar_envido_jugador();
                    comparar_envido(); // Calcula y determina el ganador del envido

                    if (bandera_envido == 2) {
                        break;
                    }
                }

                if (bandera_truco == 0) { // Si aun no se ha cantado truco el jugador puede cantar
                    cantar_truco_jugador(); 
                    if (bandera_truco == 1) { // Si alguien rechaza el truco se termina la ronda
                        turno = -1; // Turno de la compu
                        break;
                    }
                }
                carta_jugador = turno_jugador(); // El jugador tira su carta 

                //* Turno de la computadora
                // Si es la primera ronda y aun no se ha cantado envido, el jugador puede cantar envido
                if (ronda == 0 && puntos_envido == 0) {
                    cantar_envido_compu();
                    comparar_envido(); // Calcula y determina el ganador del envido

                    if (bandera_envido == 2) {
                        break;
                    }
                }

                if (bandera_truco == 0) { // Si todavia no se canta truco, la computadora puede cantar
                    cantar_truco_compu(); 
                    if (bandera_truco == 1) { // Si alguien rechaza el truco se termina la ronda
                        turno = 1; // Turno del jugador
                        break;
                    }
                }

                carta_computadora = turno_computadora();

            } else { //* Turno de la computadora
                if (ronda == 0 && puntos_envido == 0) {
                    cantar_envido_compu();
                    comparar_envido(); // Calcula y determina el ganador del envido

                    if (bandera_envido == 2) {
                        break;
                    }
                }

                if (bandera_truco == 0) { // Si todavia no se canta truco
                    cantar_truco_compu(); // El jugador elige si canta truco
                    if (bandera_truco == 1) {
                        turno = 1; // Turno del jugador
                        break;
                    }
                }
                carta_computadora = turno_computadora();

                //* Turno del jugador
                if (ronda == 0 && puntos_envido == 0) {
                    cantar_envido_jugador();
                    comparar_envido(); // Calcula y determina el ganador del envido

                    if (bandera_envido == 2) {
                        break;
                    }
                }

                if (bandera_truco == 0) { // Si todavia no se canta truco
                    cantar_truco_jugador(); // El jugador elige si canta truco
                    if (bandera_truco == 1) {
                        turno = -1; // Turno de la computadora
                        break;
                    }
                }
                carta_jugador = turno_jugador();
            }

            // Se determina el resultado de la ronda
            resultado = determinar_ganador_mano(carta_jugador, carta_computadora);

            if (resultado == 1) { // Si el jugador gana la mano, vuelve a tirar
                punto_mano_jugador++;
                turno = 1;
            } else { // Si la compu gana la mano, vuelve a tirar
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
            puntos_jugador += puntos_truco;
            printf("\n* El jugador gana la ronda\n");
            turno = -1; // Debido a que gano el jugador, ahora enpieza la computadora
        } else if (punto_mano_computadora > punto_mano_jugador) {
            puntos_computadora += puntos_truco;
            printf("\n* La computadora gana la ronda\n");
            turno = 1; // Debido a que gano la  computadora, ahora enpieza el jugador
        } else if (punto_mano_jugador == punto_mano_computadora && bandera_truco != 1) {
            printf("\n* Empate, Gana el jugador por ser mano\n");
            puntos_jugador += puntos_truco;
        }

        // Muestra los puntos acumulados y reinicia la baraja para la siguiente ronda
        printf("\n***** Fin de la ronda *****\n");
        printf("Puntos del jugador: %d\n", puntos_jugador);
        printf("Puntos de la computadora: %d\n", puntos_computadora);

        // Analiza si ya se han llegado a los 15 puntos
        if (puntos_jugador >= 30) {
            printf("\nEl jugador ha ganado la partida\n");
            break;
        } else if (puntos_computadora >= 30) {
            printf("\nLa computadora ha ganado la partida.\n");
            break;
        }

        reiniciar_baraja(); // Permite que las 40 cartas del mazo esten disponibles una vez que termine la ronda

        printf("\n-------------------------");
        printf("\n-----Ronda actual:%d -----\n",rondas_jugadas + 1);
        printf("-------------------------\n");

        // Reiniciar valores de variables
        bandera_truco = 0;
        puntos_truco = 1; 
        puntos_envido = 0;
        bandera_envido = 0;
        rondas_jugadas++;

        repartir(); // Vuelve a repartir las manos a cada jugador
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

int validar_DosOpciones(int opcion) {
    while (opcion < 1 || opcion > 2) {
        printf("Ingresa una opcion valida (1 o 2): ");
        scanf("%d", &opcion);
    }
    return opcion;
}

int validar_TresOpciones(int opcion) {
    while (opcion < 1 || opcion > 3) {
        printf("Ingresa una opcion valida (1, 2 o 3): ");  
        scanf("%d", &opcion);
    }
    return opcion;
}

int validar_CuatroOpciones(int opcion) {
    while (opcion < 1 || opcion > 4) {
        printf("Ingresa una opcion valida (1, 2, 3 o 4): ");
        scanf("%d", &opcion);
    }
    return opcion;
}

//* Se crean todas las respuestas posibles cuando se canta truco
void cantar_truco_jugador() {
    int cantar_truco;
        printf("\nQueres cantar truco? (1: No, 2: Si): ");
        scanf("%d", &cantar_truco);
        cantar_truco = validar_DosOpciones(cantar_truco);

    if (cantar_truco == 2) {
        printf("\n- player: TRUCO\n");
        bandera_truco = 2; // Se ha cantado truco
        respuesta_truco_compu(); // La computadora responde
        if (bandera_truco == 1) { // Si la computadora no quiere truco se termina la mano
            puntos_jugador++;
        } else if (bandera_truco == 3) { // La compu responde con retruco
            respuesta_retruco_jugador(); // El jugador responde
            if (bandera_truco == 1) { // Si no quiere retruco
                puntos_computadora += 2;
            } else if (bandera_truco == 4){ // Si quiere vale 4
                respuesta_valeCuatro_compu(); // La compu responde
                if (bandera_truco == 1) {
                    puntos_jugador += 3;
                }
            }
        }   
    } 
}

void respuesta_truco_compu() {
    int respuesta_compu;

    respuesta_compu = rand() % 3;

    switch (respuesta_compu) {
    case 0:
        printf("\n- compu: NO QUIERO\n");
        bandera_truco = 1; // Se termina el juego y el jugador gana 1 punto
        break;
    case 1:
        printf("\n- compu: QUIERO\n");
        bandera_truco = 2; // Se sigue jugando pero ahora por 2 puntos
        puntos_truco = 2;    
        break;
    case 2:
        printf("\n- compu: QUIERO RETRUCO\n");
        bandera_truco = 3; // Se sigue jugando pero ahora por 3 puntos
        puntos_truco = 3;    
        break;
    }
}

void respuesta_retruco_jugador() {
    int responder_retruco;
    printf("\nRespuestas: (1: No quiero, 2: Quiero, 3: Vale cuatro): ");
    scanf("%d", &responder_retruco);

    responder_retruco = validar_TresOpciones(responder_retruco);
    switch (responder_retruco) {
    case 1:
        printf("\n- player: NO QUIERO\n");
        bandera_truco = 1;    
        break;
    
    case 2:
        printf("\n- player: QUIERO\n");
        puntos_truco = 3;    
        break;
    case 3:
        printf("\n- player: VALE CUATRO\n");
        bandera_truco = 4;    
        break;
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
            puntos_truco = 4;
        }  
}

void cantar_truco_compu() {
    int cantar_truco;
    cantar_truco = rand() % 2; // 0: no quire, 1: quiere

    // La compu solo canta si no se ha cantado truco antes
    if (cantar_truco == 1) {
        printf("\n- computadora: TRUCO\n");
        respuesta_truco_jugador(); // El jugador responde a la compu el truco
        if (bandera_truco == 1) {  // Si el jugador no quiere
            puntos_computadora++;
        } else if (bandera_truco == 3) { // El jugador quiere retruco
            respuesta_retruco_compu(); // 
            if (bandera_truco == 1) { // Si la compu no quiere
                puntos_jugador += 2;
            } else if (bandera_truco == 4){ // Si la compu quiere vale cuatro
                respuesta_valeCuatro_jugador(); // 
                if (bandera_truco == 1) { // Si el jugador no quiere
                    puntos_computadora += 3;
                }
            }
        }
    }
}

void respuesta_truco_jugador(){
    int respuesta_jugador;

    printf("\nRespuestas (1: No quiero, 2: Quiero, 3: Retruco): "); // Responder al truco de la compu
    scanf("%d", &respuesta_jugador);
    respuesta_jugador = validar_TresOpciones(respuesta_jugador);

    switch (respuesta_jugador) {
    case 1:
        printf("\n- player: NO QUIERO\n");
        bandera_truco = 1;        
        break;
    case 2:
        printf("\n- player: QUIERO\n");
        puntos_truco = 2;
        bandera_truco = 2; 
        break;
    case 3:
        printf("\n- player: QUIERO RETRUCO\n");
        bandera_truco = 3;
        puntos_truco = 3;
        break;
    }
}

void respuesta_retruco_compu() {
    int responder_retruco;
    responder_retruco = rand() % 3;

    switch (responder_retruco) {
    case 0:
        printf("\n- compu: NO QUIERO\n");
        bandera_truco = 1;
        break;
    case 1:
        printf("\n- compu: QUIERO\n");
        puntos_truco = 3;
        break;
    case 2:
        printf("\n- compu: VALE CUATRO\n");
        bandera_truco = 4;
        break;
    }
}

void respuesta_valeCuatro_jugador() {
    int responder_valeCuatro;

    printf("\nRespuestas: (1: No quiero, 2: Quiero): ");
    scanf("%d", &responder_valeCuatro);
    responder_valeCuatro = validar_DosOpciones(responder_valeCuatro);

    switch (responder_valeCuatro)
    {
    case 1:
        printf("\n- player: NO QUIERO\n");
        bandera_truco = 1;
        break;
    
    case 2:
        printf("\n- player: QUIERO\n");
        puntos_truco = 4;
        break;
    }
}

//* Se crean todas las respuestas cuando se canta envido
void cantar_envido_jugador() {
    int opcion;
    printf("\nQueres cantar Envido? (1: No, 2: Envido, 3: Real Envido, 4: Falta Envido): ");
    scanf("%d", &opcion);
    opcion = validar_CuatroOpciones(opcion);

    switch (opcion) {
        case 1:
            bandera_envido = 0;
            break;
        case 2:
            printf("\n- player: ENVIDO\n");
            respuesta_envido_compu();
            bandera_envido = 1;
            break;
        case 3:
            printf("\n- player: REAL ENVIDO\n");
            respuesta_RealEnvido_compu();
            bandera_envido = 1;
            break;
        case 4:
            printf("\n- player: FALTA ENVIDO\n");
            respuesta_FaltaEnvido_compu();
            break;
    }
}

void respuesta_envido_compu() {
    int respuesta_envido = rand() % 3;

    switch (respuesta_envido) {
    case 0:
        printf("\n- Compu: NO QUIERO\n");
        bandera_envido = -1;
        puntos_jugador += 1;
        break;
    case 1:
        printf("\n - Compu: QUIERO\n");
        puntos_envido = 2;
        bandera_envido = 1;
        break;
    case 2:
        printf("\n- Compu: REAL ENVIDO\n");
        respuesta_RealEnvido_jugador();
    }
}

void respuesta_RealEnvido_jugador() {
    int respuesta;

    printf("\nRespuestas: (1: No quiero, 2: Quiero, 3: Falta envido): ");
    scanf("%d", &respuesta);

    respuesta = validar_TresOpciones(respuesta);

    switch (respuesta) {
    case 1:
        printf("\n- player: NO QUIERO\n");
        bandera_envido = -1;
        puntos_computadora += 1;
        break;
    case 2:
        printf("\n- player: QUIERO\n"); 
        puntos_envido = 3;
        bandera_envido = 1;
        break;
    case 3:
        printf("\n- Player: FALTA ENVIDO\n");
        bandera_envido = 1;
        respuesta_FaltaEnvido_compu();
    }
}

void respuesta_RealEnvido_compu(){
    int respuesta = rand() % 3;

    switch (respuesta) {
    case 0:
        printf("\n- Compu: NO QUIERO\n");
        bandera_envido = -1;
        puntos_jugador += 1;
        break;
    case 1:
        printf("\n- Compu: QUIERO\n"); 
        puntos_envido = 3;
        bandera_envido = 1;
        break;
    case 2:
        printf("\n- Compu: FALTA ENVIDO\n");
        respuesta_FaltaEnvido_jugador();
        break;
    }
}

void respuesta_FaltaEnvido_compu() {
    int respuesta_FaltaEnvido = rand() % 2;

    switch (respuesta_FaltaEnvido) {
    case 0:
        printf("\n- Compu: NO QUIERO\n");
        puntos_jugador += 1;
        bandera_envido = -1;
        break;
    case 1:
        printf("\n- Compu: QUIERO\n");
        puntos_envido = 15;
        bandera_envido = 2;
        break;
    }
}

void respuesta_FaltaEnvido_jugador() {
    int respuesta;

    printf("\nRespuestas: (1: No quiero, 2: Quiero): ");
    scanf("%d", &respuesta);

    respuesta = validar_DosOpciones(respuesta);
    switch (respuesta) {
    case 1:
        printf("\n- player: NO QUIERO\n");
        puntos_computadora += 1;
        bandera_envido = -1;
        break;
    case 2:
        printf("\n- player: QUIERO\n"); 
        puntos_envido = 15;
        bandera_envido = 2;
        break;
    }
}

void cantar_envido_compu () {
    int cantar_envido = rand() % 4;

    switch (cantar_envido) {
    case 0:
        break;
    case 1:
        bandera_envido = 1;
        printf("\n- Compu: ENVIDO\n");
        respuesta_envido_jugador();
        break;
    case 2:
        bandera_envido = 1;
        printf("\n- Compu: REAL ENVIDO\n");
        respuesta_RealEnvido_jugador();
        break;
    case 3:
        bandera_envido = 1;
        printf("\n- Compu: FALTA ENVIDO");
        respuesta_FaltaEnvido_jugador();
        break;
    }
}

void respuesta_envido_jugador() {
    int respuesta;
    printf("\nRespuestas: (1: No quiero, 2: Quiero, 3: Real Envido): ");
    scanf("%d", &respuesta);

    respuesta = validar_TresOpciones(respuesta);

    switch (respuesta) {
    case 1:
        printf("\n- Player: NO QUIERO");
        puntos_computadora += 1;
        bandera_envido = -1;
        break;
    case 2:
        printf("- Player: QUIERO");
        puntos_envido = 2;
        break;
    case 3:
        printf("- Player: REAL ENVIDO");
        respuesta_RealEnvido_compu();
        break;
    }
}

// Calcula el envido de una mano dada
int calcular_envido(int mano[]) {
    int envido = 0;
    int valores[10] = {1, 2, 3, 4, 5, 6, 7, 0, 0, 0};  // Valores para las cartas (considerando que el 8 y 9 no se usan en envido)

    int palo_PrimeraCarta = mano[0] / 10;   
    int numero_PrimeraCarta = mano[0] % 10;

    int palo_SegundaCarta = mano[1] / 10;
    int numero_SegundaCarta = mano[1] % 10;

    int palo_TerceraCarta = mano[2] / 10;
    int numero_TerceraCarta = mano[2] % 10;

    // Caso cuando las dos primeras cartas tienen el mismo palo
    if (palo_PrimeraCarta == palo_SegundaCarta) {
        envido = valores[numero_PrimeraCarta] + valores[numero_SegundaCarta] + 20;
    }
    // Caso cuando la primera y la tercera carta tienen el mismo palo
    else if (palo_PrimeraCarta == palo_TerceraCarta) {
        envido = valores[numero_PrimeraCarta] + valores[numero_TerceraCarta] + 20;
    }
    // Caso cuando la segunda y la tercera carta tienen el mismo palo
    else if (palo_SegundaCarta == palo_TerceraCarta) {
        envido = valores[numero_SegundaCarta] + valores[numero_TerceraCarta] + 20;
    }
    // Si no hay ninguna carta con el mismo palo, se toma la mayor combinación posible de valores
    else {
        if (valores[numero_PrimeraCarta] >= valores[numero_SegundaCarta] && valores[numero_PrimeraCarta] >= valores[numero_TerceraCarta]) {
            envido = valores[numero_PrimeraCarta];
        }else if (valores[numero_SegundaCarta] >= valores[numero_PrimeraCarta] && valores[numero_SegundaCarta] >= valores[numero_TerceraCarta]) {
            envido = valores[numero_SegundaCarta]; 
        } else if (valores[numero_TerceraCarta] >= valores[numero_PrimeraCarta] && valores[numero_TerceraCarta] >= valores[numero_SegundaCarta]){
            envido = valores[numero_TerceraCarta];
        }
        }
    return envido;
}

// Determina el ganador del envido entre el jugador y la computadora
void comparar_envido() {
    int envido_jugador = calcular_envido(mano_jugador);
    int envido_computadora = calcular_envido(mano_computadora);

    if (bandera_envido == 1) { // Solo se mostrara cuando se acepte el envido, sino no
        printf("\nEnvido del jugador: %d\n", envido_jugador);
        printf("Envido de la computadora: %d\n", envido_computadora);

        if (envido_jugador > envido_computadora) {
            printf("El jugador gana el envido\n");
            puntos_jugador += puntos_envido;
        } else if (envido_computadora > envido_jugador) {
            printf("La computadora gana el envido\n");
            puntos_computadora += puntos_envido;
        } else {
            printf("Empate en el envido, gana el jugador por ser mano\n");
            puntos_jugador += puntos_envido;
        }
    }
}