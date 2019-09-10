/*
To do list:
1. Ask Sakib to complete his tasks
2. Create "Pause", "Back to Menu", "Exit" menu in game window ///seems solved
3. Work on saving high score
4. complete boss-shooting, boss-getting-shot ///seems solved
**5. Game continues on background even if gameMode!=1 ///seems solved
**6. Bullet does not decrease after few potion
7. Create "Pause" menu and "Resume" window ///seems solved
8. (optional) Are you sure window in exit
9. khela jitle high score update kora
*/

#include "iGraphics.h"

#define MAX_BOSS_BEAM 100
#define MAX_ENEMY 13 ///eita change korte jeno bhule na jai
#define MAX_ENEMY_BEAM 10
#define MAX_POWER 100
#define MAX_BEAM 150

int i, j, k, power=MAX_POWER, score=0, highScore, max_beam_count = MAX_BEAM;
float intervalForNewEnemy=3000,intervalForEnemyMove=50, intervalForEnemyBeam=2000, intervalForEnemyBeamMove=30;
float spaceship_pos_x=100, spaceship_pos_y=300;
float boss_pos_x=1000, boss_pos_y=400, dy=2;
char scoreString[6], powerString[6], beamString[6], highScoreString[6];
int gameMode = -1;

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
    int life = 10; ///eita change korte jeno bhule na jai
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

    itoa(min(max_beam_count-beamIndex, MAX_BEAM), beamString, 10);
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

void iDraw()
{
    if (gameMode==-1) ///shows menu
    {
        iClear();
        iShowBMP(0, 0, "BackgoundImages//Home.bmp");
        iShowBMP2(200, 550, "MenuImages//title.bmp", 0);
        iShowBMP2(490, 400, "MenuImages//1-newgame.bmp", 0);
        iShowBMP2(535, 350, "MenuImages//2-story.bmp", 0);
        iShowBMP2(450, 300, "MenuImages//3-instructions.bmp", 0);
        iShowBMP2(470, 250, "MenuImages//4-highscore.bmp", 0);
        iShowBMP2(520, 203, "MenuImages//5-credits.bmp", 0);
        iShowBMP2(530, 30, "MenuImages//exit.bmp", 0);
    }

    if (gameMode==1) ///shows game window
    {
        iClear();
        iShowBMP(0, 0, "BackgoundImages//background.bmp");
        iShowBMP(0, 600, "MenuImages//Menubar.bmp");
        DrawLifeBar(power);
        Menubar();

        iShowBMP2(spaceship_pos_x, spaceship_pos_y, "ship.bmp", 0);

        if (enemyNumber<MAX_ENEMY)
        {
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
                    max_beam_count += 15;
                }
            }
            for (j=0; j<beamIndex; j++) ///for shooting my beam
            {
                if (beamarray[j].is_shoot==1)
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
                            if (enemyBeamArray[j][i].x>spaceship_pos_x && enemyBeamArray[j][i].x<spaceship_pos_x+50 && enemyBeamArray[j][i].y>spaceship_pos_y && enemyBeamArray[j][i].y<spaceship_pos_y+50)
                            {
                                power--;
                                enemyBeamArray[j][i].is_shoot = 0;
                            }
                        }
                    }
            }
        }

        if (enemyNumber>=MAX_ENEMY)
        {
            if (boss.alive == 1)
            {
                iShowBMP2(boss.x, boss.y, "monster.bmp", 0); ///###right till now in this section

                for (j=0; j<beamIndex; j++) ///for shooting my beam
                {
                    if (beamarray[j].is_shoot==1)
                    {
                        DrawMyBeam(beamarray[j].x, beamarray[j].y);

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

                for (j=0; j<MAX_BOSS_BEAM; j++)
                {
                    if (bossBeamArray[j].is_shoot==1)
                    {
                        DrawBossBeam(bossBeamArray[j].x, bossBeamArray[j].y);

                        if (bossBeamArray[j].x>spaceship_pos_x && bossBeamArray[j].x<spaceship_pos_x+50 && bossBeamArray[j].y>spaceship_pos_y && bossBeamArray[j].y<spaceship_pos_y+50)
                        {
                            bossBeamArray[j].is_shoot = 0;
                            power -= 15;
                            break;
                        }
                    }
                }
            }
        }

        if (power<=0 || beamIndex==max_beam_count)
        {
            gameMode = 11;
        }
    }

    if (gameMode==2) ///sakib - shows the background story
    {
        iClear();
        iShowBMP(0, 0, "BackgoundImages//story.bmp");
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
    }

    if (gameMode==3) ///sakib - shows instructions
    {
        iClear();
        iShowBMP(0, 0, "BackgoundImages//instruction.bmp");
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
    }

    if (gameMode==4) ///sakib - shows high score
    {
        iClear();
        iText(100, 100, "High Score", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
    }

    if (gameMode==5) ///sakib - shows credits
    {
        iClear();
        iText(100, 100, "Credits", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
    }

    if (gameMode==6)
    {
        iClear();
        iShowBMP(0, 0, "BackgoundImages//Resume.bmp");
        iShowBMP2(200, 550, "MenuImages//title.bmp", 0);
        iShowBMP2(575, 350, "resume.bmp", 0);
        iShowBMP2(550, 290, "resume-newgame.bmp", 0);
        iShowBMP2(595, 230, "quit.bmp", 0);
    }

    if (gameMode==11) ///sakib - if player don't win the game, just for showing the score and check whether he/she achieved high score
    {
        iClear();
        FILE *fp, *fp1;

        fp = fopen("highscore.txt", "r");
        int n;
        fscanf(fp, "%d", &n);

        if (score>n)
        {
            fp1 = fopen("highscore.txt", "w");
            fprintf(fp1, "%d", score);
            fclose(fp1);
        }
        fclose(fp);

        fp = fopen("highscore.txt", "r");
        fscanf(fp, "%d", &n);

        iSetColor(255, 255, 255);
        itoa(score, scoreString, 10);
        itoa(n, highScoreString, 10);

        iText(600, 200, scoreString, GLUT_BITMAP_TIMES_ROMAN_24);
        iText(400, 200, "Score: ", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(600, 500, highScoreString, GLUT_BITMAP_TIMES_ROMAN_24);
        iText(400, 500, "High Score: ", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
        fclose(fp);
    }

    if (gameMode==12) ///sakib - if player wins the game, updates high score also
    {
        iClear();
        iShowBMP(0, 0, "BackgoundImages//congratulation.bmp");

        FILE *fp, *fp1;

        fp = fopen("highscore.txt", "r");
        int n;
        fscanf(fp, "%d", &n);

        if (score>n)
        {
            fp1 = fopen("highscore.txt", "w");
            fprintf(fp1, "%d", score);
            fclose(fp1);
        }
        fclose(fp);

        fp = fopen("highscore.txt", "r");
        fscanf(fp, "%d", &n);

        iSetColor(255, 255, 255);
        itoa(score, scoreString, 10);
        itoa(n, highScoreString, 10);

        iText(600, 200, scoreString, GLUT_BITMAP_TIMES_ROMAN_24);
        iText(400, 200, "Score: ", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(600, 500, highScoreString, GLUT_BITMAP_TIMES_ROMAN_24);
        iText(400, 500, "High Score: ", GLUT_BITMAP_TIMES_ROMAN_24);
        iShowBMP2(510, 30, "backtomenu.bmp", 0);
        fclose(fp);
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
                max_beam_count = MAX_BEAM;
                beamIndex = 0;
                enemyNumber = 0;
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
            if (mx>=1211 && mx<=1295 && my>=645 && my<=670) gameMode = 6; ///############### better with another gamemode with only exit and resume button

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
            if (mx>=575 && mx<=724 && my>=350 && my<=395) gameMode = 1;
            if (mx>=550 && mx<=751 && my>=290 && my<=335)
            {
                power=MAX_POWER;
                score=0;
                max_beam_count = MAX_BEAM;
                beamIndex = 0;
                enemyNumber = 0;
                gameMode = 1;
            }

            if (mx>=595 && mx<=690 && my>=230 && my<=285)
            {
                exit(0);
            }
        }

        if (gameMode==11)
        {
            if (mx>=510 && mx<=750 && my>=30 && my<=59) gameMode = -1;
        }

        if (gameMode==12)
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
        if (enemyNumber<MAX_ENEMY)
        {
            if (enemyNumber%5==0 && enemyNumber%10!=0) lifePotion.alive = 0;

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

            if (enemyNumber%18==0) bulletPotion.alive = 0;

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
    iSetTimer(1500, bossBeamCreate);
    iSetTimer(5, bossBeamMove);

    iInitialize(1300, 680, "Game Window!");

    return 0;
}
