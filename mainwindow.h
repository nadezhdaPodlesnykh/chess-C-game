#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include "BoardWidget.h"
#include "game_logic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClicked(int row, int col);

    void on_ButtonMakeRandomBoard_clicked();
    void show_playersTurn();
    void show_eatenFiguresB();
    void show_eatenFiguresW();
    void updateGameStatus();


private:
    void createBoard();

    Ui::MainWindow *ui;
    QGridLayout *gridLayout;
    QPushButton* buttons[8][8];
    BoardWidget *boardWidget;
    Board _board;
    GameLogic _gameLogic;
};

#endif // MAINWINDOW_H
