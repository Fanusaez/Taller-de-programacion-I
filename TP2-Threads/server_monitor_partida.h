// "Copyright 2023 E. Francisco Saez

#ifndef SERVER_MONITOR_PARTIDA_H_
#define SERVER_MONITOR_PARTIDA_H_

#include <vector>
#include <string>
#include "server_queue.h"

class MonitorPartida {
 private:
    std::vector<std::reference_wrapper<Queue<std::string>>>
    jugadores_partida;
    std::mutex mutex_partida;

 public:
explicit MonitorPartida(Queue<std::string>& queue);

/*
 * Recibe una referencia de la queue del jugador a unir.
 * Se la agrega al vector de colas.
 */
void unir_jugador(Queue<std::string>& queue);

/*
 * Se itera sobre las queues de los jugadores y se pushea el mensaje a broadcastear.
 * En caso de que la queue este cerrada, se la elimina de la lista.
 */
void broadcastear(const std::string& buffer);

/*
 * Deshabilitamos copias
 */
MonitorPartida(const MonitorPartida&) = delete;
MonitorPartida& operator=(const MonitorPartida&) = delete;
};
#endif  // SERVER_MONITOR_PARTIDA_H_
