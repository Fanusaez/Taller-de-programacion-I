// "Copyright 2023 E. Francisco Saez

#ifndef SERVER_ACEPTADOR_H_
#define SERVER_ACEPTADOR_H_

#include <vector>
#include <atomic>
#include <string>
#include "server_thread.h"
#include "common_socket.h"
#include "server_monitor_partidas.h"

class Acceptador : public Thread {
 private:
    Socket skt_escuchador;
    std::atomic<bool> sigo_vivo{true};
    std::vector<Thread*> threads;

/*
 * Crea un Thread Jugador en el heap y llama a su metodo start.
 */
void lanzar_hilo_manejador(Socket skt_cliente,
                           MonitorPartidas &monitor_partidas);
/*
 * Recorre los clientes Jugadores, si estan muertos/inactivos llama a su
 * metodo kill y depsues join.
 * Finaliza liberando la memoria de los eliminados
 */
void limpiar_clientes();

/*
 * Elimina y libera la memoria de todos los clientes Jugadores
 */
void eliminar_todos_clientes();

/*
 * Devuelve el estado en que esta el hilo aceptador
 */
bool sigue_vivo() override;

 public:
explicit Acceptador(std::string& servname);

/*
 * Metodo donde se ejecuta el loop principal del hilo aceptador
 */
void run() override;

/*
 * Cierra el socket aceptador del hilo.
 */
void kill() override;

/*
 * Deshabilitamos copias
 */
Acceptador(const Acceptador&) = delete;
Acceptador& operator=(const Acceptador&) = delete;
};
#endif  // SERVER_ACEPTADOR_H_
