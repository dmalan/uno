#ifndef UNO_SERVER_HDR
#define UNO_SERVER_HDR

#define SERVER_VERSION 1

// the number of connections that can attempt to a listening socket
#define BACKLOG 50

// the following constants would all ideally be optimized for a particular 
// machine, or should possibly even be dynamically determined

#define MAX_CONNECTIONS 1000

// the number of threads in the threadpool of each pipeline
#define NUM_CONNECT_THREADS 20
#define NUM_GAME_THREADS 20
#define NUM_SEND_THREADS 20

// how many elements can queue up in each threadpool
#define GAME_Q_LEN 500
#define CONNECT_Q_LEN 500
#define SEND_Q_LEN 500

#endif
