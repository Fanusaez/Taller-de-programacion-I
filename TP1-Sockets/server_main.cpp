// "Copyright 2023 E. Francisco Saez

#include <string>
#include <iostream>
#include "server_protocol.h"

int main(int argc, char* argv[]) { try {
        if (argc == 4) {
                std::string servname = std::string(argv[1]);
                std::string max_x = std::string(argv[2]);
                std::string max_y = std::string(argv[3]);

                std::uint16_t max_num_x = std::stoi(max_x);
                std::uint16_t max_num_y = std::stoi(max_y);

                ProtocoloServer protocolo_server
                (servname, max_num_x, max_num_y);
                protocolo_server.iniciar_comunicacion();
                return 0;
        } else {
            std::cerr << "Bad program call. Expected "
                      << argv[0]
                      << "<servername> <limitemapa_x> <limitemapa_y>\n";
            return -1;
        }
    }  catch (const std::exception& err) {
        std::cerr
                << "Something went wrong and an exception was caught: "
                << err.what()
                << "\n";
        return -1;
    } catch (...) {
        std::cerr << "Something went wrong and "
                     "an unknown exception was caught.\n";
        return -1;
    }
}


