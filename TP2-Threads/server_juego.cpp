// "Copyright 2023 E. Francisco Saez

#include <netinet/in.h>
#include <iostream>
#include <vector>
#include "server_juego.h"

#define EXITO 0
#define FRACASO 1


Juego::Juego(MonitorPartidas &monitor_partidas) :
                               monitor_partidas(monitor_partidas) {}


std::uint32_t
Juego::crear_partida(Queue<std::string> &cola_jugador) {
    monitor_partida =
            monitor_partidas.crear_partida(cola_jugador,
                                           codigo_partida);
    std::string mensaje = "Created match: ";
    imprimir(mensaje, codigo_partida);
    return codigo_partida;
}

bool Juego::unir_a_partida(Queue<std::string> &cola_jugador,
                                    std::uint32_t codigo_partida_join) {
    monitor_partida = monitor_partidas.unirse_a_partida(cola_jugador,
                                                   codigo_partida_join);
    if (!monitor_partida) {
        std::string mensaje = "Match does not exist: ";
        imprimir(mensaje, codigo_partida_join);
        return false;
    } else {
        codigo_partida = codigo_partida_join;
        std::string mensaje = "Joined to match: ";
        imprimir(mensaje, codigo_partida);
        return true;
    }
}

void Juego::broadcastear(const std::string& broadcast) {
    monitor_partida->broadcastear(broadcast);
}

void Juego::imprimir(const std::string &mensaje,
                              const std::uint32_t& codigo) {
    std::string mensaje_completo =
            mensaje + std::to_string(codigo) + "\n";
    std::cout << mensaje_completo;
}
