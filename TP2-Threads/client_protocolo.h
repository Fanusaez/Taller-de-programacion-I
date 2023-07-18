// "Copyright 2023 E. Francisco Saez

#ifndef CLIENT_PROTOCOLO_H_
#define CLIENT_PROTOCOLO_H_

#include <map>
#include <vector>
#include <string>
#include "common_socket.h"

class ProtocoloCliente {
 private:
    Socket skt;
    std::map<std::string, std::uint8_t> accion;
/*
 * Genera los mensajes variantes en tamanio a enviar por el socket.
 * Por ejemplo create y broadcast
 * El resultado se almacena en el buffer pasado como referencia
 */
void crear_mensaje_variante(std::string& mensaje,
                            std::uint8_t accion,
                            std::vector<char> &buffer);

/*
 * Genera el mensaje de la accion join
 * El resultado se almacena en el buffer pasado como referencia
 */
void crear_mensaje_join(std::uint32_t partida, std::vector<char> &buffer);

/*
 * Obtiene los bytes restantes del a recibir a traves del socket
 */
std::int16_t obtener_bytes_restantes(std::vector<char>& buffer);

/*
 * Recibe la confirmacion de habarse unido erronea o exitosamente
 * a la partida, el mensaje es 1 byte.
 */
std::uint8_t recibir_confirmacion_partida();

/*
 * Recibe el numero de partida asignado por el server
 * El numero es un entero  sin signo de 4 bytes.
 */
std::uint32_t recibir_numero_partida();

 public:
ProtocoloCliente(const std::string& hostname, const std::string& servname);

/*
 * Recibe el nombre de la partida y llama al metodo crear_mensaje_variante.
 * Una vez obtenido el mensjae lo envia por el socket
 */
std::uint32_t crear_partida(std::string nombre_partida);

/*
 * Recibe el numero de partida a unirse, llama al metodo crear_mensaje_join
 * Una vez obtenido el mensjae lo envia por el socket
 */
std::uint8_t unirse_partida(std::uint32_t partida);

/*
 * Recibe un string a ser broadcasteado y llama al metodo crear_mensaje_variante.
 * Una vez obtenido el mensaje lo envia por el socket.
 */
void broadcastear(std::string& mensaje);

/*
 * Recibe por el socket el broadcast, lo decodifica y devuelve un string
 * conteniendo el mensaje a comunicar.
 * En caso de no haber ningun mensaje, el programa se quedara esperando uno.
 */
std::string obtener_mensaje_broadcast();

/*
 * Cierra el socker del cliente
 */
void cerrar_conexion();

/*
 * Deshabilitamos copias
 */
ProtocoloCliente(const ProtocoloCliente&) = delete;
ProtocoloCliente& operator=(const ProtocoloCliente&) = delete;
};

#endif  // CLIENT_PROTOCOLO_H_
