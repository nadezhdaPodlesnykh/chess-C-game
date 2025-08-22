#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "BoardWidget.h"
#include "boardTests.h"
#include "game_logic.h"
#include "Board.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gridLayout(new QGridLayout)
    , boardWidget(new BoardWidget(ui->boardWidget))
{
    ui->setupUi(this);
    createBoard();
    boardWidget->setupBoard(buttons, gridLayout);
    _board.setupInitialState();
    boardWidget->setBoard(buttons, gridLayout, _board);
    ui->boardWidget->setLayout(gridLayout);
}

MainWindow::~MainWindow()
{
    delete boardWidget;
    delete ui;
}

void MainWindow::createBoard() {
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);
    QFont font("Arial", 24);
    for(int i = 0; i < 8; ++i){
        auto coordLabel = new QLabel(this);
        coordLabel->setFont(font);
        coordLabel->setAlignment(Qt::AlignCenter);
        coordLabel->setText(QString((char)('a' + i)));
        gridLayout->addWidget(coordLabel, 8, i);


    }

    for (int i = 8; i > 0; i--) {
        auto coordLabel = new QLabel(this);
        coordLabel->setFont(font);
        coordLabel->setAlignment(Qt::AlignCenter);
        coordLabel->setText(QString::number(i));
        gridLayout->addWidget(coordLabel, 8 - i, 8);
    }

    QFont pieceFont("Arial", 38);
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            auto button = new QPushButton(this);
            buttons[row][col] = button;
            buttons[row][col]->setFont(pieceFont);
            buttons[row][col]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            gridLayout->addWidget(button, row, col);
            connect(button, &QPushButton::clicked, [this, row, col]() {
                onButtonClicked(row, col);
            });
        }
    }
}

void MainWindow::onButtonClicked(int row, int col) {

    _gameLogic.click(row, col, _board);
    boardWidget->show_possible_moves(buttons, gridLayout, _gameLogic._possibleMoves);
    boardWidget->setBoard(buttons, gridLayout, _board);

    updateGameStatus();
    show_playersTurn();
    show_eatenFiguresB();
    show_eatenFiguresW();
}

void MainWindow::updateGameStatus() {
    QString statusText;

    if (_board.isMate(_gameLogic._currentPlayer)) {
        QString winner = (_gameLogic._currentPlayer == PlayerColour::White) ? "Black" : "White";
        statusText = "Checkmate! " + winner + " wins.";
    } else {
        if (_board.check(_gameLogic._currentPlayer)) {
            statusText = "Check! King is under attack!";
        } else if (_board.Stalemate(_gameLogic._currentPlayer)) {
            statusText = "Stalemate! It's a draw.";
        } else{
            statusText = "";
        }
    }


    ui->lableShowCheckMate->setText(statusText);
}

void MainWindow::on_ButtonMakeRandomBoard_clicked()
{
    _board.clear();
    BoardTests::setUprandomBoard(_board, 15);
    boardWidget->setBoard(buttons, gridLayout, _board);
}

void MainWindow::show_playersTurn() {
    QString player = (_gameLogic._currentPlayer == PlayerColour::White) ? "White" : "Black";
    ui->PlayersTurn->setText(player + " to move");
}

void MainWindow::show_eatenFiguresB() {
    QString str = "Black captured : ";
    for (int iPiece = 0; iPiece < _gameLogic._eatenPiecesBlack.size(); ++iPiece) {
        Piece piece(_gameLogic._eatenPiecesBlack[iPiece], PlayerColour::Black);
        str += BoardWidget::pieceToUnicode(piece);
    }
    ui->lableEatenPiecesBlack->setText(str);
}

void MainWindow::show_eatenFiguresW(){
    QString str = "White captured : ";

    for (int iPiece = 0; iPiece < _gameLogic._eatenPiecesWhite.size(); ++iPiece) {
        Piece piece(_gameLogic._eatenPiecesWhite[iPiece], PlayerColour::White);
        str += BoardWidget::pieceToUnicode(piece);
    }
    ui->lableEatenPiecesWhite->setText(str);
}






