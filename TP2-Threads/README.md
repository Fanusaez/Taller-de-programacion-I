# Taller de Programacion - TP Threads

Enunciado: [enunciado_threads.pdf](https://github.com/Taller-de-Programacion-TPs/threads-2023c1-Fanusaez/files/11431839/enunciado_threads.pdf)

## Modelo
Para el trabajo se requería implementar una comunicación entre cliente - servidor en donde los mismos se comunican a través de sockets.
El servidor debe poder soportar varios clientes a la vez y estar a la espera de nuevos clientes que deseen unirse.
El trabajo se dividió en dos partes:

### Cliente 

Esta parte es donde se lee de la entrada estándar las acciones que el cliente realiza y a través de la clase ProtocoloCliente podemos comunicar esta acción con el protocolo acordado para enviarlo al servidor.
Entre las acciones que podemos realizar se encuentran las de crear una nueva partida, unirse a alguna partida con un código de acceso o enviar un mensaje al grupo de chat de la partida.


![diagrama_clases_cliente](https://github.com/Taller-de-Programacion-TPs/threads-2023c1-Fanusaez/assets/79915723/fde38f3d-9ef6-4125-b615-d1cf909ef039)

_Imagen 1: Clases del modelo cliente_

### Servidor

El servidor lo podríamos dividir en dos, el *Aceptador* y  el *Jugador*.

El Aceptador se encarga de estar a la espera de nuevos clientes que quieran unirse a una partida o crear la misma. Este tiene la responsabilidad de lanzar nuevos hilos (Threads) para poder atender varios clientes simultáneamente.

Por el lado de thread *Jugador*, este es inicialmente un único thread, que después de entablar una comunicación inicial con el cliente, lanza otro thread *ClienteRecibidor*. Por lo que resta de la conexión, *Jugador* entra en un loop para enviar mensajes y *ClienteRecibidor* pare recibir mensajes que delegan en la clase *PrtocoloServer*

![diagr_clases_servidor_1](https://github.com/Taller-de-Programacion-TPs/threads-2023c1-Fanusaez/assets/79915723/d51e2aca-7562-4aa3-8ce0-547cf663576b)

_Imagen 2: Clases del modelo Servidor 1_

### Protocolo y Monitores

#### Handshake
Inicialmente el protocolo espera recibir mensajes para poder agregar a una partida al *Jugador* quien llamará al método handshake(), el protocolo creará/unirá a este jugador a una partida correspondiente, pasando por parámetro la queue del jugador y el código de partida en caso de unirse a la Clase *Juego*.

#### Post-HandShake 

El protocolo es el encargado de enviar y recibir mensajes a través del socket, este protocolo es accedido en simultáneo por el *Jugador* y *ClienteRecibidor*. Los mensajes que necesiten ser enviados como broadcast serán puestos a la queue y sacados por el protocolo para serializarlos de la manera que indica el contrato para posteriormente ser enviados a través del socket.

Al momento de recibir un mensaje para broadcastear a la partida, el protocolo se encargará de colocar en un string el mensaje a broadcastear y pasarle a la clase *Juego* el mismo

#### Monitores

Para que no haya race conditions entre los diferentes hilos de los clientes, implementamos dos monitores. Uno es *MonitorPartidas* que se encarga de almacenar todas las partidas que son creadas por diferentes clientes, esta se encarga de dar un código único por partida al momento de crearse la partida.

También tenemos el monitor *MonitorPartida* que tiene la responsabilidad de agregar la queue de los clientes nuevos y colocar los diferentes mensajes de broadcast en las queues de los participantes de la partida.  

![diagr_clases_servidor_2](https://github.com/Taller-de-Programacion-TPs/threads-2023c1-Fanusaez/assets/79915723/eb7bb3d3-51ca-4911-b272-fad82822cb4c)

_Imagen 3: Clases del modelo Servidor 2_

### Diagramas de secuencia

![diagram_sec_acceptador](https://github.com/Taller-de-Programacion-TPs/threads-2023c1-Fanusaez/assets/79915723/fcf9911b-6061-4d13-b1d8-e98e92614273)

_Imagen 4: Diagrama Secuencia del modelo Servidor 1_

![diagram_sec_2](https://github.com/Taller-de-Programacion-TPs/threads-2023c1-Fanusaez/assets/79915723/655d9c96-9cb6-4a75-a594-5eb7a70fee37)

_Imagen 5: Diagrama Secuencia del modelo Servidor 2_


![diagr_sec_3](https://github.com/Taller-de-Programacion-TPs/threads-2023c1-Fanusaez/assets/79915723/01e7e47a-ba82-4718-bfbd-8e6d16e4d2e9)

_Imagen 6: Diagrama Secuencia del modelo Servidor 3_



