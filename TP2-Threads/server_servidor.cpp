// "Copyright 2023 E. Francisco Saez

#include "server_servidor.h"
#include "server_aceptador.h"

Servidor::Servidor(std::string &servname) :
    servname(servname) {}

void Servidor::run() {
    Acceptador thread_acceptador(servname);
    thread_acceptador.start();
    while (std::cin.get() != 'q') {}
    thread_acceptador.kill();
    thread_acceptador.join();
}
