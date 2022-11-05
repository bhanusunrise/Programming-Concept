#include<iostream> //handelling the inputs and outputs
#include<conio.h> //clear screen, change text and background color, move text, keypress check
//clrscr, delline, getch, getche, gotoxy, kbhit, wherex, wherey, textcolor, textbackground
#include<dos.h>  //handle inturrupts, produce sounds, date and time functions like delay()
#include <windows.h> //To interact with the basic parts of the operating system such as basic APIs for the threads etc.
#include <time.h> //Manupulate the date and time information

#define SCREEN_WIDTH 90 	//Setting up the Screen width to 90 pixels
#define SCREEN_HEIGHT 26	//Setting up the the Screen Height to 26 pixels
#define WIN_WIDTH 70 	// Setting up the window width for 70 pixels

using namespace std; 	// using the standard namespace
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); 	//handle the console's output
COORD CursorPosition; 	//Assign the new curser positions of the character

int enemyY[3]; 		// enemy's y locaion
int enemyX[3]; 		// Enemy's x location
int enemyFlag[3];
char car[4][4] = { ' ','?','?',' ', 
					'?','?','?','?', 
					' ','?','?',' ',
					'?','?','?','?' }; 		// Car's basic structure
					
int carPos = WIN_WIDTH/2; 		// Car's Position
int score = 0; 		// Measure the score

void gotoxy(int x, int y){
	CursorPosition.X = x; 		// X axis's curser position
	CursorPosition.Y = y; 		// Y axis's curser position
	SetConsoleCursorPosition(console, CursorPosition); 		// Setting up the Console's Curser Position
}
void setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;	// Set curser size to 20 if it is o
	
	CONSOLE_CURSOR_INFO lpCursor;	//Curser Info objecy
	lpCursor.bVisible = visible; 	// Setup Curser Visibility
	lpCursor.dwSize = size;		//Setup Curser size
	SetConsoleCursorInfo(console,&lpCursor); 	//Change the console curser info
}
void drawBorder(){  
	for(int i=0; i<SCREEN_HEIGHT; i++){
		for(int j=0; j<17; j++){
			gotoxy(0+j,i); cout<<"?";	//Left Border
			gotoxy(WIN_WIDTH-j,i); cout<<"?";	//Right Border
		}
	} 
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(SCREEN_WIDTH,i); cout<<"?"; 		//Change the Border after the instructions
		
	} 
}
void genEnemy(int ind){
	enemyX[ind] = 17 + rand()%(33);  	//Enemy Generating Position
}
void drawEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]);   cout<<"****";  	//Draw the Enemy line by line -> Art
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<" ** "; 
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"****"; 
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<" ** ";  
	} 
}
void eraseEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]); cout<<"    ";  	//Destroy the enemy -> Art
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"    "; 
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"    "; 
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<"    "; 
	} 
}
void resetEnemy(int ind){
	eraseEnemy(ind);	//	Destroy the Enemy
	enemyY[ind] = 1;	//	Destroy Position
	genEnemy(ind);		//	Generate the enemy	
}

void drawCar(){
	for(int i=0; i<4; i++){		//4 rounds for rows
		for(int j=0; j<4; j++){		//4 rounds for coloumns
			gotoxy(j+carPos, i+22); 	//Setting up the position
			cout<<car[i][j];	//Draw the Car
		}
	}
}
void eraseCar(){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			gotoxy(j+carPos, i+22); cout<<" "; 		//Erase Car by printing spaces
		}
	}
}
 
int collision(){
	if( enemyY[0]+4 >= 23 ){	 //Lowest position on the screen that can be collaps with the enemy
		if( enemyX[0] + 4 - carPos >= 0 && enemyX[0] + 4 - carPos < 9  ){	//Chech the collition range of both enemy and player
			return 1;	//Return 1 if it is collapsed
		}
	}
	return 0;
} 
void gameover(){
	system("cls");	//Clear the Screen
	cout<<endl;
	cout<<"\t\t--------------------------"<<endl;		//Display the Message
	cout<<"\t\t-------- Game Over -------"<<endl;
	cout<<"\t\t--------------------------"<<endl<<endl;
	cout<<"\t\tPress any key to go back to menu.";
	getch();	//Take a single input from keyboard
}
void updateScore(){
	gotoxy(WIN_WIDTH + 7, 5);cout<<"Score: "<<score<<endl;	//Score Update
}

void instructions(){
	
	system("cls");	//Instructions
	cout<<"Instructions";
	cout<<"\n----------------";
	cout<<"\n Avoid Cars by moving left or right. ";
	cout<<"\n\n Press 'a' to move left";
	cout<<"\n Press 'd' to move right";
	cout<<"\n Press 'escape' to exit";
	cout<<"\n\nPress any key to go back to menu";
	getch();	//Take a single input from keyboard
}

void play(){
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
	
	gotoxy(WIN_WIDTH + 7, 2);cout<<"Car Game";
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
		if(kbhit()){ //kbhit() stands for keyboard hit
			char ch = getch();
			if( ch=='a' || ch=='A' ){
				if( carPos > 18 )
					carPos -= 4;	//Move Left
			}
			if( ch=='d' || ch=='D' ){
				if( carPos < 50 )
					carPos += 4;	//Move Right
			} 
			if(ch==27){
				break;
			}
		} 
		
		drawCar(); 
		drawEnemy(0); 
		drawEnemy(1); 
		if( collision() == 1  ){	//When Get a Collision run the gameover() function
			gameover();
			return;
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
			score++;	//when the enemy collapsed with Screen height-4 run the resetenmy() function. score will get increased by 1. Update the score by updateScore() function.
			updateScore();
		}
		if( enemyY[1] > SCREEN_HEIGHT-4 ){
			resetEnemy(1);
			score++;	//when the enemy collapsed with Screen height-4 run the resetenmy() function. score will get increased by 1. Update the score by updateScore() function.
			updateScore();
		}
	}
}

int main()
{
	setcursor(0,0); // Set cusser at the (0,0)
	srand( (unsigned)time(NULL)); //time(NULL) -> Current Time srand() generate a random number
	 
	do{
		system("cls"); // Clear the Screen
		gotoxy(10,5); cout<<" -------------------------- "; 
		gotoxy(10,6); cout<<" |        Car Game        | "; 
		gotoxy(10,7); cout<<" --------------------------";
		gotoxy(10,9); cout<<"1. Start Game";
		gotoxy(10,10); cout<<"2. Instructions";	 
		gotoxy(10,11); cout<<"3. Quit";
		gotoxy(10,13); cout<<"Select option: ";
		char op = getche();		//get a char value from user 
		
		if( op=='1') play();	//If value is 1, player can play the game
		else if( op=='2') instructions();	//If value is 2, show the instructions
		else if( op=='3') exit(0);	//If the value is 3, exit the program
		
	}while(1);
	
	return 0;
}
