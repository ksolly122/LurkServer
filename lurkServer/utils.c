#include "server.h"

// Send VERSION message
void send_version_message(int client_fd) {
    uint8_t version_message[5];

    version_message[0] = VERSION_TYPE;   
    version_message[1] = 2;              
    version_message[2] = 3;              
    uint16_t extensions_size = 0; 
    version_message[3] = extensions_size & 0xFF;  
    version_message[4] = (extensions_size >> 8) & 0xFF;  

    ssize_t bytes_sent = send(client_fd, version_message, sizeof(version_message), 0);
    if (bytes_sent == -1) {
        perror("send VERSION");
    } else {
        printf("Sent VERSION message\n");
    }
}

// Send GAME message
void send_game_message(int client_fd) {
    uint16_t initial_points = INITIAL_POINTS_MAX;
    uint16_t stat_limit = STAT_LIMIT_MAX;
    const char *description = DESCRIPTION;
    uint16_t description_length = strlen(description);

    size_t message_size = 7 + description_length;
    uint8_t *message = (uint8_t *)malloc(message_size);

    if (message == NULL) {
        perror("malloc");
        close(client_fd);
        return;
    }

    message[0] = GAME_TYPE;
    message[1] = initial_points & 0xFF;
    message[2] = (initial_points >> 8) & 0xFF;
    message[3] = stat_limit & 0xFF;
    message[4] = (stat_limit >> 8) & 0xFF;
    message[5] = description_length & 0xFF;
    message[6] = (description_length >> 8) & 0xFF;
    memcpy(message + 7, description, description_length);

    ssize_t bytes_sent = send(client_fd, message, message_size, 0);
    if (bytes_sent == -1) {
        perror("send GAME");
    } else {
        printf("Sent GAME message\n");
    }

    free(message);
}

// Error handling function
void send_error_message(int client_fd, uint8_t error_code, const char *error_message) {
    uint16_t message_length = strlen(error_message);  
    uint16_t total_length = 4 + message_length; 

    uint8_t *message = (uint8_t *)malloc(total_length);
    if (message == NULL) {
        perror("malloc");
        return; 
    }

    message[0] = ERROR_TYPE; 
    message[1] = error_code;
    message[2] = message_length & 0xFF; 
    message[3] = (message_length >> 8) & 0xFF; 
    memcpy(message + 4, error_message, message_length);

    ssize_t bytes_sent = send(client_fd, message, total_length, 0);
    if (bytes_sent == -1) {
        perror("send ERROR");
    } else {
        printf("Sent ERROR message: %s\n", error_message);
    }

    free(message);
}

// Accept message function
void send_accept_message(int client_fd, uint8_t action_type) {
    uint8_t accept_message[2];
    accept_message[0] = ACCEPT_TYPE;  
    accept_message[1] = action_type;  

    ssize_t bytes_sent = send(client_fd, accept_message, sizeof(accept_message), 0);
    if (bytes_sent == -1) {
        perror("send ACCEPT");
    } else {
        printf("Sent ACCEPT message for action type %d\n", action_type);
    }

    Character* state = find_pp_by_fd(client_fd);
    if (state != NULL) {
        const char* player_name = state->name;
        const char* player_description = state->description;
        // send_character_message(client_fd, state, player_name, player_description);
    }
}

// Function to send CHARACTER message
void send_character_message(int client_fd, Character *character_state, const char *player_name, const char *description) {
    // Define message size and create a buffer
    uint8_t message[512]; // Adjust size as needed

    // Initialize message with CHARACTER type
    message[0] = CHARACTER_TYPE; // CHARACTER message type

    // Copy the player name (1-32)
    strncpy((char*)&message[1], player_name, 32);

    // Set Flags (33)
    message[33] = (character_state->alive << 7) | 
                  (character_state->join_battle << 6) | 
                  (character_state->monster << 5) | 
                  (character_state->started << 4) | 
                  (character_state->ready << 3);
    // Ensure lower 3 bits are zero
    message[33] |= character_state->flags; // Mask lower bits to maintain the flags

    // Set Attack (34-35)
    message[34] = character_state->attack & 0xFF;
    message[35] = (character_state->attack >> 8) & 0xFF;

    // Set Defense (36-37)
    message[36] = character_state->defense & 0xFF;
    message[37] = (character_state->defense >> 8) & 0xFF;

    // Set Regen (38-39)
    message[38] = character_state->regen & 0xFF;
    message[39] = (character_state->regen >> 8) & 0xFF;

    // Set Health (40-41) - signed
    message[40] = character_state->health & 0xFF;
    message[41] = (character_state->health >> 8) & 0xFF;

    // Set Gold (42-43)
    message[42] = character_state->gold & 0xFF;
    message[43] = (character_state->gold >> 8) & 0xFF;

    // Set Current Room Number (44-45)
    message[44] = character_state->room_number & 0xFF;
    message[45] = (character_state->room_number >> 8) & 0xFF;

    // Set Description Length (46-47)
    size_t description_length = strlen(description);
    message[46] = description_length & 0xFF;
    message[47] = (description_length >> 8) & 0xFF;

    // Copy Player Description (48+)
    strncpy((char*)&message[48], description, description_length);
    message[48 + description_length - 1] = '\0'; // Ensure null-termination

    // Send the message
    ssize_t bytes_sent = send(client_fd, message, 48 + description_length, 0);
    if (bytes_sent == -1) {
        perror("send CHARACTER");
    } else {
        printf("Sent CHARACTER message for player %s in room %d to fd %d\n", player_name, character_state->room_number, client_fd);
    }
}

// Room message function
void send_room_message(int client_fd, Room *room, Character *character_state) {
    uint16_t description_length = strlen(room->description);
    size_t message_size = 37 + description_length;
    uint8_t *message = (uint8_t *)malloc(message_size);

    if (message == NULL) {
        perror("malloc");
        return;
    }

    message[0] = ROOM_TYPE; // ROOM message type
    message[1] = room->room_number & 0xFF;
    message[2] = (room->room_number >> 8) & 0xFF;

    strncpy((char *)message + 3, room->name, MAX_NAME_LENGTH);
    message[3 + MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

    message[35] = description_length & 0xFF;
    message[36] = (description_length >> 8) & 0xFF;
    memcpy(message + 37, room->description, description_length);

    ssize_t bytes_sent = send(client_fd, message, message_size, 0);
    if (bytes_sent == -1) {
        perror("send ROOM");
    } else {
        printf("Sent ROOM message for room %d\n", room->room_number);
    }

    free(message);  // Free the message memory
}

// Connection message function
void send_connection_message(int client_fd, Room *room) {
    uint16_t description_length = strlen(room->description);
    size_t message_size = 37 + description_length;
    uint8_t *message = (uint8_t *)malloc(message_size);

    if (message == NULL) {
        perror("malloc");
        return;
    }

    message[0] = CONNECTION_TYPE; // CONNECTION message type
    message[1] = room->room_number & 0xFF;
    message[2] = (room->room_number >> 8) & 0xFF;

    strncpy((char *)message + 3, room->name, MAX_NAME_LENGTH);
    message[3 + MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

    message[35] = description_length & 0xFF;
    message[36] = (description_length >> 8) & 0xFF;
    memcpy(message + 37, room->description, description_length);

    ssize_t bytes_sent = send(client_fd, message, message_size, 0);
    if (bytes_sent == -1) {
        perror("send CONNECTION");
    } else {
        printf("Sent CONNECTION message for room %d\n", room->room_number);
    }

    free(message);
    printf("send_connection_message is ending\n");
}

// Function to receive messages
void receive_message(int sender_fd, char *buffer, size_t buffer_size) {
    // Read the message from the sender
    ssize_t bytes_received = recv(sender_fd, buffer, 66, MSG_WAITALL);
    char msg_buffer[MAX_MESSAGE_LENGTH]; // Buffer for the actual message body
    printf("Read message from %d\n", sender_fd);
    
    if (bytes_received <= 0) {
        if (bytes_received == 0) {
            printf("Client disconnected\n");
        } else {
            perror("recv");
        }
        return;
    }

    // Check if the message is valid and has enough bytes
    if (bytes_received < 66) { // Need at least 66 bytes
        printf("Received message too short: %ld bytes\n", bytes_received);
        return;
    }

    // Extract message length
    uint16_t message_length;
    memcpy(&message_length, buffer, sizeof(message_length)); // Copy the first two bytes
    //message_length = ntohs(message_length); // Convert to host byte order
    printf("Message length: %d\n", message_length);

    if (message_length > MAX_MESSAGE_LENGTH) {
        printf("Message length exceeds maximum size.\n");
        return;
    }

    // Extract recipient name
    char recipient_name[MAX_NAME_LENGTH + 1]; // 32 bytes + null terminator
    memcpy(recipient_name, buffer + 2, MAX_NAME_LENGTH); // Start from buffer[2]
    recipient_name[MAX_NAME_LENGTH] = '\0'; // Null-terminate
    printf("Recipient Name: %s\n", recipient_name);

    // Extract sender name
    char sender_name[MAX_NAME_LENGTH + 1]; // 32 bytes + null terminator
    memcpy(sender_name, buffer + 34, MAX_NAME_LENGTH); // Start from buffer[34]
    sender_name[MAX_NAME_LENGTH] = '\0'; // Null-terminate
    printf("Sender Name: %s\n", sender_name);

    // Handle narration marker
    uint8_t narration_marker = buffer[64]; // At buffer[64]
    printf("Narration marker: %d\n", narration_marker);

    // Extract the actual message
    ssize_t msg_bytes_received = recv(sender_fd, msg_buffer, message_length, MSG_WAITALL);
    if (msg_bytes_received <= 0) {
        if (msg_bytes_received == 0) {
            printf("Client disconnected during message body read.\n");
        } else {
            perror("recv");
        }
        return;
    }

    if (msg_bytes_received < message_length) {
        printf("Received only %ld bytes of the message body.\n", msg_bytes_received);
        return; // Handle partial message as needed
    }

    // Null-terminate the message body
    msg_buffer[msg_bytes_received] = '\0'; // Ensure null-terminated string
    printf("Message: %s\n", msg_buffer);

    // Prepare to send this message to the correct client
    send_message(recipient_name, sender_name, msg_buffer, msg_bytes_received, narration_marker);
}

// Function to send messages
void send_message(const char *recipient_name, const char *sender_name, const char *message, size_t message_length, uint8_t narration_marker) {
    // Allocate buffer for the entire message
    size_t total_length = 67 + message_length; // 67 bytes for header + message length
    char *buffer = (char *)malloc(total_length);
    if (!buffer) {
        perror("malloc");
        return;
    }

    // Set the message type (1)
    buffer[0] = 1;

    // Set the message length (little-endian)
    buffer[1] = (message_length & 0xFF);           // Lower byte
    buffer[2] = ((message_length >> 8) & 0xFF);    // Upper byte

    // Copy recipient name
    strncpy(buffer + 3, recipient_name, MAX_NAME_LENGTH);
    buffer[3 + MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

    // Copy sender name
    strncpy(buffer + 35, sender_name, MAX_NAME_LENGTH);
    buffer[35 + MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
    buffer[66] = narration_marker; // Set narration marker

    // Copy the actual message body
    memcpy(buffer + 67, message, message_length);

    // Find the recipient using thread_args (not defined here)
    for (int i = 0; i < thread_count; i++) { // Assumes thread_count is defined elsewhere
        ThreadArgs *args = thread_args[i]; // Assumes thread_args is defined elsewhere
        Character player = find_player_by_fd(args->client_fd); // Assumes find_player_by_fd is defined elsewhere

        if (strcmp(player.name, recipient_name) == 0) {
            ssize_t bytes_sent = send(args->client_fd, buffer, total_length, 0);
            if (bytes_sent == -1) {
                perror("send");
            }
            printf("Message sent to %s\n", player.name);
            free(buffer); // Free the allocated buffer
            return; // Exit after sending the message
        }
    }

    printf("Recipient not found: %s\n", recipient_name);
    free(buffer); // Free buffer if recipient not found
}

void send_message_to_player(int player_fd, const char *message) {
    // Allocate buffer for the entire message
    size_t message_length = strlen(message);
    size_t total_length = 67 + message_length; // 67 bytes for header + message length
    char *buffer = (char *)malloc(total_length);
    if (!buffer) {
        perror("malloc");
        return;
    }

    // Set the message type (1) — for simplicity, we assume it’s a standard message
    buffer[0] = 1;

    // Set the message length (little-endian)
    buffer[1] = (message_length & 0xFF);           // Lower byte
    buffer[2] = ((message_length >> 8) & 0xFF);    // Upper byte

    // Copy player name (from the player_fd or player object)
    // Assuming you have a way to get the player's name from their FD, e.g. `find_player_by_fd()`
    Character player = find_player_by_fd(player_fd); // Assuming this function exists

    strncpy(buffer + 3, player.name, MAX_NAME_LENGTH);
    buffer[3 + MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

    // Copy sender name (e.g., the server or NPC)
    const char *sender_name = "Server"; // Or something else depending on your design
    strncpy(buffer + 35, sender_name, MAX_NAME_LENGTH);
    buffer[35 + MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination

    buffer[66] = 0; // No narration marker needed, or you can customize this

    // Copy the actual message body
    memcpy(buffer + 67, message, message_length);

    // Send the message
    ssize_t bytes_sent = send(player_fd, buffer, total_length, 0);
    if (bytes_sent == -1) {
        perror("send");
    }

    printf("Message sent to player: %s\n", player.name);
    free(buffer); // Free the allocated buffer
}

// Function to handle room change requests
void change_room(ThreadArgs *args, uint16_t requested_room_number) {
    int client_fd = args->client_fd;
    Character *player = find_pp_by_fd(client_fd);
    
    // Check if the requested room exists
    Room *new_room = &rooms[requested_room_number - 1];
    Room *old_room = &rooms[player->room_number - 1];

    if (new_room == NULL) {
        // Send an error if the room is invalid
        send_error_message(client_fd, 1, "Invalid room number.");
        return;
    }
    
    int is_valid_connection = 0; // Use int for boolean in C
    for (size_t i = 0; i < old_room->num_connections; i++) {
        if (old_room->connections[i]->room_number == requested_room_number) {
            is_valid_connection = 1; // true
            break;
        }
    }

    if (!is_valid_connection) {
        send_error_message(client_fd, 1, "You cannot access that room from here.");
        return;
    }

    // Remove player from the current room
    remove_player_from_room(&rooms[player->room_number - 1], player); // Correct room reference

    // Update player's room number
    player->room_number = new_room->room_number;

    // Notify existing players in the old room about the player leaving
    for (size_t i = 0; i < old_room->num_players; i++) {
        if (old_room->players[i] != NULL && old_room->players[i] != player) {
            send_character_message(old_room->players[i]->client_fd, player, player->name, player->description);
        }
    }

    // Add player to the new room
    add_player_to_room(new_room, player);
    
    // Notify existing players in the new room about the new player
    for (size_t i = 0; i < new_room->num_players; i++) {
        if (new_room->players[i] != NULL && new_room->players[i] != player) {
            send_character_message(new_room->players[i]->client_fd, player, player->name, player->description);
        }
    }
    
    send_room_message(client_fd, new_room, player);

    // Send the updated CHARACTER message for the player
    send_character_message(client_fd, player, player->name, player->description);

    // Send CHARACTER messages for all players in the new room
    for (size_t i = 0; i < new_room->num_players; i++) {
        if (new_room->players[i] != NULL) {
            send_character_message(client_fd, new_room->players[i], new_room->players[i]->name, new_room->players[i]->description);
        }
    }

    // Send CHARACTER messages for all monsters in the new room
    for (size_t i = 0; i < new_room->num_monsters; i++) {
        if (new_room->monsters[i] != NULL) {
            send_character_message(client_fd, new_room->monsters[i], new_room->monsters[i]->name, new_room->monsters[i]->description);
        }
    }

    // Send CONNECTION messages for the new room
    for (size_t i = 0; i < new_room->num_connections; i++) {
        send_connection_message(client_fd, new_room->connections[i]);
    }
}

// Function to allocate memory for a string
char* allocate_string(const char* str) {
    // Allocate memory for the string
    char* new_string = (char*)malloc(strlen(str) + 1); // +1 for null terminator
    if (new_string != NULL) {
        strcpy(new_string, str); // Copy the string
    }
    return new_string; // Return the allocated string (or NULL if allocation failed)
}

void calc_damage(Character *attacker, Character *defender) {
    if (!is_alive(attacker) || !is_alive(defender)) {
        printf("One of the characters is not alive.\n");
        return;
    }

    // Damage calculation logic
    int damage = attacker->attack - defender->defense;
    if (damage < 0) damage = 0;

    defender->health -= damage;

    // Update alive flag if health falls to 0 or below
    if (defender->health <= 0) {
        defender->health = 0;

        // Set flags for defeated monster
        if (defender->flags & 0x10) { // If it's a monster
            defender->flags = 0x78; // Set to: alive = 0, join battle = 1, monster = 1, started = 1, ready = 1
        } else { // Otherwise, it's a player
            defender->flags &= ~0x80; // Only set alive = 0 for players
        }

        printf("%s has been defeated! Flags: 0x%X\n", defender->name, defender->flags);
    }
}

void fight(Character *player, Character *monster) {
    // Check if both characters are alive
    if (!is_alive(player)) {
        printf("%s cannot fight because they are not alive.\n", player->name);
        return;
    }
    
    if (!is_alive(monster)) {
        printf("%s cannot fight because they are not alive.\n", monster->name);
        return;
    }

    // Player attacks the monster
    printf("%s attacks %s!\n", player->name, monster->name);
    calc_damage(player, monster);

    // Send character messages to other players in the room
    notify_players_in_room(player, monster);

    // Check if the monster is still alive
    if (!is_alive(monster)) {
        printf("%s has been defeated!\n", monster->name);

        // Send a message to the player after the monster is defeated
        char message[256];
        snprintf(message, sizeof(message), "You have defeated %s!", monster->name);
        send_message_to_player(player->client_fd, message);
        return; // Monster has been defeated; exit the fight function
    }

    // Monster attacks the player
    printf("%s attacks %s!\n", monster->name, player->name);
    calc_damage(monster, player);

    // Send character messages to other players in the room
    notify_players_in_room(player, monster);

    // Check if the player is still alive
    if (!is_alive(player)) {
        printf("%s has been defeated!\n", player->name);
        return; // Player has been defeated; exit the fight function
    }
}

void loot(ThreadArgs *args) {
    int client_fd = args->client_fd;
    uint8_t buffer[32]; // 32 bytes for the name
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);

    if (bytes_received <= 0) {
        if (bytes_received == 0) {
            printf("Client disconnected\n");
        } else {
            perror("recv");
        }
        return;
    }

    // Extract the target player's name (ensure null-termination)
    char target_name[NAME_LENGTH + 1]; // Assuming NAME_LENGTH is defined
    memcpy(target_name, buffer, NAME_LENGTH);
    target_name[NAME_LENGTH] = '\0';

    // Find the target character (monster or player) in the room
    Room *room = find_room_by_character(find_pp_by_fd(client_fd));
    if (room == NULL) {
        printf("Could not find the room for the character.\n");
        return;
    }

    Character *target = NULL;

    // Check for dead monsters in the room (case insensitive)
    for (size_t i = 0; i < room->num_monsters; i++) {
        if (room->monsters[i] != NULL && strcasecmp(room->monsters[i]->name, target_name) == 0 && !is_alive(room->monsters[i])) {
            target = room->monsters[i];
            break;
        }
    }

    // Check for dead players in the room (case insensitive)
    if (target == NULL) {
        pthread_mutex_lock(&player_mutex);
        for (size_t i = 0; i < player_count; i++) {
            if (strcasecmp(global_players[i].name, target_name) == 0 && !is_alive(&global_players[i])) {
                target = &global_players[i];
                break;
            }
        }
        pthread_mutex_unlock(&player_mutex);
    }

    // If the target is found and is dead, loot their gold
    if (target != NULL) {
        // Add gold to the player's character state
        Character *player = find_pp_by_fd(client_fd);
        player->gold += target->gold;

        // Send updated CHARACTER message to the player
        send_character_message(client_fd, player, player->name, player->description);

        printf("%s looted %d gold from %s.\n", player->name, target->gold, target_name);
        // Optionally, set the target's gold to 0
        target->gold = 0;
    } else {
        printf("No dead player or monster found with the name: %s\n", target_name);
        // Optionally send an error message back to the player
        send_error_message(client_fd, 6, "No dead target found to loot.");
    }
}

void notify_players_in_room(Character *player, Character *monster) {
    Room *room = &rooms[player->room_number - 1]; // Assuming room numbers are 1-indexed

    for (size_t i = 0; i < room->num_players; i++) {
        if (room->players[i] != NULL && room->players[i] != player) {
            // Send character update messages for both player and monster
            send_character_message(room->players[i]->client_fd, player, player->name, player->description);
            send_character_message(room->players[i]->client_fd, monster, monster->name, monster->description);
        }
    }
}

char* read_description(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open description file");
        return NULL;
    }

    char* description = malloc(MAX_DESCRIPTION_LENGTH);
    if (!description) {
        perror("Failed to allocate memory for description");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(description, sizeof(char), MAX_DESCRIPTION_LENGTH - 1, file);
    description[bytesRead] = '\0'; // Null-terminate the string

    fclose(file);
    return description; // Return the dynamically allocated buffer
}