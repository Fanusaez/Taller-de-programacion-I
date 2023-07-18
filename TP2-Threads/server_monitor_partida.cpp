// "Copyright 2023 E. Francisco Saez

#include "server_monitor_partida.h"

MonitorPartida::MonitorPartida(Queue<std::string>& queue) {
    std::lock_guard<std::mutex> lck(mutex_partida);
    jugadores_partida.push_back(queue);
}

void MonitorPartida::unir_jugador(Queue<std::string> &queue) {
    jugadores_partida.push_back(queue);
}

void MonitorPartida::broadcastear(const std::string& broadcast) {
    std::lock_guard<std::mutex> lck(mutex_partida);
    auto cola_it = jugadores_partida.begin();
    while (cola_it != jugadores_partida.end()) {
        Queue<std::string>& cola = cola_it->get();
        try {
            cola.push(broadcast);
            ++cola_it;
        } catch (const std::exception& err) {
            cola_it = jugadores_partida.erase(cola_it);
        }
    }
}
