// "Copyright 2023 E. Francisco Saez

#ifndef SERVER_MONITOR_PARTIDAS_H_
#define SERVER_MONITOR_PARTIDAS_H_


#include <map>
#include <vector>
#include <atomic>
#include <string>
#include "server_queue.h"
#include "server_monitor_partida.h"

class MonitorPartidas {
 private:
    std::map<std::uint32_t, MonitorPartida*> partidas;
    std::mutex mutex_map;
    std::atomic<std::uint32_t> codigo_unico{0};

 public:
MonitorPartidas();

/*
 * Recibe una referencia a la queue del jugador y una referencia a un codigo de partida.
 * Crea un MonitorPartida en el heap y obtiene un codigo para la partida
 * Guarda en el map el codigo como clave y la partida como valor.
 * Devuelve un puntero al MonitorPartida creado.
 */
MonitorPartida* crear_partida(Queue<std::string> &cola_jugador,
                              std::uint32_t& codigo_partida);

/*
 * Recibe una referencia a la queue del jugador y una referencia a un codigo de partida.
 * Busca en el map con el codigo de la partida la instancia de la partida.
 * En caso de no existir devuelve NULL, de lo contrario devuelve un puntero a la instancia de la partida
 * con el codigo coincidente.
 */
MonitorPartida* unirse_a_partida(Queue<std::string> &cola_jugador,
                      std::uint32_t codigo_partida);

/*
 *
 */
~MonitorPartidas();

/*
 * Deshabilitamos copias
 */
MonitorPartidas(const MonitorPartidas&) = delete;
MonitorPartidas& operator=(const MonitorPartidas&) = delete;
};

#endif  //  SERVER_MONITOR_PARTIDAS_H_
