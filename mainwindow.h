#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "PlayerNameForm.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <memory>

class Player;
class Tile;
class Piece;

struct MoveData{
    MoveData(){
        move_ = nullptr;
        direction_ = "";
    }
    MoveData(Tile* m, QString d){
        move_ = m;
        direction_ = d;
    }
    Tile* move_;
    QString direction_;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void StartGame();                                         // Starts a new game
    void EndGame();                                           // Ends the current game
    void CreateBoard();                                       // Creates the board
    void CreatePlayers();                                     // Creates the players
    void CreatePlayerPieces(Player* player);                  // Creates pieces for a player of a given color
    void AddPiecesToBoard(Player* player1, Player* player2);  // Adds player pieces to the board
    void recolor_board();                                     // Colors the checkerboard
    void get_valid_moves(Piece* p);                           // Returns valid moves for a given piece
    void get_up_moves(Piece* p, int jump_row_up, int jump_col_left, int jump_col_right, int row_up, int col_right, int col_left);
    void get_down_moves(Piece* p, int jump_row_down, int jump_col_left, int jump_col_right, int row_down, int col_right, int col_left);
    void get_left_moves(Piece* p, MoveData* left_diagonal, int jump_row, int jump_col);
    void get_right_moves(Piece* p, MoveData* right_diagonal, int jump_row, int jump_col);
    void set_piece_pointers(Player* player, Piece* cur_piece, Tile* cur_tile, int y, int x, int* piece);
    void JumpHandler(Piece* jump);
    void KingHandler(Tile* t);
    bool is_player_2(Player* p);
    void EndTurn();

private slots:

    void on_startButton_clicked();
    void on_quitButton_clicked();
    void on_endButton_clicked();

    /* CUSTOM SLOTS */
    void PieceSelectedSlot(Piece *p);  // When the user selects a piece
    void TileSelectedSlot(Tile* t);    // When the user selects a tile

private:

    Ui::MainWindow *ui;
    QGraphicsScene* board;
    QGraphicsScene* playerNamesScene;
    QGraphicsView* view;

    PlayerNameForm* playerNamesWindow;

    std::vector<Player*> players_;        // Vector of players
    Tile* board_2d_[8][8];                // 8x8 2D Array that represents the game board state
    std::vector<MoveData*> valid_moves_;  // Vector of valid moves for a given piece

    Player* current_player_;  // Keeps track of current player
    Piece* piece_selected_;   // Keeps track of selected piece
    Piece* left_jump;         // Keeps track of the piece to the right that could be jumped
    Piece* right_jump;        // Keeps track of the piece to the left that could be jumped

    bool end_game_;

protected:
    std::shared_ptr<Piece> jumping_;
};

#endif // MAINWINDOW_H
