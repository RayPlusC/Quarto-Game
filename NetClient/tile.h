#ifndef TILE_H
#define TILE_H

class Tile
{
    public:
        //Tile();
        bool tall;
        bool shape;
        bool color;
        bool hole;
        bool isOnBoard;
        unsigned number:4;
        int position;
        Tile (bool t, bool s, bool c, bool h, int n){
        /*
        Constructor for single tile
        args:
            values for basic properties:
                t - if is tile tall/short
                s - shape cilindrical/cubical
                c - color black/white
                h - has/hasnt hole from top
            n - coresponding number as well as binary value o 4 bits where those bits corespond basic properties
                tile number 1 in binary 0001 -> FFFT -> short/cilinder/black/has hole
        */
            tall = t;
            shape = s;
            color = c;
            hole = h;
            isOnBoard = false;
            number = n;
            position = -1;
        };
        
        ~Tile(){
        };

        void print(Tile tile){            
            //std::cout << tile.tall << tile.shape << tile.color << tile.hole << " - " << tile.position << std::endl;
            //return tile.color;  
        };

    protected:

    private:
        
};

#endif // TILE_H