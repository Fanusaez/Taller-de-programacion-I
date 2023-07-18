// "Copyright 2023 E. Francisco Saez

#ifndef COMMON_IMPRESOR_ESTADO_H_
#define COMMON_IMPRESOR_ESTADO_H_

#include <cstdint>
#include <map>
#include <vector>
#include <string>

class ImpresorEstado {
 private:
/*
 * Recibe un numero que representa un estado y con este
 * generamos un diccionario.
 * Retorna el diccionario
 */
std::map<std::string, short int>
        crear_diccionario_estado(const std::uint8_t& estado);

 public:
ImpresorEstado();

/*
 * Recibe 4 numeros que representan el estado actualizado
 * del juego.
 * Imprime por pantalla el estado del juego
 */
void imprimir_estado(const std::uint8_t& estado,
                     const std::uint16_t& pos_x,
                     const std::uint16_t& pos_y,
                     const std::uint16_t& balas);
/*
 * Deshabilito copias
 */
ImpresorEstado(const ImpresorEstado&) = delete;
ImpresorEstado& operator=(const ImpresorEstado&) = delete;
};

#endif  //  COMMON_IMPRESOR_ESTADO_H_
