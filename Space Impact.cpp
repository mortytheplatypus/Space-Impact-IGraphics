/*
All bugs seems to be fixed.
Space Impact 2018 by Maruf and Sakib
*/

#include "iGraphics.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_BOSS_BEAM 100
#define MAX_ENEMY 15 ///value may be changed for presentation purpose
#define MAX_ENEMY_BEAM 10
#define MAX_POWER 100
#define MAX_BEAM 150

int i, j, k, power=MAX_POWER, score=0, highScore, beam_count = MAX_BEAM;
float intervalForNewEnemy=3000,intervalForEnemyMove=50, intervalForEnemyBeam=2000, intervalForEnemyBeamMove=30;
float spaceship_pos_x=100, spaceship_pos_y=300;
float boss_pos_x=1000, boss_pos_y=400, dy=2;
char scoreString[6], powerString[6], beamString[6], highScoreString[6];
int gameMode = -1;

char str[50], str2[50];
int len, mode;

typedef struct
{
    int scr;
    char plr[20];
} PLAYER;

typedef struct
{
    int x;
    int y;
    int is_shoot;
} BEAM;
BEAM beamarray[500];
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
    int life = 59; ///value may be changed for presentation purpose
} BOSS;
BOSS boss;
BEAM bossBeamArray[100];
int bossBeamNumber;

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
    iSetColor(255, 255, 255);
    iText(200, 650, "Lifebar: ", GLUT_BITMAP_HELVETICA_18);
    float temp = n/MAX_POWER;
    iSetColor(255, 255, 255);
    iRectangle(300, 650, 130, 20);
    iSetColor(150, 255, 255);
    iFilledRectangle(302, 652, 126*temp, 16);
}

void Menubar()
{
    iShowBMP2(1211, 645, "pause.bmp", 0);
    iShowBMP2(1234, 615, "gameExit.bmp", 0);
    iShowBMP2(500, 610, "MenuImages//menutitle.bmp", 0);

    itoa(score, scoreString, 10);
    iText(10,620,"Score : ", GLUT_BITMAP_HELVETICA_18);
    iText(80,620, scoreString, GLUT_BITMAP_HELVETICA_18);

    itoa(power, powerString, 10);
    iText(10,650,"Power : ", GLUT_BITMAP_HELVETICA_18);
    iText(80,650, powerString, GLUT_BITMAP_HELVETICA_18);

    itoa(beam_count, beamString, 10);
    iText(200, 620, "Bullets: ", GLUT_BITMAP_HELVETICA_18);
    iText(300,620, beamString, GLUT_BITMAP_HELVETICA_18);
}

void DrawBossBeam(float m, float n)
{
    float x = m+10, y1 = n-8, y2 = n+8;
    double a[] = {m, x, x};
    double b[] = {n, y1, y2};
    iSetColor(240, 138, 0);
    iFilledPolygon(a, b, 3);
}

void highScoreUpdate(char str2[])
{
    PLAYER scores[5];
    FILE *p;
    p = fopen("highscore.txt", "r");
    for (i=0; i<5; i++)
    {
        fscanf(p, "%s %d", &scores[i].plr, &scores[i].scr);
    }
    fclose(p);

    if (scores[4].scr < score)
    {
        scores[4].scr = score;
        strcpy(scores[4].plr, str2);
    }

    for (i=0; i<5; i++)
    {
        for (j=0; j<5; j++)
        {
            if (scores[i].scr>scores[j].scr)
            {
                int tmp;
                char tmpar[30];

                tmp = scores[i].scr;
                scores[i].scr = scores[j].scr;
                scores[j].scr = tmp;

                strcpy(tmpar, scores[i].plr);
                strcpy(scores[i].plr, scores[j].plr);
                strcpy(scores[j].plr, tmpar);
            }
        }
    }

    p = fopen("highscore.txt", "w");
    for (i=0; i<5; i++)
    {
        fprintf(p, "%s %d\n", scores[i].plr, scores[i].scr);
    }
    fclose(p);
}

void iDraw()
{
    if (gameMode==-1) ///menu - DONE!!
    {
        iClear();
        iShowBMP(0, 0, "BackgroundImages//Home.bmp");
        iShowBMP2(200, 550, "MenuImages//title.bmp", 0);
        iShowBMP2(490, 400, "MenuImages//1-newgame.bmp", 0);
        iShowBMP2(535, 350, "MenuImages//2-story.bmp", 0);
        iShowBMP2(450, 300, "MenuImages//3-instructions.bmp", 0);
        iShowBMP2(470, 250, "MenuImages//4-highscore.bmp", 0);
        iShowBMP2(520, 203, "MenuImages//5-credits.bmp", 0);
        iShowBMP2(530, 30, "MenuImages//exit.bmp", 0);


        power=MAX_POWER;
        score=0;
        beam_count = MAX_BEAM;
        beamIndex = 0;
        enemyNumber = 0;
        len = 0;
        boss_pos_x = 1320;
        boss_pos_y = 300;
    }

    if (gameMode==1) ///game window - DONE!!
    {
        iClear();
        iShowBMP(0, 0, "BackgroundImages//background.bmp");
        iShowBMP(0, 600, "MenuImages//Menubar.bmp");
        DrawLifeBar(power);
        Menubar();

        iShowBMP2(spaceship_pos_x, spaceship_pos_y, "ship.bmp", 0);

        if (lifePotion.alive==1)
        {
            iShowBMP2(lifePotion.x, lifePotion.y, "red-potion.bmp", 0);
            if (spaceship_pos_x>lifePotion.x-50 && spaceship_pos_x<lifePotion.x+32 && spaceship_pos_y>lifePotion.y-50 && spaceship_pos_y<lifePotion.y+32)
            {
                lifePotion.alive = 0;
                lifePotion.y = 680;
                power += 10;
                power = min(MAX_POWER, power);
            }
        }

        if (bulletPotion.alive==1)
        {
            iShowBMP2(bulletPotion.x, bulletPotion.y, "bomb.bmp", 0);
            if (spaceship_pos_x>bulletPotion.x-50 && spaceship_pos_x<bulletPotion.x+75 && spaceship_pos_y>bulletPotion.y-50 && spaceship_pos_y<bulletPotion.y+44)
            {
                bulletPotion.alive = 0;
                bulletPotion.y = 680;
                beam_count += 15;
                beam_count = min(150, beam_count);
            }
        }
        for (j=0; j<beamIndex; j++) ///for shooting my beam
        {
            if (beamarray[j].is_shoot==1)
            {
                if (enemyNumber<MAX_ENEMY)
                {
                    DrawMyBeam(beamarray[j].x, beamarray[j].y);
                    //iShowBMP2(beamarray[j].x, beamarray[j].y, "beam.bmp", 0);
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

                if (enemyNumber>=MAX_ENEMY)
                {
                     //DrawMyBeam(beamarray[j].x, beamarray[j].y);
                    iShowBMP2(beamarray[j].x, beamarray[j].y, "beam.bmp", 0);

                    if (beamarray[j].x>boss.x && beamarray[j].x<boss.x+100 && beamarray[j].y>boss.y && beamarray[j].y<boss.y+101)
                    {
                        score += 15;
                        beamarray[j].is_shoot = 0;
                        boss.life = boss.life-1;
                        if (boss.life==0)
                        {
                            score += 500;
                            gameMode = 12;
                        }
                    }
                }
            }
        }

        for (j=0; j<enemyNumber; j++)
        {
            if (enemyArray[j].alive==1) ///to make sure it has not been hit
            {
                iShowBMP2(enemyArray[j].x, enemyArray[j].y, "enemy_ship.bmp", 0);
                if (spaceship_pos_x>enemyArray[j].x-50 && spaceship_pos_x<enemyArray[j].x+50 && spaceship_pos_y>enemyArray[j].y-50 && spaceship_pos_y<enemyArray[j].y+50)
                {
                    enemyArray[j].alive = 0;
                    power-=10;
                    score+=10;
                    continue;
                }
            }

            for (i=0; i<MAX_ENEMY_BEAM; i++)
            {
                if (enemyBeamArray[j][i].is_shoot==1)
                {
                    DrawEnemyBeam(enemyBeamArray[j][i].x, enemyBeamArray[j][i].y);
                    //iShowBMP2(enemyBeamArray[j][i].x, enemyBeamArray[j][i].y, "enemy_beam.bmp", 0);
                    if (enemyBeamArray[j][i].x>spaceship_pos_x && enemyBeamArray[j][i].x<spaceship_pos_x+50 && enemyBeamArray[j][i].y>spaceship_pos_y && enemyBeamArray[j][i].y<spaceship_pos_y+50)
                    {
                        power--;
                        enemyBeamArray[j][i].is_shoot = 0;
                    }
                }
            }
        }


        if (enemyNumber>=MAX_ENEMY)
        {
            if (boss.alive == 1)
            {
                iShowBMP2(boss.x, boss.y, "monster.bmp", 0);

                for (j=0; j<MAX_BOSS_BEAM; j++)
                {
                    if (bossBeamArray[j].is_shoot==1)
                    {
                        DrawBossBeam(bossBeamArray[j].x, bossBeamArray[j].y);

                        if (bossBeamArray[j].x>spaceship_pos_x && bossBeamArray[j].x<spaceship_pos_x+50 && bossBeamArray[j].y>spaceship_pos_y && bossBeamArray[j].y<spaceship_pos_y+50)
                        {
                            bossBeamArray[j].is_shoot = 0;
                            power -= 15;
                        }
                    }
                }
            }
        }

        if (power<=0 || beam_count==0)
        {
            gameMode = 11;
        }
    }

    if (gameMode==2) ///background story - DONE!!
    {
        iClear();
        iShowBMP(0, 0, "BackgroundImages//story.bmp");
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
    }

    if (gameMode==3) ///instructions - DONE!!
    {
        iClear();
        iShowBMP(0, 0, "BackgroundImages//instruction.bmp");
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
        iShowBMP2(193, 418, "red-potion.bmp", 0);
        iShowBMP2(758, 418, "red-potion.bmp", 0);
        iShowBMP2(185, 375, "bomb.bmp", 0);
        iShowBMP2(770, 375, "bomb.bmp", 0);
    }

    if (gameMode==4) ///high score - DONE!
    {
        iClear();
        iShowBMP(0, 0, "BackgroundImages//highscore.bmp");
        iShowBMP2(200, 580, "MenuImages//title.bmp", 0);
        iShowBMP2(510, 30, "backtomenu.bmp", 0);

        PLAYER scores[5];
        FILE *hs = fopen("highscore.txt", "r");
        for (i=0; i<5; i++)
        {
            fscanf(hs, "%s %d", &scores[i].plr, &scores[i].scr);
        }
        fclose(hs);

        iSetColor(240, 240, 240);
        int xxx=500, yyy=450;
        for (i=0; i<5; i++)
        {
            char tmpo[5], tmpn[20];
            itoa(i+1, tmpo, 10);
            itoa(scores[i].scr, tmpn, 10);

            iText(500, yyy, tmpo, GLUT_BITMAP_TIMES_ROMAN_24);
            iText(510, yyy, ". ", GLUT_BITMAP_TIMES_ROMAN_24);
            iText(530, yyy, scores[i].plr, GLUT_BITMAP_TIMES_ROMAN_24);
            iText(700, yyy, tmpn, GLUT_BITMAP_TIMES_ROMAN_24);
            yyy -= 50;
        }
    }

    if (gameMode==5) ///credits - DONE!!
    {
        iClear();
        iShowBMP(0, 0, "BackgroundImages//credits.bmp");
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
    }

    if (gameMode==6) ///Resume menu - DONE!!
    {
        iClear();
        iShowBMP(0, 0, "BackgroundImages//Resume.bmp");
        iShowBMP2(200, 550, "MenuImages//title.bmp", 0);
        iShowBMP2(550, 350, "resume.bmp", 0);
        iShowBMP2(525, 290, "resume-newgame.bmp", 0);
        iShowBMP2(570, 230, "quit.bmp", 0);
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
    }

    if (gameMode==11) ///if player don't win the game, just shows score, high score and takes name input - DONE!
    {
        iClear();
        iShowBMP(0, 0, "BackgroundImages//losingscore.bmp");
        char temp1[6], temp2[6], temp3[30];
        int nn;

        FILE *fp = fopen("highscore.txt", "r");
        fscanf(fp, "%s %d", &temp3, &nn);
        fclose(fp);

        itoa(score, temp1, 10);
        itoa(nn, temp2, 10);

        iSetColor(235, 235, 235);
        iText(665, 400, temp2, GLUT_BITMAP_TIMES_ROMAN_24);
        iText(665, 355, temp1, GLUT_BITMAP_TIMES_ROMAN_24);

        iSetColor(225, 225, 225);
        iFilledRectangle(805, 265, 230, 30);
        if (mode==1)
        {
            iSetColor(50, 50, 50);
            iText(810, 275, str, GLUT_BITMAP_HELVETICA_18);
        }
    }

    if (gameMode==12) ///if player wins the game, updates high score also - DONE!
    {
        iClear();
        iShowBMP(0, 0, "BackgroundImages//congratulation.bmp");
        iSetColor(240, 240, 240);
        char tem[6];
        itoa(score, tem, 10);
        iText(700, 168, tem, GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(225, 225, 225);
        iFilledRectangle(660, 35, 230, 30);
        if (mode==1)
        {
            iSetColor(50, 50, 50);
            iText(665, 45, str, GLUT_BITMAP_HELVETICA_18);
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
        if (gameMode==-1)
        {
            if (mx>=490 && mx<=730 && my>=400 && my<=430)
            {
                power=MAX_POWER;
                score=0;
                beam_count = MAX_BEAM;
                beamIndex = 0;
                enemyNumber = 0;
                len = 0;
                boss_pos_x = 1320;
                boss_pos_y = 300;
                gameMode = 1;
            }

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
            if (mx>=1211 && mx<=1295 && my>=645 && my<=670) gameMode = 6;

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

        if (gameMode==6)
        {
            if (mx>=510 && mx<=750 && my>=30 && my<=59) gameMode = -1;
            if (mx>=550 && mx<=699 && my>=350 && my<=395) gameMode = 1;
            if (mx>=525 && mx<=726 && my>=290 && my<=335)
            {
                power=MAX_POWER;
                score=0;
                beam_count = MAX_BEAM;
                beamIndex = 0;
                enemyNumber = 0;
                len = 0;
                boss_pos_x = 1300;
                boss_pos_y = 300;
                gameMode = 1;
            }

            if (mx>=570 && mx<=665 && my>=230 && my<=285)
            {
                exit(0);
            }
        }

        if (gameMode==11)
        {
            if (mx>=810 && mx<=1080 && my>=265 && my<=315) mode = 1; //805, 265, 280, 40
        }

        if (gameMode==12)
        {
            if (mx>=665 && mx<=910 && my>=35 && my<=65) mode = 1;
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
            beam_count--;
        }
    }

    if (gameMode==11 || gameMode==12) /// for name input
    {
        if (mode==1)
        {
            if(key == '\r' && len!=0)
            {
                mode = 0;
                strcpy(str2, str);
                highScoreUpdate(str2);
                for (i=0; i<len; i++)
                {
                    str[i] = 0;
                    str2[i] = 0;
                }
                len = 0;
                gameMode = -1;
            }
            else if (key != ' ')
            {
                str[len] = key;
                len++;
            }
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
        for (j=0; j<beamIndex; j++)
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
        if (enemyNumber<MAX_ENEMY)
        {
            if (enemyNumber%5==0 && enemyNumber%10!=0) lifePotion.alive = 0;
            if (enemyNumber%18==0) bulletPotion.alive = 0;

            if (enemyNumber%10==0 && enemyNumber!=0) ///value may be changed for presentation purpose
            {
                lifePotion.alive = 1;
                lifePotion.x = 500 + rand()%700;
                lifePotion.y = rand()%500;
            }

            if (enemyNumber%13==0 && enemyNumber!=0) ///value may be changed for presentation purpose
            {
                bulletPotion.alive = 1;
                bulletPotion.x = 600 + rand()%600;
                bulletPotion.y = 200 + rand()%300;
            }

            enemyArray[enemyNumber].x = 1300;
            enemyArray[enemyNumber].y = (rand()%520);
            enemyArray[enemyNumber].alive = 1;
            enemyNumber++;
        }

        if (enemyNumber>=MAX_ENEMY)
        {
            boss.alive = 1;
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
                if (enemyArray[j].alive==1)
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
                for (i=0; i<MAX_ENEMY_BEAM; i++)
                {
                    enemyBeamArray[j][i].x -= 25;
                }
            }
        }
    }
}

void bossMove() ///to create boss
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

void bossBeamCreate() ///to create beams for boss
{
    if (gameMode==1)
    {
        if (boss.alive==1)
        {
            bossBeamArray[bossBeamNumber].x = boss.x+4;
            bossBeamArray[bossBeamNumber].y = boss.y+51;
            bossBeamArray[bossBeamNumber].is_shoot = 1;
            bossBeamNumber++;
            bossBeamNumber %= MAX_BOSS_BEAM;
        }
    }
}

void bossBeamMove() ///to move the beams of boss
{
    if (gameMode==1)
    {
        if (boss.alive==1)
        {
            for (j=0; j<bossBeamNumber; j++)
            {
                if (bossBeamArray[j].is_shoot==1) bossBeamArray[j].x -= 7;
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
    iSetTimer(1000, bossBeamCreate);
    iSetTimer(5, bossBeamMove);

    iInitialize(1300, 680, "Game Window!");

    return 0;
}
