// "Copyright 2023 E. Francisco Saez

#ifndef SERVER_PROTOCOL_H_
#define SERVER_PROTOCOL_H_

#include <vector>
#include <string>
#include "common_socket.h"
#include "server_soldier_status.h"

class ProtocoloServer {
 private:
    const std::string servname;
    Socket skt;
    Soldado soldado;

    static constexpr std::uint8_t
    CANT_BYTES_ENVIAR_POR_PROTOCOLO = 7;
    static constexpr std::uint8_t
    CANT_BYTES_MAX_RECIBIR = 3;

/*
 * Se encarga de enviar a traves del socket un mensaje de 7 bytes
 * el cual lo obtiene a traves del soldado con obtener_estado_juego()
 * y con el metodo serializar_mensaje().
 * En caso de que el socket este cerrado, se lanza una excepcion
 *
 */
void enviar_mensaje();

/* Recibe un puntero a una variable booleana server_vivo
 * Se encarga de recibir a traves del socket un mensaje de n bytes
 * Una vez que recibe el primer byte, sabe cuanto sera la cantidad exacta
 * de bytes a recibir
 * En caso de que el socket este cerrado, pone server_vivo en falso
 * En caso de que no se reciba la cantidad correcta de bytes
 * arroja una excepcion
 */
void recibir_mensaje(bool *server_vivo);

/*
 * Delega al soldado la responsabilidad de imprimir el
 * estado de juego por pantalla
 */
void imprimir_estado_juego();

/*
 * Recibe un vector y a cada uno de sus elementos
 * (excepto al primero) le aplica la funcion htons
 * y lo coloca en otro buffer que sera devuelto
 */
std::vector<char> serializar_mensaje(const std::vector<std::uint16_t>& mensaje);

/*
 * Llama a la funcion accept() de la clase Socket
 */
Socket accept();

/*
 * Recibe una referencia a un buffer y devuelve la cantidad
 * de bytes que faltan recibir por protocolo
 */
int obtener_bytes_restantes(const std::vector<std::int8_t> &buffer);

 public:
/*
 * Constructor de Protocolo server, recibe el nombre del
 * server tambien el tamanio del mapa e el eje x y eje y.
 */
ProtocoloServer(const std::string &servname,
                const std::uint16_t& tamanio_mapa_x,
                const std::uint16_t& tamanio_mapa_y);

/*
 * Acepta un cliente por el socket de escucha y entra a un loop
 * llamando a recibir_mensaje, enviar_mensaje e imprimir estado_de_juego
 * hasta que el socker se cierre y la conexion haya terminado.
 */
int iniciar_comunicacion();

/*
 * Deshabilito copias
 */
ProtocoloServer(const ProtocoloServer&) = delete;
ProtocoloServer& operator=(const ProtocoloServer&) = delete;
};

#endif  //  SERVER_PROTOCOL_H_

