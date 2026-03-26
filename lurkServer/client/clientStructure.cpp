#include "clientStructure.h"

int max_height, max_width, log_height, log_width;
using namespace std;
extern characterPKG me;
extern gamePKG game;
extern vector < characterPKG > Rplayer;
extern vector < characterPKG > monsters;
extern vector < connectionPKG > connections;
extern vector < characterPKG > lootables;
extern roomPKG currentRm;

// Function to update the stats after character creation
void update_stats_window(WINDOW* stats, const std::string& name, int atk, int def, int regen, int health, int gold, const std::string& description) {
    wclear(stats); // Clear the window
    box(stats, 0, 0); // Redraw borders
    mvwprintw(stats, 1, 1, "Name: %s", name.c_str());
    mvwprintw(stats, 2, 1, "Atk: %d", atk);
    mvwprintw(stats, 3, 1, "Def: %d", def);
    mvwprintw(stats, 4, 1, "Regen: %d", regen);
    mvwprintw(stats, 5, 1, "Health: %d", health);
    mvwprintw(stats, 6, 1, "Gold: %d", gold);
    mvwprintw(stats, 8, 1, "Desc: %s", description.c_str());
    wrefresh(stats); // Refresh the window
}
// Function to display text with proper wrapping and scrolling
void displayText(WINDOW* window, const std::string& text) {
    wclear(window); // Clear the window first
    int max_height, max_width;
    getmaxyx(window, max_height, max_width);

    // Break text into lines that fit within the window width
    std::istringstream stream(text);
    std::string word;
    std::string line;
    int y = 0;

    while (getline(stream, word, '\n')) {  // Split by newline
        std::istringstream wordStream(word);
        while (wordStream >> word) {  // Split by space
            if (line.length() + word.length() + 1 > max_width) {
                // Print the current line and reset
                mvwprintw(window, y++, 0, line.c_str());
                line.clear();

                // If the window is full, scroll
                if (y >= max_height) {
                    wscrl(window, 1); // Scroll up one line
                    y = max_height - 1; // Keep overwriting the last line
                }
            }
            line += word + " ";
        }

        // Print the current line and reset
        if (!line.empty()) {
            mvwprintw(window, y++, 0, line.c_str());
            line.clear();
        }

        // If the window is full, scroll
        if (y >= max_height) {
            wscrl(window, 1); // Scroll up one line
            y = max_height - 1; // Keep overwriting the last line
        }

        // Add an extra newline between different blocks of text
        if (y < max_height) {
            mvwprintw(window, y++, 0, "\n");
        }
    }

    wrefresh(window); // Refresh the window to display changes
}

void pushMessage(msgPKG &package, WINDOW* log) {
    // Display sender
    mvwprintw(log, 0, 1, "From: %s", package.sender.c_str());
    wscrl(log, -1 * (1 + (package.sender.size() / max_width)));

    // Display recipient
    mvwprintw(log, 1, 1, "To: %s", package.receiver.c_str());
    wscrl(log, -1 * (1 + (package.receiver.size() / max_width)));

    // Display the actual message
    mvwprintw(log, 2, 1, "Message: %s", package.message.c_str());
    wscrl(log, -1 * (1 + (package.message.size() / max_width)));

    wrefresh(log);
}
void pushChangeRoom(changeroomPKG &package, WINDOW* log){
	getmaxyx(stdscr, max_height, max_width);
	mvwprintw(log,0,1, "changing to room");
	wscrl(log,-1);
	mvwprintw(log,0,1, "%d", package.newRoom);
	wscrl(log,-1);
	wrefresh(log);
	//chngRoom(package, 3);
}
void pushFight(fightPKG &package, WINDOW* log){
	getmaxyx(stdscr, max_height, max_width);
	mvwprintw(log,0,1, "fighting...");
	wscrl(log,-1);
	wrefresh(log);
	//fight(package, 3);
}
void pushPVP(pvpPKG &package, WINDOW* log){
	getmaxyx(stdscr, max_height, max_width);
	mvwprintw(log,0,1, "fighting player...");
	wscrl(log,-1);
	mvwprintw(log,0,1, "%s", package.targetPlayer.c_str());
	wscrl(log,-1);
	wrefresh(log);
	//PvP(package, 3);
}
void pushLoot(lootPKG &package, WINDOW* log){
	getmaxyx(stdscr, max_height, max_width);
	mvwprintw(log,0,1, "looting...");
	wscrl(log,-1);
	mvwprintw(log,0,1, "%s",package.targetPlayer.c_str());
	wscrl(log, -1);
	wrefresh(log);
	//Loot(package, 3);
}
void pushStart(startPKG &package, WINDOW* log){
	getmaxyx(stdscr, max_height, max_width);
	mvwprintw(log,0,1, "starting....");
	wscrl(log,-1);
	wrefresh(log);
	//Start(package,3);
}
void pushError(errorPKG &package, WINDOW* log) {
    int max_height, max_width;
    getmaxyx(log, max_height, max_width);
    
    // Truncate the error message if it's too long
    std::string truncatedMsg = package.errorMsg;
    if (truncatedMsg.size() > (size_t)(max_width * (max_height - 2))) {
        truncatedMsg = truncatedMsg.substr(0, max_width * (max_height - 3)) + "...";
    }

    wclear(log);
    box(log, 0, 0);
    mvwprintw(log, 1, 1, "Error Code: %d", package.errorCode);
    mvwprintw(log, 2, 1, "%s", truncatedMsg.c_str());
    wrefresh(log);
}
void pushAccept(acceptPKG &package, WINDOW* log){
	getmaxyx(stdscr, max_height, max_width);
	mvwprintw(log,0,1, "%d",package.acception);
	wscrl(log,-1);
	wrefresh(log);
	Accept(package,3);
}
void pushRoom(roomPKG &package, WINDOW* players) {
    // Clear previous room and related data
    Rplayer.clear();
    monsters.clear();
    connections.clear();
    lootables.clear();
    currentRm = package;

    // Start building the room details with newlines for proper spacing
    std::ostringstream roomDetails;
    roomDetails << "Current Room Number: " << package.roomNum << "\n"
                << "Room Name: " << package.roomName << "\n"
                << "Description: " << package.roomDescription << "\n\n";

    // Display the updated room details
    displayText(players, roomDetails.str());
}
void pushCharacter(characterPKG &package, WINDOW* stats, WINDOW* filler, WINDOW* enemies) {
    if (me.name == package.name && !(package.flags & (1 << 5))) {
        // Update stats window for the current player
        me = package;
        update_stats_window(stats, package.name, package.atk, package.def, package.regen, package.health, package.gold, package.characterDescription);
    } else if (!(package.flags & (1 << 7))) {
        // Handle lootables
        lootables.push_back(package);
    } else if (!(package.flags & (1 << 5))) {
        // Handle other players
        auto it = std::find_if(Rplayer.begin(), Rplayer.end(), [&](const characterPKG& c) { return c.name == package.name; });
        if (it != Rplayer.end()) {
            *it = package; // Update existing player
        } else {
            Rplayer.push_back(package); // Add new player
            update_stats_window(filler, package.name, package.atk, package.def, package.regen, package.health, package.gold, package.characterDescription);
        }
    } else if (package.flags & (1 << 5)) {
        // Handle monsters
        auto it = std::find_if(monsters.begin(), monsters.end(), [&](const characterPKG& c) { return c.name == package.name; });
        if (it != monsters.end()) {
            *it = package; // Update existing monster
        } else {
            monsters.push_back(package); // Add new monster
            update_stats_window(enemies, package.name, package.atk, package.def, package.regen, package.health, package.gold, package.characterDescription);
        }
    }

    // Rebuild and display all character information
    std::ostringstream charDetails;
    charDetails << "Characters in Room:\n";

    // Add other players
    for (const auto& player : Rplayer) {
        charDetails << "Player Name: " << player.name << ", Atk: " << player.atk << ", Regen: " << player.regen
                    << ", Gold: " << player.gold << ", Def: " << player.def << ", Health: " << player.health << endl;
    }

    // Add monsters
    for (const auto& monster : monsters) {
        charDetails << "Monster Name: " << monster.name << ", Atk: " << monster.atk << ", Regen: " << monster.regen
                    << ", Gold: " << monster.gold << ", Def: " << monster.def << ", Health: " << monster.health << endl;
    }

    // Display updated character details in a different window (not the stats window)
    displayText(filler, charDetails.str()); // Or use `enemies`, depending on where you want it
}
void pushGame(gamePKG &package, WINDOW* log){
	game = package;
	getmaxyx(stdscr, max_height,  max_width);
	mvwprintw(log,0,1, "%s",package.description.c_str());
	wscrl(log, -1*(1+(package.description.size()/max_width)));
	wrefresh(log);
}
void pushLeave(leavePKG &package, WINDOW* log){
	mvwprintw(log,0,1, "Leaving server.");
	getmaxyx(stdscr, max_height, max_width);
	wrefresh(log);
	//exit(package,3);
}
void pushConnection(connectionPKG &package, WINDOW* players) {
    // Add the new connection
    connections.push_back(package);

    // Combine room, connections, and characters into a single display with proper spacing
    std::ostringstream fullDetails;

    // Room details
    fullDetails << "Room Number: " << currentRm.roomNum << ", Room Name: " << currentRm.roomName
                    << ", Description: " << currentRm.roomDescription << "\n";

	// Character details
    fullDetails << "Characters in Room:\n";
    for (const auto& player : Rplayer) {
        fullDetails << "Player Name: " << player.name << ", Atk: " << player.atk << ", Regen: " << player.regen
                    << ", Gold: " << player.gold << ", Def: " << player.def << ", Health: " << player.health << endl;
    }
    for (const auto& monster : monsters) {
        fullDetails << "Monster Name: " << monster.name << ", Atk: " << monster.atk << ", Regen: " << monster.regen
                    << ", Gold: " << monster.gold << ", Def: " << monster.def << ", Health: " << monster.health << endl;
    }

    // Connection details
    fullDetails << "Room Connections:\n";
    for (const auto& conn : connections) {
        fullDetails << "Room Number: " << conn.roomNum << ", Room Name: " << conn.roomName << ", ";
    }
    // fullDetails << "\n";

    // Display combined details
    displayText(players, fullDetails.str());
}