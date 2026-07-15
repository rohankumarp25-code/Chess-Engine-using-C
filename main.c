#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct {
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
    int score;
} Move;
void initBoard(char board[8][8]){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            board[i][j]='.';
        }
    }
    board[0][0] = 'r';
board[0][1] = 'n';
board[0][2] = 'b';
board[0][3] = 'q';
board[0][4] = 'k';
board[0][5] = 'b';
board[0][6] = 'n';
board[0][7] = 'r';
board[7][0]='R';
board[7][1]='N';
board[7][2]='B';
board[7][3]='Q';
board[7][4]='K';
board[7][5]='B';
board[7][6]='N';
board[7][7]='R';
    for(int i=0;i<8;i++){
        board[1][i]='p';
        board[6][i]='P';
    }
}
void printBoard(char board[8][8]) {
    printf("\n");
    for (int m = 0; m < 8; m++) {
        // 1. Print row numbers (8 down to 1)
        printf("%d  ", 8 - m); 

        for (int n = 0; n < 8; n++) {
            printf("%c ", board[m][n]);
        }
        printf("\n");
    }

    // 2. Print column letters (a through h)
    printf("   ----------------\n");
    printf("   a b c d e f g h\n\n");
}
void getMove(int *fromRow, int *fromCol, int *toRow, int *toCol) {
    char from[3], to[3];
   

    printf("Enter move (e.g., e2 e4): ");
    scanf("%2s %2s", from, to);

     *fromCol = from[0] - 'a';
     *fromRow = 8 - (from[1] - '0');

     *toCol = to[0] - 'a';
     *toRow = 8 - (to[1] - '0');

    printf("From: %d %d\n", *fromRow, *fromCol);
    printf("To: %d %d\n", *toRow, *toCol);
}

void movePiece(char board[8][8], int fromRow, int fromCol, int toRow, int toCol){
    char piece=board[fromRow][fromCol];
    board[fromRow][fromCol]='.';
    board[toRow][toCol]=piece;
}


int getPieceValue(char piece) {

    switch(piece) {

        case 'P':
        case 'p':
            return 1;

        case 'N':
        case 'n':
            return 3;

        case 'B':
        case 'b':
            return 3;

        case 'R':
        case 'r':
            return 5;

        case 'Q':
        case 'q':
            return 9;

        case 'K':
        case 'k':
            return 1000;
    }

    return 0;
}

Move findBestMove(Move moves[], int moveCount) {

    Move emptyMove;

    emptyMove.fromRow=-1;
    emptyMove.fromCol=-1;
    emptyMove.toRow=-1;
    emptyMove.toCol=-1;
    emptyMove.score=-9999;

    if(moveCount==0)
        return emptyMove;

    int bestScore=moves[0].score;

    Move bestMoves[500];

    int bestCount=0;

    for(int i=0;i<moveCount;i++){

        if(moves[i].score>bestScore){

            bestScore=moves[i].score;

            bestCount=0;

            bestMoves[bestCount++]=moves[i];
        }

        else if(moves[i].score==bestScore){

            bestMoves[bestCount++]=moves[i];
        }
    }

    return bestMoves[rand()%bestCount];
}
void printAIMove(Move move) {

    char fromFile='a'+move.fromCol;
    char toFile='a'+move.toCol;

    int fromRank=8-move.fromRow;
    int toRank=8-move.toRow;

    printf("\n====================\n");

    printf("AI MOVE PREDICTION\n");

    printf("Move: %c%d -> %c%d\n",
           fromFile,
           fromRank,
           toFile,
           toRank);

    printf("Move Score: %d\n",
           move.score);

    printf("====================\n");
}

int isValidMove(char board[8][8], int fromRow, int fromCol, int toRow, int toCol, int turn) {
    // 1. Boundary Checks
    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8 ||
        toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8)
        return 0;

    char piece = board[fromRow][fromCol];
    char dest = board[toRow][toCol];

    // 2. Basic Checks (Empty start, Turn check)
    if (piece == '.') return 0;

    if (turn == 0 && !(piece >= 'A' && piece <= 'Z')) return 0;
    if (turn == 1 && !(piece >= 'a' && piece <= 'z')) return 0;

    // 3. Friendly Fire Prevention
    if (dest != '.') {
        if ((turn == 0 && (dest >= 'A' && dest <= 'Z')) ||
            (turn == 1 && (dest >= 'a' && dest <= 'z'))) {
            return 0;
        }
    }

    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);

    // 4. Piece Specific Logic
    if (piece == 'P') { // White Pawn
        if (toRow == fromRow - 1 && toCol == fromCol && dest == '.') return 1;
        if (fromRow == 6 && toRow == 4 && toCol == fromCol && board[5][fromCol] == '.' && dest == '.') return 1;
        if (toRow == fromRow - 1 && colDiff == 1 && dest != '.') return 1;
        return 0;
    }
    if (piece == 'p') { // Black Pawn
        if (toRow == fromRow + 1 && toCol == fromCol && dest == '.') return 1;
        if (fromRow == 1 && toRow == 3 && toCol == fromCol && board[2][fromCol] == '.' && dest == '.') return 1;
        if (toRow == fromRow + 1 && colDiff == 1 && dest != '.') return 1;
        return 0;
    }
    if (piece == 'R' || piece == 'r' || piece == 'Q' || piece == 'q' || piece == 'B' || piece == 'b') {
        // Rook logic part
        if ((piece == 'R' || piece == 'r' || piece == 'Q' || piece == 'q') && (fromRow == toRow || fromCol == toCol)) {
            int stepR = (toRow == fromRow) ? 0 : (toRow > fromRow ? 1 : -1);
            int stepC = (toCol == fromCol) ? 0 : (toCol > fromCol ? 1 : -1);
            int r = fromRow + stepR, c = fromCol + stepC;
            while (r != toRow || c != toCol) {
                if (board[r][c] != '.') return 0;
                r += stepR; c += stepC;
            }
            return 1;
        }
        // Bishop logic part
        if ((piece == 'B' || piece == 'b' || piece == 'Q' || piece == 'q') && (rowDiff == colDiff)) {
            int stepR = (toRow > fromRow) ? 1 : -1;
            int stepC = (toCol > fromCol) ? 1 : -1;
            int r = fromRow + stepR, c = fromCol + stepC;
            while (r != toRow) {
                if (board[r][c] != '.') return 0;
                r += stepR; c += stepC;
            }
            return 1;
        }
        return 0;
    }
    if (piece == 'N' || piece == 'n') {
        return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
    }
    if (piece == 'K' || piece == 'k') {
        return (rowDiff <= 1 && colDiff <= 1 && !(rowDiff == 0 && colDiff == 0));
    }
    return 0;
}
void evaluateMoves(char board[8][8], Move moves[], int moveCount) {

    for(int i=0;i<moveCount;i++){

        int score=0;

        int fromRow= moves[i].fromRow;
        int fromCol= moves[i].fromCol;

        int toRow= moves[i].toRow;
        int toCol= moves[i].toCol;

        char movingPiece=board[fromRow][fromCol];
        char capturedPiece=board[toRow][toCol];

        // Capture reward
        score += getPieceValue(capturedPiece);

        // Center bonus
        if((toRow>=3 && toRow<=4) &&
           (toCol>=3 && toCol<=4))
            score +=2;

        // Near-center bonus
        else if((toRow>=2 && toRow<=5) &&
                (toCol>=2 && toCol<=5))
            score +=1;

        // -------- TEMP MOVE --------
        board[toRow][toCol]=movingPiece;
        board[fromRow][fromCol]='.';

        // Check whether moved piece can be captured
        for(int r=0;r<8;r++){

            for(int c=0;c<8;c++){

                char enemy=board[r][c];

                if(enemy>='A' && enemy<='Z'){

                    if(isValidMove(board,
                                   r,c,
                                   toRow,toCol,
                                   0)){

                        score -= getPieceValue(movingPiece);

                    }
                }
            }
        }

        // UNDO MOVE
        board[fromRow][fromCol]=movingPiece;
        board[toRow][toCol]=capturedPiece;

        moves[i].score=score;
    }
}
int isCheck(char board[8][8], int turn) {
    int kingRow = -1, kingCol = -1;
    char kingChar = (turn == 0) ? 'K' : 'k';

    // 1. Find the current player's King
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == kingChar) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
    }

    // 2. Check if any enemy piece can hit this King
    int enemyTurn = 1 - turn;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char piece = board[r][c];
            // If it's an enemy piece
            if (piece != '.' && 
               ((turn == 0 && piece >= 'a' && piece <= 'z') || 
                (turn == 1 && piece >= 'A' && piece <= 'Z'))) {
                
                // Use your existing isValidMove logic!
                if (isValidMove(board, r, c, kingRow, kingCol, enemyTurn)) {
                    return 1; // King is under attack!
                }
            }
        }
    }
    return 0; // King is safe
}
int generateAllMoves(char board[8][8], int turn, Move moves[]) {

    int moveCount = 0;

    // Loop through entire board
    for (int fromRow = 0; fromRow < 8; fromRow++) {

        for (int fromCol = 0; fromCol < 8; fromCol++) {

            char piece = board[fromRow][fromCol];

            // Skip empty squares
            if (piece == '.')
                continue;

            // White turn -> uppercase only
            if (turn == 0 &&
                !(piece >= 'A' && piece <= 'Z'))
                continue;

            // Black turn -> lowercase only
            if (turn == 1 &&
                !(piece >= 'a' && piece <= 'z'))
                continue;

            // Try every destination square
            for (int toRow = 0; toRow < 8; toRow++) {

                for (int toCol = 0; toCol < 8; toCol++) {

                    // Check basic move validity
                    if (isValidMove(board,
                                    fromRow, fromCol,
                                    toRow, toCol,
                                    turn)) {

                        // TEMPORARY MOVE
                        char movingPiece = board[fromRow][fromCol];
                        char capturedPiece = board[toRow][toCol];

                        board[toRow][toCol] = movingPiece;
                        board[fromRow][fromCol] = '.';

                        // Reject moves leaving own king in check
                        if (!isCheck(board, turn)) {

                            moves[moveCount].fromRow = fromRow;
                            moves[moveCount].fromCol = fromCol;
                            moves[moveCount].toRow = toRow;
                            moves[moveCount].toCol = toCol;
                            moves[moveCount].score = 0;

                            moveCount++;
                        }

                        // UNDO MOVE
                        board[fromRow][fromCol] = movingPiece;
                        board[toRow][toCol] = capturedPiece;
                    }
                }
            }
        }
    }

    return moveCount;
}
int isGameOver(char board[8][8], int turn) {

    Move moves[500];

    int moveCount =
        generateAllMoves(board, turn, moves);

    return (moveCount == 0);
}

int main() {
    char board[8][8];
    int fromRow, fromCol, toRow, toCol;
    int turn = 0;

    initBoard(board);
    srand(time(NULL));

   while (1) {

    printBoard(board);

    // =========================
    // HUMAN TURN (WHITE)
    // =========================
    if (turn == 0) {

        printf("White's move\n");

        getMove(&fromRow, &fromCol, &toRow, &toCol);

        if (isValidMove(board,
                        fromRow, fromCol,
                        toRow, toCol,
                        turn)) {

            char movingPiece = board[fromRow][fromCol];
            char capturedPiece = board[toRow][toCol];

            board[toRow][toCol] = movingPiece;
            board[fromRow][fromCol] = '.';

            if (isCheck(board, turn)) {

                printf("\nInvalid! King in check.\n");

                board[fromRow][fromCol] = movingPiece;
                board[toRow][toCol] = capturedPiece;
            }
            else {

                if (isCheck(board,1-turn))
    printf("\n*** CHECK! ***\n");

if (isGameOver(board,1-turn)) {

    printBoard(board);

    if (isCheck(board,1-turn))
        printf("\nCHECKMATE! White Wins!\n");
    else
        printf("\nSTALEMATE!\n");

    break;
}

turn=1;
            }
        }
        else {

            printf("Invalid move!\n");
        }
    }

    // =========================
    // AI TURN (BLACK)
    // =========================
    else {

        printf("\nAI is thinking...\n");

        Move moves[500];

        int moveCount =
            generateAllMoves(board, 1, moves);

        evaluateMoves(board, moves, moveCount);

        Move bestMove =
            findBestMove(moves, moveCount);
            if(bestMove.fromRow==-1){

    printf("\nNo legal moves left.\n");

    break;
}

        movePiece(board,
                  bestMove.fromRow,
                  bestMove.fromCol,
                  bestMove.toRow,
                  bestMove.toCol);

        printAIMove(bestMove);

        if (isCheck(board, 0))
            printf("\n*** CHECK! ***\n");

        if (isGameOver(board,0)) {

    printBoard(board);

    if (isCheck(board,0))
        printf("\nCHECKMATE! Black Wins!\n");
    else
        printf("\nSTALEMATE!\n");

    break;
}

turn=0;
    }
}
    return 0;
}