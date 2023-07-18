// "Copyright 2023 E. Francisco Saez

#include <utility>
#include "server_cliente.h"
#include "server_cliente_recibidor.h"

Jugador::Jugador(Socket skt_jugador,
                 MonitorPartidas &monitor_partidas) :
        cola_jugador(1000),
        juego(monitor_partidas),
        protocolo(std::move(skt_jugador),
                  juego,
                  cola_jugador) {}

void Jugador::run() {
    protocolo.handshake();
    ClienteRecibidor thread_recibidor(protocolo);
    thread_recibidor.start();
    enviar_mensaje();
    thread_recibidor.join();
}

void Jugador::enviar_mensaje() { try {
        while (sigo_vivo) {
            protocolo.enviar_mensaje();
            sigo_vivo = protocolo.sigue_vivo();
        }
    } catch (const std::exception& err) {
        if (sigo_vivo) {
            std::cerr << "Unexpected exception: " << err.what() << "\n";
        }
    }
}

bool Jugador::sigue_vivo() {
    return protocolo.sigue_vivo();
}

void Jugador::kill() {
    cola_jugador.close();
    sigo_vivo = false;
    protocolo.cerrar_todo();
}
