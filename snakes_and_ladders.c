#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10

typedef struct point {
    int row;
    int col;
} point;

int equal_points(point p1, point p2) {
    return p1.row == p2.row && p1.col == p2.col;
}

int RollDice() {
    return (rand() % 6) + 1;
}

point MovePlayer(point currentPosition) {
    int diceRoll = RollDice();
    int currentIndex = currentPosition.row * N + currentPosition.col;
    int newPosition = currentIndex + diceRoll;

    if (newPosition >= N * N) {
        newPosition = N * N - 1;
    }

    point newPoint = {newPosition / N, newPosition % N};

    printf("Dice roll: %d | New position: (%d, %d)\n",
           diceRoll, newPoint.row, newPoint.col);

    return newPoint;
}

int CountNeighbors(int board[N][N], point position) {
    int val = board[position.row][position.col];
    int count = 0;

    if (position.row > 0 && board[position.row - 1][position.col] == val) count++;
    if (position.row < N - 1 && board[position.row + 1][position.col] == val) count++;
    if (position.col > 0 && board[position.row][position.col - 1] == val) count++;
    if (position.col < N - 1 && board[position.row][position.col + 1] == val) count++;

    return count;
}

point End2End_Rec(int board[N][N], point lastPoint, point currentPoint) {
    int val = board[currentPoint.row][currentPoint.col];
    int i = currentPoint.row;
    int j = currentPoint.col;

    if (CountNeighbors(board, currentPoint) == 1 && lastPoint.row != -1) {
        return currentPoint;
    }

    if (i > 0 && board[i - 1][j] == val) {
        point nextPoint = {i - 1, j};
        if (!equal_points(nextPoint, lastPoint)) {
            return End2End_Rec(board, currentPoint, nextPoint);
        }
    }

    if (i < N - 1 && board[i + 1][j] == val) {
        point nextPoint = {i + 1, j};
        if (!equal_points(nextPoint, lastPoint)) {
            return End2End_Rec(board, currentPoint, nextPoint);
        }
    }

    if (j > 0 && board[i][j - 1] == val) {
        point nextPoint = {i, j - 1};
        if (!equal_points(nextPoint, lastPoint)) {
            return End2End_Rec(board, currentPoint, nextPoint);
        }
    }

    if (j < N - 1 && board[i][j + 1] == val) {
        point nextPoint = {i, j + 1};
        if (!equal_points(nextPoint, lastPoint)) {
            return End2End_Rec(board, currentPoint, nextPoint);
        }
    }

    point invalidPoint = {-1, -1};
    return invalidPoint;
}

point End2End(int board[N][N], point p) {
    point nullPoint = {-1, -1};
    return End2End_Rec(board, nullPoint, p);
}

point MoveLadderOrSnake(int board[N][N], point position) {
    int cellValue = board[position.row][position.col];

    if ((cellValue == 1 || cellValue == 2) && CountNeighbors(board, position) == 1) {
        point end = End2End(board, position);

        if (end.row == -1) {
            return position;
        }

        if (cellValue == 1 && end.row > position.row) {
            printf("Snake! Moving to (%d, %d)\n", end.row, end.col);
            position = end;
        }

        if (cellValue == 2 && end.row < position.row) {
            printf("Ladder! Moving to (%d, %d)\n", end.row, end.col);
            position = end;
        }
    }

    return position;
}

int RunGame(int board[N][N]) {
    point playerPosition = {0, 0};
    point endPosition = {N - 1, N - 1};

    int turns = 0;

    while (!equal_points(playerPosition, endPosition)) {
        turns++;

        printf("\nTurn %d\n", turns);

        playerPosition = MovePlayer(playerPosition);
        playerPosition = MoveLadderOrSnake(board, playerPosition);

        printf("Player position after turn: (%d, %d)\n",
               playerPosition.row, playerPosition.col);
    }

    return turns;
}

int main() {
    srand(time(NULL));

    int board[N][N] = {0};

    board[1][1] = 2;
    board[2][1] = 2;
    board[3][1] = 2;

    board[6][5] = 1;
    board[7][5] = 1;
    board[8][5] = 1;

    board[4][3] = 2;
    board[4][4] = 2;
    board[4][5] = 2;

    board[5][8] = 1;
    board[6][8] = 1;
    board[7][8] = 1;

    int totalTurns = RunGame(board);

    printf("\nGame finished successfully!\n");
    printf("Total turns: %d\n", totalTurns);

    return 0;
}
