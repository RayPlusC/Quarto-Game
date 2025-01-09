#include "player.h"
#include "game.h"
#include "board.h"



Player::Player(int number, std::string name, bool bot)
{

}

Player::~Player()
{
    //dtor
}

int chooseTile(Board& board, std::vector <Tile>& tiles, Player& playerOnTurn, Game& game){};

int minimaxPick(Board& board, std::vector <Tile>& tiles, int depth, bool isMaximizing, Player& playerOnTurn, Game& game){};