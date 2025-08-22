#include "BoardWidget.h"
#include "mainwindow.cpp"
#include <QDebug>
#include "Board.h"

const QString BoardWidget::COLOR_BLACK_CELL = "#A66918";
const QString BoardWidget::COLOR_WHITE_CELL = "#D7C479";
const QString BoardWidget::COLOR_BLACK_PIECE = "black";
const QString BoardWidget::COLOR_WHITE_PIECE = "white";
const QString BoardWidget::COLOR_GREEN_CELL = "green";


BoardWidget::BoardWidget(QWidget *parent){};

void BoardWidget::setBoard(QPushButton *buttons[8][8], QGridLayout *layout, const Board& board) {


    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            auto& button = buttons[row][col];
            auto piece = board.getCell(row, col);
            auto piece_ = piece;
            piece_.colour = PlayerColour::Black;
            QString pieceText = pieceToUnicode(piece_);
            button->setText(pieceText);
            if(piece.kind != PieceKind::Empty){
                auto palette = button->palette();
                palette.setColor(QPalette::ButtonText, piece.colour == PlayerColour::White ? QColor(COLOR_WHITE_PIECE) : QColor(COLOR_BLACK_PIECE));
                button->setPalette(palette);
            }
        }
    }
}

void BoardWidget::setupBoard(QPushButton *buttons[8][8], QGridLayout *layout) {
    QFont pieceFont;
    pieceFont.setFamily("Arial");
    pieceFont.setPointSize(34);


    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            auto button = buttons[row][col];
            if ((row + col) % 2 == 0) {
                button->setStyleSheet("background-color: " + COLOR_WHITE_CELL);
            } else {
                button->setStyleSheet("background-color: " + COLOR_BLACK_CELL);
            }
            layout->addWidget(buttons[row][col], row, col);
            button->setFont(pieceFont);
        }
    }
}

void BoardWidget::show_possible_moves(QPushButton *buttons[8][8], QGridLayout *layout, const std::vector<Int2> &possibleMoves) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            QString color = ((i + j) % 2 == 0) ? COLOR_WHITE_CELL : COLOR_BLACK_CELL;
            buttons[i][j]->setStyleSheet("background-color: " + color + ";");
        }
    }
    for (const auto& coords : possibleMoves) {
        if (coords.y >= 0 && coords.y < 8 && coords.x >= 0 && coords.x < 8) {
            buttons[coords.y][coords.x]->setStyleSheet("background-color: " + COLOR_GREEN_CELL);
        }
    }
}

QString BoardWidget::pieceToUnicode(const Piece &piece){
        if(piece.kind == PieceKind::Empty){
            return "";
        }
        if(piece.colour == PlayerColour::White){
            switch(piece.kind){
                case PieceKind::Pawn: return QString::fromUtf8("♙");
                case PieceKind::Bishop: return QString::fromUtf8("♗");
                case PieceKind::King: return QString::fromUtf8("♔");
                case PieceKind::Knight: return QString::fromUtf8("♘");
                case PieceKind::Queen: return QString::fromUtf8("♕");
                case PieceKind::Rook: return QString::fromUtf8("♖");
                default:return "";
            }

        } else {
            switch(piece.kind){
                case PieceKind::Pawn: return QString::fromUtf8("♟");
                case PieceKind::Bishop: return QString::fromUtf8("♝");
                case PieceKind::King: return QString::fromUtf8("♚");
                case PieceKind::Knight: return QString::fromUtf8("♞");
                case PieceKind::Queen: return QString::fromUtf8("♛");
                case PieceKind::Rook: return QString::fromUtf8("♜");
                default:return "";
            }
        }
    }
