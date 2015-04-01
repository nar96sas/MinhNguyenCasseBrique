/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <fcntl.h>
using namespace std;
/* retourne 0 si aucune touche n'est appuyée,
* sinon retourne le code ASCII de la touche */
int read_keybord();
/******************************************************************************/
#define H 20
#define L 50
char screen[H][L];
int status[H][L];
double x = H - 7, y = L/2;
bool movex,movey;
int l = 20 , r = 30 , vitesse_barre = 3 ;
int brique_1 = 3, // une fois
	brique_2 = 4, // deux fois
	brique_3 = 1, // + live
	brique_4 = 2,brique_5 = -1, // +- vitesse barre
	brique_6 = -2,brique_7 = -3, //+- longuer barre
	brique_8 = -4, // random
  brique_9 = -5; // le brique qui ne peut pas detruit
/******************************************************************************/
void titre_jeux()
{
  system ("clear");
  cout <<"\t" <<" 0000 " << "   000    " <<" 00000" <<" 00000 " <<"00000 " << endl; 
  cout <<"\t" <<"00    " << "  00 00   " <<"00    " <<"00     " <<"00    " << endl;
  cout <<"\t" <<"00    " << " 00   00  " <<" 0000 " <<" 0000  " <<"0000  " << endl;
  cout <<"\t" <<"00    " << "000000000 " <<"    00" <<"    00 " <<"00    " << endl;
  cout <<"\t" <<" 0000 " << "00     00 " <<"00000 " <<"00000  " <<"00000 " << endl << endl;
  
  cout <<"\t" <<"00000  " << "00000   " << "00 " << " 00000   " << "00   00 " << "00000 " << endl;
  cout <<"\t" <<"00  00 " << "00  00  " << "00 " << "00   00  " << "00   00 " << "00    " << endl;
  cout <<"\t" <<"00000  " << "00000   " << "00 " << "00  0 0  " << "00   00 " << "0000  " << endl;
  cout <<"\t" <<"00  00 " << "00  00  " << "00 " << "00   00  " << "00   00 " << "00    " << endl;
  cout <<"\t" <<"00000  " << "00   00 " << "00 " << " 0000  0 " << " 00000  " << "00000 " << endl;
       
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
void clear_screen() {
  for (int i=0; i<H; i++)
    for (int j=0; j<L; j++)
      screen[i][j] = ' ';
}
/******************************************************************************/
void wall() {    //La rectangle
	cout << endl << endl;
	for (int i=0; i<H; i++)
	  for (int j=0; j<L; j++)
	    if (i == 0 || j == 0 || j == L-1)
	      screen[i][j] = 'I';
	    else screen[i][j] = ' ';
}
/******************************************************************************/
bool special_brique(double &x , double &y)
{
	switch (screen[(int)x][(int)y])
	{
		case 'l'  :
		case '>' : 
		case '<' : 
		case '+' : 
		case '-' :
		case '*' : 
		case '?' :
		case '#' :
		case '~' :
			return true; 
			break;
		default : return false; break;
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
	if (screen[(int)x][(int)y] == '#')
	  {
	    status[(int)x][(int)y] += 3;
	    status[(int)x][(int)y+1] += 3;
	    status[(int)x][(int)y+2] += 3;
	    point += 50;
	  }
	else
	  {
	    if (screen[(int)x][(int)y] == 'l') live++;		       // 
	    if (screen[(int)x][(int)y] == '>') vitesse_barre += 2;     //
	    if (screen[(int)x][(int)y] == '<') vitesse_barre -= 2;     //
	    if (screen[(int)x][(int)y] == '+') r += 2;		       //
	    if (screen[(int)x][(int)y] == '-') r -= 2;	  	       //
	    if (screen[(int)x][(int)y] == '?')		               //   effet des 
	      {								// special briques
		hasard = rand()% + 7;					//
		switch (hasard)						//
		  {							//
		  case 1 : vitesse_barre += 2; break;			//
		  case 2 : vitesse_barre -= 2; break;			//
		  case 4 : r += 2; break;				//
		  case 5 : r -= 2; break;				//
		  case 6 : point += 200; break;				//
		  default : live++; break;			        //
		  }							//
	      }							        //
	    if (screen[(int)x][(int)y] != '~')
	      {
		status[(int)x][(int)y] = 0;
		status[(int)x][(int)y+1] = 0;
		status[(int)x][(int)y+2] = 0;
	      }
	    if (screen[(int)x][(int)y] == '*') point += 100;;
	  }
      }
    else detruit_brique(x,y-1,live,point);
  else detruit_brique(x,y-2,live,point); 
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
	    if ((x == H-3) && (screen[(int)x][(int)y] == '=') || impact)
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
void barre(int key)  // Creer un barre et son mouvement
{
  for (int j = l ; j <= r ; j++)
    screen[H-3][j] = '=';
  
  if (key=='a'  && (l > 1)){	
    l -= vitesse_barre; r -= vitesse_barre;
    if (l < vitesse_barre) {
      r = r-l+1; l = 1; }
  }
  if (key=='d' && (r < L-2)){
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
void brique()
{	
	for (int i = 1 ; i < H-3 ; i++)
		for (int j = 1 ; j < L-1 ; j++)
			switch (status[i][j])
			{
				case 0  : screen[i][j] = ' '; break;
				case 1  : screen[i][j] = 'l'; break;
				case 2  : screen[i][j] = '>'; break;
				case -1 : screen[i][j] = '<'; break;
				case -2 : screen[i][j] = '+'; break;
				case -3 : screen[i][j] = '-'; break;
				case -4 : screen[i][j] = '?'; break;
				case -5 : screen[i][j] = '~'; break; 
				default :
					if (status[i][j] % 2 == 0)
						screen[i][j] = '#';
					else screen[i][j] = '*'; 
				break;
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
void afficher_points_niveau(int &point,int &level)
{
	cout << " | POINTS : " << point;
	cout << " | NIVEAU : " << level;
}
/******************************************************************************/
void lose()
{
  cout <<"\n \n"; 
  cout << "~~~~~~GAME OVER~~~~~~" << endl;  	
}
/******************************************************************************/
void win()
{
  cout <<"\n \n"; 
  cout << "~~~~~~WIN~~~~~~" << endl;  
}
/******************************************************************************/
void niveau_clear()
{
	cout <<"\n \n"; 
	cout << " STAGE CLEAR " << endl; 

	// Refresh niveau
	for (int i = 1 ; i < H-3 ; i++)
		for (int j = 1 ; j < L-2; j++)
			status[i][j] = 0;
	// Refresh ball
	x = H - 7; y = L/2;	
}
/******************************************************************************/
void boutons()
{
	cout << "\n -----------------------------------------------------";
	cout << "\n   Press 's' to start";
	cout << "\n   Press 'b' back to menu ";
}
/******************************************************************************/
void lives(int &live)
{
  cout << "\n   LIVES : " << live;
  if (x > H+1) 
    {
      live--;
      x = H - 7; y = L/2;
      l = 20; r = 30;
      vitesse_barre = 3;
    }
}
/******************************************************************************/
void mainLoop(bool &end,int &live,int &point,int &level,int key)
{
	do {
		system("clear");
		screen_display();
		lives(live);
		afficher_points_niveau(point,level);
		wall();
		brique();
		key = read_keybord();
		barre(key);
		ball(end,live,point);
		usleep(100 * 1000);
		end = finir(live);
  } while (!end);	
}
/******************************************************************************/
void menu();
void niveau(bool &end,int &live,int &point,int &level)
{
	int hasard,key,keep = 1;
	
	system ("clear");
	creerniveau(level);
	wall();
	brique();
	barre(key);
	ball(end,live,point); 
	screen_display();
	lives(live);
	afficher_points_niveau(point,level);
	boutons();
	
	do {
		key = read_keybord();
		if (key == 's') // Press "s" to start
		{ 
			hasard = rand() % 2;
			if (hasard == 1) {movex = false; movey = true;}
			else {movex = false; movey = false;}
			mainLoop(end,live,point,level,key); 
			keep = 2;
		}
		
		if (key == 'b') 
		{
			system("clear");
			titre_jeux();
			menu();
			keep = 2;
		}
	}while (keep != 2);
}
/******************************************************************************/
void menu() 
{
	int choix, live = 3 , point = 0 , level = 1 , key , keep = 1;
	bool end = false;
	
	cout << endl;
	cout <<"\t \t       JOUER (1)     "  <<"\n";
	cout <<"\t \t  INTRUCTIONS (2)  "  <<"\n";
	cout <<"\t \t     SORTIR (3)    "  <<"\n";
	cout << "Choose : ";
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
			system ("clear");
			  cout << "\n \n _ Utiliser " << "<-" << "," << "->" << " pour controle la barre de garder la balle en jeu. ";
			  cout << "Ne paniquez pas ou vous allez tomber les boules et perdre. \n";
			  cout << "\n Il y a 9 types du brique \n";
			  cout << "\t" << '*' << " : un brique normal \n";
			  cout << "\t" << '#' << " : " << '*' << "x2 \n";
			  cout << "\t" << 'l'   << " : +1 lives \n";
			  cout << "\t" << '>' << " : + La vitesse du barre \n";
			  cout << "\t" << '<' << " : - La vitesse du barre \n";
			  cout << "\t" << "+++ : + La longuer du barre \n";
			  cout << "\t" << "--- : - La longuer du barre \n";
			  cout << "\t" << "??? : Chooser au hasard un effet \n";
			  cout << "\t" << '~' << " : un brique qui ne peut pas detruit \n";
			  cout << "-----------------------------------------------------------------------";
			  cout << "\n   Press 'b' back to menu ";
			  do {
			    key = read_keybord();
			    if (key == 'b')
			    {
					system("clear");
					titre_jeux();
	                menu();
	                keep = 1;
				}
			  } while (keep != 2);			  
			 
		case 3 : break;
	}
}
/******************************************************************************/
int main() {
  srand(time(NULL));
  
  titre_jeux();
  menu();
}

/******************************************************************************/
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if(ch != EOF)
    {
      ungetc(ch, stdin);
      return 1;
    }
  return 0;
}

int read_keybord() {
  int key;
  if (kbhit())
    key = getc(stdin);
  else
    key = 0;
  return key;
}
/******************************************************************************/
/******************************************************************************/
