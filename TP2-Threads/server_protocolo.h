// "Copyright 2023 E. Francisco Saez

#ifndef SERVER_PROTOCOLO_H_
#define SERVER_PROTOCOLO_H_

#include <string>
#include <vector>
#include <atomic>
#include "common_socket.h"
#include "server_queue.h"
#include "server_juego.h"

class ProtocoloServer {
 private:
    Socket skt;
    Juego& juego;
    std::atomic<bool> sigo_vivo{true};
    Queue<std::string>& cola_cliente;

/*
 * Devuelve los bytes restantes a leer del socket
 */
std::int16_t obtener_bytes_restantes(std::vector<char>& buffer);

/*
 * Convierte el endianness del mensjae join
 */
void convertir_endianness_join(const std::vector<char>& buffer_viejo,
                               std::vector<char>& buffer_nuevo);
/*
 * Recibe un buffer, en caso de ser para broadcastear, llama a
 * obtener_mensaje_broadcast que devuelve el string a broadcastear.
 * Llama al metodo broadcastear de Juego.
 */
void realizar_accion(std::vector<char>& buffer);

/*
 * Envia el codigo asignado de la partida
 * Son 4 bytes
 */
void enviar_codigo_partida(std::uint32_t codigo);

/*
 * Envia el mensaje de confirmacion de union.
 * 00 en caso de exito, 01 en caso de error.
 */
void enviar_mensaje_join(std::uint8_t confirmacion);

/*
 * Recibe mensajes variantes de tamanio en bytes.
 * Ejemplos de estos son create y broadcast
 */
void recibir_mensaje_variante(std::vector<char>& buffer);

/*
 * Recibe un buffer el cual lo deserializa para obtener el mensaje a broadcastear.
 * Devuelve un string con el mensaje a broadcastear.
 */
std::string obtener_mensaje_broadcast(std::vector<char>& buffer);

/*
 * Genera el buffer serializado como broadcast para ser enviado.
 */
void crear_mensaje_broadcast(std::vector<char>& buffer,
                             std::string& mensaje);

/*
 * Recibe un buffer serializado con las acciones create o join.
 * En base a cual sea, la deserializa y delega a juego.
 * Envia a cliente por el socket lo que juego retorne despues de serializarlo.
 */
void accion_handshake(std::vector<char>& buffer);

 public:
ProtocoloServer(Socket skt_cliente,
                Juego& juego,
                Queue<std::string>& cola_cliente);

/*
 * Recibe por el socket un mensaje para unirse o crear una nueva partida.
 * Llama al metodo accion_handshake para que realice la accion.
 */
void handshake();

/*
 * Realiza un pop de la queue del jugador y llama al metodo crear_mensaje_broadcast
 * Una vez creado el mensaje lo envia por el socket al cliente
 */
void enviar_mensaje();

/*
 * Recibe un mensaje para ser broadcasteado.
 * Llama al metodo realizar_accion
 */
void recibir_mensaje();

/*
 * Retorna el estado del server
 */
bool sigue_vivo();

/*
 * Cierra el socket del protocolo
 */
void cerrar_todo();

/*
 *
 */
~ProtocoloServer();

/*
 * Deshabilitamos copias
 */
ProtocoloServer(const ProtocoloServer&) = delete;
ProtocoloServer& operator=(const ProtocoloServer&) = delete;
};

#endif  // SERVER_PROTOCOLO_H_
