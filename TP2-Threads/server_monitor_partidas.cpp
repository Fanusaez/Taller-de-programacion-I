// "Copyright 2023 E. Francisco Saez

#include "server_monitor_partidas.h"


MonitorPartidas::MonitorPartidas() {}

MonitorPartida*
MonitorPartidas::crear_partida(Queue<std::string> &cola_jugador,
                               std::uint32_t& codigo_partida) {
    MonitorPartida* sala = new MonitorPartida(cola_jugador);
    std::lock_guard<std::mutex> lck(mutex_map);
    codigo_partida = codigo_unico++;
    partidas[codigo_partida] = sala;
    return sala;
}

MonitorPartida*
MonitorPartidas::unirse_a_partida(Queue<std::string> &cola_jugador,
                                       std::uint32_t codigo_partida) {
    std::lock_guard<std::mutex> lck(mutex_map);
    int existe = partidas.count(codigo_partida);
    if (existe == 0) {
        return NULL;
    }
    partidas[codigo_partida]->unir_jugador(cola_jugador);
    return partidas[codigo_partida];
}

MonitorPartidas::~MonitorPartidas() {
    std::lock_guard<std::mutex> lck(mutex_map);
    for (auto it = partidas.begin(); it != partidas.end(); ++it) {
        delete it->second;
    }
    partidas.clear();
}
