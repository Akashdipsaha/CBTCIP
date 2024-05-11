#include <iostream>
#include <vector>
#include <cstring>   
#include <unistd.h>     
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>  

using namespace std;

class TicTacToe {
private:
    vector<vector<char>> board;

public:
    TicTacToe() : board(3, vector<char>(3, ' ')) {}

    void printBoard() {
        cout << "\n";
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cout << board[i][j];
                if (j < 2) cout << " | ";
            }
            cout << endl;
            if (i < 2) cout << "---------" << endl;
        }
        cout << "\n";
    }

    bool makeMove(int row, int col, char player) {
        if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ') {
            board[row][col] = player;
            return true;
        }
        return false;
    }

    bool checkWin() {
        for (int i = 0; i < 3; i++) {
            if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2] ||
                board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
                return true;
        }
        if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2] ||
            board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
            return true;
        return false;
    }

    bool checkDraw() {
        for (const auto& row : board) {
            for (char cell : row) {
                if (cell == ' ') return false;
            }
        }
        return true;
    }
};

class NetworkGame {
private:
    int sock;
    struct sockaddr_in server;

public:
    NetworkGame(const char* ipAddress, int port, bool isServer) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            cerr << "Cannot create socket!" << endl;
            exit(EXIT_FAILURE);
        }

        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        if (isServer) {
            server.sin_addr.s_addr = INADDR_ANY;
            if (bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0 ||
                listen(sock, 1) < 0) {
                cerr << "Binding or listening failed!" << endl;
                close(sock);
                exit(EXIT_FAILURE);
            }
            cout << "Server setup complete. Waiting for opponent to connect..." << endl;
            socklen_t c = sizeof(sockaddr_in);
            sock = accept(sock, (struct sockaddr*)&server, &c);
            if (sock < 0) {
                cerr << "Failed to accept connection!" << endl;
                close(sock);
                exit(EXIT_FAILURE);
            }
        } else {
            server.sin_addr.s_addr = inet_addr(ipAddress);
            if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
                cerr << "Connection failed!" << endl;
                close(sock);
                exit(EXIT_FAILURE);
            }
        }
    }

    ~NetworkGame() {
        close(sock);
    }

    void sendMove(int row, int col, char player) {
        int data[3] = {row, col, player};
        if (send(sock, data, sizeof(data), 0) < 0) {
            cerr << "Failed to send move!" << endl;
            close(sock);
            exit(EXIT_FAILURE);
        }
    }

    void receiveMove(int& row, int& col, char& player) {
        int data[3];
        if (recv(sock, data, sizeof(data), 0) < 0) {
            cerr << "Failed to receive move!" << endl;
            close(sock);
            exit(EXIT_FAILURE);
        }
        row = data[0];
        col = data[1];
        player = data[2];
    }
};

int main() {
    string ipAddress;
    int port;
    cout << "Enter IP address (client mode) or leave blank for server mode: ";
    getline(cin, ipAddress);
    cout << "Enter port number: ";
    cin >> port;
    cin.ignore();

    bool isServer = ipAddress.empty();
    NetworkGame game(isServer ? nullptr : ipAddress.c_str(), port, isServer);
    TicTacToe ticTacToe;
    char currentPlayer = isServer ? 'X' : 'O';
    bool isMyTurn = isServer;

    while (true) {
        if (isMyTurn) {
            ticTacToe.printBoard();
            int row, col;
            do {
                cout << "Your turn. Enter row and column (0-based): ";
                cin >> row >> col;
            } while (!ticTacToe.makeMove(row, col, currentPlayer));

            game.sendMove(row, col, currentPlayer);
            if (ticTacToe.checkWin()) {
                ticTacToe.printBoard();
                cout << "You win!" << endl;
                break;
            }
            if (ticTacToe.checkDraw()) {
                ticTacToe.printBoard();
                cout << "It's a draw!" << endl;
                break;
            }
            isMyTurn = false;
        } else {
            cout << "Waiting for opponent's move..." << endl;
            int row, col;
            char player;
            game.receiveMove(row, col, player);
            ticTacToe.makeMove(row, col, player);
            if (ticTacToe.checkWin()) {
                ticTacToe.printBoard();
                cout << "Opponent wins!" << endl;
                break;
            }
            if (ticTacToe.checkDraw()) {
                ticTacToe.printBoard();
                cout << "It's a draw!" << endl;
                break;
            }
            isMyTurn = true;
        }
    }
    return 0;
}
