#include "server.h"

Character find_player_by_fd(int client_fd) {
    for (int i = 0; i < player_count; i++) {
        if (global_players[i].client_fd == client_fd) {
            return global_players[i];  // Return a copy of the character state
        }
    }
    // Return a default state if not found
    return (Character){0};  // Assuming zeroed values indicate not found
}

Character* find_pp_by_fd(int client_fd) {
    // Assume players is an array of Character
    for (int i = 0; i < player_count; i++) {
        if (global_players[i].client_fd == client_fd) {
            return &global_players[i]; // Return a pointer to the found player
        }
    }
    return NULL; // Return NULL if not found
}

int is_alive(Character *character) {
    return (character->flags & 0x80) != 0; // Check if the highest bit (Bit 7) is set
}

int is_started(Character *character) {
    return (character->flags & FLAG_STARTED) != 0;
}

void get_monsters() {
    // Check if there's space for more monsters
    if (num_monsters >= MAX_MONSTERS) {
        printf("Maximum number of monsters reached.\n");
        return;
    }

     // Create monster instances
    Character monster1;
    monster1.alive = 1;
    monster1.join_battle = 1;
    monster1.monster = 1;
    monster1.started = 1;
    monster1.ready = 1;
    monster1.type = 0; 
    strcpy(monster1.name, "Soldier of Godrick");
    monster1.flags = 0xb8;
    monster1.attack = 7;
    monster1.defense = 3;
    monster1.regen = 1;
    monster1.health = 15;
    monster1.gold = 10;
    monster1.room_number = 2;
    const char *description1 = "Warriors who have pledged their loyalty to Godrick the Grafted. Clad in makeshift armor, they wield rusted swords and shields, ready to defend their lord against any intruder.";
    monster1.description_length = strlen(description1);
    monster1.description = (char *)malloc(strlen(description1) + 1);
    if (monster1.description != NULL) {
        strcpy(monster1.description, description1);
    }

    Character monster2;
    monster2.alive = 1;
    monster2.join_battle = 1;
    monster2.monster = 1;
    monster2.started = 1;
    monster2.ready = 1;
    monster2.type = 0;
    strcpy(monster2.name, "Grafted Scion");
    monster2.flags = 0xb8;
    monster2.attack = 10;
    monster2.defense = 5;
    monster2.regen = 2;
    monster2.health = 25;
    monster2.gold = 15;
    monster2.room_number = 5;
    const char *description2 = "A grotesque amalgamation of limbs, the Grafted Scion serves as a chilling reminder of the cost of ambition. Its multiple appendages and eerie form make it a formidable foe.";
    monster2.description_length = strlen(description2);
    monster2.description = (char *)malloc(strlen(description2) + 1);
    if (monster2.description != NULL) {
        strcpy(monster2.description, description2);
    }

    Character monster3;
    monster3.alive = 1;
    monster3.join_battle = 1;
    monster3.monster = 1;
    monster3.started = 1;
    monster3.ready = 1;
    monster3.type = 0;
    strcpy(monster3.name, "Misbegotten Warrior");
    monster3.flags = 0xb8;
    monster3.attack = 9;
    monster3.defense = 4;
    monster3.regen = 2;
    monster3.health = 22;
    monster3.gold = 12;
    monster3.room_number = 8;
    const char *description3 = "Twisted by the influence of the Greater Will, these warriors are marked by their grotesque features. They are driven by rage and the desire to reclaim their lost honor.";
    monster3.description_length = strlen(description3);
    monster3.description = (char *)malloc(strlen(description3) + 1);
    if (monster3.description != NULL) {
        strcpy(monster3.description, description3);
    }

    Character monster4;
    monster4.alive = 1;
    monster4.join_battle = 1;
    monster4.monster = 1;
    monster4.started = 1;
    monster4.ready = 1;
    monster4.type = 0;
    strcpy(monster4.name, "Demi-Human"); 
    monster4.flags = 0xb8;
    monster4.attack = 6;
    monster4.defense = 3;
    monster4.regen = 2;
    monster4.health = 18;
    monster4.gold = 8;
    monster4.room_number = 3;
    const char *description4 = "Beasts that lurk in the shadows, Demi-Humans are known for their agility and cunning. Their chaotic nature makes them unpredictable in battle.";
    monster4.description_length = strlen(description4);
    monster4.description = (char *)malloc(strlen(description4) + 1);
    if (monster4.description != NULL) {
        strcpy(monster4.description, description4);
    }

    Character monster5;
    monster5.alive = 1;
    monster5.join_battle = 1;
    monster5.monster = 1;
    monster5.started = 1;
    monster5.ready = 1;
    monster5.type = 0;
    strcpy(monster5.name, "Ghost of the Fallen");
    monster5.flags = 0xb8;
    monster5.attack = 50;
    monster5.defense = 15;
    monster5.regen = 1;
    monster5.health = 90;
    monster5.gold = 15;
    monster5.room_number = 6;
    const char *description5 = "Once noble warriors, now reduced to mere shadows of their former selves. These spirits guard their resting places, wielding their ancient weapons with ghostly precision.";
    monster5.description_length = strlen(description5);
    monster5.description = (char *)malloc(strlen(description5) + 1);
    if (monster5.description != NULL) {
        strcpy(monster5.description, description5);
    }

    Character monster6;
    monster6.alive = 1;
    monster6.join_battle = 1;
    monster6.monster = 1;
    monster6.started = 1;
    monster6.ready = 1;
    monster6.type = 0;
    strcpy(monster6.name, "Deathbird");
    monster6.flags = 0xb8;
    monster6.attack = 70;
    monster6.defense = 5;
    monster6.regen = 2;
    monster6.health = 60;
    monster6.gold = 20;
    monster6.room_number = 9;
    const char *description6 = "A sinister creature of the night, the Deathbird swoops down on unsuspecting prey with its razor-sharp talons. Legends speak of its harbingers of death, striking fear into the hearts of the weak.";
    monster6.description_length = strlen(description6);
    monster6.description = (char *)malloc(strlen(description6) + 1);
    if (monster6.description != NULL) {
        strcpy(monster6.description, description6);
    }

    Character monster7;
    monster7.alive = 1;
    monster7.join_battle = 1;
    monster7.monster = 1;
    monster7.started = 1;
    monster7.ready = 1;
    monster7.type = 0;
    strcpy(monster7.name, "Landswitch Beast");
    monster7.flags = 0xb8;
    monster7.attack = 35;
    monster7.defense = 6;
    monster7.regen = 3;
    monster7.health = 50;
    monster7.gold = 20;
    monster7.room_number = 4;
    const char *description7 = "A formidable creature born of chaos, the Landswitch Beast can manipulate the terrain around it. Its massive size and ferocity make it a terrifying opponent.";
    monster7.description_length = strlen(description7);
    monster7.description = (char *)malloc(strlen(description7) + 1);
    if (monster7.description != NULL) {
        strcpy(monster7.description, description7);
    }

    Character monster8;
    monster8.alive = 1;
    monster8.join_battle = 1;
    monster8.monster = 1;
    monster8.started = 1;
    monster8.ready = 1;
    monster8.type = 0;
    strcpy(monster8.name, "Hollow Knight");
    monster8.flags = 0xb8;
    monster8.attack = 50;
    monster8.defense = 30;
    monster8.regen = 2;
    monster8.health = 45;
    monster8.gold = 18;
    monster8.room_number = 7;
    const char *description8 = "Once a noble protector of the realm, the Hollow Knight now wanders in search of purpose. Clad in rusted armor, it swings a longsword with surprising grace and power.";
    monster8.description_length = strlen(description8);
    monster8.description = (char *)malloc(strlen(description8) + 1);
    if (monster8.description != NULL) {
        strcpy(monster8.description, description8);
    }

    // Populate the monster array
    monsters[num_monsters++] = monster1;
    monsters[num_monsters++] = monster2;
    monsters[num_monsters++] = monster3;
    monsters[num_monsters++] = monster4;
    monsters[num_monsters++] = monster5;
    monsters[num_monsters++] = monster6;
    monsters[num_monsters++] = monster7;
    monsters[num_monsters++] = monster8;

    // Assign monsters to their respective rooms
    for (size_t i = 0; i < num_monsters; i++) {
        Character *monster = &monsters[i];
        Room *room = &rooms[monster->room_number - 1]; // Assuming room numbers are 1-indexed

        // Add to the room's monster list if space allows
        if (room->num_monsters < MAX_MONSTERS) {
            room->monsters[room->num_monsters++] = monster;
            
            // Debugging line to see which monster is added to which room
            printf("Monster '%s' added to room %d.\n", monster->name, monster->room_number);
        }
    }

    // Output message
    printf("Monsters populated in the rooms.\n");
}

/* void mini_boss() {
    // Create a mini-boss instance
    Character monster10;
    monster10.alive = 1;
    monster10.join_battle = 1;
    monster10.monster = 1;
    monster10.started = 1;
    monster10.ready = 1;
    monster10.type = 0;
    strcpy(monster10.name, "Royal Knight Loretta");
    monster10.flags = 0xb8;
    monster10.attack = 40; 
    monster10.defense = 10; 
    monster10.regen = 5; 
    monster10.health = 120; 
    monster10.gold = 50; // Reward for defeating the mini-boss
    monster10.room_number = 14;

    const char *description = "A valiant knight adorned in ornate armor, Royal Knight Loretta wields a powerful spear and is known for her unparalleled skill in combat. With grace and ferocity, she can unleash devastating attacks and summon magical arts to dominate the battlefield. Loretta's unwavering loyalty to her fallen king fuels her relentless pursuit of intruders, making her a daunting challenge for any who dare to face her.";
    monster10.description_length = strlen(description);
    monster10.description = (char *)malloc(strlen(description) + 1);
    if (monster10.description != NULL) {
        strcpy(monster10.description, description);
    }
    monster10.description_received = 0;
    monster10.total_received = 0;
    monster10.client_fd = -1;

    // Add the monster to the monsters array
    monsters[num_monsters++] = monster10;

    // Access the monster and room data
    Character *monster = &monsters[9];
    Room *room = &rooms[13]; // Assuming room numbers are 1-indexed

    // Add to the room's monster list if space allows
    if (room->num_monsters < MAX_MONSTERS) {
        room->monsters[room->num_monsters++] = monster;
    }
}*/

void handle_character(ThreadArgs *args) {
    int client_fd = args->client_fd;
    Character state = {0};
    state.description = NULL;
    state.description_received = 0;
    state.total_received = 0;
    state.client_fd = client_fd;

    uint16_t remaining_stat_limit = STAT_LIMIT_MAX;

    while (1) {
        uint8_t buffer[1024];
        uint8_t description_buffer[512];
        ssize_t bytes_received = recv(client_fd, buffer, 47, MSG_WAITALL);

        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                printf("Client disconnected\n");
            } else {
                perror("recv");
            }
            return;
        }

        // Handle the initial part of the message
        if (state.total_received == 0) {
            if (bytes_received < 47) { // Adjusted to 47 to align with the expected length
                printf("Received insufficient data: %ld bytes\n", bytes_received);
                continue;
            }

            // Now buffer[0] is not CHARACTER_TYPE, so we skip it
            // Extract initial data (shifting indices by 1)
            memcpy(state.name, buffer, NAME_LENGTH);
            state.name[NAME_LENGTH] = '\0';
            state.flags = buffer[32]; // Previous buffer[33]
            state.attack = buffer[33] | (buffer[34] << 8); // Previous buffer[34] and buffer[35]
            state.defense = buffer[35] | (buffer[36] << 8); // Previous buffer[36] and buffer[37]
            state.regen = buffer[37] | (buffer[38] << 8); // Previous buffer[38] and buffer[39]
            state.health = (int16_t)(buffer[39] | (buffer[40] << 8)); // Previous buffer[40] and buffer[41]
            state.gold = buffer[41] | (buffer[42] << 8); // Previous buffer[42] and buffer[43]
            state.room_number = buffer[43] | (buffer[44] << 8); // Previous buffer[44] and buffer[45]
            state.description_length = buffer[45] | (buffer[46] << 8); // Previous buffer[46] and buffer[47]
            state.total_received += 47;

            // Allocate memory for description
            state.description = (char *)malloc(state.description_length + 1);
            if (state.description == NULL) {
                perror("malloc");
                return;
            }

            // Check if description length is valid
            if (state.description_length > sizeof(description_buffer)) {
                fprintf(stderr, "Description length exceeds buffer size\n");
                free(state.description);
                return;
            }

            ssize_t desc_bytes_received = recv(client_fd, description_buffer, state.description_length, MSG_WAITALL);
            if (desc_bytes_received < 0) {
                perror("recv for description");
                free(state.description);
                return;
            }

            // Ensure we received the expected amount of data
            if (desc_bytes_received != state.description_length) {
                fprintf(stderr, "Received incorrect description length: expected %d, got %ld\n",
                        state.description_length, desc_bytes_received);
                free(state.description);
                return;
            }

            // Copy the description correctly
            memcpy(state.description, description_buffer, state.description_length);
            state.description[state.description_length] = '\0'; // Null-terminate

            // Validate individual stats
            uint16_t total_stats = state.attack + state.defense + state.regen;
            if (total_stats > remaining_stat_limit) {
                // Calculate exceeding amount
                uint16_t excess = total_stats - remaining_stat_limit;

                // Send error message
                char error_message[128];
                snprintf(error_message, sizeof(error_message), "Total stats exceed limit by %d", excess);
                send_error_message(client_fd, 4, error_message);

                // Await corrected stats
                int try_again = 0;
                while (try_again < 5) {
                    try_again++;
                    bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
                    if (bytes_received <= 0) {
                        if (bytes_received == 0) {
                            printf("Client disconnected\n");
                        } else {
                            perror("recv");
                        }
                        return;
                    }

                    // Check the first byte (now we have already handled it)
                    // Assuming we still want to check it
                    if (buffer[0] != CHARACTER_TYPE) {
                        printf("Received message with unexpected type %d\n", buffer[0]);
                        continue;
                    }

                    // Extract corrected stats (again shifting indices by 1)
                    state.attack = buffer[34] | (buffer[35] << 8);
                    state.defense = buffer[36] | (buffer[37] << 8);
                    state.regen = buffer[38] | (buffer[39] << 8);

                    total_stats = state.attack + state.defense + state.regen;
                    if (total_stats <= remaining_stat_limit) {
                        break; // Valid stats
                    } else {
                        snprintf(error_message, sizeof(error_message), "Corrected stats still exceed limit by %d", total_stats - remaining_stat_limit);
                        send_error_message(client_fd, 4, error_message);
                    }
                }
                if (try_again >= 5) {
                    handle_client_disconnection(client_fd);
                }
            }

            // Check for existing player with the same name
            Character *existing_player = NULL;

            pthread_mutex_lock(&player_mutex);
            for (size_t i = 0; i < player_count; i++) {
                if (strcasecmp(global_players[i].name, state.name) == 0) {
                    existing_player = &global_players[i];
                    break;
                }
            }

            // If a match is found, reuse that player
            if (existing_player) {
                printf("Reusing existing character: %s\n", existing_player->name);

                // Assign the new client's state to the existing character
                existing_player->client_fd = client_fd; // Update client file descriptor
                existing_player->flags = 0xC8;     // Update flags

                // Free old description and assign new one
                free(existing_player->description);
                existing_player->description_length = state.description_length;
                existing_player->description = (char *)malloc(state.description_length + 1);
                if (existing_player->description != NULL) {
                    strcpy(existing_player->description, state.description);
                }

                pthread_mutex_unlock(&player_mutex);
                send_accept_message(client_fd, CHARACTER_TYPE);
                return; // Exit after updating existing player
            }

            // Initialize new character if no match is found
            if (player_count < MAX_PLAYERS) {
                // Set default values
                state.room_number = 1;  // Set room to 1
                state.flags = 0xC8;     // Set flags to C8
                state.health = 100;      // Set health to 100
                state.gold = 0;
                // state.regen = 0;

                global_players[player_count++] = state; // Store the character state
            }

            pthread_mutex_unlock(&player_mutex);

            // Print valid character data
            printf("Received valid character data:\n");
            printf("Name: %s\n", state.name);
            printf("Flags: %02X\n", state.flags);
            printf("Attack: %d\n", state.attack);
            printf("Defense: %d\n", state.defense);
            printf("Regen: %d\n", state.regen);
            printf("Health: %d\n", state.health);
            printf("Gold: %d\n", state.gold);
            printf("Room Number: %d\n", state.room_number);
            printf("Description Length: %d\n", state.description_length);
            printf("Description: %s\n", state.description);

            // Send ACCEPT message after confirming valid stats
            send_accept_message(client_fd, CHARACTER_TYPE);
            break; // Exit the main loop after sending the accept message
        }
    }
}
