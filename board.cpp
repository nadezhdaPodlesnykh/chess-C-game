#include "Board.h"

std::vector<Int2> Board::directionsBishop{
    Int2(1, 1),
    Int2(-1, 1),
    Int2(1, -1),
    Int2(-1, -1)
};

std::vector<Int2> Board::directionsRook{
    Int2(1, 0),
    Int2(-1, 0),
    Int2(0, -1),
    Int2(0, 1)
};

std::vector<Int2> Board::movesKnight{
    Int2(1, 2),
    Int2(2, 1),
    Int2(2, -1),
    Int2(1, -2),
    Int2(-1, -2),
    Int2(-2, -1),
    Int2(-2, 1),
    Int2(-1, 2)
};

void Board::setupInitialState(){
    clear();

    _board[0][0] = Piece(PieceKind::Rook, PlayerColour::Black);
    _board[0][7] = Piece(PieceKind::Rook, PlayerColour::Black);

    _board[0][1] = Piece(PieceKind::Knight, PlayerColour::Black);
    _board[0][6] = Piece(PieceKind::Knight, PlayerColour::Black);

    _board[0][2] = Piece(PieceKind::Bishop, PlayerColour::Black);
    _board[0][5] = Piece(PieceKind::Bishop, PlayerColour::Black);

    _board[0][3] = Piece(PieceKind::Queen,PlayerColour::Black);

    _board[0][4] = Piece(PieceKind::King, PlayerColour::Black);

    _board[0][0] = Piece(PieceKind::Rook,PlayerColour::Black);

    for(int i = 0; i < 8; i++){
        _board[1][i] = Piece(PieceKind::Pawn, PlayerColour::Black);

    }

    _board[7][0] = Piece(PieceKind::Rook, PlayerColour::White);
    _board[7][7] = Piece(PieceKind::Rook, PlayerColour::White);

    _board[7][1] = Piece(PieceKind::Knight, PlayerColour::White);
    _board[7][6] = Piece(PieceKind::Knight, PlayerColour::White);

    _board[7][2] = Piece(PieceKind::Bishop, PlayerColour::White);
    _board[7][5] = Piece(PieceKind::Bishop, PlayerColour::White);

    _board[7][3] = Piece(PieceKind::Queen,PlayerColour::White);

    _board[7][4] = Piece(PieceKind::King, PlayerColour::White);

    _board[7][0] = Piece(PieceKind::Rook,PlayerColour::White);

    for(int i = 0; i < 8; i++){
        _board[6][i] = Piece(PieceKind::Pawn, PlayerColour::White);

    }
}

const Piece& Board::getCell(int row, int col) const{
    return _board[row][col];
}

const Piece& Board::getCell(const Int2& coords) const{
    return _board[coords.y][coords.x];
}

void Board::setCell(int row, int col, Piece piece){
    _board[row][col] = piece;
}
void Board::setCell(const Int2& coords, Piece piece){
    _board[coords.y][coords.x] = piece;
}

/*bool Board::WasMoveMade(const Int2& from, const Int2& to) const {
    const Piece& fromPiece = getCell(from);

    if (fromPiece.kind == PieceKind::Empty) {
        return true;
    }

    const Piece& toPiece = getCell(to);

    if (toPiece.kind != PieceKind::Empty) {
        return true;
    }

    return false;
}*/

std::vector<Int2> Board::getPossibleMoves(Int2 cell) const{
    const Piece& piece = getCell(cell);

    PlayerColour enemyColour = piece.colour == PlayerColour::White ? PlayerColour::Black : PlayerColour::White;

    switch (piece.kind) {

    case PieceKind::Pawn:   return getPossibleMovesPawn(cell, enemyColour);
    case PieceKind::King:   return getPossibleMovesKing(cell, enemyColour);
    case PieceKind::Queen:  return getPossibleMovesQueen(cell, enemyColour);
    case PieceKind::Knight: return getPossibleMovesKnight(cell, enemyColour);
    case PieceKind::Bishop: return getPossibleMovesBishop(cell, enemyColour);
    case PieceKind::Rook:   return getPossibleMovesRook(cell, enemyColour);

    default:
        return {};
    }
}

bool Board::isMovePossible(Int2 cell, Int2 target) const{
    const Piece& piece = getCell(cell);
    PlayerColour enemyColour = piece.colour == PlayerColour::White ? PlayerColour::Black : PlayerColour::White;

    switch (piece.kind) {

    case PieceKind::Pawn:   return isMovePossiblePawn(cell, target, enemyColour);
    case PieceKind::King:   return isMovePossibleKing(cell, target, enemyColour);
    case PieceKind::Queen:  return isMovePossibleQueen(cell, target, enemyColour);
    case PieceKind::Knight: return isMovePossibleKnight(cell, target, enemyColour);
    case PieceKind::Bishop: return isMovePossibleBishop(cell, target, enemyColour);
    case PieceKind::Rook:   return isMovePossibleRook(cell, target, enemyColour);

    default:
        return {};
    }
}



bool Board::isUnderAttack(const Int2& targetCoords, PlayerColour enemyColour) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Int2 cell(col, row);
            const Piece& piece = getCell(cell);
            if (piece.colour == enemyColour) {
                auto possibleMoves = getPossibleMoves(cell);
                for (const auto& move : possibleMoves) {
                    if (move.x == targetCoords.x && move.y == targetCoords.y) {
                        return true;
                    }
                }
            }
        }
    }
    return false;

}

