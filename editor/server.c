#include "shared.h"

void send_text(int fd, char *text[ROWS]) {
    for (int i = 0; i < ROWS; ++i) {
        struct message message;
        message.length = strlen(text[i]);
        memcpy(message.data, text[i], message.length);
        write_message(fd, message);
    }
}

int read_command(int fd, char *text[ROWS]) {
    const struct message message = read_message(fd);
    if (message.length < 3) {
        printf("The client has disconnected\n");
        return 0;
    }

    int y = message.data[0];
    int x = message.data[1];
    char ch = message.data[2];

    if (y < ROWS && x < strlen(text[y])) {
        text[y][x] = ch;
    }

    for (int i = 0; i < ROWS; ++i) {
        printf("%s\n", text[i]);
    }
    printf("\n");

    return 1;
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        panic("Cannot create a socket");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof addr) < 0) {
        panic("Cannot bind to the socket");
    }
    if (listen(sockfd, 1) < 0) {
        panic("Cannot listen to the socket");
    }

    int fd;
    while (fd = accept(sockfd, NULL, NULL)) {
        if (fd < 0) {
            perror("Cannot accept a connection");
            continue;
        }

        char *text[ROWS] = {
            strdup("Hello, this is a text for you to edit!"),
            strdup("Here are some words, and more and more..."),
            strdup("Blah, blah..."),
            strdup("Do not forget to save the changes once you are done!"),
            strdup("Just kidding, it is saved automatically:)"),
        };

        send_text(fd, text);
        while (read_command(fd, text))
            ;
        close(fd);
    }
}
