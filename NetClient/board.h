#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include "tile.h"
#include <iostream>
#include <iomanip>

class Board
{
    public:
        bool emptyPositions[16];
        Board(){
            /*
            Create empty playing board
            */
            for(int i = 0; i < 16; i++){
                emptyPositions[i] = 1;
            }
        };

        //displays tiles on board         
        void displayBoard(std::vector <Tile>& tiles){
            for(auto tile: tiles){
                if(tile.isOnBoard){
                    tile.print(tile);
                }
            }
        }

        //displays tiles still in common stock
        void displayStock(std::vector <Tile>& tiles){
            for(auto tile: tiles){
                if(!tile.isOnBoard){
                    tile.print(tile);
                }
            }
        }

        //show all tiles
        void displayTiles(std::vector <Tile>& tiles) {
            for (auto tile: tiles) {
                std::cout << std::setfill('0') << std::setw(2) << tile.number << " - " << tile.tall << tile.shape << tile.color << tile.hole << " on position - " << tile.position << std::endl;
            }
        }
        
        //puts tile from stock in specific position on playing board
        bool putOnPosition(Tile &tile, int position){
            if(emptyPositions[position]){
                tile.position = position;
                tile.isOnBoard = true;
                emptyPositions[position] = 0;
                return true;
            }else{
                return false;
            }
        }

        //removes tile from specific position on playing board
        bool removeTile(int position, std::vector <Tile>& tiles){
            if(emptyPositions[position]){ 
                return false;
            }else{
                for(int i = 0; i < 16; i++){
                    if(tiles[i].position == position){
                        tiles[i].isOnBoard = false;
                        tiles[i].position = -1;
                    }
                }
                emptyPositions[position] = true;
                return true;
            }
        }

        //compares properties of 4 tiles given
        bool compareTiles(Board board, std::vector <Tile> tiles) {
            bool same = false;
            int count = 0;
            bool win[4] = {false,false,false,false};
            if(tiles[0].color == tiles[1].color && tiles[1].color == tiles[2].color && tiles[2].color == tiles[3].color)
            {
                count++;
                win[0] = true;
                same = true;
            }
            if(tiles[0].tall == tiles[1].tall && tiles[1].tall == tiles[2].tall && tiles[2].tall == tiles[3].tall)
            {
                count++;
                win[1] = true;
                same = true;
            }
            if(tiles[0].hole == tiles[1].hole && tiles[1].hole == tiles[2].hole && tiles[2].hole == tiles[3].hole)
            {
                count++;
                win[1] = true;
                same = true;
            }
            if(tiles[0].shape == tiles[1].shape && tiles[1].shape == tiles[2].shape && tiles[2].shape == tiles[3].shape)
            {
                count++;
                win[3] = true;
                same = true;
            }
            if(count>0){
                std::cout << "Found " << count << " similiarities." << std::endl;
            }
            for(int i = 0; i < 4; i++){
                switch(i){
                    case 0:
                        if(win[i]){
                            std::cout << "Same Color" << std::endl;
                        }
                        break;
                    case 1:
                        if(win[i]){
                            std::cout << "Same height" << std::endl;
                        }
                        break;
                    case 2:
                        if(win[i]){
                            std::cout << "Same holes" << std::endl;
                        }
                        break;
                    case 3:
                        if(win[i]){
                            std::cout << "Same shape" << std::endl;
                        }
                        break;
                    default:
                        break;
                }
            }
            return same;
        }
    protected:

    private:
        
};

#endif // BOARD_H
