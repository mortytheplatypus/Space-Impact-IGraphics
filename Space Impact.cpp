#include "iGraphics.h"
#include <math.h>
#include <windows.h>

#define MAX_ENEMY 500
int mode=0, i, j, k;
float intervalForNewEnemy=3000,intervalForEnemyMove=50, intervalForEnemyBeam=2000;
float spaceship_pos_x=100, spaceship_pos_y=300;

typedef struct
{
    int x;
    int y;
} BEAM;
BEAM beamarray[1000];
int beamIndex;

typedef struct
{
    int x, y;
    int alive;
} ENEMY;
ENEMY enemyArray[1000];
int enemyNumber;

typedef struct
{
    int x, y;
} ENEMYBEAM;
ENEMYBEAM enemyBeamArray[1000][50];
int enemyBeamNumber;


int checkWhetherHit(float x1, float y1, float x2, float y2)
{
    if ((x2-x1)<50 && x2-x1>0 && y2-y1>0 && y2-y1<50)
    {
        return 1;
    }

    return 0;
}

void DrawMyBeam(float m, float n)
{
    iSetColor(255, 0, 0);
    iFilledEllipse(m, n, 8, 4, 10);
}

void DrawEnemyBeam(float m, float n)
{
    iSetColor(255, 255, 255);
    iFilledEllipse(m, n, 7, 3, 10);
}


void iDraw()
{
    iClear();
    iShowBMP(0, 0, "background.bmp");
    iShowBMP2(spaceship_pos_x, spaceship_pos_y, "ship.bmp", 0);

    for (j=0; j<1000; j++)
    {
        if (beamarray[j].x<=1320)
        {
            //iShowBMP2(beamarray[j].x, beamarray[j].y,"beam.bmp", 0);
            DrawMyBeam(beamarray[j].x, beamarray[j].y);
        }
    }

    if (enemyNumber<MAX_ENEMY)
    {
        for (j=0; j<enemyNumber; j++)
        {
            if (enemyArray[j].alive!=0)
            {
                iShowBMP2(enemyArray[j].x, enemyArray[j].y, "enemy_ship.bmp", 0);
            }
        }

        for (j=0; j<enemyNumber; j++)
        {
            if (enemyArray[j].alive!=0)
            {
                for (i=0; i<50; i++)
                {
                   //iShowBMP2(enemyBeamArray[j][i].x, enemyBeamArray[j][i].y, "enemy_beam.bmp", 0);
                   DrawEnemyBeam(enemyBeamArray[j][i].x, enemyBeamArray[j][i].y);
                }
            }
        }
    }

    if (enemyNumber>=MAX_ENEMY)
    {
        iShowBMP2(1200, 400, "monster_lvl1.bmp", 0);
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
        beamIndex++;
        beamIndex = beamIndex%1000;
    }
}

void myBeamMove() ///to move my beam
{
    for (j=0; j<1000; j++)
    {
        beamarray[j].x += 20;
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
    enemyArray[enemyNumber].y = (rand()%50)*20;
    enemyArray[enemyNumber].alive = 1;
    enemyNumber++;
}

void enemyMove() ///to move the enemy
{
    for (j=0; j<enemyNumber; j++)
    {
        enemyArray[j].x -= 4;
    }
}

void enemyBeamCreate() ///to create beams for each individual enemy
{
    if (enemyNumber<MAX_ENEMY)
    {
        for (j=0; j<enemyNumber; j++)
        {
            if (enemyArray[j].alive!=0)
            {
                for (i=0; i<50; i++)
                {
                    enemyBeamArray[j][i].x = enemyArray[j].x-15;
                    enemyBeamArray[j][i].y = enemyArray[j].y+22;
                }
            }
        }
    }
}

void enemyBeamMove() ///to move beams of each individual enemy
{
    if (enemyNumber<MAX_ENEMY)
    {
        for (j=0; j<enemyNumber; j++)
        {
            if (enemyArray[j].alive!=0)
            {
                for (i=0; i<50; i++)
                {
                    enemyBeamArray[j][i].x -= 15;
                }
            }
        }
    }
}

int main()
{
    iSetTimer(2, myBeamMove);
    iSetTimer(intervalForNewEnemy, newEnemyCreate);
    iSetTimer(intervalForEnemyMove, enemyMove);
    iSetTimer(intervalForEnemyBeam, enemyBeamCreate);
    iSetTimer(15, enemyBeamMove);

    iInitialize(1300, 680, "Game Window!");

    return 0;
}
