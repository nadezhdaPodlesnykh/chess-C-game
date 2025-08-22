#ifndef BOARDTESTS_H
#define BOARDTESTS_H

#include "Board.h"

class BoardTests{
public:
    static inline void setUprandomBoard(Board& board, int nPieces){
        for(int i = 0; i < nPieces; i++){
            PieceKind kind = (PieceKind)(rand()%(int)(PieceKind::Empty));
            PlayerColour color = (PlayerColour)(rand()%2);
            board._board[rand() % 8][rand() % 8] = Piece (kind, color);
        }
}

    static inline bool testMovesPossibility(Board& board, Int2 cell){
        auto possibleMoves = board.getPossibleMoves(cell);
        std::vector<bool> isMovePossible;
        isMovePossible.resize(64);
        std::fill(isMovePossible.begin(), isMovePossible.end(), false);
        for(Int2 coord : possibleMoves){
            isMovePossible[coord.y*8 + coord.x] = true;
        }
        for(int x = 0; x < 8; ++x){
            for(int y = 0; y < 8; ++y){
                int i = y*8 + x;
                if(isMovePossible[i] != board.isMovePossible(cell, Int2(x, y))){
                    return false;
                }
            }
        }
        return true;
    }

};



#endif // BOARDTESTS_H
