#ifndef GAME_HPP
#define GAME_HPP

#include "Utilities/HyperList.hpp"

#include "Player.hpp"
#include "Team.hpp"

class GameMode {
public:
    virtual void start() = 0;
    virtual void end() = 0;
};

class Game {
public:
    HyperList<Player> players;
    HyperList<Team> teams;

    Game() {
        players = HyperList<Player>();
        teams = HyperList<Team>();
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