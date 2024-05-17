#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#include <string.h>

#define MAX_COUNT_CLIENTS 100

int clients[MAX_COUNT_CLIENTS];
char is_active[MAX_COUNT_CLIENTS];
int count_active_clients;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static inline int reserve_socket_cell() {
 
}

static inline void free_socket_cell(int cell) {

}

static inline void notify_all(char *buffer, char message_len, int skip) {
 
}

static void* client_handler(void * arg) {

    return NULL;
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    uint16_t portno;
    unsigned int clilen;
    struct sockaddr_in serv_addr, cli_addr;
    (void)argc;
    (void)argv;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        return 1;
    }

    if (argc != 2) {
        fprintf(stderr, "usage: %s port\n", argv[0]);
        exit(0);
    }

    portno = (uint16_t) atoi(argv[1]);

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        close(sockfd);
        return 1;
    }

    /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
    */

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;
        }

        if (count_active_clients + 1 > MAX_COUNT_CLIENTS) {
            perror("Customer limit exceeded");
            close(newsockfd);
            continue;
        }

        int cell = reserve_socket_cell();

        if (cell == -1) {
            perror("Customer limit exceeded");
            close(newsockfd);
            continue;
        }

        clients[cell] = newsockfd;

        pthread_t thread_id;

        if (pthread_create(&thread_id, NULL, client_handler, is_active + cell) != 0) {
            continue;
        }

        pthread_detach(thread_id);
    }

    return 0;
}
