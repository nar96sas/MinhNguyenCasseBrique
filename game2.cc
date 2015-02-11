#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h> // utiliser dans operation windows
#include <conio.h>
#include <ctime>

using namespace std;

/******************************************************************************/

#define    KEY_LEFT    VK_LEFT
#define    KEY_RIGHT   VK_RIGHT
#define    KEY_ESC     VK_ESCAPE
#define    KEY_P    0x50
#define    KEY_R    0x52
const int H = 20;
const int L = 50;

char screen[H][L];
int status[H][L];
double x = H - 7, y = L/2;
bool movex,movey;
int l = 20 , r = 30 , vitesse_barre = 3 ;
char a = 179 /* width*/ , b = 196 /*height*/ , 
	 tl = 218 /*top-left*/,bl = 192/*bot-left*/,tr = 191/*top-right*/,br = 217/*bot-right*/; 
int brique_1 = 3, // une fois
	brique_2 = 4, // deux fois
	brique_3 = 1, // + live
	brique_4 = 2,brique_5 = -1, // +- vitesse barre
	brique_6 = -2,brique_7 = -3, //+- longuer barre
	brique_8 = -4, // random
	brique_9 = -5, // le brique qui ne peut pas detruit
			  			
bool checkKey(int key)
{
    return GetAsyncKeyState(key);
}

void gotoxy(int x,int y)
{
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = {x-1,y-1};
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput , Cursor_an_Pos);
}

void SetColor(WORD color)
{
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
 
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
 
    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    wAttributes &= 0xfff0;
    wAttributes |= color;
 
    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
/******************************************************************************/
void titre_jeux()
{
	SetColor(8);
	gotoxy(22,3); cout <<" 0000 " << "   000    " <<" 00000" <<" 00000 " <<"00000 "; 
	gotoxy(22,4); cout <<"00    " << "  00 00   " <<"00    " <<"00     " <<"00    ";
	gotoxy(22,5); cout <<"00    " << " 00   00  " <<" 0000 " <<" 0000  " <<"0000  ";
	gotoxy(22,6); cout <<"00    " << "000000000 " <<"    00" <<"    00 " <<"00    ";
	gotoxy(22,7); cout <<" 0000 " << "00     00 " <<"00000 " <<"00000  " <<"00000 ";

	gotoxy(20,9);  cout <<"00000  " << "00000   " << "00 " << " 00000   " << "00   00 " << "00000 ";
	gotoxy(20,10); cout <<"00  00 " << "00  00  " << "00 " << "00   00  " << "00   00 " << "00    ";
	gotoxy(20,11); cout <<"00000  " << "00000   " << "00 " << "00  0 0  " << "00   00 " << "0000  ";
	gotoxy(20,12); cout <<"00  00 " << "00  00  " << "00 " << "00   00  " << "00   00 " << "00    ";
	gotoxy(20,13); cout <<"00000  " << "00   00 " << "00 " << " 0000  0 " << " 00000  " << "00000 ";

	SetColor(7);
}
/******************************************************************************/
void screen_display() { //afficher les objets du jeux
	for (int i=0; i<H; i++) 
	{
		cout << "   " ;
		for (int j=0; j<L; j++)
			cout << screen[i][j]; 
		cout << endl;
	}
}
/******************************************************************************/
void wall() {    //La rectangle
	for (int i=0; i<H; i++)
    	for (int j=0; j<L; j++)
		{
      		if (i == 0)
			{
				if (j == 0) screen[i][j] = 218;
				else 
					if (j == L-1) screen[i][j] = 191;
					else screen[i][j] = 196;
      		}
			else 
				if (j == 0 || j == L-1) screen[i][j] = 179;
      			else screen[i][j] = ' ';
    	}
}

/******************************************************************************/
void detruit_brique(double x, double y,int &live,int &point)
{
	int hasard;
	srand(time(NULL));
	
	if (status[(int)x][(int)y] == status[(int)x][(int)y+1])
		if (status[(int)x][(int)y+1] == status[(int)x][(int)y+2])
		{
			if (screen[(int)x][(int)y] == (char)176)
			{
				status[(int)x][(int)y] += 3;
				status[(int)x][(int)y+1] += 3;
				status[(int)x][(int)y+2] += 3;
				point += 50;
			}
			else
			{
				if (screen[(int)x][(int)y] == (char)3) live++;				// 
				if (screen[(int)x][(int)y] == (char)175) vitesse_barre += 2;//
				if (screen[(int)x][(int)y] == (char)174) vitesse_barre -= 2;//
				if (screen[(int)x][(int)y] == (char)43) r += 2;				//
				if (screen[(int)x][(int)y] == (char)45) r -= 2;	  			//
				if (screen[(int)x][(int)y] == (char)63)						//   effet des 
					{														// special briques
						hasard = rand()% + 7;								//
						switch (hasard)										//
						{													//
							case 1 : vitesse_barre += 2; break;				//
							case 2 : vitesse_barre -= 2; break;				//
							case 4 : r += 2; break;							//
							case 5 : r -= 2; break;							//
							case 6 : point += 200; break;					//
							default : live++; break;						//
						}													//
					}														//
				if (screen[(int)x][(int)y] != (char)219)
				{
 					status[(int)x][(int)y] = 0;
					status[(int)x][(int)y+1] = 0;
					status[(int)x][(int)y+2] = 0;
				}
				if (screen[(int)x][(int)y] == (char)177) point += 100;;
			}
		}
		else detruit_brique(x,y-1,live,point);
	else detruit_brique(x,y-2,live,point); 
}
/******************************************************************************/
void afficher_points_niveau(int &point,int &level)
{
	SetColor(12); cout << " | POINTS : " << point;
	SetColor(5); cout << " | NIVEAU : " << level;
	SetColor(7);
}
/******************************************************************************/
void lives(int &live)
{
	SetColor(14); cout << "\n   LIVES : " << live;
	if (x > H+1) 
  	{
	  	live--;
	  	x = H - 7; y = L/2;
	  	l = 20; r = 30;
	  	vitesse_barre = 3;
	}
	SetColor(7);
}
/******************************************************************************/
bool special_brique(double &x , double &y)
{
	switch (screen[(int)x][(int)y])
	{
		case (char)3   :
		case (char)175 : 
		case (char)177 : 
		case (char)176 : 
		case (char)174 :
		case (char)43  : 
		case (char)45  :
		case (char)63  :
		case (char)219 :
			return true; 
			break;
		default : return false; break;
	}
}

/******************************************************************************/
void ball(bool &end,int &live,int &point)   //Le mouvement du ball
{     
	bool impact;
	
	screen[(int)x][(int)y] = 'o'; 

	if (movex)
	{
		x--;
		impact = special_brique(x,y);
    	if (x == 0 || impact) 
    	{
    		detruit_brique(x,y,live,point);
			movex = false;
		}
	}
  	if (!movex) 
	{
		x++;
		impact = special_brique(x,y);
		if ((x == H-3) && (screen[(int)x][(int)y] == (char)205) || impact)
		{
			detruit_brique(x,y,live,point);
	  		movex = true;
	  	}
	}
  
  	if (movey)
	{
    	y++;
    	impact = special_brique(x,y);
	    if (y == L-1 || impact)
		{
	    	detruit_brique(x,y,live,point);
		  	movey = false;
		}
  	}
  
  	if (!movey) 
	{
	    y--;
	    impact = special_brique(x,y);
	    if (y == 1 || impact)   
		{
	    	detruit_brique(x,y,live,point);
		  	movey = true;
		}
  	}
}
	
/******************************************************************************/
void barre()  // Creer un barre et son mouvement
{
	for (int j = l ; j <= r ; j++)
		screen[H-3][j] = 205;
		
	if (checkKey(KEY_LEFT)  && (l > 1)){	
    	l -= vitesse_barre; r -= vitesse_barre;
    	if (l < vitesse_barre) {
			r = r-l+1; l = 1; }
		}
  	if (checkKey(KEY_RIGHT) && (r < L-2)){
		l += vitesse_barre; r += vitesse_barre;
		if (r > L-2) {
			l = L-2-(r-l); r = L-2; }
  		}
}
/******************************************************************************/
void tailleBrique(int x, int y , int type_brique)
{
		status[x][y]   = type_brique;
		status[x][y+1] = type_brique;
		status[x][y+2] = type_brique;
}
/************************---------niveau-------*******************************/
void niveau_1()
{
	for (int i = 4 ; i < H-10 ; i++)
		for (int j = 5 ; j < L-6; j=j+3)
		{
			if (i == 4 || i == H-11 || j == 5 || j == L-7 || j == 6 || j == 7 || j == L-8 || j == L-9)
			{
				tailleBrique(i,j,brique_2);
				brique_2 += 2;
			}
			else
			{	
				tailleBrique(i,j,brique_1);
				brique_1 += 2;		
			}
			if (i == 7 && j == 23) tailleBrique(i,j,brique_3);	
			if (i == 5 && j == 8) tailleBrique(i,j,brique_5);
			if (i == 5 && j == L-12) tailleBrique(i,j,brique_4);
			if (i == 8 && j == 8) tailleBrique(i,j,brique_7);
			if (i == 8 && j == L-12) tailleBrique(i,j,brique_6);
		}
}

void niveau_2()
{
	for (int i = 4 ; i < H-10 ; i++)
		for (int j = 5 ; j < L-6; j=j+3)
		{
			if ((i == 4 || i == H-11) && j > 14 && j < 32 || (i > 4 && i < H-11) && (j == 5 || j == L-9))
			{
				tailleBrique(i,j,brique_2);
				brique_2 += 2;
			}
			else
			{	
				tailleBrique(i,j,brique_1);
				brique_1 += 2;		
			}	
			if (i == 6 && (j == 5 || j == L-9)) tailleBrique(i,j,brique_8);
			if (i == 6 && j == 23) tailleBrique(i,j,brique_7);
			if (i == 6 && j == 26) tailleBrique(i,j,brique_6);
			if ((i == 4 || i == H-11) && (j <= 14 || j >=32)) tailleBrique(i,j,brique_9);
		}
}
		
void niveau_3()
{			
	for (int i = 4 ; i < H-10 ; i++)
		for (int j = 10 ; j < L-10; j=j+3)
			if (i == 4 || j == 10 || (i == H-11 && (j < 20 || j > 26)) || j == L-13)
			{
				tailleBrique(i,j,brique_2);
				brique_2 += 2;
			}
			
	for (int i = 5 ; i < H-11 ; i++)
		for (int j = 13 ; j < L-13; j=j+3)
		{
			if (i == 5 || j == 13 || (i == H-12 && (j < 20 || j > 26)) || j == L-16)
			{
				tailleBrique(i,j,brique_1);
				brique_1 += 2;
			}
			
			if (i == 6 && j == 13) tailleBrique(i,j,brique_7);
			if (i == 6 && j == L-16) tailleBrique(i,j,brique_6);
		}
}

void niveau_4()
{
	for (int j = 19 ; j < L-19; j=j+3)
	{
		tailleBrique(3,j,brique_1);
		brique_1 += 2;
	}
	
	for (int i = 4 ; i < H-11 ; i++)
		for (int j = 16 ; j < L-16; j=j+3)
		{	
			tailleBrique(i,j,brique_1);
			brique_1 += 2;
			if (i == 6 && j == 19) tailleBrique(i,j,brique_7);
			if (i == 6 && j == L-22) tailleBrique(i,j,brique_6);
		}
		
	for (int i = H-11 ; i < H-9 ; i++)
		for (int j = 19 ; j < L-19 ; j=j+3)
			if (i == H-10 && j > 19 && j < L-22)
			{
				tailleBrique(i,j,0);
				brique_1 += 2;
			}
			else 
			{	
				tailleBrique(i,j,brique_1);
				brique_1 += 2;
			}
	
	for (int i = H-9 ; i < H-6 ; i++)
		for (int j = 13 ; j < L-13 ; j=j+3)
			if ((i == H-9 || i == H-7) && (j == 13 || j == L-16))
			{		
				tailleBrique(i,j,brique_9);
				brique_1 += 2;
			}
		
	for (int j = 16 ; j < L-16 ; j=j+3)
	{	
		tailleBrique(H-8,j,brique_9);
		brique_1 += 2;
	}
}

void niveau_5()
{
	for (int i = 1 ; i < 13 ; i++)
		for (int j = (int)L/3 ; j < L-18 ; j=j+3)
		{	
			tailleBrique(i,j,brique_1);
			brique_1 += 2;
		}
		
	for (int i = 5 ; i < 9 ; i++)
		for (int j = 1 ; j < L-2 ; j=j+3)
		{
			if ((i == 5 || i == 8) && ((j > 3 && j < 13) || (j > L-15 && j < L-5)))
			{
				tailleBrique(i,j,brique_9);
				brique_1 += 2;
			}
			else
			{	
				tailleBrique(i,j,brique_1);
				brique_1 += 2;
			}
			if ((i == 6 || i == 7) && ((j > 6 && j < 10) || (j > L-12 && j < L-8))) tailleBrique(i,j,brique_8);
			if (i == 7 && (j == (int)L/2-3 || j == (int)L/2)) tailleBrique(i,j,brique_3);	
		}
}
/******************************************************************************/
void creerniveau(int &level)
{
	switch (level)
	{
		case 1 : niveau_1(); break;
		case 2 : niveau_2(); break;
		case 3 : niveau_3(); break;
		case 4 : niveau_4(); break;
		case 5 : niveau_5(); break;
	}
}
/******************************************************************************/
void brique()
{	
	for (int i = 1 ; i < H-3 ; i++)
		for (int j = 1 ; j < L-1 ; j++)
			switch (status[i][j])
			{
				case 0  : screen[i][j] = ' '; break;
				case 1  : screen[i][j] = 3; break;
				case 2  : screen[i][j] = 175; break;
				case -1 : screen[i][j] = 174; break;
				case -2 : screen[i][j] = '+'; break;
				case -3 : screen[i][j] = '-'; break;
				case -4 : screen[i][j] = '?'; break;
				case -5 : screen[i][j] = 219; break; 
				default :
					if (status[i][j] % 2 == 0)
						screen[i][j] = 176;
					else screen[i][j] = 177; 
				break;
			} 
}
/******************************************************************************/
bool finir(int live)
{
	bool check = false;
	
	for (int i = 1 ; (i < H-4) && (!check) ; i++)
		for (int j = 1 ; (j < L-2) && (!check) ; j++)
			if (screen[i][j] == ' ' || screen[i][j] == (char)219)
				check = false;
			else check = true;

	if (live < 0) return true;
			
	if (check) return false;
	else return true;
}
/******************************************************************************/
void lose()
{
	cout <<"\n \n"; 
	cout <<"\t \t "<<tl; for (int i=0 ; i < 15 ; i ++) cout << b; cout << tr << "\n";
	cout <<"\t \t "<<a; SetColor(12); cout << "   GAME OVER   ";  SetColor(7); cout << a << "\n"; 
	cout <<"\t \t "<<bl; for (int i=0 ; i < 15 ; i ++) cout << b; cout << br << "\n";		
}
/******************************************************************************/
void niveau_clear()
{
	cout <<"\n \n"; 
	cout <<"\t \t \t"<<tl; for (int i=0 ; i < 15 ; i ++) cout << b; cout << tr << "\n";
	cout <<"\t \t \t"<<a; SetColor(12); cout << " STAGE CLEAR ";  SetColor(7); cout << a << "\n"; 
	cout <<"\t \t \t"<<bl; for (int i=0 ; i < 15 ; i ++) cout << b; cout << br << "\n";
	
	// Refresh niveau
	for (int i = 1 ; i < H-3 ; i++)
		for (int j = 1 ; j < L-2; j++)
			status[i][j] = 0;
	// Refresh ball
	x = H - 7; y = L/2;	
}
/******************************************************************************/
void win()
{
	cout <<"\n \n"; 
	cout <<"\t \t \t"<<tl; for (int i=0 ; i < 10 ; i ++) cout << b; cout << tr << "\n";
	cout <<"\t \t \t"<<a; SetColor(12); cout << "    WIN   ";  SetColor(7); cout << a << "\n"; 
	cout <<"\t \t \t"<<bl; for (int i=0 ; i < 10 ; i ++) cout << b; cout << br << "\n";		
}
/******************************************************************************/
void boutons()
{
	cout << "\n -----------------------------------------------------";
	cout << "\n   Press 's' to start";
	cout << "\n   Press 'b' back to menu ";
}
/******************************************************************************/
void pause()
{
	bool end = true;
	
	cout << "\n -----------------------------------------------------";
	cout << "\n   Press 'p' to pause";
	cout << "\n   Press 'r' to resume";
	
	if (checkKey(KEY_P)) end = false;
	
	while(!end)
	{
		if (checkKey(KEY_R)) end = true;	
	}
}
/******************************************************************************/
void mainLoop(bool &end,int &live,int point,int &level)
{
	do {
 		system("cls");
 		screen_display();
 		lives(live);
 		afficher_points_niveau(point,level);
    	wall();
    	brique();
  		barre();
    	ball(end,live,point);
    	pause();
    	Sleep(50);
    	end = finir(live);
	} while (!end);
	
}
/******************************************************************************/
void menu();
void niveau(bool &end,int &live,int point,int &level)
{
	int hasard,key;
	
	system ("cls");
	creerniveau(level);
	wall();
	brique();
	barre();
	ball(end,live,point); 
	screen_display();
	lives(live);
	afficher_points_niveau(point,level);
	boutons();
	key = getch();
	if (key == 115) // Press "s" to start
	{ 
		hasard = rand() % 2;
		if (hasard == 1) {movex = false; movey = true;}
		else {movex = false; movey = false;}
		mainLoop(end,live,point,level); 
	}
	if (key == 98) 
	{
		system("cls");
		titre_jeux();
		menu();
	}
}
/******************************************************************************/
void menu() 
{
	int choix, live = 3 , point = 0 , level = 1 , key;
	bool end = false;
	
	gotoxy (27,15); cout << tl; for (int i = 0 ; i < 20 ; i++) cout << b; cout <<tr<< "\n";
	gotoxy (27,16); cout << a <<"      JOUER (1)     " << a <<"\n";
	gotoxy (27,17); cout << a <<"   INTRUCTIONS (2)  " << a <<"\n";
	gotoxy (27,18); cout << a <<"      SORTIR (3)    " << a <<"\n";
	gotoxy (27,19); cout << bl; for (int i = 0 ; i < 20 ; i++) cout << b; cout <<br<< "\n";
	gotoxy (21,20); cout << "Choose : ";
	cin >> choix;
	switch (choix)
	{
		case 1 :
			while (level < 6)
			{
				niveau(end,live,point,level);
				if (live < 0) 
				{
					lose();
					break;
				}
				else
				{
				 	niveau_clear();	
					level++;
				}
			}
			if (level == 5) win();
			break;
		case 2 : 
			system ("cls");
			cout << "\n \n _ Utiliser " << (char)26 << "," << (char)27 << " pour controle la barre de garder la balle en jeu. Ne paniquez pas ou vous allez tomber les boules et perdre. Autre brique speciale comprend des points supplementaires, plus grand, plus petit barre et bien plus encore. Essayez de le faire a travers autant de niveaux que vous le pouvez. \n";
			cout << "\n Il y a 9 types du brique \n";
			cout << "\t" << (char)177 << " : un brique normal \n";
			cout << "\t" << (char)176 << " : " << (char)177 << "x2 \n";
			cout << "\t" << (char)3   << " : +1 lives \n";
			cout << "\t" << (char)175 << " : + La vitesse du barre \n";
			cout << "\t" << (char)174 << " : - La vitesse du barre \n";
			cout << "\t" << "+++ : + La longuer du barre \n";
			cout << "\t" << "--- : - La longuer du barre \n";
			cout << "\t" << "??? : Chooser au hasard un effet \n";
			cout << "\t" << (char)219 << " : un brique qui ne peut pas detruit \n";
			cout << "-----------------------------------------------------------------------------------------";
			cout << "\n   Press 'b' back to menu ";
			key = getch();
			if (key == 98) 
			{	
				system("cls");
				titre_jeux();
				menu();
			}
		case 3 : break;
	}
}
/******************************************************************************/
int main() 
{
	srand(time(NULL));
	
	titre_jeux();
	menu();
}

/******************************************************************************/

