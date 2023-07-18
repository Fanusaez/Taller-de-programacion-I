// "Copyright 2023 E. Francisco Saez

#include <iostream>
#include <string>
#include "common_impresor_estado.h"

ImpresorEstado::ImpresorEstado() {}


void ImpresorEstado::imprimir_estado(const std::uint8_t &estado,
                                     const std::uint16_t& pos_x,
                                     const std::uint16_t& pos_y,
                                     const std::uint16_t& balas) {
    std::map<std::string, short int> flags_estados
    = crear_diccionario_estado(estado);
    std::cout <<
              "Shooting? " << flags_estados["Shooting"] << "\n" <<
              "Moving? "   << flags_estados["Moving"] << "\n" <<
              "Reloading? "<< flags_estados["Reloading"] << "\n" <<
              "Position? " << pos_x << " " <<
              pos_y << "\n" <<
              "Rounds? " << balas << "\n"
              << "\n";
}

std::map<std::string, short int> ImpresorEstado::
crear_diccionario_estado(const std::uint8_t &estado) {
    std::map<std::string, short int> flags_estados;
    flags_estados["Shooting"] = 0;
    flags_estados["Moving"] = 0;
    flags_estados["Reloading"] = 0;
    if (estado == 1) {
        flags_estados["Shooting"] = 1;
    } else if (estado == 2) {
        flags_estados["Moving"] = 1;
    } else if (estado == 3) {
        flags_estados["Shooting"] = 1;
        flags_estados["Moving"] = 1;
    }
    return flags_estados;
}
