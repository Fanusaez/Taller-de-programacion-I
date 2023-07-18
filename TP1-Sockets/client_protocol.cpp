// "Copyright 2023 E. Francisco Saez

#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <sstream>
#include <algorithm>
#include "client_protocol.h"
#include "common_liberror.h"
#include "common_impresor_estado.h"


ProtocoloCliente::ProtocoloCliente(const std::string& hostname,
                                   const std::string& servname) :
    hostname(hostname),
    servname(servname),
    skt(hostname.c_str(), servname.c_str()),
    impresor_estado() {
    actions["Shooting"] =   0x01;
    actions["Moving"]   =   0x02;
    actions["NOP"]      =   0x00;
    actions["Reload"]   =   0x03;
    actions["0"]        =   0x00;
    actions["1"]        =   0x01;
    actions["-1"]       =   -1;
}

void ProtocoloCliente::enviar_mensaje(const std::string &linea) {
    bool was_closed = false;
    std::vector<std::string> tokens = tokenizar_linea(linea);
    std::vector<int8_t> accion_a_enviar = serializar_acciones(tokens);
    int tamanio_en_bytes = sizeof(int8_t) * accion_a_enviar.size();
    skt.sendall(accion_a_enviar.data(), tamanio_en_bytes, &was_closed);
    if (was_closed) {
        throw LibError(errno, "No se pudo enviar el mensaje del cliente, "
                              "el socket del server se cerro");
    }
}

void ProtocoloCliente::recibir_mensaje() {
    std::vector<char> buffer(CANT_BYTES_RECIBIR_POR_PROTOCOLO);
    bool was_closed = false;
    skt.recvall(buffer.data(), CANT_BYTES_RECIBIR_POR_PROTOCOLO, &was_closed);
    if (was_closed) {
        throw LibError(errno, "No se pudo recibir el mensaje del servidor\n");
    }
    std::vector<char> buffer_convertido =
            conversion_endianness(buffer);
    mostrar_mensaje(buffer_convertido);
}

std::vector<std::string> ProtocoloCliente::
tokenizar_linea(const std::string &linea) {
    std::stringstream ss(linea);
    std::vector<std::string> tokens;
    std::string token = "";

    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<int8_t> ProtocoloCliente::serializar_acciones(
         const std::vector<std::string> &tokens) {
    std::vector<int8_t> action_to_send;
    std::transform(tokens.begin(),
                   tokens.end(),
                   std::back_inserter(action_to_send),
                   [&](const std::string& token) {
                       return actions[token];
                   });
    return action_to_send;
}

std::vector<char> ProtocoloCliente::
conversion_endianness(const std::vector<char> &buffer) {
    std::vector<char> buffer_nuevo(CANT_BYTES_RECIBIR_POR_PROTOCOLO);

    char* ptr;
    std::uint16_t* ptr_elem;

    buffer_nuevo[0] = buffer[0];

    ptr = &buffer_nuevo.at(1);
    ptr_elem = (std::uint16_t*) &buffer[1];
    *ptr = ntohs(*ptr_elem);

    ptr = &buffer_nuevo.at(3);
    ptr_elem = (std::uint16_t*)&buffer[3];
    *ptr = ntohs(*ptr_elem);

    ptr = &buffer_nuevo.at(5);
    ptr_elem =(std::uint16_t*)&buffer[5];
    *ptr = ntohs(*ptr_elem);

    return buffer_nuevo;
}

void ProtocoloCliente::mostrar_mensaje(const std::vector<char> &mensaje) {
    std::uint8_t estado = (std::uint16_t) (mensaje[0]);
    std::uint16_t x = mensaje[1];
    std::uint16_t y = mensaje[3];
    std::uint16_t balas = mensaje[5];
    impresor_estado.imprimir_estado(estado, x, y, balas);
}
