#include "iGraphics.h"
#include <math.h>
#include <windows.h>

typedef struct
{
    int x, y;
    int is_shoot;
} BEAM;
BEAM beamarray[1000];
int beamIndex;

typedef struct
{
    int x, y;
} ENEMY;
ENEMY enemyArray[1000];

ENEMY enemyBeamArray[1000][50];

int enemyNumber, enemyBeamNumber;


float spaceship_pos_x=100, spaceship_pos_y=300;

int mode=0, i, j, k;
float intervalForNewEnemy=3000,intervalForEnemyMove=50, intervalForEnemyBeam=2100;

void iDraw()
{
    iClear();
    iShowBMP(0, 0, "background2.bmp");
    iShowBMP2(spaceship_pos_x, spaceship_pos_y, "ship.bmp", 0);

    for (j=0; j<1000; j++)
    {
        if (beamarray[j].is_shoot==1) iShowBMP2(beamarray[j].x, beamarray[j].y,"beam.bmp", 0);
    }

    for (j=0; j<enemyNumber; j++)
    {
        iShowBMP2(enemyArray[j].x, enemyArray[j].y, "enemy_ship.bmp", 0);
    }
    
    for (j=0; j<enemyNumber; j++)
    {
        for (i=0; i<50; i++)
        {
           iShowBMP2(enemyBeamArray[j][i].x, enemyBeamArray[j][i].y, "enemy_beam.bmp", 0);
        }
    }
}


void iMouseMove(int mx, int my)
{

}

void iMouse(int button, int state, int mx, int my)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {

    }
}

void iKeyboard(unsigned char key) ///to fire my beam
{
    if (key=='w')
    {
        beamarray[beamIndex].x = spaceship_pos_x+50;
        beamarray[beamIndex].y = spaceship_pos_y+22.5;
        beamarray[beamIndex].is_shoot = 1;
        beamIndex++;
        beamIndex = beamIndex%1000;
    }
}

void myBeamMove() ///to move my beam
{
    for (j=0; j<1000; j++)
    {
        beamarray[j].x += 5;
    }
}

void iSpecialKeyboard(unsigned char key) ///to move my spaceship
{
    if (key == GLUT_KEY_RIGHT)
    {
        if (spaceship_pos_x+10<1250) spaceship_pos_x+=10;
    }
    if (key == GLUT_KEY_LEFT)
    {
        if (spaceship_pos_x-10>0) spaceship_pos_x-=10;
    }
    if (key == GLUT_KEY_UP)
    {
        if (spaceship_pos_y+10<630) spaceship_pos_y+=10;
    }
    if (key == GLUT_KEY_DOWN)
    {
        if (spaceship_pos_y-10>0) spaceship_pos_y-=10;
    }
}

void newEnemyCreate() ///to create new enemy
{
    enemyArray[enemyNumber].x = 1300;
    enemyArray[enemyNumber].y = (rand()%50)*10;
    enemyNumber++;
}

void enemyMove() ///to move the enemy
{
    for (j=0; j<enemyNumber; j++)
    {
        enemyArray[j].x -= 4;
    }
}

void enemyBeamCreate() ///has issues
{
    for (j=0; j<enemyNumber; j++)
    {
        for (i=0; i<50; i++)
        {
            enemyBeamArray[j][i].x = enemyArray[j].x-15;
            enemyBeamArray[j][i].y = enemyArray[j].y+22;
        }
    }
}

void enemyBeamMove() 
{ ///beams that are generated later, do not make it to the end of screen
    for (j=0; j<enemyNumber; j++)
    {
        for (i=0; i<50; i++)
        {
            enemyBeamArray[j][i].x -= 15;
        }
    }
}

int main()
{
    iSetTimer(5, myBeamMove);
    iSetTimer(intervalForNewEnemy, newEnemyCreate);
    iSetTimer(intervalForEnemyMove, enemyMove);

    iInitialize(1300, 680, "Game Window!");

    return 0;
}
