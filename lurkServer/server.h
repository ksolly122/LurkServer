#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>

#define FLAG_ALIVE       0x01 // 00000001
#define FLAG_JOIN_BATTLE 0x02 // 00000010
#define FLAG_MONSTER     0x04 // 00000100
#define FLAG_STARTED (1 << 4)
#define FLAG_READY       0x10 // 00010000

#define MESSAGE_TYPE 1
#define CHANGE_ROOM_TYPE 2
#define FIGHT_TYPE 3
#define PVP_TYPE 4
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

#define MAX_CONNECTIONS 100
#define INITIAL_POINTS_MAX 100
#define STAT_LIMIT_MAX 100
#define NAME_LENGTH 32
#define MAX_DESCRIPTION_LENGTH 10000
#define MAX_MESSAGE_LENGTH 1024
#define MAX_PLAYERS 100
#define MAX_MONSTERS 100
#define MAX_ROOMS 20
#define MAX_NAME_LENGTH 32
#define MAX_SENDER_NAME_LENGTH 30

typedef struct {
    int client_fd;
    int final_message_sent;
    int end_message_sent;
} ThreadArgs;

// Declare global variables as extern
extern char* DESCRIPTION; 
extern ThreadArgs *thread_args[MAX_CONNECTIONS];
extern int thread_count;
extern pthread_mutex_t thread_count_mutex;

typedef struct {
    int alive;      // 1 for alive, 0 for not
    int join_battle; // 1 for join battles, 0 for do not
    int monster;    // 1 for monster, 0 for player
    int started;    // 1 for started, 0 for not
    int ready;      // 1 for ready, 0 for not
    uint8_t type;
    char name[NAME_LENGTH + 1];
    uint8_t flags;
    uint16_t attack;
    uint16_t defense;
    uint16_t regen;
    int16_t health;
    uint16_t gold;
    uint16_t room_number;
    uint16_t description_length;
    char *description;
    size_t description_received;
    size_t total_received;
    int client_fd;
} Character;

// Declare global variables as extern
extern Character global_players[MAX_PLAYERS];
extern size_t player_count;
extern pthread_mutex_t player_mutex;
extern Character monsters[MAX_MONSTERS];
extern size_t num_monsters;

typedef struct Room {
    uint16_t room_number;
    char *name;
    char *description;
    struct Room *connections[4];
    size_t num_connections;

    Character *players[MAX_PLAYERS];
    size_t num_players;

    Character *monsters[MAX_MONSTERS];
    size_t num_monsters;
} Room;

// Declare global variables as extern
extern Room rooms[MAX_ROOMS];

// Function prototypes
char* allocate_string(const char* str);
int add_player_to_room(Room *room, Character *player);
int remove_player_from_room(Room *room, Character *player);
void send_version_message(int client_fd);
void send_game_message(int client_fd);
void send_accept_message(int client_fd, uint8_t action_type);
void send_error_message(int client_fd, uint8_t error_code, const char *error_message);
void send_character_message(int client_fd, Character *character_state, const char *player_name, const char *description);
void handle_character(ThreadArgs *args);
void change_room(ThreadArgs *args, uint16_t requested_room_number);
void loot(ThreadArgs *args);
void *handle_client(void *args);
void handle_client_disconnection(int client_fd);
void send_room_message(int client_fd, Room *room, Character *character_state);
void send_connection_message(int client_fd, Room *room);
void send_message(const char *recipient_name, const char *sender_name, const char *message, size_t message_length, uint8_t narration_marker);
void send_message_to_player(int player_fd, const char *message);
void receive_message(int sender_fd, char *buffer, size_t buffer_size);
void calc_damage(Character *attacker, Character *defender);
void fight(Character *player, Character *monster);
Character *get_monster_in_room(int room_number, int monster_index);
int is_alive(Character *character);
int is_started(Character *character);
void notify_players_in_room(Character *player, Character *monster);
int is_valid_room_index(int index);
void initialize_rooms();
void get_monsters();
Character find_player_by_fd(int client_fd);
Character* find_pp_by_fd(int client_fd);
Room* find_room_by_character(Character *character);
char* read_description(const char* filename);

#endif 
