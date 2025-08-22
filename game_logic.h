#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include "Board.h"
#include "CastlingState.h"

class GameLogic{
public:
    PlayerColour _currentPlayer;
    std::vector<Int2> _possibleMoves;
    Int2 _selectedPieceCoords = Int2{-1, -1};
    std::vector<PieceKind> _eatenPiecesWhite;
    std::vector<PieceKind> _eatenPiecesBlack;
    CastlingState _castlingState;


    GameLogic() : _currentPlayer(PlayerColour::White) {}

    PlayerColour switchPlayer() {
        _currentPlayer = (_currentPlayer == PlayerColour::White) ? PlayerColour::Black : PlayerColour::White;
        return _currentPlayer;
    }
    PlayerColour getCurrentPlayer() const {
        return _currentPlayer;
    }

    void click(int row, int col, Board &board){


        const Piece& piece = board.getCell(row, col);

        if(piece.colour == _currentPlayer && piece.kind != PieceKind::Empty){
            _selectedPieceCoords = Int2(col, row);
            _possibleMoves = board.getPossibleMoves(_selectedPieceCoords);
            return;
        }

        if(_selectedPieceCoords.x != -1) {
            for (size_t i = 0; i < _possibleMoves.size(); i++) {
                if (col == _possibleMoves[i].x && row == _possibleMoves[i].y) {
                    Piece eatenPiece = board.getCell(row, col);
                    if (eatenPiece.kind != PieceKind::Empty) {
                        if (eatenPiece.colour == PlayerColour::Black) {
                            _eatenPiecesBlack.push_back(eatenPiece.kind);
                        } else {
                            _eatenPiecesWhite.push_back(eatenPiece.kind);
                        }
                    }

                    /*if (piece.kind == PieceKind::King && abs(col - _selectedPieceCoords.x) == 2) {
                        _castlingState.performCastling(board, _selectedPieceCoords, Int2(col, row));
                    } else {

                    }*/

                    board.setCell(_possibleMoves[i], board.getCell(_selectedPieceCoords));
                    board.setCell(_selectedPieceCoords, Piece{PieceKind::Empty, PlayerColour::Black});
                    switchPlayer();
                    break;
                }
            }
        }

        _selectedPieceCoords = Int2{-1, -1};
        _possibleMoves.clear();
    }


};

#endif // GAME_LOGIC_H
