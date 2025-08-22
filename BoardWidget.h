#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include "Board.h"

class BoardWidget {
public:

    explicit BoardWidget(QWidget *parent = nullptr);
    void setBoard(QPushButton *buttons[8][8], QGridLayout *layout, const Board& board);
    void setupBoard(QPushButton *buttons[8][8], QGridLayout *layout);
    void show_possible_moves(QPushButton *buttons[8][8], QGridLayout *layout, const std::vector<Int2> &possibleMoves);
    static QString pieceToUnicode(const Piece &piece);


private:
    QWidget *parentWidget;
    static const QString COLOR_BLACK_CELL, COLOR_WHITE_CELL, COLOR_BLACK_PIECE, COLOR_WHITE_PIECE, COLOR_GREEN_CELL;
};

#endif // BOARDWIDGET_H
