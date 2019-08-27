#include "iGraphics.h"
#include <math.h>
#include <windows.h>

#define MAX_ENEMY 500
int mode=0, i, j, k, power=10, score=0;
float intervalForNewEnemy=3000,intervalForEnemyMove=50, intervalForEnemyBeam=2000;
float spaceship_pos_x=100, spaceship_pos_y=300;
char scoreString[6], powerString[6];

typedef struct
{
    int x;
    int y;
    int is_shoot;
} BEAM;
BEAM beamarray[1000];
int beamIndex;

typedef struct
{
    int x, y;
    int alive;
} ENEMY;
ENEMY enemyArray[MAX_ENEMY];
int enemyNumber;

typedef struct
{
    int x, y;
    int is_shoot;
} ENEMYBEAM;
ENEMYBEAM enemyBeamArray[MAX_ENEMY][50];
int enemyBeamNumber;


int collision(float x1, float y1, float x2, float y2)
{ ///(x1, y1) = position of beam; (x2, y2) = position of enemy spaceship
    if ((x2-x1)<8 && (x2-x1)>0 && (y2-y1)>0 && (y2-y1)<4)
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
    iShowBMP(0, 600, "Menubar.bmp");

    itoa(score, scoreString, 10);
    iText(10,620,"Score : ", GLUT_BITMAP_HELVETICA_18);
    iText(80,620, scoreString, GLUT_BITMAP_HELVETICA_18);

    itoa(power, powerString, 10);
    iText(10,645,"Power : ", GLUT_BITMAP_HELVETICA_18);
    iText(80,645, powerString, GLUT_BITMAP_HELVETICA_18);

    iShowBMP2(spaceship_pos_x, spaceship_pos_y, "ship.bmp", 0);

    for (j=0; j<1000; j++)
    {
        if (beamarray[j].is_shoot==1 && beamarray[j].x<=1320)
        {
            //iShowBMP2(beamarray[j].x, beamarray[j].y,"beam.bmp", 0);
            DrawMyBeam(beamarray[j].x, beamarray[j].y);
            for (k=0; k<enemyNumber; k++) ///checks whether the bullet hits the enemy, if so, both of them go extinct
            {
                if (enemyArray[k].alive==1)
                {
                    if (beamarray[j].x>=enemyArray[k].x && beamarray[j].x<=enemyArray[k].x+50 && beamarray[j].y>=enemyArray[k].y && beamarray[j].y<=enemyArray[k].y+50)
                    {
                        enemyArray[k].alive = 0;
                        beamarray[j].is_shoot = 0;
                        score += 10;
                    }
                }
            }
        }
    }

    if (enemyNumber<MAX_ENEMY)
    {
        for (j=0; j<enemyNumber; j++)
        {
            if (enemyArray[j].alive!=0) ///to make sure it has not been hit
            {
                iShowBMP2(enemyArray[j].x, enemyArray[j].y, "enemy_ship.bmp", 0);
            }
        }

        for (j=0; j<enemyNumber; j++)
        {
            if (enemyArray[j].alive!=0) ///to make sure it has not been hit
            {
                for (i=0; i<50; i++)
                {
                   //iShowBMP2(enemyBeamArray[j][i].x, enemyBeamArray[j][i].y, "enemy_beam.bmp", 0);
                    if (enemyBeamArray[j][i].is_shoot==1)
                    {
                        DrawEnemyBeam(enemyBeamArray[j][i].x, enemyBeamArray[j][i].y);
                        if (enemyBeamArray[j][i].x>spaceship_pos_x && enemyBeamArray[j][i].x<spaceship_pos_x+50 && enemyBeamArray[j][i].y>spaceship_pos_y && enemyBeamArray[j][i].y<spaceship_pos_y+50)
                        {
                            power--;
                            enemyBeamArray[j][i].is_shoot = 0;
                        }
                    }
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
        beamarray[beamIndex].is_shoot = 1;
        beamIndex++;
    }
}

void myBeamMove() ///to move my beam
{
    for (j=0; j<1000; j++)
    {
        beamarray[j].x += 20;
        if (beamarray[j].x > 1310)
        {
            beamarray[j].is_shoot = 0;
        }
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
        if (spaceship_pos_y+10<550) spaceship_pos_y+=10;
    }
    if (key == GLUT_KEY_DOWN)
    {
        if (spaceship_pos_y-10>0) spaceship_pos_y-=10;
    }
}

void newEnemyCreate() ///to create new enemy
{
    enemyArray[enemyNumber].x = 1300;
    enemyArray[enemyNumber].y = (rand()%580);
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
                    enemyBeamArray[j][i].is_shoot = 1;
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
