// "Copyright 2023 E. Francisco Saez

#include <iostream>
#include <sstream>
#include <valarray>
#include <vector>
#include <utility>
#include <netinet/in.h>
#include "server_protocol.h"
#include "common_liberror.h"

ProtocoloServer::ProtocoloServer(
        const std::string &servname,
        const std::uint16_t &tamanio_mapa_x,
        const std::uint16_t &tamanio_mapa_y) :
    servname(servname),
    skt(servname.c_str()),
    soldado(tamanio_mapa_x, tamanio_mapa_y) {
}

int ProtocoloServer::iniciar_comunicacion() {
    Socket skt_cliente = this->accept();
    skt = std::move(skt_cliente);

    bool en_conexion = true;
    while (en_conexion) {
        recibir_mensaje(&en_conexion);
        if (!en_conexion) break;
        enviar_mensaje();
        imprimir_estado_juego();
    }
    return 0;
}

void ProtocoloServer::enviar_mensaje() {
    std::vector<std::uint16_t> estado_juego = soldado.obtener_estado_juego();
    std::vector<char> estado_serializado = serializar_mensaje(estado_juego);

    bool was_closed = false;
    skt.sendall(estado_serializado.data(), CANT_BYTES_ENVIAR_POR_PROTOCOLO,
                &was_closed);
    if (was_closed) {
        throw LibError(errno, "No se pudo enviar el mensaje desde el server, "
                              "el socket del cliente se cerro");
    }
}

void ProtocoloServer::recibir_mensaje(bool *server_vivo) {
    std::vector<std::int8_t> buffer(CANT_BYTES_MAX_RECIBIR);
    bool was_closed = false;
    unsigned int primer_byte = 1;
    //  Recibo primer byte
    skt.recvsome(buffer.data(), primer_byte, &was_closed);
    if (was_closed) {
        *server_vivo = false;
        return;
    }
    // Recibo el resto de byes, si es cero no pasa nada
    unsigned int nuevos_bytes_esperados = obtener_bytes_restantes(buffer);
    skt.recvall(buffer.data() + primer_byte, nuevos_bytes_esperados,
                &was_closed);

    if (was_closed) {
        *server_vivo = false;
        return;
    }
    soldado.realizar_accion(buffer);
}

int ProtocoloServer::obtener_bytes_restantes(
        const std::vector<std::int8_t> &buffer) {
    unsigned int nuevos_bytes_esperados;

    if (buffer[0] == 0x01) {
        nuevos_bytes_esperados = 1;
    } else if (buffer[0] == 0x02) {
        nuevos_bytes_esperados = 2;
    } else {
        nuevos_bytes_esperados = 0;
    }
    return nuevos_bytes_esperados;
}

Socket ProtocoloServer::accept() {
    return skt.accept();
}

void ProtocoloServer::imprimir_estado_juego() {
    soldado.imprimir_estado();
}

std::vector<char> ProtocoloServer::serializar_mensaje
(const std::vector<std::uint16_t>& buffer) {
    std::vector<char> buffer_serializado(
            buffer.size() * sizeof(std::uint16_t));

    buffer_serializado.at(0) = buffer[0];
    uint16_t *ptr;
    ptr = (uint16_t*) &buffer_serializado.at(1);
    *ptr = htons(buffer[1]);
    ptr = (uint16_t*) &buffer_serializado.at(3);
    *ptr = htons(buffer[2]);
    ptr = (uint16_t*) &buffer_serializado.at(5);
    *ptr = htons(buffer[3]);

    return buffer_serializado;
}
