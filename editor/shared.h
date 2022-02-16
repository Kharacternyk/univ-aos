#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

struct message {
    unsigned char length;
    char data[255];
};

#define PORT 1026

#define ROWS 5

#define CMD_DELETE 'd'

#define panic(desc)                                                                    \
    ({                                                                                 \
        perror(desc);                                                                  \
        exit(1);                                                                       \
    })

#define do_till_end(action, fd, target, size)                                          \
    ({                                                                                 \
        size_t processed = 0;                                                          \
        while (processed < size) {                                                     \
            processed += action(fd, target + processed, size - processed);             \
        }                                                                              \
    })

#define log_message(message, prefix)                                                   \
    ({                                                                                 \
        fprintf(stderr, prefix "\nLength: %hhu\n", message.length);                    \
        fprintf(stderr, "Data:");                                                      \
        for (int i = 0; i < message.length; ++i) {                                     \
            fprintf(stderr, " %hhu", message.data[i]);                                 \
        }                                                                              \
        fprintf(stderr, "\n" prefix "\n\n");                                           \
    })

#define read_message(fd)                                                               \
    ({                                                                                 \
        struct message message;                                                        \
        read(fd, &message.length, 1);                                                  \
        do_till_end(read, fd, message.data, message.length);                           \
        log_message(message, "<<<");                                                   \
        message;                                                                       \
    })

#define write_message(fd, message)                                                     \
    ({                                                                                 \
        log_message(message, ">>>");                                                   \
        write(fd, &message.length, 1);                                                 \
        do_till_end(write, fd, message.data, message.length);                          \
    })
