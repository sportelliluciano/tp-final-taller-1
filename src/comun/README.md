### Paquete conexión

Este paquete encapsula la el protocolo de conexión entre el cliente y el 
servidor y los eventos que pueden enviarse entre ellos.

Se definen dos clases de excepciones:
- ErrorSocket indicando un error a nivel sistema operativo ocurrido sobre un
socket.
- ErrorConexion indicando un error a nivel protocolo de conexión.

Se definen las siguientes clases:
- Conexion: Representa una conexión entre un servidor y un cliente
- Evento: Una notificación que puede venir del cliente hacia el servidor o bien
desde el servidor hacia el cliente
- FactoryEvento: Factory para deserializar eventos
- eventos/*: Cada clase de evento