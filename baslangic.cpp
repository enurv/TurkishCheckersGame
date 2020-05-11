#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;


//function declarations
void initializeBoardFromScratch();
void showCurrentBoard();
void changeTurn();
void saveMove(char player, string m0ve);
void initializeMoves();
char currentPlayer();
void determineCheckers();


//Global Variables
int row = 10; //number of characters by row;

//Class for move;
class Move{

    public: int currentRow;
    public: int currentColumn;
    public: int destinationRow;
    public: int destinationColumn;
    public: char player; //team character (w,b)
    public: char playersChecker; //checkers character (white = x, black = y)
    public: bool switchPlayerTurn; //specify if it is time to switch the player turn

    Move(){
    }

    //constructor
    Move(string m0ve){ //parse moves for following file operations
        if(m0ve.size() != 4){ //controls the string
            cout<<"Invalid move!!"<<endl;
        }else{
            player = currentPlayer();
            if(player == 'w'){
                playersChecker = 'x';
            }else{
                playersChecker = 'y';
            }
            currentColumn=letterToNumber(m0ve[0]);
            currentRow= formatRows(m0ve[1] - '0');
            destinationColumn=letterToNumber(m0ve[2]);
            destinationRow= formatRows(m0ve[3] - '0');
        }
        switchPlayerTurn = hamle(currentRow, currentColumn, destinationRow, destinationColumn, player, playersChecker);
        saveMove(player, m0ve);
    }

    //makes a move and decides if turn is switched
    bool hamle(int currentRow, int currentColumn, int destinationRow, int destinationColumn, char player, char playersChecker){
        bool switchTurn = true;
        fstream outfile;
        char current;
        char avaliable;
        currentRow--;
        currentColumn--;
        destinationRow--;
        destinationColumn--;
        outfile.open("index.dat");
        outfile.seekg((row*destinationRow) + destinationColumn, ios::beg);
        outfile>>avaliable;
        if(avaliable != '-'){
            cout<<"The tile has already taken."<<endl;
            outfile.close();
            switchTurn = false;
            return switchTurn;
        }
        outfile.seekg((row*currentRow) + currentColumn, ios::beg);
        outfile>>current;
        if(current != player && current != playersChecker){
            cout<<"It's not your stamp"<<endl;
            outfile.close();
            switchTurn = false;
            return switchTurn;
        }
        //if it is normal stamp
        if(current == player){
            if(current == 'b'){
                if(destinationRow > currentRow){ //can not move backwards
                    cout<<"A stamp can not move backwards"<<endl;
                    outfile.close();
                    switchTurn = false;
                    return switchTurn;
                }
            }else if(current == 'w'){
                if(destinationRow < currentRow){ //can not move backwards
                    cout<<"A stamp can not move backwards"<<endl;
                    outfile.close();
                    switchTurn = false;
                    return switchTurn;
                }
            }
            if(destinationColumn != currentColumn && destinationRow != currentRow){ //can not move diagonally
                cout<<"A stamp can not move diagonally"<<endl;
                outfile.close();
                switchTurn = false;
                return switchTurn;
            }
            if(destinationColumn - currentColumn == 2){ //if player jumps over a tile horizontally to right
                char control;
                outfile.seekg((row*destinationRow) + destinationColumn - 1, ios::beg); //checks the tile that is jumped over
                outfile>>control;
                if(player == 'w'){ //if player is white
                    if(control == 'y' || control=='b'){ //and the opponent is standing in that tile
                        outfile.seekg((row*destinationRow) + destinationColumn - 1, ios::beg);
                        outfile<<'-'; //removes the opponent
                        switchTurn = false;
                    }else{ //else it is not allowed
                        cout<<"This move is not allowed"<<endl;
                        outfile.close();
                        switchTurn = false;
                        return switchTurn;
                    }
                }else if(player == 'b'){ //if player is black
                    if(control == 'x' || control=='w'){ //and the opponent is standing in that tile
                        outfile.seekg((row*destinationRow) + destinationColumn - 1, ios::beg);
                        outfile<<'-'; //removes the opponent
                        switchTurn = false;
                    }else{
                        cout<<"This move is not allowed"<<endl;
                        outfile.close();
                        switchTurn = false;
                        return switchTurn;
                    }
                }
            }else if(destinationColumn - currentColumn == -2){ //if player jumps over a tile horizontally to left
                char control;
                outfile.seekg((row*destinationRow) + destinationColumn + 1, ios::beg); //checks the tile that is jumped over
                outfile>>control;
                if(player == 'w'){ //if player is white
                    if(control == 'y' || control=='b'){ //and the opponent is standing in that tile
                        outfile.seekg((row*destinationRow) + destinationColumn + 1, ios::beg);
                        outfile<<'-'; //removes the opponent
                        switchTurn = false;
                    }else{ //else it is not allowed
                        cout<<"This move is not allowed"<<endl;
                        outfile.close();
                        switchTurn = false;
                        return switchTurn;
                    }
                }else if(player == 'b'){ //if player is black
                    if(control == 'x' || control=='w'){ //and the opponent is standing in that tile
                        outfile.seekg((row*destinationRow) + destinationColumn + 1, ios::beg);
                        outfile<<'-'; //removes the opponent
                        switchTurn = false;
                    }else{
                        cout<<"This move is not allowed"<<endl;
                        outfile.close();
                        switchTurn = false;
                        return switchTurn;
                    }
                }
            }
            else if(destinationRow - currentRow == 2 || destinationRow - currentRow == -2){ //if player jumps over a tile vertically
                char control;
                if(player == 'w'){ //if player is white
                    outfile.seekg((row*(destinationRow-1)) + destinationColumn, ios::beg); //checks the tile that is jumped over
                    outfile>>control;
                    if(control == 'y' || control=='b'){ //and the opponent is standing in that tile
                        outfile.seekg((row*(destinationRow-1)) + destinationColumn, ios::beg);
                        outfile<<'-'; //removes the opponent
                        switchTurn = false;
                    }else{ //else it is not allowed
                        cout<<"This move is not allowed"<<endl;
                        outfile.close();
                        switchTurn = false;
                        return switchTurn;
                    }
                }else if(player == 'b'){ //if player is black
                    outfile.seekg((row*(destinationRow+1)) + destinationColumn, ios::beg); //checks the tile that is jumped over
                    outfile>>control;
                    if(control == 'x' || control=='w'){ //and the opponent is standing in that tile
                        outfile.seekg((row*(destinationRow+1)) + destinationColumn, ios::beg);
                        outfile<<'-'; //removes the opponent
                        switchTurn = false;
                    }else{
                        cout<<"This move is not allowed"<<endl;
                        outfile.close();
                        switchTurn = false;
                        return switchTurn;
                    }
                }
            }

        }else if(current == playersChecker){ //or it is a checker
            if(destinationColumn != currentColumn && destinationRow != currentRow){ //can not move diagonally
                cout<<"A stamp can not move diagonally"<<endl;
                outfile.close();
                switchTurn = false;
                return switchTurn;
            }else if(destinationColumn - currentColumn > 1){ //if player jumps over a tile horizontally to right
                char control;
                outfile.seekg((row*destinationRow) + destinationColumn - 1, ios::beg); //checks the tile that is jumped over
                outfile>>control;
                if(playersChecker == 'x'){ //if player is white
                    if(control == 'y' || control=='b'){ //and the opponent is standing in that tile
                        outfile.seekg((row*destinationRow) + destinationColumn - 1, ios::beg);
                        outfile<<'-'; //removes the opponent
                        switchTurn = false;
                    }else if(control == 'w' || control == 'x'){
                        cout<<"This move is not allowed"<<endl;
                        switchTurn = false;
                        return switchTurn;
                    }
                }else if(playersChecker == 'y'){ //if player is black
                    if(control == 'x' || control=='w'){ //and the opponent is standing in that tile
                        outfile.seekg((row*destinationRow) + destinationColumn - 1, ios::beg);
                        outfile<<'-'; //removes the opponent
                        switchTurn = false;
                    }else if(control == 'b' || control == 'y'){
                        cout<<"This move is not allowed"<<endl;
                        switchTurn = false;
                        return switchTurn;
                    }
                }
            }else if(destinationColumn - currentColumn < -1){ //if player jumps over a tile horizontally to left
                char control;
                outfile.seekg((row*destinationRow) + destinationColumn + 1, ios::beg); //checks the tile that is jumped over
                outfile>>control;
                if(playersChecker == 'x'){ //if player is white
                    if(control == 'y' || control=='b'){ //and the opponent is standing in that tile
                        outfile.seekg((row*destinationRow) + destinationColumn + 1, ios::beg);
                        outfile<<'-'; //removes the opponent
                        switchTurn = false;
                    }else if(control == 'w' || control == 'x'){
                        cout<<"This move is not allowed"<<endl;
                        switchTurn = false;
                        return switchTurn;
                    }
                }else if(playersChecker == 'y'){ //if player is black
                    if(control == 'x' || control=='w'){ //and the opponent is standing in that tile
                        outfile.seekg((row*destinationRow) + destinationColumn + 1, ios::beg);
                        outfile<<'-'; //removes the opponent
                        switchTurn = false;
                    }else if(control == 'b' || control == 'y'){
                        cout<<"This move is not allowed"<<endl;
                        switchTurn = false;
                        return switchTurn;
                    }
                }
            }
            else if(destinationRow - currentRow > 1 || destinationRow - currentRow < -1){ //if player jumps over a tile vertically
                char control;
                if(playersChecker == 'x'){ //if player is white
                    if(destinationRow - currentRow > 1){ //to down (player white)
                        outfile.seekg((row*(destinationRow-1)) + destinationColumn, ios::beg); //checks the tile that is jumped over
                        outfile>>control;
                        if(control == 'y' || control=='b'){ //and the opponent is standing in that tile
                            outfile.seekg((row*(destinationRow-1)) + destinationColumn, ios::beg);
                            outfile<<'-'; //removes the opponent
                            switchTurn = false;
                        }else if(control == 'w' || control == 'x'){
                            cout<<"This move is not allowed"<<endl;
                            switchTurn = false;
                            return switchTurn;
                        }
                    }else if(destinationRow - currentRow < -1){ //to up (player white)
                        outfile.seekg((row*(destinationRow+1)) + destinationColumn, ios::beg); //checks the tile that is jumped over
                        outfile>>control;
                        if(control == 'y' || control=='b'){ //and the opponent is standing in that tile
                            outfile.seekg((row*(destinationRow+1)) + destinationColumn, ios::beg);
                            outfile<<'-'; //remove the opponent
                            switchTurn = false;
                        }else if(control == 'w' || control == 'x'){
                            cout<<"This move is not allowed"<<endl;
                            switchTurn = false;
                            return switchTurn;
                        }
                    }
                }else if(playersChecker == 'y'){ //if player is black
                    if(destinationRow - currentRow > 1){ //to down (player black)
                        outfile.seekg((row*(destinationRow-1)) + destinationColumn, ios::beg); //checks the tile that is jumped over
                        outfile>>control;
                        if(control == 'x' || control=='w'){ //and the opponent is standing in that tile
                            outfile.seekg((row*(destinationRow-1)) + destinationColumn, ios::beg);
                            outfile<<'-'; //removes the opponent
                            switchTurn = false;
                        }else if(control == 'b' || control == 'y'){
                            cout<<"This move is not allowed"<<endl;
                            switchTurn = false;
                            return switchTurn;
                        }
                    }else if(destinationRow - currentRow < -1){ //to up (player black)
                        outfile.seekg((row*(destinationRow+1)) + destinationColumn, ios::beg); //checks the tile that is jumped over
                        outfile>>control;
                        if(control == 'x' || control=='w'){ //and the opponent is standing in that tile
                            outfile.seekg((row*(destinationRow+1)) + destinationColumn, ios::beg);
                            outfile<<'-'; //remove the opponent
                            switchTurn = false;
                        }else if(control == 'b' || control == 'y'){
                            cout<<"This move is not allowed"<<endl;
                            switchTurn = false;
                            return switchTurn;
                        }
                    }

                }
            }
        }
        outfile.seekp((row*currentRow) + currentColumn, ios::beg);
        outfile<<"-";
        outfile.seekp((row*destinationRow) + destinationColumn, ios::beg);
        outfile<<current;
        outfile.close();
        return switchTurn;
    }



    //decodes given character for its use in following file operations
    int letterToNumber(char letter){
        switch(letter){
        case 'A':
        case 'a':
            return 1;
        case 'B':
        case 'b':
            return 2;
        case 'C':
        case 'c':
            return 3;
        case 'D':
        case 'd':
            return 4;
        case 'E':
        case 'e':
            return 5;
        case 'F':
        case 'f':
            return 6;
        case 'G':
        case 'g':
            return 7;
        case 'H':
        case 'h':
            return 8;
        }
    }

    //since the rows are numbered upside down, this function formats rows for other functions
    int formatRows(int row){
        switch(row){
        case 8:
            return 1;
        case 7:
            return 2;
        case 6:
            return 3;
        case 5:
            return 4;
        case 4:
            return 5;
        case 3:
            return 6;
        case 2:
            return 7;
        case 1:
            return 8;
        }
    }

};


int main () {
    cout<<"Welcome to Turkish Checkers Game"<<endl; //welcomes the player with menu
    cout<<"Select an option below"<<endl;
    cout<<"1. New Game"<<endl;
    cout<<"2. Continue Game"<<endl;
    cout<<"3. End Game"<<endl;

    int selectedOption;
    cin>>selectedOption;

    string hamle;
    string team;

    if(selectedOption == 1){ //if new game selected initializes files
        initializeBoardFromScratch();
        initializeMoves();
    }

    while(selectedOption != 3){

        determineCheckers();
        showCurrentBoard();

        cin>>hamle;
        Move m = Move(hamle);
        if(m.switchPlayerTurn){ //changes the turn if it is valid
            changeTurn();
        }

        string keepPlaying;
        cout<<"Do you want to continue playing? Enter n if you want to quit, or y otherwise."<<endl;
        cin>>keepPlaying;
        if(keepPlaying == "n"){
            selectedOption = 3;
        }
    }
    cout<<"I hope you enjoyed the game. Goodbye!"<<endl;
}

//checks for stamps which has evolved to checker
void determineCheckers(){
    string firstRow, secondRow;
    fstream infile;
    infile.open("index.dat");
    getline(infile, firstRow);
    replace(firstRow.begin(), firstRow.end(), 'b', 'y');
    infile.seekp(0, ios::beg);
    infile<<firstRow;
    infile.seekg((7*row), ios::beg);
    getline(infile, secondRow);
    replace(secondRow.begin(), secondRow.end(), 'w', 'x');
    infile.seekg(7*row, ios::beg);
    infile<<secondRow;
    infile.close();
}

//returns the character for current player
char currentPlayer(){
    fstream infile;
    infile.open("index.dat");
    infile.seekg(80);
    char turn;
    infile>>turn;
    return turn;
}


// truncates the books.dat
void initializeMoves(){
    fstream outfile;
    outfile.open("books.dat", ios::out | ios::trunc);
    outfile.close();
}

//saves every move players make in books.dat
void saveMove(char player, string m0ve){
    fstream outfile;
    outfile.open("books.dat", ios_base::app);
    outfile<<player<<": "<<m0ve<<endl;
}

//shifts the turn for next player and saves it on index.dat
void changeTurn(){
    fstream infile;
    string current;
    infile.open("index.dat");
    infile.seekg(80);
    char turn;
    infile>>turn;
    if(turn == 'w'){
        infile.seekg(80);
        infile<<"b";
    }else{
        infile.seekg(80);
        infile<<"w";
    }

}

//formats the stamps as described in the assignment and prints it to the console
void showCurrentBoard(){
    //x == white checker
    //y == black checker
   string row;
   ifstream infile;
   infile.open("index.dat");
   cout<<endl;
   int numbersOnTheSide = 8;
   for(int i = 0; i < 8; i++){
        getline (infile, row);
        cout<<numbersOnTheSide<<"|"<<"\t";
        replace(row.begin(), row.end(), '-', ' ');
        int j;
        for(j = 0; j < row.size(); j++){
            if(row[j] != 'a'){
                if(row[j] == 'x'){
                    cout<<"ww"<<"\t";
                }
                else if(row[j] == 'y'){
                    cout<<"bb"<<"\t";
                }else{
                    cout<<row[j]<<"\t";
                }
            }else{
                cout<<row[j]<<"\t";
            }

        }
        cout<<endl;
        numbersOnTheSide--;

   }
        cout<<"\tA\tB\tC\tD\tE\tF\tG\tH"<<endl;
        string team;
        getline (infile, row);
        if(row == "w"){
            team = "White";
        }else if(row == "b"){
            team = "Black";
        }
        cout<<"Team "<<team<<", please enter your command."<<endl;

   infile.close();
}

//Resets the board
void initializeBoardFromScratch(){
   ofstream outfile;
   outfile.open("index.dat");

   outfile<<"--------"<<endl;
   outfile<<"wwwwwwww"<<endl;
   outfile<<"wwwwwwww"<<endl;
   outfile<<"--------"<<endl;
   outfile<<"--------"<<endl;
   outfile<<"bbbbbbbb"<<endl;
   outfile<<"bbbbbbbb"<<endl;
   outfile<<"--------"<<endl;
   outfile<<"w";

   outfile.close();
}
