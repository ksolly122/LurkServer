#include "server.h"

// Definitions of global variables
char* DESCRIPTION; 
ThreadArgs *thread_args[MAX_CONNECTIONS];
int thread_count = 0;
pthread_mutex_t thread_count_mutex = PTHREAD_MUTEX_INITIALIZER;

Character global_players[MAX_PLAYERS];
size_t player_count = 0;
pthread_mutex_t player_mutex = PTHREAD_MUTEX_INITIALIZER;
Character monsters[MAX_MONSTERS];
size_t num_monsters = 0;
Room rooms[MAX_ROOMS];

int main(int argc, char **argv) {
    DESCRIPTION = read_description("description.txt");
    
    signal(SIGPIPE, SIG_IGN);  // Ignore SIGPIPE signals

    struct sockaddr_in sad;
    if (argc > 1)
        sad.sin_port = htons(atoi(argv[1]));
    else
        sad.sin_port = htons(5041);
    
    sad.sin_addr.s_addr = INADDR_ANY;
    sad.sin_family = AF_INET;

    printf("Listening on port: %d\n", ntohs(sad.sin_port));

    int skt = socket(AF_INET, SOCK_STREAM, 0);
    if (skt == -1) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    if (setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(skt);
        return 1;
    }

    if (bind(skt, (struct sockaddr *)(&sad), sizeof(struct sockaddr_in))) {
        perror("bind");
        close(skt);
        return 1;
    }
    
    if (listen(skt, MAX_CONNECTIONS)) {
        perror("listen");
        close(skt);
        return 1;
    }


    initialize_rooms();
    get_monsters();

    while (1) {
        int client_fd;
        struct sockaddr_in client_address;
        socklen_t address_size = sizeof(struct sockaddr_in);
        
        client_fd = accept(skt, (struct sockaddr *)(&client_address), &address_size);
        if (client_fd == -1) {
            perror("accept");
            continue;  // Allow the server to accept new connections
        }

        printf("Connection made from address %s\n", inet_ntoa(client_address.sin_addr));
        send_version_message(client_fd);
        send_game_message(client_fd);

        // Check if we have reached the maximum thread limit
        if (thread_count >= MAX_CONNECTIONS) {
            fprintf(stderr, "Maximum number of clients reached.\n");
            close(client_fd);
            continue;
        }

        // Allocate memory for thread arguments
        ThreadArgs *args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
        if (args == NULL) {
            perror("malloc");
            close(client_fd);
            continue;
        }

        args->client_fd = client_fd;

        // Store the thread args in the global array
        thread_args[thread_count++] = args; // Store the pointer

        // Create a new thread for handling the client
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, args) != 0) {
            perror("pthread_create");
            free(args); // Free the allocated memory if thread creation fails
            close(client_fd);
            continue;
        }
        pthread_detach(thread);
    }

    if (close(skt) == -1) {
        perror("close skt");
    }
    return 0;
}
