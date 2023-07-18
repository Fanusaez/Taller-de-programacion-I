// "Copyright 2023 E. Francisco Saez

#include <iostream>
#include <sstream>
#include "client_protocolo.h"
#include "client_cliente.h"

Cliente::Cliente(const std::string& hostname, const std::string& servname):
        protocolo_cliente(hostname, servname) {}

void Cliente::run() {
    bool seguir_leyendo = true;
    while (seguir_leyendo) {
        std::string input;
        std::getline(std::cin, input);
        std::vector<std::string> mensaje;
        std::string token;
        std::stringstream ss(input);
        while (std::getline(ss, token, ' ')) {
            if (token == "leave") {
                seguir_leyendo = false;
                protocolo_cliente.cerrar_conexion();
                break;
            }
            mensaje.push_back(token);
        }
        if (seguir_leyendo) {
            enviar_mensaje(mensaje);
        }
    }
}

void Cliente::enviar_mensaje(std::vector<std::string> &mensaje) {
    if (mensaje[0] == "create") {
        std::uint32_t codigo = protocolo_cliente.crear_partida(mensaje[1]);
        std::string mensaje_imprimir
        = "Created match: " + std::to_string(codigo) + "\n";
        std::cout << mensaje_imprimir;
    } else if (mensaje[0] == "broadcast") {
        protocolo_cliente.broadcastear(mensaje[1]);
    } else if (mensaje[0] == "join") {
        std::uint32_t codigo = std::stoul(mensaje[1]);
        std::uint8_t confirmacion = protocolo_cliente.unirse_partida(codigo);
        imprimir_join(codigo, confirmacion);
    } else if (mensaje[0] == "read") {
        leer_broadcast(mensaje);
    }
}

void Cliente::imprimir_join(std::uint32_t codigo, std::uint8_t confirmacion) {
    if (confirmacion == 0) {
        std::string mensaje =
                "Joined to match: " +  std::to_string(codigo) + "\n";
        std::cout << mensaje;
    } else {
        std::string mensaje =
                "Match does not exist: " +  std::to_string(codigo) + "\n";
        std::cout << mensaje;
    }
}

void Cliente::leer_broadcast(const std::vector<std::string> &mensaje) {
    int cant_broadcast = std::stoul(mensaje[1]);
    for (int i = 0; i < cant_broadcast; i++) {
        std::string broadcast = protocolo_cliente.obtener_mensaje_broadcast();
        std::string mensaje_imprimir = "Broadcasted: " + broadcast + "\n";
        std::cout << mensaje_imprimir;
    }
}
