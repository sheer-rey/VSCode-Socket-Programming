# Version Log

## v1.0
- Initial version, simply implementation of "Hello Socket" server and client.

## v1.1
- Initially completed the structed message mode of server and client: use struct to store messages, and transmit between server and client by converting to byte stream.
- Implemented "Perfect echo server" by checking the lenth of message.

## v1.2
### Completed
- Encapsulated the "echo message" and "calculator" modules to functions.
- Comment everywhere need to.

### Shortcut
- Client do not use the half-close status.
- Server do not check wether client is disconnect.
- Maybe use class is better than struct?

### BUG
- Server will only handle the request once after accept a client's connection, and then waitting for a new connection.

## v1.3
### Completed
- Resolve the BUG of v1.2, now server can handle the requests of client continuously.
- Modified the code of client and server so that the same code can run under Windows and Linux.

## v1.4
- Little modify to codes and comments in server and client, no effect to program functions.

## v3.0
## Completed
- The server side uses a select model for I/O reuse, allowing multiple requests from the client side to be processed continuously.
- The same server-side code can run on both Windows and Linux platforms.
