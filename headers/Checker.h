#ifndef CHECKER_H
#define CHECKER_H

enum Checker {empty = 0, white_pawn = 1, white_queen = 2, black_pawn = 3, black_queen = 4};

inline bool isWhite(const Checker& checker){
    if(checker == white_pawn || checker == white_queen)
        return true;
    return false;
}
inline  bool isBlack(const Checker& checker){
    if(checker == black_pawn || checker == black_queen)
        return true;
    return false;
}
inline  bool isMyChecker(const bool amIwhite, const Checker& checker){
    if(amIwhite)
        return isWhite(checker);
    else
        return isBlack(checker);
}
inline  bool isEnemyChecker(const bool amIwhite, const Checker& checker){
    if(amIwhite)
        return isBlack(checker);
    else
        return isWhite(checker);
}


#endif // CHECKER_H
