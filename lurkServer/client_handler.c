#include "server.h"

// Function to handle each client connection in a separate thread
void *handle_client(void *args) {
    printf("Starting handler\n");
    ThreadArgs *thread_args = (ThreadArgs *)args;
    char buffer[1024];

    uint8_t player_choice;
    int try_again = 0;
    while (try_again < 5) {
        player_choice = 0;
        ssize_t recv_result = recv(thread_args->client_fd, &player_choice, sizeof(player_choice), 0);
        printf("%d, %d\n", thread_args->client_fd, player_choice);
        if (recv_result <= 0) {
            perror("recv failed");
            handle_client_disconnection(thread_args->client_fd);
            break; // Exit loop on error or disconnection
        }

        // Process player choice
        if (player_choice == CHARACTER_TYPE) {
            // Call handle_character and then break
            printf("handle_character is being run\n");
            handle_character(thread_args);
            Character* state = find_pp_by_fd(thread_args->client_fd);
            send_character_message(thread_args->client_fd, state, state->name, state->description);
            break; // Exit the loop after handling
        } else if (player_choice == MESSAGE_TYPE) {
            char randbuffer[1024];
            const char* message = "Not ready to perform actions yet";
            recv(thread_args->client_fd, &randbuffer, 66, MSG_WAITALL);
            recv(thread_args->client_fd, &randbuffer, sizeof(randbuffer), 0);
            send_error_message(thread_args->client_fd, 5, message);
            try_again++;
        } else if (player_choice == CHANGE_ROOM_TYPE) {
            char randbuffer[1024];
            const char* message = "Not ready to perform actions yet";
            recv(thread_args->client_fd, &randbuffer, 2, MSG_WAITALL);
            send_error_message(thread_args->client_fd, 5, message);
            try_again++;
        } else if (player_choice == FIGHT_TYPE) {
            const char* message = "Not ready to perform actions yet";
            send_error_message(thread_args->client_fd, 5, message);
            try_again++;
        } else if (player_choice == PVP_TYPE) {
            char randbuffer[1024];
            const char* message = "Not ready to perform actions yet";
            recv(thread_args->client_fd, &randbuffer, 32, MSG_WAITALL);
            send_error_message(thread_args->client_fd, 5, message);
            try_again++;
        } else if (player_choice == LOOT_TYPE) {
            char randbuffer[1024];
            const char* message = "Not ready to perform actions yet";
            recv(thread_args->client_fd, &randbuffer, 32, MSG_WAITALL);
            send_error_message(thread_args->client_fd, 5, message);
            try_again++;
        } else if (player_choice == START_TYPE) {
            const char* message = "Not ready to perform actions yet";
            send_error_message(thread_args->client_fd, 5, message);
            try_again++;
        } else if (player_choice == ERROR_TYPE) {
            const char* message = "Booting connection due to invalid choice.";
            send_error_message(thread_args->client_fd, 0, message);
            break;
        } else if (player_choice == ACCEPT_TYPE) {
            const char* message = "Booting connection due to invalid choice.";
            send_error_message(thread_args->client_fd, 0, message);
            break;
        } else if (player_choice == ROOM_TYPE) {
            const char* message = "Booting connection due to invalid choice.";
            send_error_message(thread_args->client_fd, 0, message);
            break;
        } else if (player_choice == GAME_TYPE) {
            const char* message = "Booting connection due to invalid choice.";
            send_error_message(thread_args->client_fd, 0, message);
            break;
        } else if (player_choice == LEAVE_TYPE) {
            handle_client_disconnection(thread_args->client_fd);
            break;
        } else if (player_choice == CONNECTION_TYPE) {
            const char* message = "Booting connection due to invalid choice.";
            send_error_message(thread_args->client_fd, 0, message);
            break;
        } else if (player_choice > 13) {
            handle_client_disconnection(thread_args->client_fd);
            break;
        } else if (player_choice == 0) {
            continue;
        }
    }
    if (try_again >= 5) {
        const char* message = "Booting connection due to repeated invalid choice.";
        send_error_message(thread_args->client_fd, 0, message);
    }
    
    // Check the state for the client before proceeding
    Character* state = find_pp_by_fd(thread_args->client_fd);
    if (state == NULL) {
        fprintf(stderr, "Client disconnected before accessing state for fd: %d\n", thread_args->client_fd);
        handle_client_disconnection(thread_args->client_fd);
        return NULL;
    }

    const char* player_name = state->name;
    const char* player_description = state->description;

    send_room_message(thread_args->client_fd, &rooms[state->room_number - 1], NULL);
    Room *current_room = &rooms[state->room_number - 1];
    for (size_t i = 0; i < current_room->num_connections; i++) {
        send_connection_message(thread_args->client_fd, current_room->connections[i]);
    }
    printf("handle_character is done\n");
    
    int try_again2 = 0;

    while (try_again2 < 10) {
        player_choice = 0;
        printf("waiting for input again\n");
        printf("%02X", state->flags);
        printf("%d, %d\n", thread_args->client_fd, player_choice);
        ssize_t recv_result = recv(thread_args->client_fd, &player_choice, sizeof(player_choice), 0);

        // Check if recv failed or client disconnected
        if (recv_result <= 0) {
            fprintf(stderr, "Client disconnected or recv error for fd: %d\n", thread_args->client_fd);
            handle_client_disconnection(thread_args->client_fd);
            break;
        }
        if (player_choice != START_TYPE && !is_started(state)) {
            if (player_choice == CHARACTER_TYPE) {
                const char* message = "You haven't started the game yet!";
                send_error_message(thread_args->client_fd, 5, message);
                try_again2++;
                continue;
            } else if (player_choice == MESSAGE_TYPE) {
                char randbuffer[1024];
                const char* message = "You haven't started the game yet!";
                recv(thread_args->client_fd, &randbuffer, 66, MSG_WAITALL);
                recv(thread_args->client_fd, &randbuffer, sizeof(randbuffer), 0);
                send_error_message(thread_args->client_fd, 5, message);
                try_again2++;
                continue;
            } else if (player_choice == CHANGE_ROOM_TYPE) {
                char randbuffer[1024];
                const char* message = "You haven't started the game yet!";
                recv(thread_args->client_fd, &randbuffer, 2, MSG_WAITALL);
                send_error_message(thread_args->client_fd, 5, message);
                try_again2++;
                continue;
            } else if (player_choice == FIGHT_TYPE) {
                const char* message = "You haven't started the game yet!";
                send_error_message(thread_args->client_fd, 5, message);
                try_again2++;
                continue;
            } else if (player_choice == PVP_TYPE) {
                char randbuffer[1024];
                const char* message = "You haven't started the game yet!";
                recv(thread_args->client_fd, &randbuffer, 32, MSG_WAITALL);
                send_error_message(thread_args->client_fd, 5, message);
                try_again2++;
                continue;
            } else if (player_choice == LOOT_TYPE) {
                char randbuffer[1024];
                const char* message = "You haven't started the game yet!";
                recv(thread_args->client_fd, &randbuffer, 32, MSG_WAITALL);
                send_error_message(thread_args->client_fd, 5, message);
                try_again2++;
                continue;
            } else if (player_choice == ERROR_TYPE) {
                const char* message = "#1 Booting connection due to invalid choice.";
                send_error_message(thread_args->client_fd, 0, message);
                break;
            } else if (player_choice == ACCEPT_TYPE) {
                const char* message = "#2 Booting connection due to invalid choice.";
                send_error_message(thread_args->client_fd, 0, message);
                break;
            } else if (player_choice == ROOM_TYPE) {
                const char* message = "#3 Booting connection due to invalid choice.";
                send_error_message(thread_args->client_fd, 0, message);
                break;
            } else if (player_choice == GAME_TYPE) {
                const char* message = "#4 Booting connection due to invalid choice.";
                send_error_message(thread_args->client_fd, 0, message);
                break;
            } else if (player_choice == LEAVE_TYPE) {
                handle_client_disconnection(thread_args->client_fd);
                break;
            } else if (player_choice == CONNECTION_TYPE) {
                const char* message = "#5 Booting connection due to invalid choice.";
                send_error_message(thread_args->client_fd, 0, message);
                break;
            } else if (player_choice > 13) {
                continue;
            } else if (player_choice == 0) {
                continue;
            }
        } 
        if (player_choice == CHANGE_ROOM_TYPE) {
            printf("Received type 2\n");
            int room_choice;
            recv(thread_args->client_fd, &room_choice, sizeof(room_choice), 0);
            change_room(thread_args, room_choice);
            continue;
        } else if (player_choice == START_TYPE) {
            send_accept_message(thread_args->client_fd, player_choice);
            state->flags |= (1 << 4); // Started
            const char *start_message = "Welcome to the game!";
            send_message(player_name, "Server", start_message, strlen(start_message), 1);
            send_character_message(thread_args->client_fd, state, player_name, player_description);
            printf("Character message is finished");
            send_room_message(thread_args->client_fd, &rooms[state->room_number - 1], NULL);

            for (size_t i = 0; i < current_room->num_connections; i++) {
                send_connection_message(thread_args->client_fd, current_room->connections[i]);
            }
            add_player_to_room(&rooms[0], state);
            Room *room_1 = &rooms[0];

            for (size_t i = 0; i < room_1->num_players; i++) {
                if (room_1->players[i] != NULL && room_1->players[i] != state) {
                    send_character_message(room_1->players[i]->client_fd, state, state->name, state->description);
                }
            }

            // Send CHARACTER messages for all players in the new room
            for (size_t i = 0; i < room_1->num_players; i++) {
                if (room_1->players[i] != NULL) {
                    send_character_message(thread_args->client_fd, room_1->players[i], room_1->players[i]->name, room_1->players[i]->description);
                }
            }
            continue;
        } else if (player_choice == MESSAGE_TYPE) {
            printf("Received type 1\n");
            receive_message(thread_args->client_fd, buffer, sizeof(buffer));
            send_accept_message(thread_args->client_fd, player_choice);
            continue;
        } else if (player_choice == LEAVE_TYPE) {
            printf("Received type 12\n");
            handle_client_disconnection(thread_args->client_fd);
            break;
        } else if (player_choice == FIGHT_TYPE) {
            printf("Received type 3\n");
            Character *monster = get_monster_in_room(state->room_number, 0);
            
            if (monster != NULL) {
                printf("Monster found: %s\n", monster->name);
            } else {
                const char* message = "There are no monsters to fight!";
                printf("Monster not found.\n");
                send_error_message(thread_args->client_fd, 7, message);
                continue;
            }

            if (!is_alive(monster)) {
                const char* message = "There are no monsters to fight!";
                printf("%s cannot fight because they are not alive.\n", monster->name);
                send_error_message(thread_args->client_fd, 7, message);
                continue;
            }

            send_accept_message(thread_args->client_fd, player_choice);
            fight(state, monster);
            send_character_message(thread_args->client_fd, state, player_name, player_description);
            send_character_message(thread_args->client_fd, monster, monster->name, monster->description);
            continue;
        } else if (player_choice == PVP_TYPE) {
            const char* message = "Server does not support PVP combat";
            send_error_message(thread_args->client_fd, 8, message);
            continue;
        } else if (player_choice == LOOT_TYPE) {
            printf("Received type 5\n");
            send_accept_message(thread_args->client_fd, player_choice);
            loot(thread_args);
            continue;
        } else if (player_choice == 0) {
            printf("Received type 0\n");
            const char* message = "#6 Booting connection due to invalid choice.";
            send_error_message(thread_args->client_fd, 0, message);
            handle_client_disconnection(thread_args->client_fd);
            break;
        } else {
            printf("Received type: %d from :%d\n", player_choice, thread_args->client_fd);
            continue;
        }
        
    }
    if (try_again2 >= 10) {
        const char* message = "#7 Booting connection due to repeated invalid choice.";
        send_error_message(thread_args->client_fd, 0, message);
    }

}

// Assuming you have a mechanism to detect when a player wants to leave
void handle_client_disconnection(int client_fd) {
    // Find the player associated with the client_fd
    Character *player = find_pp_by_fd(client_fd);
    if (player != NULL) {
        player->client_fd = -1; // Indicate disconnection
    } else {
        printf("No player found associated with fd: %d\n", client_fd);
    }

    // Close the client socket
    if (client_fd >= 0) {
        if (close(client_fd) == -1) {
            perror("Error closing client socket");
        } else {
            printf("Client with fd %d has left the game.\n", client_fd);
        }
    } else {
        printf("Invalid client file descriptor: %d\n", client_fd);
    }

    // Adjust thread count and remove the thread args entry
    pthread_mutex_lock(&thread_count_mutex);
    for (int i = 0; i < thread_count; i++) {
        if (thread_args[i]->client_fd == client_fd) {
            free(thread_args[i]); // Free the allocated memory
            // Shift remaining threads down
            for (int j = i; j < thread_count - 1; j++) {
                thread_args[j] = thread_args[j + 1];
            }
            thread_args[thread_count - 1] = NULL; // Optional: Nullify the last entry
            thread_count--; // Decrement thread count
            break; // Exit loop after handling
        }
    }
    pthread_mutex_unlock(&thread_count_mutex);
}

Character *get_monster_in_room(int room_number, int monster_index) {
    // Check if the room number is valid
    if (room_number < 1 || room_number > MAX_ROOMS) { // Assuming MAX_ROOMS is defined
        printf("Invalid room number.\n");
        return NULL;
    }

    Room *room = &rooms[room_number - 1]; // Access the room (1-indexed to 0-indexed)

    // Check if the monster index is valid
    if (monster_index < 0 || monster_index >= room->num_monsters) {
        printf("Invalid monster index.\n");
        return NULL;
    }

    // Return the requested monster
    return room->monsters[monster_index];
}