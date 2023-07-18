// "Copyright 2023 E. Francisco Saez

#ifndef SERVER_SERVIDOR_H_
#define SERVER_SERVIDOR_H_


#include <string>

class Servidor {
 private:
    std::string& servname;
 public:
explicit Servidor(std::string& servname);

/*
 * Lanza el hilo aceptador y espera por entrada estandar una "q" para finalizar el programa.
 */
void run();

/*
 * Deshabilitamos copias
 */
Servidor(const Servidor&) = delete;
Servidor& operator=(const Servidor&) = delete;
};

#endif  // SERVER_SERVIDOR_H_
