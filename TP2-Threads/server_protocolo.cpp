// "Copyright 2023 E. Francisco Saez

#include <vector>
#include <netinet/in.h>
#include <iostream>
#include <utility>
#include <cstring>
#include "server_protocolo.h"
#include "common_liberror.h"

#define TAM_BUFF_INICIAL 3
#define BYTES_RESTANTES_JOIN 2
#define CANT_BYTES_CODIGO 4
#define CREAR_PARTIDA 0X01
#define UNIRSE_PARTIDA 0X02
#define BROADCASTEAR_CLIENTE 0X03
#define BROADCASTEAR_SERVER 0X04
#define EXITO 0
#define FRACASO 1

ProtocoloServer::ProtocoloServer(Socket skt_cliente,
                                 Juego& juego,
                                 Queue<std::string>& cola_cliente) :
    skt(std::move(skt_cliente)),
    juego(juego),
    cola_cliente(cola_cliente) {}

void ProtocoloServer::handshake() {
    std::vector<char> buffer;
    recibir_mensaje_variante(buffer);
    accion_handshake(buffer);
}

void ProtocoloServer::recibir_mensaje_variante(std::vector<char>& buffer) {
    buffer.resize((TAM_BUFF_INICIAL));
    bool was_closed = false;
    skt.recvall(buffer.data(), buffer.size(), &was_closed);
    if (was_closed) {
        sigo_vivo = false;
        return;
    }
    std::int16_t bytes_restantes = obtener_bytes_restantes(buffer);
    buffer.resize(TAM_BUFF_INICIAL + bytes_restantes);
    skt.recvall(buffer.data() + TAM_BUFF_INICIAL, bytes_restantes, &was_closed);
    if (was_closed) {
        sigo_vivo = false;
        return;
    }
}

void ProtocoloServer::accion_handshake(std::vector<char>& buffer) {
    std::uint8_t accion = buffer[0];
    if (accion == CREAR_PARTIDA) {
        std::uint32_t codigo =
                juego.crear_partida(cola_cliente);
        enviar_codigo_partida(codigo);
    } else if (accion == UNIRSE_PARTIDA) {
        std::vector<char> buffer_host(sizeof(buffer));
        convertir_endianness_join(buffer, buffer_host);
        std::uint32_t *ptr = (std::uint32_t*) &buffer_host.at(1);
        std::uint32_t codigo_cliente = *ptr;
        bool exito = juego.unir_a_partida(cola_cliente, codigo_cliente);
        if (exito) {
            enviar_mensaje_join(EXITO);
        } else {
            enviar_mensaje_join(FRACASO);
        }
    }
}

void ProtocoloServer::enviar_codigo_partida(std::uint32_t codigo) {
    std::vector<char> buffer(CANT_BYTES_CODIGO);
    std::uint32_t *ptr = (std::uint32_t*) &buffer.at(0);
    *ptr = htonl(codigo);
    bool was_closed = false;
    skt.sendall(buffer.data(), buffer.size(), &was_closed);
    if (was_closed) sigo_vivo = false;
}

void
ProtocoloServer::enviar_mensaje_join(std::uint8_t mensaje) {
    std::vector<char> buffer(1);
    buffer[0] = mensaje;
    bool was_closed = false;
    skt.sendall(buffer.data(), buffer.size(), &was_closed);
    if (was_closed) sigo_vivo = false;
}

void ProtocoloServer::
convertir_endianness_join(const std::vector<char> &buffer_viejo,
                          std::vector<char> &buffer_nuevo) {
    std::uint32_t *ptr = (std::uint32_t*) &buffer_viejo.at(1);
    std::uint32_t *ptr_nuevo = (std::uint32_t*) &buffer_nuevo[1];
    *ptr_nuevo = ntohl(*ptr);
}

void ProtocoloServer::enviar_mensaje() {
    std::string broadcast = cola_cliente.pop();
    std::vector<char> buffer_a_enviar;
    crear_mensaje_broadcast(buffer_a_enviar, broadcast);
    bool was_closed = false;
    skt.sendall(buffer_a_enviar.data(), buffer_a_enviar.size(), &was_closed);
    if (was_closed) sigo_vivo = false;
}

void ProtocoloServer::recibir_mensaje()  {
    std::vector<char> buffer;
    recibir_mensaje_variante(buffer);
    realizar_accion(buffer);
}

std::int16_t ProtocoloServer::
obtener_bytes_restantes(std::vector<char>& buffer) {
    if (buffer[0] == CREAR_PARTIDA || buffer[0] == BROADCASTEAR_CLIENTE) {
        std::uint16_t *ptr = (std::uint16_t*) &buffer.at(1);
        return ntohs(*ptr);
    }
    return BYTES_RESTANTES_JOIN;
}

void ProtocoloServer::realizar_accion(std::vector<char>& buffer) {
    if (buffer[0] == BROADCASTEAR_CLIENTE) {
        std::string broadcast = obtener_mensaje_broadcast(buffer);
        juego.broadcastear(broadcast);
    }
}

void ProtocoloServer::
crear_mensaje_broadcast(std::vector<char> &buffer, std::string& mensaje) {
    std::uint16_t bytes_escenario = mensaje.size();
    std::uint16_t bytes_totales =
            TAM_BUFF_INICIAL + bytes_escenario;

    buffer.resize(bytes_totales);
    buffer[0] = BROADCASTEAR_SERVER;
    uint16_t* ptr;

    ptr = (uint16_t*) &buffer.at(1);
    *ptr = htons(bytes_escenario);

    std::memcpy(&buffer[3], mensaje.c_str(), mensaje.size());
}

std::string ProtocoloServer::
obtener_mensaje_broadcast(std::vector<char>& buffer) {
    std::string broadcast(buffer.begin() + 3, buffer.end());
    return broadcast;
}

bool ProtocoloServer::sigue_vivo() {
    return sigo_vivo;
}

void ProtocoloServer::cerrar_todo() {
    sigo_vivo = false;
    skt.shutdown(2);
    skt.close();
}

ProtocoloServer::~ProtocoloServer() {}
