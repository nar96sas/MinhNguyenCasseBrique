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

/***************************************************************************/
#define H  20
#define L  50

char screen[H][L];

void screen_display() {
  system("clear");
  for (int i=0; i<H; i++) {
    for (int j=0; j<L; j++)
      cout << screen[i][j];
    cout << endl;
  }
}

void display() {
  for (int i=0; i<H; i++){
    for (int j=0; j<L; j++){
      if (i==0 || i==H-1 || j==0 || j==L-1)
	screen[i][j] = '*';
      else
	screen[i][j] = ' ';
    }
    cout << endl;
  }
}
/******************************************************************************/
double x = H-2;
double y = L/2;
bool movex=true; 
bool movey=true;

void update_game(int key) {
  screen[(int)x][(int)y] = 'o';
  if (movex){
    x--; 
    if (x == 0)
      movex=false;
  }
  
  if (!movex) {
    x++;
    if (x == H-2)
      movex=true;
  }
  
  if (movey){
    y++; 
    if (y == L-2)
      movey=false;
  }

  if (!movey) {
    y--;
    if (y == 1)
      movey=true;
  }  
}

  




/******************************************************************************/
int main() {
  int key;
  do {
    key = read_keybord();
    display();
    update_game(key);
    screen_display();
    usleep(100 * 1000);
  } 
  while (key != 'q');
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
