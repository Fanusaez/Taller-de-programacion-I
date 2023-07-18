// "Copyright 2023 E. Francisco Saez

#ifndef SERVER_JUEGO_H_
#define SERVER_JUEGO_H_

#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include "server_queue.h"
#include "server_monitor_partidas.h"
#include "server_monitor_partida.h"

class Juego {
 private:
    std::uint32_t codigo_partida = -1;
    MonitorPartidas &monitor_partidas;
    MonitorPartida* monitor_partida = NULL;

/*
 * Imprime por salida estandar el mensaje + codigo que se le pase por parametro.
 */
void imprimir(const std::string& mensaje, const std::uint32_t& codigo);

 public:
explicit Juego(MonitorPartidas &monitor_partidas);

/*
 * Recibe una referencia de la cola del jugador.
 * Se llama a al metodo de crear_partida de MonitorPartidas
 * Se finaliza imprimiendo el mensaje de creacion de partida a traves del metodo imprimir.
 */
std::uint32_t crear_partida(Queue<std::string> &cola_jugador);

/*
 * Recibe una referencia de la cola del jugador.
 * Se llama a al metodo de unirse_a_partida de MonitorPartidas
 * Se finaliza imprimiendo el mensaje de union a partida a traves del metodo imprimir.
 */
bool unir_a_partida(Queue<std::string> &cola_jugador,
                    std::uint32_t codigo_partida);

/*
 * Recibe un string a ser broadcasteado.
 * Se llama a broadcastear de MonitorPartida
 */
void broadcastear(const std::string& broadcast);

/*
 * Deshabilitamos copias
 */
Juego(const Juego&) = delete;
Juego& operator=(const Juego&) = delete;
};

#endif  // SERVER_JUEGO_H_
