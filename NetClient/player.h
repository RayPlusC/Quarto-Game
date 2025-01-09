#include "game.h"

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

class Player
{
    public:
        //Player();
        bool isOnTurn, won, isBot;

        std::string playersName = "";
        int playerNo=0;
        int tile;
        int depthLimit = 2;
        class Game;

    public:
        Player(int number, std::string name, bool bot){
        /*
        Constructor for Player
        args:
        */
            playersName = name;
            playerNo = number;
            isOnTurn, won = false;
            isBot = bot;
        };
        
        ~Player(){
        };

        int chooseTile(Board& board, std::vector <Tile>& tiles, Player& playerOnTurn, Game& game){
            int score = 0;
            int bestScore = 0;
            // for each remaining tile
            for(int i = 0; i < 16; i++){
                // if tile is available
                if(tiles[i].isOnBoard == -1){
                    // for all empty fields
                    for(int n = 0; n<16; n++){
                        board.putOnPosition(tiles[i], n);
                        // calculate score of the outcome
                        score = minimaxPick(board, tiles, 0, true, playerOnTurn, game);
                        board.removeTile(n, tiles);
                    }
                    
                }
                if (score > bestScore){
                    bestScore = score;
                    tile = i;
                }
            }
            return tile;
        }   
/*
        int placeTile(Board board, std::vector <Tile>& tiles, int n){
            int score = 0;
            int bestScore = 0;
            // for available fields on board
            for(int i = 0; i < 16; i++){
                //board.putOnPosition(tiles[n], i);
                score = minimaxPlace(board, tiles[n]);
                if(score < bestScore){
                    bestScore = score;
                }
            }
            return position;
        }
*/
        int minimaxPick(Board& board, std::vector <Tile>& tiles, int depth, bool isMaximizing, Player& playerOnTurn, Game& thisGame){
            int score = 0;
            std::cout << playerOnTurn.playersName << std::endl;
            if(thisGame.playing(board, tiles, playerOnTurn)){
                if(depth < depthLimit){
                    if(isMaximizing){
                        int bestScore = -100;
                        for(int i = 0; i < 16; i++){
                            if (!tiles[i].isOnBoard){
                                for(int n = 0; n < 16; n++){
                                    if (board.emptyPositions[n]){
                                        board.putOnPosition(tiles[i], n);
                                        score = minimaxPick(board, tiles,  depth+1, false, playerOnTurn, thisGame);
                                        board.removeTile(n, tiles);
                                        if(score > bestScore){
                                            bestScore = score;
                                        }
                                    }
                                }
                                
                            }
                        }
                        return bestScore;
                    }else{
                        int bestScore = 100;
                        for(int i = 0; i < 16; i++){
                            if (!tiles[i].isOnBoard){
                                for(int n = 0; n < 16; n++){
                                    if (board.emptyPositions[n]){
                                        board.putOnPosition(tiles[i], n);
                                        score = minimaxPick(board, tiles,  depth+1, true, playerOnTurn, thisGame);
                                        board.removeTile(n, tiles);
                                        if(score < bestScore){
                                            bestScore = score;
                                        }
                                    }
                                    
                                }
                                
                            }
                        }
                        return bestScore;
                    }
                }
            }else{
                return score;
            }

            return score;
        }
        
        int minimaxPlace(Board& board, Tile& tile){
            
            
            return 0;
        }

    protected:

    private:
        
};

#endif // PLAYER_H
