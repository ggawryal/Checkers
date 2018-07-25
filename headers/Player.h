#ifndef PLAYER_H
#define PLAYER_H


class Player{
    bool color;
    public:
        Player();
        virtual ~Player();
        virtual void processTurn();
        bool playsWhite(){
            return color;
        }
        bool setColor(bool white){
            color = white;
        }

};

#endif // PLAYER_H
