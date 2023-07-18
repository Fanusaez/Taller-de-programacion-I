// "Copyright 2023 E. Francisco Saez

#ifndef SERVER_SOLDIER_STATUS_H_
#define SERVER_SOLDIER_STATUS_H_

#include <vector>
#include <cstdint>
#include "common_impresor_estado.h"

class Soldado {
 private:
    std::int8_t movimiento_x = 0;
    std::int8_t movimiento_y = 0;
    const std::uint16_t limite_mapa_x;
    const std::uint16_t limite_mapa_y;
    std::uint8_t estado = 0;
    std::vector<std::uint16_t> atributos_jugador = {0, 0, 10};
    ImpresorEstado impresor_estado;

    static constexpr int POS_X = 0;
    static constexpr int POS_Y = 1;
    static constexpr int BALAS = 2;
    static constexpr std::uint8_t INACTIVO = 0;
    static constexpr std::uint8_t DISPARANDO = 1;
    static constexpr std::uint8_t MOVIENDOSE = 2;
    static constexpr std::uint8_t DISPARANDO_MOVIENDOSE = 3;
    static constexpr std::uint8_t RECARGANDO = 4;

/*
 * Recibe numero (1 para disparar, 0 para no)
 * Se cambia de estado depen:string &linea  [runtime/references] [2]
client_protocol.h:70:  Is this a non-const reference? If so, make const odiendo del estado actual y otros factores
 * Llama a la funcion realizar_acciones() para realizar accion correspondiente al estado nuevo
 */
void disparar(const std::uint8_t &disparando);

/*
 * Recibe dos numeros (moimiento_en_x, movimiento_en_y)
 * Se cambia de estado dependiendo de la posicion actual y el estado actual
 * En caso de que el movimiento sea invalido, el jugador no estara mas en movimiento
 * Llama a la funcion realizar_acciones() para realizar accion correspondiente al estado nuevo
 */
void mover(std::int8_t x, std::int8_t y);

/*
 * Resta 1 bala al juador
 */
void restar_bala();

/*
 * Recibe el rango maximo de movimiento, la posicion del jugador y la direccion a moverse
 * En caso de que el jugador no se pueda movera dicha direccion, se retorna false
 * de lo contrario true.
 */
bool en_rango(const std::uint16_t &rango_maximo,
                  const std::uint16_t &posicion,
                  const int &direccion);

/*
 * Cambia el estado a "recargando" y despues llama a realizar_acciones()
 */
void reload();

/*
 * En caso de estar en estado "moviendose" o "disparando_moviendose" se llama a mover(...) y termina.
 * De lo contrario llama a realizar_acciones()
 */
void nop();

/*
 * Opera sobre los atributos dependiendo del estado que el jugador se encuentra
 * Ejemplo : si estado == disparando, resta balas al jugador y cambia de estado
 * en caso de ser necesario.
 */
void realizar_acciones();

 public:
/*
 * Constructor
 */
explicit Soldado(const std::uint16_t& max_x, const std::uint16_t& max_y);

/*
 * Devuelve en un vector el estado en que se encuentra el jugador al momento de la llamada
 *  de la forma (estado_actual, pos_en_x, pos_en_y, balas_restantes)
 */
std::vector<std::uint16_t> obtener_estado_juego();

/*
 * Llama al metodo imprimir estado de ImpresorEstado
 */
void imprimir_estado();

/*
 * Recibe un vector con una accion serializada en bytes a ser ejecutada por el "soldado".
 * La deserializa y segun esta, ordena al soldado que accion realizar.
 */
void realizar_accion(const std::vector<std::int8_t>& accion);

/*
 * Prohibo copias
 */
Soldado(const Soldado&) = delete;
Soldado& operator=(const Soldado&) = delete;
};
#endif  // SERVER_SOLDIER_STATUS_H_
