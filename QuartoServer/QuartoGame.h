#pragma once
#include <iostream>

class QuartoGame
{
public:
    QuartoGame() {
    };

    QuartoGame(uint32_t id) {
        gameID = id;
        start = false;
        isOver = false;
        CPUoponent = false;
        //initTiles();
    };

    ~QuartoGame() {
    };

    bool play() {
        std::cout << "game ID is " << gameID << std::endl;
        return true;
    };

    uint32_t gameID;
    bool isOver;
    bool CPUoponent;
    bool start;
    uint32_t player1;
    uint32_t player2;
    int activeTile;
    int tiles[16] = {0};
    void initTiles() {
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                int tile = y * 4 + x;
                tiles[tile] = tile;
            }
        }
    }
    
};

struct sPlayerDescription
{
    uint32_t nUniqueID = 0;
    uint32_t nAvatarID = 0;
};
