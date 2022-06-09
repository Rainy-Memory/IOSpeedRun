#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "header.h"

char msg[256] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
char buf[256];
const char * ipaddr = "127.0.0.1";

int main() {
    int sockfd, send_res, rec_len;
    struct sockaddr_in server_addr;
    clock_t start, end;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_aton(ipaddr, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *) & server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    int err_cnt = 0;

    start = clock();
    for (int i = 0; i < TEST_SIZE; i++) {
        // printf("%d\n", i);
        send_res = send(sockfd, msg, strlen(msg), 0);
        // if (send_res == -1) perror("send"), exit(1);
        memset(buf, 0, sizeof(buf));
        rec_len = recv(sockfd, buf, sizeof(buf), 0);
        // if (rec_len == -1) perror("recv"), exit(1);
        // puts(buf);
        if (send_res == -1 || rec_len == -1) err_cnt++;
    }
    end = clock();

    printf("time: %f\n", ((double) (end - start)) / CLOCKS_PER_SEC);

    close(sockfd);

    if (err_cnt > 0) printf("failed %d times", err_cnt);

    return 0;
}