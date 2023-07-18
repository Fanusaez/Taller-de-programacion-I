// "Copyright 2023 E. Francisco Saez

#ifndef SERVER_CLIENTE_RECIBIDOR_H_
#define SERVER_CLIENTE_RECIBIDOR_H_


#include <atomic>
#include "server_thread.h"
#include "server_protocolo.h"

class ClienteRecibidor : public Thread {
 private:
    ProtocoloServer& protocolo;
    std::atomic<bool> sigo_vivo{true};

 public:
explicit ClienteRecibidor(ProtocoloServer& protocolo);

/*
 * Loop donde el hilo CLieneteRecibidor llama a recibir_mensaje()
 * de la clase protocolo
 */
void run() override;

/*
 * Retorna un booleano.
 * En caso de ser verdadero el hilo sigue activo
 * De lo contrario, el hilo esta muerto.
 */
bool sigue_vivo() override;

/*
 * Pone el booleano sigo_vivo del hilo en false.
 */
void kill() override;

/*
 * Deshabilitamos copias
 */
ClienteRecibidor(const ClienteRecibidor&) = delete;
ClienteRecibidor& operator=(const ClienteRecibidor&) = delete;
};



#endif  // SERVER_CLIENTE_RECIBIDOR_H_
