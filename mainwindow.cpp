#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "tile.h"
#include "player.h"
#include "piece.h"
#include "utilities.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    view = ui->boardView;
    board = new QGraphicsScene;
    view->setScene(board);
    board->setSceneRect(0, 0, view->frameSize().width()-10, view->frameSize().height()-10);

    piece_selected_ = nullptr;
    current_player_ = nullptr;
    ui->endButton->setDisabled(true);
    this->setFixedSize(575,485);
}

MainWindow::~MainWindow()
{
    delete ui;
    piece_selected_ = nullptr;
    current_player_ = nullptr;
}

void MainWindow::StartGame(){
    /* Sets up a game to be played */

    playerNamesWindow = new PlayerNameForm(nullptr);
    playerNamesWindow->setModal(true);
    playerNamesWindow->setFixedSize(playerNamesWindow->width(), playerNamesWindow->height());
    playerNamesWindow->exec();

    CreateBoard();
    CreatePlayers();


    current_player_ = players_[1];
    ui->curPlayerLabel->setText(current_player_->get_name());
    ui->curPlayerLabel->setStyleSheet("color: " + current_player_->get_color().name(QColor::HexArgb) +
                                      "; font: 900 22pt 'Segoe UI'; background-color : black;");

    ui->player1Score->setText("0");
    ui->player2Score->setText("0");

    ui->endButton->setDisabled(false);
}

void MainWindow::CreateBoard(){
    /* Creates the tiles for the game board */

    int tile_id = 1;
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){

            Tile* t = new Tile(get_checkerboard_color(x, y), 50*x, 50*y, tile_id);
            t->set_col(x);
            t->set_row(y);
            connect(t, &Tile::TileSelected, this, &MainWindow::TileSelectedSlot);
            board_2d_[y][x] = t;
            board->addItem(t);
            tile_id++;

        }
    }
}

void MainWindow::CreatePlayerPieces(Player* player){
    /* Sets up each piece for each player */

    for(int i = 1; i <= 12; i++){
        Piece* p = new Piece(player->get_color(), i, player, nullptr);
        connect(p, &Piece::PieceSelected, this, &MainWindow::PieceSelectedSlot);
        player->add_piece(p);
    }
}

void MainWindow::CreatePlayers(){
    /* Sets up each player */

    QString player1_name = playerNamesWindow->get_player1();
    QString player2_name = playerNamesWindow->get_player2();

    /* CREATE PLAYERS */
    Player* player1 = new Player(player1_name, get_piece_color1());
    Player* player2 = new Player(player2_name, get_piece_color2());

    /* SET UI LABEL NAMES */
    ui->staticPlayer1Score->setText(player1_name + " Score");
    ui->staticPlayer2Score->setText(player2_name + " Score");

    /* CREATE PIECES */
    CreatePlayerPieces(player1);
    CreatePlayerPieces(player2);

    /* ADD TO PLAYERS VECTOR */
    players_.push_back(player1);
    players_.push_back(player2);

    /* ADD PIECES TO BOARD */
    AddPiecesToBoard(player1, player2);

}

void MainWindow::set_piece_pointers(Player* player, Piece* cur_piece, Tile* cur_tile, int y, int x, int* piece){
    /* Sets the pointers for the pieces and tiles */

    // set pointers to currently selected tile / piece
    cur_piece = player->get_pieces()[*piece];
    cur_tile = board_2d_[y][x];

    // set position of piece and pointers to each object going both ways
    cur_piece->set_pos(cur_tile->get_x()+5,
                       cur_tile->get_y()+5);
    cur_tile->set_piece(cur_piece);
    cur_piece->set_tile(cur_tile);
    cur_piece->set_row(y);
    cur_piece->set_col(x);
    *piece+=1;
}

void MainWindow::AddPiecesToBoard(Player* player1, Player* player2){
    /* Adds pieces to the game board */

    int piece = 0;

    /* SETTING POSITION FOR PIECES AND TILE->PIECE POINTERS (PLAYER 1) */
    Piece* cur_piece = nullptr;
    Tile* cur_tile = nullptr;

    for(int y = 0; y < 3; y++){
        for(int x = 0; x < 8; x++){
            if(!is_even(y)){
                if(!is_even(x)){
                    set_piece_pointers(player1, cur_piece, cur_tile, y, x, &piece);
                }
            }
            else {
                if(is_even(x)) {
                    set_piece_pointers(player1, cur_piece, cur_tile, y, x, &piece);
                }
            }
        }
    }

    /* SETTING POSITION FOR PIECES AND TILE->PIECE POINTERS (PLAYER 2) */
    piece = 0;
    cur_piece = nullptr;
    cur_tile = nullptr;

    for(int y = 7; y > 4; y--){
        for(int x = 0; x < 8; x++){
            if(!is_even(y)){
                if(!is_even(x)){
                    set_piece_pointers(player2, cur_piece, cur_tile, y, x, &piece);
                }
            }
            else{
                if(is_even(x)){
                    set_piece_pointers(player2, cur_piece, cur_tile, y, x, &piece);
                }
            }
        }
    }

    /* ADDING EACH PIECE TO THE BOARD (PLAYER 1) */
    for(int i = 0; i < 12; i++){
        board->addItem(player1->get_pieces()[i]);
    }
    /* ADDING EACH PIECE TO THE BOARD (PLAYER 2) */
    for(int i = 0; i < 12; i++){
        board->addItem(player2->get_pieces()[i]);
    }

    delete cur_piece;
    delete cur_tile;

}

void MainWindow::on_startButton_clicked()
{
    StartGame();
}

void MainWindow::recolor_board(){
    /* Recolors the checkboard */

    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            board_2d_[y][x]->set_color(get_checkerboard_color(x, y));
        }
    }
}

void MainWindow::PieceSelectedSlot(Piece* p){
    /* Handles when a player clicks on a piece */

    /* IF NEW PIECE SELECTED WAS SAME PIECE */
    if(piece_selected_ == p){
        recolor_board();
        piece_selected_ = nullptr;
        valid_moves_.clear();
        if(p->get_owner() == players_[1]){
            p->set_color(get_piece_color2());
        }
        else{
            p->set_color(get_piece_color1());
        }
        return;
    }

    valid_moves_.clear();

    /* IF THE PIECE BELONGS TO THE CURRENT PLAYER */
    if(p->get_owner() == current_player_){

        /* HIGHLIGHT SELECTED PIECE */
        p->set_color(p->get_color().lighter(150));

        /* RESET COLOR IF A NEW PIECE WAS SELECTED */
        if(piece_selected_){

            recolor_board();  // Recolor the board

            /* RESETTING COLORS IF LAST PIECE SELECTED WAS KING */
            if(piece_selected_->get_owner() == players_[1]){
                piece_selected_->set_color(get_piece_color2());
            }
            else{
                piece_selected_->set_color(get_piece_color1());
            }
        }

        piece_selected_ = p;  // Set the piece_selected_ pointer
        get_valid_moves(p);   // Get the valid moves for the selected piece

    }
}

void MainWindow::JumpHandler(Piece* jump){
    /* If a player jumped a piece, this function is called to handle the logic
     *
     *  Piece* jump - Pointer to jumped piece
    */

    /* Remove the jumped piece and if it was a king remove the king icon */
    board->removeItem(jump);
    if(jump->is_king()) jump->get_img_label()->hide();

    /* Add to current player score */
    current_player_->add_score();

    /* Set the tile where the piece that was jumped pointer to tile to null */
    board_2d_[jump->get_row()][jump->get_col()]->set_piece(nullptr);
}

void MainWindow::KingHandler(Tile* t){
    /* Handles kinging logic
     *
     * Tile* t - Pointer to tile where the piece got kinged
    */

    /* Create a label while calling set_king on the piece on the tile t */
    QLabel* l = t->get_piece()->set_king();

    /* Set the background to transparent and add the crown logo to the tile / board */
    l->setStyleSheet("background-color: transparent");
    board->addWidget(l);
}

void MainWindow::TileSelectedSlot(Tile* t){
    /* Handles when a player clicks on a tile */

    /* IF A PIECE HAS BEEN PREVIOUSLY SELECTED */
    if(piece_selected_){

        /* IF A VALID MOVE EXISTS */
        if(valid_moves_.size() != 0){

            /* LOOP THROUGH VALID MOVES VECTOR */
            for(int i = 0; i < valid_moves_.size(); i++){

                /* IF THE TILE SELECTED EXISTS IN THE VALID MOVES VECTOR */
                if(valid_moves_[i]->move_ == t){

                    piece_selected_->get_tile()->set_piece(nullptr);  // Set the tile's pointer to a piece where the original piece was to null
                    piece_selected_->set_pos(valid_moves_[i]->move_->get_x()+5, valid_moves_[i]->move_->get_y()+5);  // Set position to new tile position
                    piece_selected_->set_tile(t);                     // Set the piece's pointer to a tile to the new tile
                    piece_selected_->set_row(t->get_row());           // Set the row of the piece to the new tile
                    piece_selected_->set_col(t->get_col());           // Set the colomn of the piece to the new tile
                    t->set_piece(piece_selected_);                    // Set the tile selected pointer to piece to the piece that was selected

                    /* UPDATE CROWN POSITION */
                    if(t->get_piece()->is_king()){
                        t->get_piece()->set_crown_pos(t->get_x()+5, t->get_y()+5);
                    }
                    /* ===================== */

                    /* ADD TO PLAYER SCORE IF THE MOVE JUMPED AN ENEMY */
                    if(valid_moves_[i]->direction_ == "jump right"){
                        JumpHandler(right_jump);
                    }
                    else if(valid_moves_[i]->direction_ == "jump left"){
                        JumpHandler(left_jump);
                    }
                    /* =============================================== */

                    /* KING PIECE IF OPPOSITE END REACHED */
                    if(t->get_piece()->get_owner() == players_[1] && t->get_row() == 0 ||
                            t->get_piece()->get_owner() == players_[0] && t->get_row() == 7 &&
                            !t->get_piece()->is_king()){
                        KingHandler(t);
                    }

                    /* ================================= */

                    recolor_board();  // Recolor the checkerboard
                    EndTurn();

                    // TIME DELAYS
                    QTime whatever = QTime::currentTime().addMSecs(100);
                    while(QTime::currentTime() < whatever){
                        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
                    }
                }
            }
        }
    }
}

bool MainWindow::is_player_2(Player* p){
    if(current_player_ == players_[0]){
        return true;
    }
    return false;
}

void MainWindow::EndTurn(){

    /* Ensure each piece is colored correctly */
    for(int i = 0; i < current_player_->get_pieces().size(); i++){
        current_player_->get_pieces()[i]->set_color(current_player_->get_color());
    }

    /* Set the new score */
    if(is_player_2(current_player_)){
        ui->player2Score->setText(QString::number(players_[0]->get_score()));
    }
    else{
        ui->player1Score->setText(QString::number(players_[1]->get_score()));
    }

    /* If player won, end the game */
    if(current_player_->get_score() == 12){
        EndGame();
        return;
    }

    /* Set current player to next player */
    if(is_player_2(current_player_)){
        current_player_ = players_[1];
    }
    else{
        current_player_ = players_[0];
    }

    /* Set labels to reflect new player */
    ui->curPlayerLabel->setText(current_player_->get_name());
    ui->curPlayerLabel->setStyleSheet("color: " + current_player_->get_color().name(QColor::HexArgb) +
                                      "; font: 900 22pt 'Segoe UI'; background-color : black;");
}

void MainWindow::get_right_moves(Piece* p, MoveData* right_diagonal, int jump_row, int jump_col){

    /* TILE IS EMPTY */
    if(!right_diagonal->move_->get_piece()){
        if(right_diagonal->move_->get_color() != get_tile_off_color()){
            right_diagonal->move_->highlight();
            valid_moves_.push_back(right_diagonal);
        }
    }
    /* TILE HAS PIECE */
    else{

        // if piece belongs to opposite player
        if(right_diagonal->move_->get_piece()->get_owner() != current_player_){
            /* BOUNDS CHECKING */
            if(jump_row < 8 && jump_col < 8 && jump_row >= 0 && jump_col >= 0){

                MoveData* jump_right_diagonal = new MoveData(board_2d_[jump_row][jump_col], "jump right");
                if(jump_right_diagonal->move_->get_color() != get_tile_off_color()){
                    if(!jump_right_diagonal->move_->get_piece()){
                        right_jump = right_diagonal->move_->get_piece();
                        jump_right_diagonal->move_->highlight();
                        valid_moves_.push_back(jump_right_diagonal);
                    }
                }
            }
        }
    }
}
void MainWindow::get_left_moves(Piece* p, MoveData* left_diagonal, int jump_row, int jump_col){

    /* TILE IS EMPTY */
    if(!left_diagonal->move_->get_piece()){
        if(left_diagonal->move_->get_color() != get_tile_off_color()){
            left_diagonal->move_->highlight();
            valid_moves_.push_back(left_diagonal);
        }
    }
    /* TILE HAS PIECE */
    else{
        if(left_diagonal->move_->get_piece()->get_owner() != current_player_){
            /* BOUNDS CHECKING */
            if(jump_row < 8 && jump_col < 8 && jump_row >= 0 && jump_col >= 0){
                MoveData* jump_left_diagonal = new MoveData(board_2d_[jump_row][jump_col], "jump left");
                if(jump_left_diagonal->move_->get_color() != get_tile_off_color()){
                    if(!jump_left_diagonal->move_->get_piece()){
                        left_jump = left_diagonal->move_->get_piece();
                        jump_left_diagonal->move_->highlight();
                        valid_moves_.push_back(jump_left_diagonal);
                    }
                }
            }
        }
    }
}
void MainWindow::get_up_moves(Piece* p, int jump_row_up, int jump_col_left, int jump_col_right, int row_up, int col_right, int col_left){

    // IF CURRENT IS BOTTOM PLAYER OR PIECE IS KING
    if(current_player_ == players_[1] || p->is_king()){

        // PIECE ISN'T AT VERY TOP ALREADY
        if(p->get_row() != 0){

            /* CREATE POINTERS TO LEFT AND RIGHT DIAGONAL MOVES */
            MoveData* right_diagonal = new MoveData(board_2d_[row_up][col_right], "right");
            MoveData* left_diagonal = new MoveData(board_2d_[row_up][col_left], "left");

            /* RIGHT */
            if(p->get_col() != 7){
                get_right_moves(p, right_diagonal, jump_row_up, jump_col_right);
            }
            /* LEFT */
            if(p->get_col() != 0){
                get_left_moves(p, left_diagonal, jump_row_up, jump_col_left);

            }
        }
    }
}
void MainWindow::get_down_moves(Piece* p, int jump_row_down, int jump_col_left, int jump_col_right, int row_down, int col_right, int col_left){

    if(current_player_ == players_[0] || p->is_king()){

        /* IF PIECE ISNT AT VERY BOTTOM ALREADY */
        if(p->get_row() != 7){

            MoveData* right_diagonal = new MoveData(board_2d_[row_down][col_right], "right");
            MoveData* left_diagonal = new MoveData(board_2d_[row_down][col_left], "left");

            /* RIGHT */
            if(p->get_col() != 7){
                get_right_moves(p, right_diagonal, jump_row_down, jump_col_right);
            }
            /* LEFT */
            if(p->get_col() != 0){
                get_left_moves(p, left_diagonal, jump_row_down, jump_col_left);
            }
        }
    }
}
void MainWindow::get_valid_moves(Piece* p){
    /* Populates the valid moves vector with pointers to valid moves for a selected piece */

    /* 2 distance away positions (jump) */
    int jump_row_down = p->get_row()+2;
    int jump_row_up = p->get_row()-2;
    int jump_col_right = p->get_col()+2;
    int jump_col_left = p->get_col()-2;

    /* 1 distance away positions */
    int row_down = p->get_row()+1;
    int row_up = p->get_row()-1;
    int col_right = p->get_col()+1;
    int col_left = p->get_col()-1;

    get_down_moves(p, jump_row_down, jump_col_left, jump_col_right, row_down, col_right, col_left);  // Get downward moves
    get_up_moves(p, jump_row_up, jump_col_left, jump_col_right, row_up, col_right, col_left);        // Get upwards moves

}

void MainWindow::on_quitButton_clicked()
{
    QApplication::quit();
}


void MainWindow::on_endButton_clicked()
{
    EndGame();
}

void MainWindow::EndGame(){
    /* Tears down the game */

    ui->staticCurPlayerLabel->setText("WINNER");
    ui->curPlayerLabel->setText(current_player_->get_name());


    /* DELETE TILE MEM */
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            delete board_2d_[y][x];
        }
    }

    /* DELETE PIECES AND PLAYER MEM */
    for(int player = 0; player < 2; player++){
        for(int piece = 0; piece < 12; piece++){
            if(players_[player]->get_pieces()[piece]->get_img_label()) players_[player]->get_pieces()[piece]->get_img_label()->hide();
            delete players_[player]->get_pieces()[piece];
        }
        delete players_[player];
    }

    /* CLEAR VECTORS */
    valid_moves_.clear();
    players_.clear();

    /* SET NULLPTR AND DELETE MEMORY */
    piece_selected_ = nullptr;
    delete playerNamesWindow;

    /* RESETTING UI ELEMENTS */
    ui->staticPlayer1Score->setText("");
    ui->staticPlayer2Score->setText("");
    ui->player1Score->setText("");
    ui->player2Score->setText("");

    ui->endButton->setDisabled(true);
}
