#include<iostream>
#include<conio.h>
#include<dos.h>		//Handles inturrupts, Produce sounds,
#include <windows.h>
#include <time.h>
#include <ctime>
#include <sstream>
#include <cstring>
#include <fstream>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std; 
 
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

// Basic Level Drawings and Variables

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char car[4][4] = { ' ','*','*',' ', 
					'<','|','|','>', 
					' ','|','|',' ',
					'<','|','|','>' }; 
					
int carPos = WIN_WIDTH/2;
int score = 0; 
int lives = 2;

// Prints the game name

void gameName(){
	SetConsoleTextAttribute(console, 10);
	cout << "\t=======   ===      ===     ====      ====   =======    ====         =           ====" << endl;
	cout << "\t=        =   =    =   =   =    =    =    =  =         =    =       ==          =    =" << endl;
	cout << "\t=       =     =  =       =      =  =     =  =        =     =      = =         =      =" << endl;
	cout << "\t=        =       =       =      =  =    =   =        =    =         =         =      =" << endl;
	cout << "\t=====     ===    =       ========  =====    =====    =====          =         =      =" << endl;
	cout << "\t=            =   =       =      =  =        =        ==             =         =      =" << endl;
	cout << "\t=             =  =       =      =  =        =        = =            =         =      =" << endl;
	cout << "\t=        =    =   =   =  =      =  =        =        =  =           =      ==  =    =" << endl;
	cout << "\t=======   ===      ===   =      =  =        =======  =   =       ========  ==   ====" <<endl;
}

// Prints the win message

void winMessage(){
	cout << "=     =    =    =   =    =     =   ===   === =   =" << endl;
	cout << " =   =    = =   =   =    =     =    =    = = =  = =" << endl;
	cout << "  = =    =   =  =   =    = === =    =    = = =  = =" << endl;
	cout << "   =     =   =  =   =    = = = =    =    = = =   =" << endl;
	cout << "   =     =   =  =   =    = = = =    =    = = =    " << endl;
	cout << "   =      = =   =   =    = = = =    =    = = =   =" << endl;
	cout << "   =       =     ===     === ===  =====  = ===  ===" << endl;
}

//	Prints the lost message

void lostMessage(){
	cout << "=     =    =    =   =    =        =     ===   =====   =" << endl;
	cout << " =   =    = =   =   =    =       = =   =   =    =    = =" << endl;
	cout << "  = =    =   =  =   =    =      =   =   =       =    = =" << endl;
	cout << "   =     =   =  =   =    =      =   =    =      =     = " << endl;
	cout << "   =     =   =  =   =    =      =   =     =     =    " << endl;
	cout << "   =      = =   =   =    =       = =   =   =    =     =" << endl;
	cout << "   =       =     ===     =====    =     ===     =    ===" << endl;
}

// Audio

void mainMenuAudio(int audioCode){
	mciSendString("open \"m1.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	if(audioCode==1){
		mciSendString("play mp3 repeat", NULL, 0, NULL);
	}else{
		mciSendString("close mp3", NULL, 0, NULL);
	}

}

void mainMenuClickAudio(int audioCode){
	mciSendString("open \"c2.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	if(audioCode==1){
		mciSendString("play mp3", NULL, 0, NULL);
	}else{
		mciSendString("close mp3", NULL, 0, NULL);
	}
}

void gameOverAudio(int audioCode){
	mciSendString("open \"go1.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	if(audioCode==1){
		mciSendString("play mp3", NULL, 0, NULL);
	}else{
		mciSendString("close mp3", NULL, 0, NULL);
	}

}

void gamePlayAudio(int audioCode){
	mciSendString("open \"g1.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	if(audioCode==1){
		mciSendString("play mp3 repeat", NULL, 0, NULL);
	}else{
		mciSendString("close mp3", NULL, 0, NULL);
	}

}

void playerHitAudio(int audioCode){
	mciSendString("open \"phit1.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	if(audioCode==1){
		mciSendString("play mp3", NULL, 0, NULL);
	}else{
		mciSendString("close mp3", NULL, 0, NULL);
	}

}
// Change the console's cursor position

void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

// Set the cursor according to the game

void setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;	
	
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}

// Draw the left and right borders

void drawBorder(){  
	for(int i=0; i<SCREEN_HEIGHT; i++){
		for(int j=0; j<17; j++){
			gotoxy(0+j,i); cout<<"|";
			gotoxy(WIN_WIDTH-j,i); cout<<"|";
		}
	} 
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(SCREEN_WIDTH,i); cout<<"|";
	} 
}

// Generate enemies

void genEnemy(int ind){
	enemyX[ind] = 17 + rand()%(33);  
}

// Draw the enemy

void drawEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]);   cout<<"****";  
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<" ** "; 
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"****"; 
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<" ** ";  
	} 
}

//	Draw the ememy's erased version

void eraseEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]); cout<<"    ";  
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"    "; 
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"    "; 
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<"    "; 
	} 
}

//	Powerup
/*
	This fuction is made for all functions.
	Pass the parameters as follows
	freezer = 'f'
	life = 'l' 
*/ 

int powerUp(char mode){
	if(mode == 'f'){
		//freezer();
		
	}else if(mode == 'l'){
		
		return 1;
	}else{
		return 0;
	}
}

//	Regernerate the destroyed enemy

void resetEnemy(int ind){
	eraseEnemy(ind);
	enemyY[ind] = 1;
	genEnemy(ind);
}

// Draw the player

void drawCar(){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			gotoxy(j+carPos, i+22); cout<<car[i][j];
		}
	}
}

// Erase the player

void eraseCar(){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			gotoxy(j+carPos, i+22); cout<<" ";
		}
	}
}

// Check the Collision
 
int collision(){
	if( enemyY[0]+4 >= 23 ){
		if( enemyX[0] + 4 - carPos >= 0 && enemyX[0] + 4 - carPos < 9  ){
			return 1;
		}
	}
	return 0;
}

//	String Multiplier for scoreboard setup

string operator * (string a, unsigned int b) {
    string output = "";
    while (b--) {
        output += a;
    }
    return output;
}

// String trim on right side

string& rightTrim(string& str, string& chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

//	Score board File writer

void scoreBoardWriter(string player, int scores){
	
	time_t currentTime;
 	struct tm *localTime;

 	time( &currentTime );                   // Get the current time
 	localTime = localtime( &currentTime );
	
	// Structure of scoreboard
	struct scoreBoard{
		
		//	Time variables
		int playYear;
		int playMonth;
		int playDate;
		int playHour;
		int playMinutes;
		int playSeconds;
		
		//	variables string version
		string playYear_str;
		string playMonth_str;
		string playDate_str;
		string playHour_str;
		string playMinutes_str;
		string playSeconds_str;
		
		string scores_str;
		
		// Finalized player data
		string playerName;
		string dateAndTime;
		int playerScores;
		
		string printData;
	};
	
	struct scoreBoard scB1;
	
	// Date and time
	scB1.playYear = 1900 + localTime->tm_year;
	scB1.playMonth = 1 + localTime->tm_mon;
	scB1.playDate = localTime->tm_mday;
	scB1.playHour =  localTime->tm_hour;
	scB1.playMinutes = localTime->tm_min;
	scB1.playSeconds = localTime->tm_min;
	
	//	Date and time string version
	stringstream ss1;	
	ss1 << scB1.playYear;	
	ss1 >> scB1.playYear_str; //cout << scB1.playYear_str;
	
	stringstream ss2;
	ss2 << scB1.playMonth;
	ss2 >> scB1.playMonth_str; //cout << scB1.playMonth_str;
	
	stringstream ss3;
	ss3 << scB1.playDate;
	ss3 >> scB1.playDate_str;
	
	stringstream ss4;
	ss4 << scB1.playHour;
	ss4 >> scB1.playHour_str;
	
	stringstream ss5;
	ss5 << scB1.playMinutes;
	ss5 >> scB1.playMinutes_str;
	
	stringstream ss6;
	ss6 << scB1.playSeconds;
	ss6 >> scB1.playSeconds_str;
	
	scB1.playerName = player;
	scB1.playerScores = scores;	
	scB1.dateAndTime = scB1.playYear_str + "-" + scB1.playMonth_str + "-" + scB1.playDate_str + " at " + scB1.playHour_str + ":" + scB1.playMinutes_str + ":" + scB1.playSeconds_str;

	stringstream ss7;
	ss7 << scB1.playerScores;
	ss7 >> scB1.scores_str;

	// Adding additional spaces and reducing the space
	int nameLength = scB1.playerName.length();
	const int MAX_NAME_LENGTH = 15;
	char playerNameArr[nameLength];
	const string appender = " ";
	int lengthDiff;
	
	if(nameLength < MAX_NAME_LENGTH){
		
		lengthDiff = MAX_NAME_LENGTH - nameLength;
		scB1.playerName += (appender*(lengthDiff+3));
		
	}else if(nameLength > MAX_NAME_LENGTH){
		
		lengthDiff = nameLength - MAX_NAME_LENGTH;
		for(int i = lengthDiff; i > 0; i--){
			
			scB1.playerName = scB1.playerName.erase(scB1.playerName.size()-1, 1);
		}
		scB1.playerName += "   ";
	}else{
		scB1.playerName += "   ";
	}
	
	scB1.printData = " " + scB1.playerName + scB1.dateAndTime + "        " + scB1.scores_str + "\n";
	//cout << scB1.printData;
	
	// Copy scores to the table
	fstream f;
    ofstream fout;
    ifstream fin;
    fin.open("Score_Table.txt");
    fout.open ("Score_Table.txt",ios::app);
    if(fin.is_open())
      fout<<scB1.printData;
    fin.close();
    fout.close();
}

// Print the highScore table by reading the score_table file

void scoreBoardReader(){
	mainMenuClickAudio(2);
	mainMenuAudio(1);
	system("cls");
	
	FILE* ptr;
    char ch;
 
    // Opening file in reading mode
    ptr = fopen("Score_Table.txt", "r");
 
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }
     // Printing what is written in file
    // character by character using loop.
    do {
        ch = fgetc(ptr);
        printf("%c", ch);
 
        // Checking if character is not EOF.
        // If it is EOF stop eading.
    } while (ch != EOF);
 
    // Closing the file
    fclose(ptr);
	
    cout<<endl;
    gameName();
    
    cout << "Press any key to return to the main menu";
	getch();
    
}

// Game over message
 
void gameover(int scores){
	system("cls");
	lostMessage();
	string playerName;
	cout << "Your Name : ";
	cin >> playerName;
	scoreBoardWriter(playerName, scores);
	cout<<endl;
	cout<<"\t\tPress any key to go back to menu.";
	getch();
}

// You win message

void win(int scores){
	system("cls");
	string playerName;
	cout << "Your Name : ";
	cin >> playerName;
	scoreBoardWriter(playerName, scores);
	cout<<endl;
	winMessage();
	cout<<"\t\tPress any key to go back to menu.";
	getch();
}

// Update the score

void updateScore(){
	gotoxy(WIN_WIDTH + 7, 5);cout<<"Score: "<<score<<endl;
}

// Show the instructions

void instructions(){
	mainMenuClickAudio(2);
	mainMenuAudio(1);
	system("cls");
	gameName();
	gotoxy(15,11);cout<<"\tInstructions";
	gotoxy(15,12);cout<<"\t------------";
	gotoxy(15,13);cout<<"\t #Avoid Cars by moving left or right. ";
	gotoxy(15,14);cout<<"\t #Press 'a' to move left";
	gotoxy(15,15);cout<<"\t #Press 'd' to move right";
	gotoxy(15,16);cout<<"\t #Press 'escape' to exit";
	gotoxy(15,17);cout<<"\t #Press any key to go back to menu";
	getch();
}

// Gameplay environment

//	Level 3 -> Both enemies are +1 faster than level 1

int level_3(int level_2_scores){
	carPos = -1 + WIN_WIDTH/2;
	score = level_2_scores;
	
	enemyFlag[0] = 1;
	enemyFlag[1] = 0;
	enemyY[0] = enemyY[1] = 1;
	  
	system("cls"); 
	drawBorder(); 
	updateScore();
	genEnemy(0);
	genEnemy(1);
	
	gotoxy(WIN_WIDTH + 7, 2);cout<<"Level 03" << "\n";
	gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
	gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";
	gotoxy(WIN_WIDTH + 7, 12);cout<<"Control ";
	gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
	gotoxy(WIN_WIDTH + 2, 14);cout<<" A Key - Left";
	gotoxy(WIN_WIDTH + 2, 15);cout<<" D Key - Right"; 
	
	gotoxy(18, 5);cout<<"Press any key to start";
	getch();
	gotoxy(18, 5);cout<<"                      ";
	
	while(1){
		
		if(score >= 400){
			system("cls");
			win(score);
			
			system("cls");
			break;
			
		}
		
		if(kbhit()){
			char ch = getch();
			if( ch=='a' || ch=='A' ){
				if( carPos > 18 )
					carPos -= 5;
			}
			if( ch=='d' || ch=='D' ){
				if( carPos < 50 )
					carPos += 5;
			} 
			if(ch==27){ // Escape key
				break;
			}
		} 
		
		drawCar(); 
		drawEnemy(0); 
		drawEnemy(1); 
		if( collision() == 1  ){
			gamePlayAudio(2);
			playerHitAudio(1);
			Sleep(500);
			lives--;
			if(lives <= 0){
				playerHitAudio(2);
				gamePlayAudio(2);
				gameOverAudio(1);
				Sleep(500);
				gameover(score);
			return score;
			}
		} 
		Sleep(50);
		eraseCar();
		eraseEnemy(0);
		eraseEnemy(1);   
		
		if( enemyY[0] == 10 )
			if( enemyFlag[1] == 0 )
				enemyFlag[1] = 1;
		
		if( enemyFlag[0] == 1 )
			enemyY[0] += 1;
		
		if( enemyFlag[1] == 1 )
			enemyY[1] += 2;
		 
		if( enemyY[0] > SCREEN_HEIGHT-4 ){
			resetEnemy(0);
			score++;
			updateScore();
		}
		if( enemyY[1] > SCREEN_HEIGHT-4 ){
			resetEnemy(1);
			score+=2;
			updateScore();
		}
	}
}

// Level 2 - 0ne enemy is +1 faster than hte other

int level_2(int level_1_scores){
	
	carPos = -1 + WIN_WIDTH/2;
	score = level_1_scores;
	
	enemyFlag[0] = 1;
	enemyFlag[1] = 0;
	enemyY[0] = enemyY[1] = 1;
	  
	system("cls"); 
	drawBorder(); 
	updateScore();
	genEnemy(0);
	genEnemy(1);
	
	gotoxy(WIN_WIDTH + 7, 2);cout<<"Level 02" << "\n";
	gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
	gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";
	gotoxy(WIN_WIDTH + 7, 12);cout<<"Control ";
	gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
	gotoxy(WIN_WIDTH + 2, 14);cout<<" A Key - Left";
	gotoxy(WIN_WIDTH + 2, 15);cout<<" D Key - Right"; 
	
	gotoxy(18, 5);cout<<"Press any key to start";
	getch();
	gotoxy(18, 5);cout<<"                      ";
	
	while(1){
		
		if(score >= 150){
			system("cls");
			level_3(score);
			system("cls");
			break;
			
		}
		
		if(kbhit()){
			char ch = getch();
			if( ch=='a' || ch=='A' ){
				if( carPos > 18 )
					carPos -= 5;
			}
			if( ch=='d' || ch=='D' ){
				if( carPos < 50 )
					carPos += 5;
			} 
			if(ch==27){ // Escape key
				break;
			}
		} 
		
		drawCar(); 
		drawEnemy(0); 
		drawEnemy(1); 
		if( collision() == 1  ){
			gamePlayAudio(2);
			playerHitAudio(1);
			Sleep(500);
			lives--;
			if(lives <= 0){
				playerHitAudio(2);
				gamePlayAudio(2);
				gameOverAudio(1);
				Sleep(500);
				gameover(score);
			return score;
			}
		} 
		Sleep(50);
		eraseCar();
		eraseEnemy(0);
		eraseEnemy(1);   
		
		if( enemyY[0] == 10 )
			if( enemyFlag[1] == 0 )
				enemyFlag[1] = 1;
		
		if( enemyFlag[0] == 1 )
			enemyY[0] += 1;
		
		if( enemyFlag[1] == 1 )
			enemyY[1] += 2;
		 
		if( enemyY[0] > SCREEN_HEIGHT-4 ){
			resetEnemy(0);
			score++;
			updateScore();
		}
		if( enemyY[1] > SCREEN_HEIGHT-4 ){
			resetEnemy(1);
			score+=2;
			updateScore();
		}
	}
}


// Level 1

int level_1(){
	carPos = -1 + WIN_WIDTH/2;
	score = 0;
	
	enemyFlag[0] = 1;
	enemyFlag[1] = 0;
	enemyY[0] = enemyY[1] = 1;
	  
	system("cls"); 
	drawBorder(); 
	updateScore();
	genEnemy(0);
	genEnemy(1);
	
	gotoxy(WIN_WIDTH + 7, 2);cout<<"Level 01" << "\n";
	gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
	gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";
	gotoxy(WIN_WIDTH + 7, 12);cout<<"Control ";
	gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
	gotoxy(WIN_WIDTH + 2, 14);cout<<" A Key - Left";
	gotoxy(WIN_WIDTH + 2, 15);cout<<" D Key - Right"; 
	
	gotoxy(18, 5);cout<<"Press any key to start";
	
	if(getch()){
		gotoxy(18, 5);cout<<"                      ";
		while(1){
			
			if(score >= 50){
				system("cls");
				level_2(score);
				system("cls");
				break;
				
			}
			
			if(kbhit()){
				char ch = getch();
				if( ch=='a' || ch=='A' ){
					if( carPos > 18 )
						carPos -= 4;
				}
				if( ch=='d' || ch=='D' ){
					if( carPos < 50 )
						carPos += 4;
				} 
				if(ch==27){ // Escape key
					break;
				}
			} 
			
			drawCar(); 
			drawEnemy(0); 
			drawEnemy(1); 
			if( collision() == 1  ){
				gamePlayAudio(2);
				playerHitAudio(1);
				Sleep(500);
				lives--;
				if(lives <= 0){
					playerHitAudio(2);
					gamePlayAudio(2);
					gameOverAudio(1);
					Sleep(500);
					gameover(score);
				return score;
				}
			} 
			Sleep(50);
			eraseCar();
			eraseEnemy(0);
			eraseEnemy(1);   
			
			if( enemyY[0] == 10 )
				if( enemyFlag[1] == 0 )
					enemyFlag[1] = 1;
			
			if( enemyFlag[0] == 1 )
				enemyY[0] += 1;
			
			if( enemyFlag[1] == 1 )
				enemyY[1] += 1;
			 
			if( enemyY[0] > SCREEN_HEIGHT-4 ){
				resetEnemy(0);
				score++;
				updateScore();
			}
			if( enemyY[1] > SCREEN_HEIGHT-4 ){
				resetEnemy(1);
				score++;
				updateScore();
			}
		}
	}
	
}

//	Menu border

void menuBorder(){
	for(int i=0; i<51; i++){
		
		if(i==0 || i==1)
			cout << " ";
		else
			cout << "=";
	}
	cout << "\n";
	
	for(int i=0; i<20; i++){
		cout << "||                                                 ||\n";
	}
	
		for(int i=0; i<51; i++){
			if(i==0 || i==1)
				cout << " ";
			else
				cout << "=";
	}
	cout << "\n";
}

// Loading Prograssbar

void prograssBar(string message){
	system("cls");
	gameName();
	menuBorder();
	gotoxy(WIN_WIDTH/2, SCREEN_HEIGHT/2);
	srand(time(0));
    int x = 0;

    for(int i = 0; i<100; i++){
        int r = rand() % 1000;
        x++;
        cout << "\r" << "||                  " << x << "% Please Wait..." << flush;
        if(i < 43){
           Sleep(r/10);
        }else if(i > 43 && i < 74){
           Sleep(r/20);
        }else if(i < 98){
           Sleep(r/5);
        }else if(i > 97 && i != 99){
           Sleep(2000);
        }
    }

    cout << endl << endl << "||                  " << message << "\n" << flush;
    Sleep(1000);
}

//	Main function

int main()
{
	setcursor(0,0); 
	srand( (unsigned)time(NULL));
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd,SW_SHOWMAXIMIZED);  
	do{
		system("cls");
		gameName();
		mainMenuAudio(1);
		gotoxy(15,11); cout<<"1. Start Game";
		gotoxy(15,12); cout<<"2. Instructions";	 
		gotoxy(15,13); cout<<"3. High Scores";
		gotoxy(15,14); cout<<"4. Quit";
		gotoxy(15,16); cout<<"Select option: ";
		char op = getche();
		
		if( op=='1') {
			mainMenuAudio(2);
			mainMenuClickAudio(1);
			Sleep(500);
			mainMenuClickAudio(2);
			prograssBar("Let\'s Play Level 1");
			gamePlayAudio(1);
			Sleep(200);
			level_1();
		}
		else if( op=='2'){
			mainMenuAudio(2);
			mainMenuClickAudio(1);
			Sleep(500);
			instructions();
		} 
		else if( op=='3'){
			mainMenuAudio(2);
			mainMenuClickAudio(1);
			Sleep(500);
			scoreBoardReader();
		}
		else if( op == '4'){
			mainMenuAudio(2);
			mainMenuClickAudio(1);
			Sleep(500);
			exit(0);
		}
		
	}while(1);
	
	return 0;
}


