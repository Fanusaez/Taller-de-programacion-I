// "Copyright 2023 E. Francisco Saez

#include <algorithm>
#include <utility>
#include <string>
#include "server_aceptador.h"
#include "server_thread.h"
#include "server_protocolo.h"
#include "server_cliente.h"

Acceptador::Acceptador(std::string& servname) :
        skt_escuchador(Socket(servname.c_str())) {}

void Acceptador::run() { try {
        // cosas que son "unicas"
        MonitorPartidas monitor_partidas;

        while (sigo_vivo) {
            Socket acceptado = skt_escuchador.accept();
            lanzar_hilo_manejador(std::move(acceptado),
                                  monitor_partidas);
            limpiar_clientes();
        }
    } catch (const std::exception& err) {
        if (sigo_vivo) {
            std::cerr << "Unexpected exception: " << err.what() << "\n";
        }
        eliminar_todos_clientes();
        sigo_vivo = false;
    }
}

void Acceptador::lanzar_hilo_manejador(
                Socket skt_cliente,
                MonitorPartidas &monitor_partidas) {
    Thread *t = new Jugador(std::move(skt_cliente),
            monitor_partidas);
    threads.push_back(t);
    t -> start();
}

void Acceptador::limpiar_clientes() {
    auto terminado = [](Thread* thread) {
        if (!thread->sigue_vivo()) {
            thread->kill();
            thread->join();
            delete thread;
            return true;
        }
        return false;
    };
    threads.erase(std::remove_if(threads.begin(),
                                 threads.end(), terminado), threads.end());
}

void Acceptador::eliminar_todos_clientes() {
    for (Thread* cliente : threads) {
        cliente->kill();
        cliente->join();
        delete cliente;
    }
    threads.clear();
}

void Acceptador::kill() {
    sigo_vivo = false;
    skt_escuchador.shutdown(2);
    skt_escuchador.close();
}

bool Acceptador::sigue_vivo() {
    return true;
}
