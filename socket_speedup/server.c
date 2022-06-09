#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "header.h"

#define MAX_LISTEN_NUM 10

char buf[4096];
const char * ipaddr = "127.0.0.1";

int main() {
    int sockfd, connfd, n, acc_len;
    struct sockaddr_in server_addr, accept_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) & server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, MAX_LISTEN_NUM) == -1) {
        perror("listen");
        exit(1);
    }

    printf("Waiting for client's request...\n");

    memset(&accept_addr, 0, sizeof(accept_addr));
    acc_len = sizeof(accept_addr);

    if ((connfd = accept(sockfd, (struct sockaddr *) & accept_addr, &acc_len)) == -1) {
        perror("accept");
        exit(1);
    }

    int err_cnt = 0;

    for (int i = 0; i < TEST_SIZE; i++) {
        memset(buf, 0, sizeof(buf));
        n = recv(connfd, buf, sizeof(buf), 0);
        strcat(buf, " accept");
        if (send(connfd, buf, strlen(buf), 0) == -1) err_cnt++; // perror("send"), exit(1);
        buf[strlen(buf)] = '\0';
        // printf("Received (%d): %s\n", i + 1, buf);
    }

    close(connfd);
    close(sockfd);

    if (err_cnt > 0) printf("failed %d times\n", err_cnt);

    return 0;
}