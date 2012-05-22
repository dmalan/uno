/**
 * Uno
 * Rob Bowden <rob@cs.harvard.edu>
 * Tommy MacWilliam <tmacwilliam@cs.harvard.edu>
 *
 * Design somewhat inspired by Matt Welsh's PhD thesis on SEDA
 * Haven't benchmarked or anything yet, but might be surprisingly scalable
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "queue.h"
#include "uno_server.h"
#include "../uno.h"


struct queue *connect_queue;
struct queue *new_game_queue;
struct queue *game_queue;
struct queue *send_queue;

// mutex and cv for 
// XXX: all games wait on the game_queue, except for the one that epolls
struct pthread_mutex_t game_mutex;
struct pthread_cond_t game_cv;

// pipe for game thread to learn of newly connected
int pipe_fd[2];

// TODO: Timeout if response takes too long... could use a heap, and have 
//       epoll delete_min on the heap
// TODO: Maybe flush anything on a socket from a user who sent data out of 
// turn? There's a ton of reasons *not* to do that, though...
// XXX: For simplicity, hardcode max 4 players
struct 
{
    int fds[4];
    int turn;
    int players;
} games[GAME_Q_LEN];

static void
die_with_error(char *message, int error)
{
    perror(message);
    exit(error);
}

static void *
connect_loop(void *arg)
{
    int fd = dequeue(connect_queue);
    // XXX: check for backpressure
    
}

static void *
game_loop(void *arg)
{
    // XXX: check for backpressure
    
}

static void *
send_loop(void *arg)
{
}

int
main(int argc, char **argv)
{
    int sock, result;

    // initialize the thread pools
    for (int i = 0; i < NUM_CONNECT_THREADS + NUM_GAME_THREADS + NUM_SEND_THREADS; i++)
    {
        pthread_t p;
        if (i >= NUM_GAME_THREADS + NUM_CONNECT_THREADS)
            loop = send_loop;
        else if (i >= NUM_GAME_THREADS)
            loop = coonnect_loop
        else
            loop = game_loop

        pthread_create(&p, NULL, loop, NULL);
    }

    queue_init(&game_queue, GAME_Q_LEN);
    queue_init(&new_game_queue, GAME_Q_LEN);
    queue_init(&connect_queue, CONNECT_Q_LEN);
    queue_init(&send_queue, SEND_Q_LEN);

    for (int i = 0; i < GAME_Q_LEN; i++)
        enqueue(new_game_queue, i);

    // set up for a stream socket listening on localhost
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    struct addrinfo *info;
    if ((result = getaddrinfo(NULL, PORT, &hints, &info)))
    {
        // getaddrinfo doesn't set errno, so have to use gai_strerror
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        exit(1);
    }

    // have to find a valid socket that we can bind to
    struct addrinfo *i;
    for (i = info; i != NULL; i = i->ai_next)
    {
        // create the socket
        if (sock = socket(i->ai_family, i->ai_socktype, i->ai_protocol) == -1)
            continue;
        
        // set the socket as reusable, to avoid annoying "socket in use" error
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (int []){1}, 
                sizeof int) == -1)
            die_with_error("setsockopt", 1);

        // bind to the socket
        if (bind(sock, i->ai_addr, p->ai_addrlen) == -1)
        {
            close(sock);
            continue;
        }
    }

    // we weren't able to bind to any socket
    if (i == NULL)
    {
        fprintf(stderr, "serve50: failed to bind\n");
        exit(1);
    }
    
    // done with the struct from getaddrinfo
    freeaddrinfo(servinfo);

    // listen on the socket we binded to
    if (listen(sock, BACKLOG) == -1)
        die_with_error("listen", 1);

    while (1)
    {
        struct sockaddr_storage their_addr;
        socklen_t sin_size = sizeof their_addr;
        
        int newfd = accept(sock, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1)
        {
            perror("accept");
            continue;
        }
        // meh... server can block, but it should be a very short block, and 
        // at least it shouldn't be able to starve
        enqueue(connect_queue, newfd);
    }
}
