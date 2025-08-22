#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <iostream>
#include "functions.h"

enum PieceKind{Pawn, Knight, Bishop, Rook, King, Queen, Empty};
enum PlayerColour{Black, White};

class Int2{
public:
    int x;
    int y;

    Int2(){
        x = 0;
        y = 0;
    }

    Int2(int x_, int y_){
        x = x_;
        y = y_;
    }
};

inline Int2 operator+(const Int2& a, const Int2& b){
    return Int2(a.x + b.x, a.y + b.y);
}

class Piece{
public:
    PieceKind kind;
    PlayerColour colour;

    Piece(){
        kind = PieceKind::Empty;
        colour = PlayerColour::Black;
    };

    Piece (PieceKind kind_, PlayerColour colour_){
        kind = kind_;
        colour = colour_;
    };


};

class Board {
public:
    friend class BoardTests;
    Board(){
        _board.resize(8);
        for (int i = 0; i < _board.size(); i++){
            _board[i].resize(8);
        }
    };

    void clear(){
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                _board[i][j] = Piece();
            }
        }
    }

    void setupInitialState();
    const Piece& getCell(int row, int col) const;
    const Piece& getCell(const Int2& coords) const;
    void setCell(int row, int col, Piece piece);
    void setCell(const Int2& coords, Piece piece);
    //bool WasMoveMade(const Int2& from, const Int2& to) const;
    std::vector<Int2> getPossibleMoves(Int2 cell) const;
    bool isMovePossible(Int2 cell, Int2 target) const;
    bool isUnderAttack(const Int2& targetCoords, PlayerColour enemyColour) const;
    static bool IsCoordsValid(const Int2& coords){
        return coords.y >= 0 && coords.y < 8 && coords.x >= 0 && coords.x < 8;
    }

    bool check(PlayerColour playerColour){
        Int2 kingPosition(-1, -1);

        for (int col = 0; col < 8; ++col) {
            for (int row = 0; row < 8; ++row) {
                Piece piece = _board[row][col];

                if (piece.colour == playerColour && piece.kind == PieceKind::King) {
                    if(kingPosition.x != -1){
                        return false;
                    }
                    kingPosition = Int2(col, row);

                }

            }
        }
        if(kingPosition.x == -1){
            return false;
        }
        PlayerColour enemyColour = (playerColour == PlayerColour::White) ? PlayerColour::Black : PlayerColour::White;
        if (isUnderAttack(kingPosition, enemyColour)) {
            return true;
        }

        return false;
    }

    bool isMate(PlayerColour playerColour) {
        if (!check(playerColour)) {
            return false;
        }

        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                Int2 piecePos(col, row);
                const Piece& piece = getCell(piecePos);

                if (piece.kind == PieceKind::Empty || piece.colour != playerColour) {
                    continue;
                }

                std::vector<Int2> possibleMoves = getPossibleMoves(piecePos);

                for (const Int2& move : possibleMoves) {
                    Board tempBoard = *this;
                    tempBoard.setCell(move, piece);
                    tempBoard.setCell(piecePos, Piece());

                    if (!tempBoard.check(playerColour)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }


    bool Stalemate(PlayerColour playerColour) {
        if (check(playerColour)) {
            return false;
        }

        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                Int2 piecePos(col, row);
                const Piece& piece = getCell(piecePos);

                if (piece.kind == PieceKind::Empty || piece.colour != playerColour) {
                    continue;
                }

                if (!getPossibleMoves(piecePos).empty()) {
                    return false;
                }
            }
        }
        return true;
    }




private:
    std::vector<std::vector<Piece>> _board;// [row][col]
    static std::vector<Int2> directionsBishop, directionsRook, movesKnight;

    std::vector<Int2> getPossibleMovesPawn(Int2 cell, PlayerColour enemyColour) const{
        int dy = enemyColour == PlayerColour::Black ? -1 : 1;
        std::vector<Int2> result;
        Int2 targetCoords = Int2(cell.x, cell.y + dy);

        if(IsCoordsValid(targetCoords) && getCell(targetCoords).kind == PieceKind::Empty){
            result.push_back(targetCoords);

            int startRow = (enemyColour == PlayerColour::Black) ? 6 : 1;
            if (cell.y == startRow) {
                Int2 targetCoords2 = Int2(cell.x, cell.y + 2 * dy);
                if (IsCoordsValid(targetCoords2) && getCell(targetCoords2).kind == PieceKind::Empty) {
                    result.push_back(targetCoords2);
                }
            }

        }
        for(int dx = -1; dx <= 1; dx += 2){
            targetCoords = Int2(cell.x + dx, cell.y + dy);
            if(!IsCoordsValid(targetCoords)){
                continue;
            }
            const Piece& target = getCell(targetCoords);
            if(target.kind != PieceKind::Empty && target.colour == enemyColour ){
                result.push_back(targetCoords);
            }
        }

        return result;
    }

    std::vector<Int2> getPossibleMovesKing(Int2 cell, PlayerColour enemyColour) const {
        std::vector<Int2> result;


        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                Int2 targetCoords = Int2(cell.x + dx, cell.y + dy);

                if (dx == 0 && dy == 0) {
                    continue;
                }

                if (!IsCoordsValid(targetCoords)) {
                    continue;
                }

                const Piece& target = getCell(targetCoords);

                if ((target.kind == PieceKind::Empty || target.colour == enemyColour)) {
                    result.push_back(targetCoords);

                }
            }
        }
        return result;
    }

    std::vector<Int2> getPossibleMovesBR(Int2 cell, PlayerColour enemyColour, std::vector<Int2> &directions) const {
        std::vector<Int2> result;


        for(int iDir = 0; iDir < 4; ++iDir){
            auto target = cell;
            while(true){

                target = target + directions[iDir];
                if(!IsCoordsValid(target)){
                    break;
                }
                const auto& targetPiece = getCell(target);
                if(targetPiece.kind == PieceKind::Empty){
                    result.push_back(target);
                } else if(targetPiece.colour == enemyColour){
                    result.push_back(target);
                    break;
                } else if(targetPiece.colour != enemyColour){
                    break;
                }
            }
        }
        return result;
    }
    std::vector<Int2> getPossibleMovesBishop(Int2 cell, PlayerColour enemyColour) const {
        return getPossibleMovesBR(cell, enemyColour, directionsBishop);
    }
    std::vector<Int2> getPossibleMovesRook(Int2 cell, PlayerColour enemyColour) const {
        return getPossibleMovesBR(cell, enemyColour, directionsRook);
    }
    std::vector<Int2> getPossibleMovesKnight(Int2 cell, PlayerColour enemyColour) const {
        std::vector<Int2> result;


        for(int iDir = 0; iDir < 8; ++iDir){
            auto target = cell + movesKnight[iDir];
            if(!IsCoordsValid(target)){
                continue;
            }
            const auto& targetPiece = getCell(target);
            if(targetPiece.kind == PieceKind::Empty){
                result.push_back(target);
            } else if(targetPiece.colour == enemyColour){
                result.push_back(target);
                continue;
            }
        }
        return result;
    }
    std::vector<Int2> getPossibleMovesQueen(Int2 cell, PlayerColour enemyColour) const {
        std::vector<Int2> bishopMoves = getPossibleMovesBR(cell, enemyColour, directionsBishop);
        std::vector<Int2> rookMoves   = getPossibleMovesBR(cell, enemyColour, directionsRook);
        bishopMoves.insert(bishopMoves.end(), rookMoves.begin(), rookMoves.end());
        return bishopMoves;
    }


    bool isMovePossibleRook(Int2 cell, Int2 target, PlayerColour enemyColour) const {

        if (cell.x == target.x && cell.y == target.y) {
            return false;
        }

        int dx = sign(target.x - cell.x);
        int dy = sign(target.y - cell.y);
        if(std::abs(dx) + std::abs(dy) != 1){
            return false;
        }


        Int2 current = cell + Int2(dx, dy);
        while (current.x != target.x || current.y != target.y) {
            if (_board[current.y][current.x].kind != PieceKind::Empty) {
                return false;
            }
            current = current + Int2(dx, dy);
        }

        const Piece& targetPiece = _board[target.y][target.x];
        if (targetPiece.kind != PieceKind::Empty && targetPiece.colour != enemyColour) {
            return false;
        }
        return true;

    }
    bool isMovePossibleKnight(Int2 cell, Int2 target, PlayerColour enemyColour) const{
        int absDX = std::abs(target.x - cell.x);
        int absDY = std::abs(target.y - cell.y);

        if(!(absDX == 2 && absDY == 1) && !(absDX == 1 && absDY == 2) ){
            return false;
        }

        const Piece& targetPiece = _board[target.y][target.x];
        if (targetPiece.kind != PieceKind::Empty && targetPiece.colour != enemyColour) {
            return false;
        }

        return true;
    }
    bool isMovePossibleBishop(Int2 cell, Int2 target, PlayerColour enemyColour) const{

        if (cell.x == target.x && cell.y == target.y) {
            return false;
        }

        if(std::abs(target.x - cell.x) != std::abs(target.y - cell.y) != 0){
            return false;
        }
        int dy = sign(target.y - cell.y);
        int dx = sign(target.x - cell.x);

        Int2 current = cell + Int2(dx, dy);
        while (current.x != target.x || current.y != target.y) {
            if (_board[current.y][current.x].kind != PieceKind::Empty) {
                return false;
            }
            current = current + Int2(dx, dy);
        }

        const Piece& targetPiece = _board[target.y][target.x];
        if (targetPiece.kind != PieceKind::Empty && targetPiece.colour != enemyColour) {
            return false;
        }
        return true;
    }
    bool isMovePossibleQueen(Int2 cell, Int2 target, PlayerColour enemyColour) const{
        return isMovePossibleBishop(cell, target, enemyColour) || isMovePossibleRook(cell, target, enemyColour);
    }
    bool isMovePossibleKing(Int2 cell, Int2 target, PlayerColour enemyColour) const{
        int absDX = std::abs(target.x - cell.x);
        int absDY = std::abs(target.y - cell.y);


        if(!(absDX == 1 && absDY == 1) && !(absDX == 0 && absDY == 1) && !(absDX == 1 && absDY == 0)){
            return false;
        }

        const Piece& targetPiece = _board[target.y][target.x];
        if (targetPiece.kind != PieceKind::Empty && targetPiece.colour != enemyColour) {
            return false;
        }

    return true;
    }
    bool isMovePossiblePawn(Int2 cell, Int2 target, PlayerColour enemyColour) const{
        auto possibleMoves = getPossibleMovesPawn(cell, enemyColour);

        for(int i = 0; i < possibleMoves.size(); ++i){
            if(possibleMoves[i].x == target.x && possibleMoves[i].y == target.y){
                return true;
            }
        }
        return false;

    }

};


#endif // BOARD_H
