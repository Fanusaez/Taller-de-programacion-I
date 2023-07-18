// "Copyright 2023 E. Francisco Saez

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "client_cliente.h"

int main(int argc, char* argv[]) { try {
        if (argc == 3) {
            const std::string hostname = std::string(argv[1]);
            const std::string servname = std::string(argv[2]);
            Cliente cliente(hostname, servname);
            cliente.run();
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

