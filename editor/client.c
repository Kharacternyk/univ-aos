#include "shared.h"
#include <ncursesw/curses.h>

void get_text(int fd) {
    for (int i = 0; i < ROWS; ++i) {
        const struct message message = read_message(fd);
        mvaddnstr(i, 0, message.data, message.length);
    }
}

void move_cursor(int y, int x) {
    int old_y, old_x;
    getyx(stdscr, old_y, old_x);
    move(old_y + y, old_x + x);
}

void send_farewell(int fd) {
    struct message message;
    message.length = 0;
    write_message(fd, message);
    close(fd);
}

void replace_char(int fd, char ch) {
    int y, x;
    getyx(stdscr, y, x);
    addch(ch);

    struct message message;
    message.length = 3;
    message.data[0] = y;
    message.data[1] = x;
    message.data[2] = ch;

    write_message(fd, message);
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

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof addr) < 0) {
        panic("Cannot connect to the socket");
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, 1);
    get_text(sockfd);
    move(0, 0);
    refresh();

    for (;;) {
        int ch = getch();
        switch (ch) {
        case KEY_LEFT:
            move_cursor(0, -1);
            break;
        case KEY_DOWN:
            move_cursor(1, 0);
            break;
        case KEY_UP:
            move_cursor(-1, 0);
            break;
        case KEY_RIGHT:
            move_cursor(0, 1);
            break;
        case KEY_BACKSPACE:
            send_farewell(sockfd);
            endwin();
            exit(0);
        default:
            replace_char(sockfd, ch);
            break;
        }

        refresh();
    }
}
