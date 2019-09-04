/*
To do list:
1. Ask Sakib to complete his tasks
2. Create "Pause", "Back to Menu", "Exit" menu in game window
3. Work on saving high score
4. complete boss-shooting, boss-getting-shot
**5. Game continues on background even if gameMode!=1 ///seems solved
**6. Bullet does not decrease after few potion
*/

#include "iGraphics.h"
#include <math.h>
#include <windows.h>

#define MAX_ENEMY 5
#define MAX_ENEMY_BEAM 10
#define MAX_POWER 100
#define MAX_BEAM 250

int i, j, k, power=MAX_POWER, score=0, highScore = 0, max_beam_count = MAX_BEAM;
float intervalForNewEnemy=3000,intervalForEnemyMove=50, intervalForEnemyBeam=2000, intervalForEnemyBeamMove=30;
float spaceship_pos_x=100, spaceship_pos_y=300;
float boss_pos_x=1000, boss_pos_y=400, dy=2;
char scoreString[6], powerString[6], beamString[6];
int gameMode = -1;  /// 1 = Menu; 2 = Game Window; 3 = High Score

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
ENEMY lifePotion;
ENEMY bulletPotion;

typedef struct
{
    int x, y;
    int is_shoot;
} ENEMYBEAM;
ENEMYBEAM enemyBeamArray[MAX_ENEMY][MAX_ENEMY_BEAM];
int enemyBeamNumber;

typedef struct
{
    int x, y, alive = 0;
    int life = 11;
} BOSS;
BOSS boss;


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

void DrawLifeBar(float n)
{
    iText(200, 650, "Lifebar: ", GLUT_BITMAP_HELVETICA_18);
    float temp = n/MAX_POWER;
    iSetColor(255, 255, 255);
    iRectangle(300, 650, 130, 20);
    iSetColor(150, 255, 255);
    iFilledRectangle(302, 652, 126*temp, 16);
}

void Menubar()
{
    iShowBMP2(1101, 645, "gameBacktomenu.bmp", 0); ///###############################################################
    iShowBMP2(1234, 615, "gameExit.bmp", 0);
    itoa(score, scoreString, 10);
    iText(10,620,"Score : ", GLUT_BITMAP_HELVETICA_18);
    iText(80,620, scoreString, GLUT_BITMAP_HELVETICA_18);

    itoa(power, powerString, 10);
    iText(10,650,"Power : ", GLUT_BITMAP_HELVETICA_18);
    iText(80,650, powerString, GLUT_BITMAP_HELVETICA_18);

    itoa(min(max_beam_count-beamIndex, MAX_BEAM), beamString, 10);
    iText(200, 620, "Bullets: ", GLUT_BITMAP_HELVETICA_18);
    iText(300,620, beamString, GLUT_BITMAP_HELVETICA_18);
}

void iDraw()
{
    if (gameMode==-1)
    {
        iClear();
        iShowBMP(0, 0, "initialbackground.bmp");
        iShowBMP2(200, 550, "title.bmp", 0);
        iShowBMP2(490, 400, "1-newgame.bmp", 0);
        iShowBMP2(535, 350, "2-story.bmp", 0);
        iShowBMP2(450, 300, "3-instructions.bmp", 0);
        iShowBMP2(470, 250, "4-highscore.bmp", 0);
        iShowBMP2(520, 203, "5-credits.bmp", 0);
        iShowBMP2(530, 30, "exit.bmp", 0);
    }

    if (gameMode==1)
    {
        iClear();
        iShowBMP(0, 0, "background.bmp");
        iShowBMP(0, 600, "Menubar.bmp");
        DrawLifeBar(power);
        Menubar();

        iShowBMP2(spaceship_pos_x, spaceship_pos_y, "ship.bmp", 0);

        if (lifePotion.alive==1)
        {
            iShowBMP2(lifePotion.x, lifePotion.y, "red-potion.bmp", 0);
            for (j=0; j<beamIndex; j++)
            {
                if (beamarray[j].is_shoot==1)
                {
                    if (beamarray[j].x>=lifePotion.x && beamarray[j].x<=lifePotion.x+32 && beamarray[j].y>=lifePotion.y && beamarray[j].y<=lifePotion.y+32 && lifePotion.alive==1)
                    {
                        beamarray[j].is_shoot = 0;
                        lifePotion.alive = 0;
                        lifePotion.y = 680;
                        power += 10;
                        power = min(MAX_POWER, power);
                        break;
                    }
                }
            }
        }

        if (bulletPotion.alive==1)
        {
            iShowBMP2(bulletPotion.x, bulletPotion.y, "bomb.bmp", 0);
            for (j=0; j<beamIndex; j++)
            {
                if (beamarray[j].is_shoot==1)
                {
                    if (beamarray[j].x>=bulletPotion.x && beamarray[j].x<=bulletPotion.x+75 && beamarray[j].y>=bulletPotion.y && beamarray[j].y<=bulletPotion.y+44 && bulletPotion.alive==1)
                    {
                        beamarray[j].is_shoot = 0;
                        bulletPotion.alive = 0;
                        bulletPotion.y = 680;
                        max_beam_count += 25;
                        break;
                    }
                }
            }
        }

        if (enemyNumber<MAX_ENEMY)
        {
            for (j=0; j<beamIndex; j++) ///for shooting my beam
            {
                if (beamarray[j].is_shoot==1 && beamarray[j].x<=1300)
                {
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
            for (j=0; j<enemyNumber; j++)
            {
                if (enemyArray[j].alive!=0) ///to make sure it has not been hit
                {
                    iShowBMP2(enemyArray[j].x, enemyArray[j].y, "enemy_ship.bmp", 0);
                    for (i=0; i<MAX_ENEMY_BEAM; i++)
                    {
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
            if (boss.alive == 1)
            {
                iShowBMP2(boss.x, boss.y, "monster_lvl1.bmp", 0);

                for (j=0; j<beamIndex; j++) ///for shooting my beam
                {
                    if (beamarray[j].is_shoot==1 && beamarray[j].x<=1300)
                    {
                        DrawMyBeam(beamarray[j].x, beamarray[j].y);

                        //if (beamarray[j].x>=enemyArray[k].x && beamarray[j].x<=enemyArray[k].x+50 && beamarray[j].y>=enemyArray[k].y && beamarray[j].y<=enemyArray[k].y+50)
                    }
                }
            }
        }

        if (power<=0 || beamIndex==max_beam_count)
        {
            gameMode = 11;
            if (highScore < score)
            {
                highScore = score;
            }
        }
    }

    if (gameMode==2) ///sakib
    {
        iClear();
        iShowBMP(0, 0, "story.bmp");
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
    }

    if (gameMode==3) ///sakib
    {
        iClear();
        iShowBMP(0, 0, "instruction.bmp");
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
    }

    if (gameMode==4) ///sakib
    {
        iClear();
        iText(100, 100, "High Score", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
    }

    if (gameMode==5) ///sakib
    {
        iClear();
        iText(100, 100, "Credits", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
    }

    if (gameMode==11)
    {
        char highScoreArray[6];
        iClear();
        iShowBMP(0, 0, "pinkfloydback.bmp");
        iSetColor(255, 255, 255);
        iText(600, 300, "Your Score: ", GLUT_BITMAP_HELVETICA_18);
        iFilledRectangle(500, 220, 300, 50);
        iSetColor(0, 0, 0);
        itoa(highScore, highScoreArray, 10);
        iText(625, 235, scoreString, GLUT_BITMAP_TIMES_ROMAN_24);
    }
}

void iMouseMove(int mx, int my)
{

}

void iMouse(int button, int state, int mx, int my)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (gameMode==-1)
        {
            if (mx>=490 && mx<=730 && my>=400 && my<=430) gameMode = 1;
            if (mx>=535 && mx<=680 && my>=350 && my<=380) gameMode = 2;
            if (mx>=450 && mx<=773 && my>=300 && my<=330) gameMode = 3;
            if (mx>=470 && mx<=750 && my>=250 && my<=280) gameMode = 4;
            if (mx>=520 && mx<=700 && my>=203 && my<=230) gameMode = 5;

            if (mx>=530 && mx<=680 && my>=30 && my<=80)
            {
                exit(0);
            }
        }

        if (gameMode==1)
        {
            if (mx>=1101 && mx<=1300 && my>=645 && my<=670) gameMode = -1; ///############### better with another gamemode with only exit and resume button

            if (mx>=1234 && mx<=1295 && my>=615 && my<=650)
            {
                exit(0);
            }
        }

        if (gameMode==2)
        {
            if (mx>=510 && mx<=750 && my>=30 && my<=59) gameMode = -1;
        }

        if (gameMode==3)
        {
            if (mx>=510 && mx<=750 && my>=30 && my<=59) gameMode = -1;
        }

        if (gameMode==4)
        {
            if (mx>=510 && mx<=750 && my>=30 && my<=59) gameMode = -1;
        }

        if (gameMode==5)
        {
            if (mx>=510 && mx<=750 && my>=30 && my<=59) gameMode = -1;
        }

    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {

    }
}

void iKeyboard(unsigned char key) ///to fire my beam
{
    if (gameMode==1)
    {
        if (key=='w' || key=='W')
        {
            beamarray[beamIndex].x = spaceship_pos_x+50;
            beamarray[beamIndex].y = spaceship_pos_y+22.5;
            beamarray[beamIndex].is_shoot = 1;
            beamIndex++;
        }
    }
}

void iSpecialKeyboard(unsigned char key) ///to move my spaceship
{
    if (gameMode==1)
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
}

void myBeamMove() ///to move my beam
{
    if (gameMode==1)
    {
        for (j=0; j<max_beam_count; j++)
        {
            beamarray[j].x += 20;
            if (beamarray[j].x > 1310)
            {
                beamarray[j].is_shoot = 0;
            }
        }
    }
}

void newEnemyCreate() ///to create new enemy; also creates lifePotion and beamPotion
{
    if (gameMode==1)
    {
        if (enemyNumber%4==0 && enemyNumber%10!=0)
        {
            lifePotion.alive = 0;
        }

        if (enemyNumber%10==0 && enemyNumber!=0)
        {
            lifePotion.alive = 1;
            lifePotion.x = 500 + rand()%700;
            lifePotion.y = rand()%500;
        }

        if (enemyNumber%13==0 && enemyNumber!=0)
        {
            bulletPotion.alive = 1;
            bulletPotion.x = 600 + rand()%600;
            bulletPotion.y = 200 + rand()%300;
        }

        if (enemyNumber%35==0)
        {
            bulletPotion.alive = 0;
        }

        if (enemyNumber<MAX_ENEMY)
        {
            enemyArray[enemyNumber].x = 1300;
            enemyArray[enemyNumber].y = (rand()%520);
            enemyArray[enemyNumber].alive = 1;
            enemyNumber++;
        }

        if (enemyNumber>=MAX_ENEMY)
        {
            boss.alive = 1;
            ///boss.x = 1300;
            ///boss.y = 350;
        }
    }
}

void enemyMove() ///to move the enemy; ok
{
    if (gameMode==1)
    {
        for (j=0; j<enemyNumber; j++)
        {
            enemyArray[j].x -= 4;
        }
    }
}

void enemyBeamCreate() ///to create beams for each individual enemy; ok, i guess
{
    if (gameMode==1)
    {
        if (enemyNumber<MAX_ENEMY)
        {
            for (j=0; j<enemyNumber; j++)
            {
                if (enemyArray[j].alive!=0)
                {
                    for (i=0; i<MAX_ENEMY_BEAM; i++)
                    {
                        enemyBeamArray[j][i].x = enemyArray[j].x-15;
                        enemyBeamArray[j][i].y = enemyArray[j].y+22;
                        enemyBeamArray[j][i].is_shoot = 1;
                    }
                }
            }
        }
    }
}

void enemyBeamMove() ///to move beams of each individual enemy
{
    if (gameMode==1)
    {
        if (enemyNumber<MAX_ENEMY)
        {
            for (j=0; j<enemyNumber; j++)
            {
                if (enemyArray[j].alive!=0)
                {
                    for (i=0; i<MAX_ENEMY_BEAM; i++)
                    {
                        enemyBeamArray[j][i].x -= 25;
                    }
                }
            }
        }
    }
}

void bossMove()
{
    if (gameMode==1)
    {
        if (boss.alive == 1)
        {
            if (boss.x>1000)
            {
                boss.x -= 2;
            }

            if (boss.x<=1000)
            {
                boss.y += dy;
                if (boss.y<=0 || boss.y>=500)
                {
                    dy = -dy;
                }
            }
        }
    }
}

int main()
{
    boss.x = 1300;
    boss.y = 300;
    iSetTimer(2, myBeamMove);
    iSetTimer(intervalForNewEnemy, newEnemyCreate);
    iSetTimer(intervalForEnemyMove, enemyMove);
    iSetTimer(intervalForEnemyBeam, enemyBeamCreate);
    iSetTimer(intervalForEnemyBeamMove, enemyBeamMove);
    iSetTimer(50, bossMove);

    iInitialize(1300, 680, "Game Window!");

    return 0;
}
