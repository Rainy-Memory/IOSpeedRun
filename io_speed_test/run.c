#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sdt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libaio.h>
#include "liburing.h"

#define BATCH_SIZE 16
#define TEST_SIZE 100
#define BUFFER_SIZE 4096
#define MY_BLOCK_SIZE 512

void fill_buffer(char * buf, int sz) {
    for (int i = 0; i < (sz >> 1); i++) buf[i] = 'a' + (i % 26);
    for (int i = (sz >> 1); i < sz; i++) buf[i] = 'A' + (i % 26);
}

void stdio_tester() {
    const char * file_name = "stdio_test_file.txt";
    int fd = open(file_name, O_RDWR | O_CREAT | O_DIRECT);
    truncate(file_name, BUFFER_SIZE << 1);
    char * buf;
    posix_memalign((void **) & buf, MY_BLOCK_SIZE, BUFFER_SIZE); // O_DIRECT need aligned buffer
    fill_buffer(buf, BUFFER_SIZE);
    for (int i = 0; i < TEST_SIZE; i++) {
        for (int j = 0; j < BATCH_SIZE; j++) {
            lseek(fd, 0, SEEK_SET);
            DTRACE_PROBE(stdio, stdio_enter);
            if (j % 2) read(fd, buf, BUFFER_SIZE);
            else write(fd, buf, BUFFER_SIZE);
            DTRACE_PROBE(stdio, stdio_leave);
        }
    }
    free(buf), close(fd), sleep(1);
    puts("stdio test finished");
}

void iouring_tester() {
    const char * file_name = "iouring_test_file.txt";
    int fd = open(file_name, O_RDWR | O_CREAT | O_DIRECT);
    truncate(file_name, BUFFER_SIZE << 1);
    char * buf;
    posix_memalign((void **) & buf, MY_BLOCK_SIZE, BUFFER_SIZE); // O_DIRECT need aligned buffer
    fill_buffer(buf, BUFFER_SIZE);
    for (int i = 0; i < TEST_SIZE; i++) {
        struct io_uring ring;
        int ret = io_uring_queue_init(BATCH_SIZE << 1, // length of the queue
                                      &ring,           // instance of io_uring
                                      0);              // flags, 0 for default
        if (ret != 0) perror("io_uring_queue_init"), exit(1);
        struct io_uring_sqe * sqe;
        struct io_uring_cqe * finish_sqe[BATCH_SIZE];
        for (int j = 0; j < BATCH_SIZE; j++) {
            sqe = io_uring_get_sqe(&ring);
            if (j % 2) io_uring_prep_read(sqe, fd, buf, BUFFER_SIZE, 0);
            else io_uring_prep_write(sqe, fd, buf, BUFFER_SIZE, 0);
        }
        for (int submit_num = 0; submit_num < BATCH_SIZE; submit_num += io_uring_submit(&ring));
        for (int finish_num = 0; finish_num < BATCH_SIZE; ) {
            int new_finish = io_uring_peek_batch_cqe(&ring, finish_sqe, BATCH_SIZE);
            if (new_finish > 0) io_uring_cq_advance(&ring, new_finish), finish_num += new_finish;
        }
    }
    free(buf), close(fd), sleep(1);
    puts("iouring test finished");
}

void aio_tester() {
    const char * file_name = "aio_test_file.txt";
    int fd = open(file_name, O_RDWR | O_CREAT | O_DIRECT);
    truncate(file_name, BUFFER_SIZE << 1);
    for (int i = 0; i < TEST_SIZE; i++) {
        char * buf;
        buf = malloc(BATCH_SIZE * BUFFER_SIZE);
        fill_buffer(buf, BATCH_SIZE * BUFFER_SIZE);
        io_context_t context;
        memset(&context, 0, sizeof(context));
        struct iocb * cbs[BATCH_SIZE];
        struct io_event events[BATCH_SIZE];
        io_setup(BATCH_SIZE << 1, &context);
        for (int j = 0; j < BATCH_SIZE; j++) {
            cbs[j] = (struct iocb *) malloc(sizeof(struct iocb));
            if (j % 2) io_prep_pread(cbs[j], fd, buf + j * BUFFER_SIZE, BUFFER_SIZE, 0);
            else io_prep_pwrite(cbs[j], fd, buf + j * BUFFER_SIZE, BUFFER_SIZE, 0);
        }
        // for (int submit_num = 0; submit_num < BATCH_SIZE; submit_num += io_submit(context, BATCH_SIZE, cbs));
        for (int submit_num = 0; submit_num < BATCH_SIZE; ) {
            int sc = io_submit(context, BATCH_SIZE, cbs);
            if (sc < 0) perror("io_submit"), exit(1);
            submit_num += sc;
        }
        for (int finish_num = 0; finish_num < BATCH_SIZE; ) {
            int new_finish = io_getevents(context, 1, BATCH_SIZE, events, NULL);
            if (new_finish > 0) finish_num += new_finish;
        }
        io_destroy(context);
        free(buf);
    }
    close(fd), sleep(1);
    puts("aio test finished");
}

int main() {
    printf("pid: %d\nPress enter to continue (make sure bpf.py is running):", getpid());
    for (char c = '\0'; c != '\n' && c != '\r'; c = getchar());
    stdio_tester();
    iouring_tester();
    aio_tester();
    return 0;
}