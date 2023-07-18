// "Copyright 2023 E. Francisco Saez

#include "server_cliente_recibidor.h"

ClienteRecibidor::ClienteRecibidor(ProtocoloServer& protocolo) :
    protocolo(protocolo) {}


void ClienteRecibidor::run() { try {
        while (sigo_vivo) {
            protocolo.recibir_mensaje();
            sigo_vivo = protocolo.sigue_vivo();
        }
    } catch (const std::exception& err) {
        if (sigo_vivo) {
            std::cerr << "Unexpected exception: " << err.what() << "\n";
        }
    }
}


bool ClienteRecibidor::sigue_vivo() {
    return sigo_vivo;
}

void ClienteRecibidor::kill() {
    sigo_vivo = false;
}

