// "Copyright 2023 E. Francisco Saez

#include <string>
#include <iostream>
#include <vector>
#include "server_servidor.h"

int main(int argc, char* argv[]) { try {
        if (argc == 2) {
            std::string servname = std::string(argv[1]);
            Servidor servidor(servname);
            servidor.run();
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


