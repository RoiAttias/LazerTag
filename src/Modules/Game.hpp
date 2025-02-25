#ifndef GAME_HPP
#define GAME_HPP

#include "Utilities/HyperList.hpp"

#include "Player.hpp"
#include "Team.hpp"

enum GameEvent {
    GAME_START,
    GAME_END,
    GAME_PAUSE,
    GAME_RESUME,
    GAME_PLAYER_HIT,
    GAME_PLAYER_HEAL,
    GAME_PLAYER_KILL,
    GAME_TEAM_SCORE,
    GAME_TEAM_WIN,
    GAME_TEAM_LOSE,
    GAME_TEAM_TIE,
    GAME_GAMEOVER,
    GameEvent_MAX,
};

struct GameEventData {
    GameEvent event;
    uint8_t playerID1 = 0;
    uint8_t playerID2 = 0;
};

enum GameStatus {
    GAME_WAITING,
    GAME_STARTING,
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_ENDING,
    GAME_ENDED,
};



class GameMode {
protected:

public:
    GameMode(Game *game) {}

    virtual bool registerEvent(GameEventData eventData) = 0;

    virtual void loop() = 0;
    
    
};

class Game {
public:
    HyperList<Player> players;
    HyperList<Team> teams;
    GameMode *gamemode;
    GameStatus status;

    Game() : players(), teams(), gamemode(nullptr), status(GAME_WAITING) {}

    void loop() {
        
    }

    void addPlayer(Player player) {
        players.addend(player);
    }

    void addTeam(Team team) {
        teams.addend(team);
    }

    void reset() {
        players.clear();
        teams.clear();
    }
};

#endif // GAME_HPP