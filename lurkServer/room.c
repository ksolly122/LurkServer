#include "server.h"

Room* find_room_by_character(Character *character) {
    if (character == NULL) {
        printf("Invalid character state provided.\n");
        return NULL;
    }

    // Assuming room_number is 1-indexed
    uint16_t room_index = character->room_number - 1;

    // Check if the room index is valid
    if (room_index < 0 || room_index >= MAX_ROOMS) { // Adjust MAX_ROOMS to your actual limit
        printf("Room number %d is out of bounds.\n", character->room_number);
        return NULL;
    }

    return &rooms[room_index]; // Return the corresponding room
}

int add_player_to_room(Room *room, Character *player) {
    if (room->num_players < MAX_PLAYERS) {
        room->players[room->num_players++] = player;
        return 0; // Success
    }
    return -1; // Room is full
}

int remove_player_from_room(Room *room, Character *player) {
    for (size_t i = 0; i < room->num_players; ++i) {
        if (room->players[i] == player) {
            // Shift remaining players down
            for (size_t j = i; j < room->num_players - 1; ++j) {
                room->players[j] = room->players[j + 1];
            }
            room->num_players--;
            return 0; // Success
        }
    }
    return -1; // Player not found
}

// Check if a room index is valid
int is_valid_room_index(int index) {
    return index >= 0 && index < MAX_ROOMS;
}

// Initialize each room with bounds checking
void initialize_rooms() {
    // Initialize room 1
    rooms[0].room_number = 1;
    rooms[0].name = allocate_string("Abandoned Catacombs Entrance");
    rooms[0].description = allocate_string("The entrance to the catacombs is marked by overgrown vines and crumbling stones...");
    rooms[0].num_connections = 2;
    if (is_valid_room_index(1)) rooms[0].connections[0] = &rooms[1];
    if (is_valid_room_index(4)) rooms[0].connections[1] = &rooms[4];
    rooms[0].num_players = 0;
    rooms[0].num_monsters = 0;

    // Initialize room 2
    rooms[1].room_number = 2;
    rooms[1].name = allocate_string("Forgotten Hall of the Elden");
    rooms[1].description = allocate_string("A dimly lit hall with ancient tapestries depicting battles of old...");
    rooms[1].num_connections = 4;
    if (is_valid_room_index(0)) rooms[1].connections[0] = &rooms[0];
    if (is_valid_room_index(2)) rooms[1].connections[1] = &rooms[2];
    if (is_valid_room_index(3)) rooms[1].connections[2] = &rooms[3];
    if (is_valid_room_index(5)) rooms[1].connections[3] = &rooms[5];
    rooms[1].num_players = 0;
    rooms[1].num_monsters = 0;

    // Initialize room 3
    rooms[2].room_number = 3;
    rooms[2].name = allocate_string("Chamber of Lost Souls");
    rooms[2].description = allocate_string("An expansive chamber filled with lingering spirits...");
    rooms[2].num_connections = 2;
    if (is_valid_room_index(1)) rooms[2].connections[0] = &rooms[1];
    if (is_valid_room_index(6)) rooms[2].connections[1] = &rooms[6];
    rooms[2].num_players = 0;
    rooms[2].num_monsters = 0;

    // Initialize room 4
    rooms[3].room_number = 4;
    rooms[3].name = allocate_string("Guardian's Resting Place");
    rooms[3].description = allocate_string("A serene yet eerie room, dominated by the remains of a fallen guardian...");
    rooms[3].num_connections = 2;
    if (is_valid_room_index(1)) rooms[3].connections[0] = &rooms[1];
    if (is_valid_room_index(7)) rooms[3].connections[1] = &rooms[7];
    rooms[3].num_players = 0;
    rooms[3].num_monsters = 0;

    // Initialize room 5
    rooms[4].room_number = 5;
    rooms[4].name = allocate_string("Secret Passage of the Ancients");
    rooms[4].description = allocate_string("A hidden passage, damp and cold, with walls lined with ancient carvings...");
    rooms[4].num_connections = 1;
    if (is_valid_room_index(0)) rooms[4].connections[0] = &rooms[0];
    rooms[4].num_players = 0;
    rooms[4].num_monsters = 0;

    // Initialize room 6
    rooms[5].room_number = 6;
    rooms[5].name = allocate_string("Cursed Crypt");
    rooms[5].description = allocate_string("A foreboding crypt, filled with the scent of decay...");
    rooms[5].num_connections = 2;
    if (is_valid_room_index(1)) rooms[5].connections[0] = &rooms[1];
    if (is_valid_room_index(8)) rooms[5].connections[1] = &rooms[8];
    rooms[5].num_players = 0;
    rooms[5].num_monsters = 0;

    // Initialize room 7
    rooms[6].room_number = 7;
    rooms[6].name = allocate_string("Altar of the Forsaken");
    rooms[6].description = allocate_string("An ancient altar draped in shadows, adorned with relics of the past...");
    rooms[6].num_connections = 2;
    if (is_valid_room_index(2)) rooms[6].connections[0] = &rooms[2];
    if (is_valid_room_index(9)) rooms[6].connections[1] = &rooms[9];
    rooms[6].num_players = 0;
    rooms[6].num_monsters = 0;

    // Initialize room 8
    rooms[7].room_number = 8;
    rooms[7].name = allocate_string("Veil of Shadows");
    rooms[7].description = allocate_string("A darkened room where shadows flicker and whisper...");
    rooms[7].num_connections = 1;
    if (is_valid_room_index(3)) rooms[7].connections[0] = &rooms[3];
    rooms[7].num_players = 0;
    rooms[7].num_monsters = 0;

    // Initialize room 9
    rooms[8].room_number = 9;
    rooms[8].name = allocate_string("Hidden Sanctum");
    rooms[8].description = allocate_string("A concealed sanctum, the walls inscribed with mystical symbols...");
    rooms[8].num_connections = 2;
    if (is_valid_room_index(5)) rooms[8].connections[0] = &rooms[5];
    if (is_valid_room_index(9)) rooms[8].connections[1] = &rooms[9];
    rooms[8].num_players = 0;
    rooms[8].num_monsters = 0;

    // Initialize room 10
    rooms[9].room_number = 10;
    rooms[9].name = allocate_string("The Grand Hall");
    rooms[9].description = allocate_string("A vast hall once filled with the echoes of laughter and feasts...");
    rooms[9].num_connections = 2;
    if (is_valid_room_index(6)) rooms[9].connections[0] = &rooms[6];
    if (is_valid_room_index(11)) rooms[9].connections[1] = &rooms[11];
    rooms[9].num_players = 0;
    rooms[9].num_monsters = 0;

    // Initialize room 11
    rooms[10].room_number = 11;
    rooms[10].name = allocate_string("Hall of Secrets");
    rooms[10].description = allocate_string("A long hall where the walls seem to breathe with untold secrets...");
    rooms[10].num_connections = 2;
    if (is_valid_room_index(9)) rooms[10].connections[0] = &rooms[9];
    if (is_valid_room_index(11)) rooms[10].connections[1] = &rooms[11];
    rooms[10].num_players = 0;
    rooms[10].num_monsters = 0;

    // Initialize room 12
    rooms[11].room_number = 12;
    rooms[11].name = allocate_string("The Abyssal Depths");
    rooms[11].description = allocate_string("A deep, dark pit echoing with distant whispers of the lost...");
    rooms[11].num_connections = 2;
    if (is_valid_room_index(10)) rooms[11].connections[0] = &rooms[10];
    if (is_valid_room_index(13)) rooms[11].connections[1] = &rooms[12];
    rooms[11].num_players = 0;
    rooms[11].num_monsters = 0;

    // Initialize room 13
    rooms[12].room_number = 13;
    rooms[12].name = allocate_string("Celestial Sanctum");
    rooms[12].description = allocate_string("A breathtaking chamber illuminated by a celestial glow...");
    rooms[12].num_connections = 1;
    if (is_valid_room_index(11)) rooms[12].connections[0] = &rooms[11];
    // if (is_valid_room_index(13)) rooms[12].connections[1] = &rooms[13];
    rooms[12].num_players = 0;
    rooms[12].num_monsters = 0;
}

// Initialize the final room (Room 14) with bounds checking
/* void initialize_final_room() {
    rooms[12].num_connections = 2;
    rooms[12].connections[1] = &rooms[13];

    
    rooms[13].room_number = 14;
    rooms[13].name = allocate_string("Heart of the Dungeon");
    rooms[13].description = allocate_string("A pulsating heart of darkness, where the very essence of the dungeon converges...");
    rooms[13].num_connections = 1;
    if (is_valid_room_index(12)) rooms[13].connections[0] = &rooms[12];
    rooms[13].num_players = 0;
    rooms[13].num_monsters = 0;
}
*/