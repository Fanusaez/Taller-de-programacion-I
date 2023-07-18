// "Copyright 2023 E. Francisco Saez

#ifndef CLIENT_PROTOCOL_H_
#define CLIENT_PROTOCOL_H_

#include <string>
#include <map>
#include <vector>
#include "common_socket.h"
#include "common_impresor_estado.h"

class ProtocoloCliente {
 private:
    const std::string hostname;
    const std::string servname;
    Socket skt;
    ImpresorEstado impresor_estado;
    std::map<std::string, signed char> actions;

    static constexpr std::uint8_t CANT_BYTES_RECIBIR_POR_PROTOCOLO = 7;
/*
 * Recibe un string y lo separa en tokens por el caracter espacio
 * devuelve un vector con los tokens en el mismo.
 */
std::vector<std::string> tokenizar_linea(const std::string &linea);

/*
 * Recibe un vector con tokens/palabras
 * siendo cada una de estas una "key" del diccionario acciones de la clase.
 * Se devuelve un vector std::vector<std::int8_t> con los
 * valores de dichas claves/tokens.
 */
std::vector<std::int8_t> serializar_acciones(
        const std::vector<std::string> &tokens);

/*
 * Recibe un vector y a cada uno de estos elementos los les aplica
 * la funcion ntohs (excepto al primer elemento que es de 1 byte).
 * Devuelve otro vector con los elementos tranformados.
 */
std::vector<char> conversion_endianness(const std::vector<char> &buffer);

/*
 * Recibe un vector con los informacion sobre
 * el estado, posiciones, y balas.
 * Las almacena en variables y utiliza para llamar a
 * imprimir_estado(...) de la clase ImpresorEstado .
 */
void mostrar_mensaje(const std::vector<char> &mensaje);

 public:
/*
 * Constructor de la clase Protocolo cliente
 */
ProtocoloCliente(const std::string& hostname, const std::string& servname);

/*
 * Genera un vector en donde almacena los bytes recibidos a traves del socket
 * En caso de no recibir la cantidad correspondientes de bytes lanzara una excecion
 * Llama a la funcion conversion_endianness y a mostrar mensaje.
 * Retorna 0
 */
void recibir_mensaje();

/*
 * Recibe un string, llama a las funciones tokenizar_linea y serializar_acciones
 * posteriormente envia a traves del socket el buffer con un mensaje de accion
 * Retorna cero si salio bien
 */
void enviar_mensaje(const std::string& linea);

/*
 * Deshabilito copias
 */
ProtocoloCliente(const ProtocoloCliente&) = delete;
ProtocoloCliente& operator=(const ProtocoloCliente&) = delete;
};
#endif  //  CLIENT_PROTOCOL_H_
