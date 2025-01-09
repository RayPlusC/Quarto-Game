#ifndef GAME_H
#define GAME_H

#include<vector>
#include"board.h"
#include"player.h"

class Game
{    
    public:
        Game() {
        };

        ~Game() {
        };
        
        //starts actual game
        void start(std::vector <Player>& players, Board& board, std::vector <Tile>& tiles, Game& game){
            int p = 0;
            int tile_N = 0; //which tile,
            int tile_X = 0; //what position on board
            players[p].isOnTurn = true;
            Player thisPlayer = players[p];
            //Game game;
            //process of putting tiles on the board until anybody completes the line of 4 tiles with at least one same property
            do {
                //system("clear");
                board.displayBoard(tiles);
                std::cout << std::endl;
                board.displayStock(tiles);

                //chosing tile for enemy to put on board
                do{
                    std::cout << "Player" << thisPlayer.playerNo << " - name: " << thisPlayer.playersName << " is picking tile." << std::endl;
                    if(thisPlayer.isBot){
                        std::cin >> tile_X;
                        tile_N = thisPlayer.chooseTile(board, tiles, thisPlayer, game);
                        std::cout << tile_N << std::endl;
                    }else{
                        std::cin >> tile_N;
                    }
                }while(tiles[tile_N].isOnBoard);
                
                tiles[tile_N].print(tiles[tile_N]);
                
                //switch players
                //players[((p + 1) % sizeof(players) ) ].isOnTurn = true;
                if (thisPlayer.isOnTurn){
                    thisPlayer.isOnTurn = false;
                    p++;
                    p = p % players.size(); 
                    //std::cout << p << ((p + 1)%sizeof(players));
                    players[p].isOnTurn = true;
                    thisPlayer = players[p];
                }
                //else players[p].isOnTurn = true; 
                
                //laying a tile on empty position on board
                do {    
                    std::cout << "Player" << thisPlayer.playerNo << " - name: " << thisPlayer.playersName << " is laying tile." << std::endl;
                    if(thisPlayer.isBot){
                        //tile_X = thisPlayer.chooseTile(game, board, tiles, thisPlayer);
                        std::cin >> tile_X;
                        std::cout << tile_X << std::endl;
                    }else{
                        std::cin >> tile_X;
                    }
                    //putTile(tileX, tileY, tilesInGame[lastTile-1]);
                } while (!board.putOnPosition(tiles[tile_N], tile_X));

            } while (playing(board, tiles, thisPlayer));
            
        }

        public:
        //function checks if is over
        bool playing(Board& board, std::vector <Tile>& tiles, Player& player){
            std::vector <Tile> line;
            bool yes = true;

            //cycle all the possible line segments of four spaces including diagonals
            for(int i = 1; i < 11; i++){
                switch (i){
                case 1:
                    std::cout << "case 1 " << std::endl;
                    if(board.emptyPositions[0] || board.emptyPositions[1] || board.emptyPositions[2] || board.emptyPositions[3]){
                        std::cout << "Incomplete line" << std::endl;
                    }else{                        
                        std::cout << "Full line" << std::endl;
                        for(auto tile: tiles){
                            if(tile.position == 0 || tile.position == 1 || tile.position == 2 || tile.position == 3)
                                line.push_back(tile);
                        }
                        if(board.compareTiles(board, line)){
                          yes = false;
                          std::cout << "Winning combination for Player: " << player.playersName << " on line " << i << std::endl;
                          player.won = true;
                        }
                    }
                    break;                 
                case 2:
                    std::cout << "case 2 " << std::endl;
                    if(board.emptyPositions[4] || board.emptyPositions[5] || board.emptyPositions[6] || board.emptyPositions[7]){
                        std::cout << "Incomplete line" << std::endl;
                    }else{                        
                        std::cout << "Full line" << std::endl;
                        for(auto tile: tiles){
                            if(tile.position == 4 || tile.position == 5 || tile.position == 6 || tile.position == 7)
                                line.push_back(tile);
                        }
                        if(board.compareTiles(board, line)){
                          yes = false;
                          std::cout << "Winning combination on line" << std::endl;
                        }
                    }
                    break;
                case 3:
                    std::cout << "case 3 " << std::endl;
                    if(board.emptyPositions[8] || board.emptyPositions[9] || board.emptyPositions[10] || board.emptyPositions[11]){
                        std::cout << "Incomplete line" << std::endl;
                    }else{                        
                        std::cout << "Full line" << std::endl;
                        for(auto tile: tiles){
                            if(tile.position == 8 || tile.position == 9 || tile.position == 10 || tile.position == 11)
                                line.push_back(tile);
                        }
                        if(board.compareTiles(board, line)){
                          yes = false;
                          std::cout << "Winning combination on line" << std::endl;
                        }
                    }
                    break;
                case 4:
                    std::cout << "case 4 " << std::endl;
                    if(board.emptyPositions[12] || board.emptyPositions[13] || board.emptyPositions[14] || board.emptyPositions[15]){
                        std::cout << "Incomplete line" << std::endl;
                    }else{                        
                        std::cout << "Full line" << std::endl;
                        for(auto tile: tiles){
                            if(tile.position == 12 || tile.position == 13 || tile.position == 14 || tile.position == 15)
                                line.push_back(tile);
                        }
                        if(board.compareTiles(board, line)){
                          yes = false;
                          std::cout << "Winning combination on line" << std::endl;
                        }
                    }
                    break;
                case 5:
                    std::cout << "case 5 " << std::endl;
                    if(board.emptyPositions[0] || board.emptyPositions[4] || board.emptyPositions[8] || board.emptyPositions[12]){
                        std::cout << "Incomplete line" << std::endl;
                    }else{                        
                        std::cout << "Full line" << std::endl;
                        for(auto tile: tiles){
                            if(tile.position == 0 || tile.position == 4 || tile.position == 8 || tile.position == 12)
                                line.push_back(tile);
                        }
                        if(board.compareTiles(board, line)){
                          yes = false;
                          std::cout << "Winning combination on line" << std::endl;
                        }
                    }
                    break;
                case 6:
                    std::cout << "case 6 " << std::endl;
                    if(board.emptyPositions[1] || board.emptyPositions[5] || board.emptyPositions[9] || board.emptyPositions[13]){
                        std::cout << "Incomplete line" << std::endl;
                    }else{                        
                        std::cout << "Full line" << std::endl;
                        for(auto tile: tiles){
                            if(tile.position == 1 || tile.position == 5 || tile.position == 9 || tile.position == 13)
                                line.push_back(tile);
                        }
                        if(board.compareTiles(board, line)){
                          yes = false;
                          std::cout << "Winning combination on line" << std::endl;
                        }
                    }
                    break;
                case 7:
                    std::cout << "case 7 " << std::endl;
                    if(board.emptyPositions[2] || board.emptyPositions[6] || board.emptyPositions[10] || board.emptyPositions[14]){
                        std::cout << "Incomplete line" << std::endl;
                    }else{                        
                        std::cout << "Full line" << std::endl;
                        for(auto tile: tiles){
                            if(tile.position == 2 || tile.position == 6 || tile.position == 10 || tile.position == 14)
                                line.push_back(tile);
                        }
                        if(board.compareTiles(board, line)){
                          yes = false;
                          std::cout << "Winning combination on line" << std::endl;
                        }
                    }
                    break;
                case 8:
                    std::cout << "case 8 " << std::endl;
                    if(board.emptyPositions[3] || board.emptyPositions[7] || board.emptyPositions[11] || board.emptyPositions[15]){
                        std::cout << "Incomplete line" << std::endl;
                    }else{                        
                        std::cout << "Full line" << std::endl;
                        for(auto tile: tiles){
                            if(tile.position == 3 || tile.position == 7 || tile.position == 11 || tile.position == 15)
                                line.push_back(tile);
                        }
                        if(board.compareTiles(board, line)){
                          yes = false;
                          std::cout << "Winning combination on line" << std::endl;
                        }
                    }
                    break;
                case 9:
                    std::cout << "case 9 " << std::endl;
                    if(board.emptyPositions[0] || board.emptyPositions[5] || board.emptyPositions[10] || board.emptyPositions[15]){
                        std::cout << "Incomplete line" << std::endl;
                    }else{                        
                        std::cout << "Full line" << std::endl;
                        for(auto tile: tiles){
                            if(tile.position == 0 || tile.position == 5 || tile.position == 10 || tile.position == 15)
                                line.push_back(tile);
                        }
                        if(board.compareTiles(board, line)){
                          yes = false;
                          std::cout << "Winning combination on line" << std::endl;
                        }
                    }
                    break;
                case 10:
                    std::cout << "case 10 " << std::endl;
                    if(board.emptyPositions[3] || board.emptyPositions[6] || board.emptyPositions[9] || board.emptyPositions[12]){
                        std::cout << "Incomplete line" << std::endl;
                    }else{                        
                        std::cout << "Full line" << std::endl;
                        for(auto tile: tiles){
                            if(tile.position == 12 || tile.position == 13 || tile.position == 14 || tile.position == 15)
                                line.push_back(tile);
                        }
                        if(board.compareTiles(board, line)){
                          yes = false;
                          std::cout << "Winning combination on line" << std::endl;
                        }
                    }
                    break;
                default:
                    break;
                }
            }
            return yes;
        }

        //generates 16 tiles to play the game with
        std::vector <Tile> createTiles() {
            std::vector <Tile> tiles;
            bool a, b, c, d;
            for (int i = 0; i < 16; i++) {
                if (i % 2 == 0) d = false;
                else d = true;

                switch (i) {
                case 0:
                    c = false;
                    break;
                case 1:
                    c = false;
                    break;
                case 2:
                    c = true;
                    break;
                case 3:
                    c = true;
                    break;
                case 4:
                    c = false;
                    break;
                case 5:
                    c = false;
                    break;
                case 6:
                    c = true;
                    break;
                case 7:
                    c = true;
                    break;
                case 8:
                    c = false;
                    break;
                case 9:
                    c = false;
                    break;
                case 10:
                    c = true;
                    break;
                case 11:
                    c = true;
                    break;
                case 12:
                    c = false;
                    break;
                case 13:
                    c = false;
                    break;
                case 14:
                    c = true;
                    break;
                case 15:
                    c = true;
                    break;
                }

                if (i > 3 && i <= 7) b = true;
                else b = false;
                if (i > 11) b = true;

                if (i <= 7) a = false;
                else a = true;
                Tile tile(a, b, c, d, i);
                tiles.push_back(tile);
            }
            return tiles;
        }
        
    protected:

    private:
    
};

#endif // GAME_H