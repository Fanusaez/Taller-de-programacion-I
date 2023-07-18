// "Copyright 2023 E. Francisco Saez

#include <iostream>
#include <fstream>
#include "client_protocol.h"

int main(int argc, char* argv[]) { try {
        if (argc == 4) {
            const std::string hostname = std::string(argv[1]);
            const std::string servname = std::string(argv[2]);
            std::ifstream input(argv[3]);
            if (!input) {
                return -1;
            }
            std::string linea;
            ProtocoloCliente protocolo_host(hostname, servname);
            while (std::getline(input, linea)) {
                if (!linea.empty() && linea[0] != '#') {
                    protocolo_host.enviar_mensaje(linea);
                    protocolo_host.recibir_mensaje();
                }
            }
            input.close();
            return 0;
        } else {
            std::cerr << "Bad program call. Expected "
                      << argv[0]
                      << " <hostname> <servername> <actions.txt>\n";
            return -1;
        }
    } catch (const std::exception& err) {
            std::cerr
                    << "Something went wrong and an exception was caught: "
                    << err.what()
                    << "\n";
            return -1;
        } catch (...) {
            std::cerr << "Something went wrong and an"
                         " unknown exception was caught.\n";
            return -1;
        }
}

