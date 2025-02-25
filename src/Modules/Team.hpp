#ifndef TEAM_HPP
#define TEAM_HPP

#include <Arduino.h>

// Optional: Define a team ID counter
uint16_t teamIDs = 0;
int newTeamID()
{
    return ++teamIDs;
}
void resetTeamIDcounter() {
    teamIDs = 0;
}

/**
 * @brief A class representing a team in a game
 */
class Team {
public:
    uint16_t ID;
    String name;

    uint8_t color[3] = {0, 0, 0};

    int score = 0;

    /**
     * @brief Construct a new Team object
     * @param teamID The ID of the team
     * @param teamName The name of the team
     */
    Team(uint8_t teamID, String teamName = "null") : ID(teamID), name(teamName) {
        if (name == "null")
        {
            name = "Player";
            name += ID;
        }

        resetScore();
    }

    /**
     * @brief Get the name of the team
     * @return The name of the team
     */
    String getName() {
        return name;
    }

    /**
     * @brief Set the name of the team
     * @param teamName The name of the team
     */

    void setName(String teamName) {
        name = teamName;
    }

    /**
     * @brief Get the color of the team
     * @return The color of the team
     */
    void setColor(uint8_t r, uint8_t g, uint8_t b) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    /**
     * @brief Set the color of the team
     * @param r A reference to the red component
     * @param g A reference to the green component
     * @param b A reference to the blue component
     */
    void getColor(uint8_t &r, uint8_t &g, uint8_t &b) {
        r = color[0];
        g = color[1];
        b = color[2];
    }

    /**
     * @brief Get the score of the team
     * @return The score of the team
     */
    int getScore() {
        return score;
    }
    
    /**
     * @brief Set the score of the team
     * @param points The score of the team
     */
    void setScore(int points) {
        score = points;
    }

    /**
     * @brief Add points to the team's score
     * @param points The points to add
     */
    void addScore(int points) {
        score += points;
    }

    /**
     * @brief Subtract points from the team's score
     * @param points The points to subtract
     */
    void subtructScore(int points) {
        score -= points;
    }

    /**
     * @brief Multiply the team's score by a factor
     * @param factor The factor to multiply by
     */
    void multiplyScore(float factor) {
        score *= factor;
    }

    /**
     * @brief Reset the team's score to 0
     */
    void resetScore() {
        score = 0;
    }
};

#endif // TEAM_HPP