# Lurk Server (C-Based Multiplayer Server)

This project is a multi-file server application written in C that simulates a text-based multiplayer environment. It demonstrates socket programming, client handling, and modular software design.

## Overview
- Handles multiple client connections
- Organizes logic into modules (rooms, characters, utilities)
- Demonstrates systems-level networking in C

## Build
Run:
make

## Run
./server

## Structure
- main.c – Server entry point
- client.c – Client logic
- client_handler.c – Handles connections
- room.c – Room logic
- character.c – Player behavior
- utils.c – Helper functions
- server.h – Shared structures

## Notes
Compiled files have been removed for clarity. Use the Makefile to rebuild.
