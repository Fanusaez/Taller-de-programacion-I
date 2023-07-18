// "Copyright 2023 E. Francisco Saez

#include <iostream>
#include <map>
#include <string>
#include "server_soldier_status.h"
#include "common_liberror.h"

Soldado::Soldado(const std::uint16_t& max_x, const std::uint16_t& max_y) :
                limite_mapa_x(max_x), limite_mapa_y(max_y),
                impresor_estado() {}

void Soldado::realizar_accion(const std::vector<std::int8_t>& accion) {
    if (accion[0] == 0x00) {
        nop();
    } else if (accion[0] == 0x01) {
        std::uint8_t disparo = accion[1];
        disparar(disparo);
    } else if (accion[0] == 0x02) {
        int8_t mover_x = accion[1];
        int8_t mover_y = accion[2];
        mover(mover_x, mover_y);
    } else if (accion[0] == 0x03) {
        reload();
    } else {
        throw LibError(errno, "La accion a realizar no existe\n");
    }
}

void Soldado::realizar_acciones() {
    if (estado == INACTIVO) {
        return;
    }
    if (estado == DISPARANDO) {
        if (atributos_jugador[BALAS] == 0) {
            estado = 0;
            return;
        }
        restar_bala();
    } else if (estado == MOVIENDOSE) {
        atributos_jugador[POS_X] += movimiento_x;
        atributos_jugador[POS_Y] += movimiento_y;
        return;
    } else if (estado == DISPARANDO_MOVIENDOSE) {
        if (atributos_jugador[BALAS] == 0) {
            estado = MOVIENDOSE;
        }
        if (estado == DISPARANDO_MOVIENDOSE) restar_bala();
        atributos_jugador[POS_X] += movimiento_x;
        atributos_jugador[POS_Y] += movimiento_y;
    } else if (estado == RECARGANDO) {
        atributos_jugador[BALAS] = 10;
        estado = 0;
    }
}

void Soldado::disparar(const std::uint8_t& disparando) {
    if ((estado == INACTIVO  || estado == DISPARANDO)
    && (atributos_jugador[BALAS] > 0)) {
        estado = disparando;
    } else if (estado == DISPARANDO && (atributos_jugador[BALAS] == 0)) {
        estado = INACTIVO;
    } else if (estado == MOVIENDOSE &&
                (disparando == 1 || atributos_jugador[BALAS] > 0)) {
        estado = DISPARANDO_MOVIENDOSE;
    } else if (estado == DISPARANDO_MOVIENDOSE && disparando == 0) {
        estado = MOVIENDOSE;
    }
    realizar_acciones();
}

void Soldado::restar_bala() {
    atributos_jugador[BALAS] -= 1;
}

void Soldado::mover(std::int8_t x, std::int8_t y) {
    movimiento_x = x;
    movimiento_y = y;

    bool verifico_mov_x = en_rango(limite_mapa_x - 1, atributos_jugador[0], x);
    if (!verifico_mov_x) movimiento_x = 0;
    bool verifico_mov_y = en_rango(limite_mapa_y - 1, atributos_jugador[1], y);
    if (!verifico_mov_y) movimiento_y = 0;
    if (verifico_mov_x && verifico_mov_y && (x != 0 || y != 0)) {
        if (estado == INACTIVO) {
            estado = MOVIENDOSE;
        } else if (estado == DISPARANDO) {
            estado = DISPARANDO_MOVIENDOSE;
        }
    }
    if (!verifico_mov_x || !verifico_mov_y || (x == 0 && y == 0)) {
        if (estado == MOVIENDOSE) {
            atributos_jugador[POS_X] += movimiento_x;
            atributos_jugador[POS_Y] += movimiento_y;
            estado = INACTIVO;
        } else if (estado == DISPARANDO_MOVIENDOSE) {
            estado = DISPARANDO;
        }
    }
    realizar_acciones();
}

void Soldado::nop() {
    if (estado == MOVIENDOSE|| estado == DISPARANDO_MOVIENDOSE) {
        mover(movimiento_x, movimiento_y);
        return;
    }
    realizar_acciones();
}

void Soldado::reload() {
    estado = RECARGANDO;
    realizar_acciones();
}

bool Soldado::en_rango(const std::uint16_t &rango_maximo,
                       const std::uint16_t &posicion,
                       const int &direccion) {
    if ((rango_maximo == posicion) && (direccion > 0)) {
        return false;
    } else if (posicion == 0 && direccion < 0) {
        return false;
    }
    return true;
}

std::vector<std::uint16_t> Soldado::obtener_estado_juego() {
     std::vector<std::uint16_t> estados = {estado,
                                           atributos_jugador[POS_X],
                                           atributos_jugador[POS_Y],
                                           atributos_jugador[BALAS]};
     return estados;
}

void Soldado::imprimir_estado() {
    impresor_estado.imprimir_estado(estado,
                                    atributos_jugador[POS_X],
                                    atributos_jugador[POS_Y],
                                    atributos_jugador[BALAS]);
}
