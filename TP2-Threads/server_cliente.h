// "Copyright 2023 E. Francisco Saez

#ifndef SERVER_CLIENTE_H_
#define SERVER_CLIENTE_H_

#include <atomic>
#include <vector>
#include <string>
#include "server_thread.h"
#include "common_socket.h"
#include "server_monitor_partidas.h"
#include "server_protocolo.h"
#include "server_cliente_recibidor.h"

class Jugador : public Thread {
 private:
    std::atomic<bool> sigo_vivo{true};
    Queue<std::string> cola_jugador;
    Juego juego;
    ProtocoloServer protocolo;
/*
 * Loop donde el hilo Jugador llama a enviar_mensaje() de la clase protocolo
 */
void enviar_mensaje();

 public:
Jugador(Socket skt_jugador,
        MonitorPartidas& monitor_partidas);

/*
 * Metodo donde se hace el handshake con el cliete, y se lanza otro hilo encargado de recibir mensajes
 */
void run() override;

/*
 * Retorna un booleano.
 * En caso de ser verdadero el hilo sigue activo
 * De lo contrario, el hilo esta muerto.
 */
bool sigue_vivo() override;

/*
 * Se cierra la cola del jugador, se llama al metodo close() de la clase protocolo
 * y se pone el booleano sigo_vivo del hilo en false
 */
void kill() override;

/*
 * Deshabilitamos copias
 */
Jugador(const Jugador&) = delete;
Jugador& operator=(const Jugador&) = delete;
};
#endif  // SERVER_CLIENTE_H_
