// "Copyright 2023 E. Francisco Saez

#ifndef CLIENT_CLIENTE_H_
#define CLIENT_CLIENTE_H_

#include <vector>
#include <string>
#include "client_protocolo.h"

class Cliente{
 private:
    ProtocoloCliente protocolo_cliente;
/*
 * Recibe un vector de stings y dependiendo del primer elemento del
 * vector, realizara alguna de las 4 acciones posibles delegando al protocolo.
 *  Las acciones son: create, join, broadcast, read
 */
void enviar_mensaje(std::vector<std::string>& mensaje);

/*
 * Imprime el resultado de la accion join
 */
void imprimir_join(std::uint32_t codigo, std::uint8_t confirmacion);

/*
 * Recibe un string con la cantidad de mensajes a leer, los obtiene a
 * traves del protocolo y los imprime por pantalla
 */
void leer_broadcast(const std::vector<std::string>& mensaje);

 public:
Cliente(const std::string& hostname, const std::string& servname);

/*
 * Ejecuta un loop en donde se ejecuta el cliente, leyendo el teclado por entrada
 * estandar
 */
void run();

/*
 * Deshabilitamos copias
 */
Cliente(const Cliente&) = delete;
Cliente& operator=(const Cliente&) = delete;
};

#endif  // CLIENT_CLIENTE_H_
