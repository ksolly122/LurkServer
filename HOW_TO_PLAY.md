# HOW TO PLAY & LURK PROTOCOL

## What is Lurk?

Lurk is a lightweight protocol designed for text-based multiplayer games (similar to MUDs).
It defines how a client and server communicate using structured messages.

The server handles:
- combat
- player stats
- rooms and movement
- rewards and gold

The client sends actions and displays results.

Full documentation:
https://isoptera.lcsc.edu/~seth/cs435/lurk_2.3.html


## How This Server Works

This project is a LURK-based multiplayer server.

Players can:
- connect to the same server
- move between rooms
- fight monsters or other players
- send messages
- collect gold

The server controls all game logic, while clients interact with it.


## How to Run the Server

In the project folder:

make
./server


## How to Connect (Play)

Option 1: Use included client

./client


Option 2: Use netcat

nc localhost <port>


## Basic Gameplay Flow

1. Connect to server
2. Send CHARACTER info
3. Send START
4. Move rooms (CHANGEROOM)
5. Use actions:
   - MESSAGE
   - FIGHT
   - PVPFIGHT
   - LOOT


## Important Notes

- Server controls all outcomes
- You receive updates at any time
- Not all servers support PvP
- Gameplay depends on server implementation
