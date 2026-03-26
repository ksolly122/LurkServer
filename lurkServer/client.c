#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>           // For close() and read()
#include <arpa/inet.h>         // For inet_pton and sockaddr_in
#include <sys/socket.h>        // For socket functions
#include <sys/select.h>        // For fd_set and select

#define SERVER_IP "localhost"   
#define SERVER_PORT 5040        

#define BUFFER_SIZE 1024
#define USERNAME_MAX 32

// LURK Message Types
#define MESSAGE_TYPE 1
#define CHANGEROOM_TYPE 2
#define FIGHT_TYPE 3
#define PVPFIGHT_TYPE 4
#define LOOT_TYPE 5
#define START_TYPE 6
#define ERROR_TYPE 7
#define ACCEPT_TYPE 8
#define ROOM_TYPE 9
#define CHARACTER_TYPE 10
#define GAME_TYPE 11
#define LEAVE_TYPE 12
#define CONNECTION_TYPE 13
#define VERSION_TYPE 14

typedef struct {
    int health;
    int attack;
    int defense;
    int regen;
    int gold;
    int room;
    char name[USERNAME_MAX];
} Character;

typedef struct {
    int socket_fd;
    struct sockaddr_in server_address;
} Client;

Client client;
Character player;

void init_client(const char *ip, int port) {
    client.socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client.socket_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    client.server_address.sin_family = AF_INET;
    client.server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &client.server_address.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(client.socket_fd);
        exit(EXIT_FAILURE);
    }

    if (connect(client.socket_fd, (struct sockaddr *)&client.server_address, sizeof(client.server_address)) < 0) {
        perror("Connection to server failed");
        close(client.socket_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server at %s:%d\n", ip, port);
}

void send_start_message() {
    uint8_t message[BUFFER_SIZE] = {0};
    message[0] = START_TYPE;  // Message Type

    if (send(client.socket_fd, message, 1, 0) < 0) {
        perror("Failed to send START message");
    } else {
        printf("START message sent.\n");
    }
}

void handle_server_message(uint8_t *buffer, ssize_t length) {
    switch (buffer[0]) {
        case MESSAGE_TYPE:
            printf("Message received from server.\n");
            break;
        case ROOM_TYPE:
            printf("Room details received.\n");
            break;
        case CHARACTER_TYPE:
            printf("Character data received.\n");
            break;
        case GAME_TYPE:
            printf("Game info received.\n");
            break;
        case ERROR_TYPE:
            printf("Error message received.\n");
            break;
        default:
            printf("Unknown message type received: %d\n", buffer[0]);
            break;
    }
}

void client_loop() {
    fd_set read_fds;
    uint8_t buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(client.socket_fd, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        int max_fd = client.socket_fd > STDIN_FILENO ? client.socket_fd : STDIN_FILENO;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("Select error");
            break;
        }

        if (FD_ISSET(client.socket_fd, &read_fds)) {
            bytes_received = recv(client.socket_fd, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) {
                printf("Connection closed by server.\n");
                break;
            }
            handle_server_message(buffer, bytes_received);
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            char input[BUFFER_SIZE];
            fgets(input, BUFFER_SIZE, stdin);

            if (strncmp(input, "start", 5) == 0) {
                send_start_message();
            } else if (strncmp(input, "changeroom", 10) == 0) {
                int room_number = atoi(input + 11);
                // Implement and call send_changeroom_message(room_number) here
            }
        }
    }
}

int main() {
    init_client(SERVER_IP, SERVER_PORT);
    client_loop();
    close(client.socket_fd);
    return 0;
}
