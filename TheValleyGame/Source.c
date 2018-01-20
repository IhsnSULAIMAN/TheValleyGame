//////////////////////////////////////////////////////////////////////////////////////////////////////////
//																										//
//							 SIMPLE PUZZLE PROJECT BY "IHSAN SULAIMAN"  							    //
//																										//
//////////////////////////////////////////////////////////////////////////////////////////////////////////







#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE 
#endif


#include <stdio.h>
#include <time.h>
#include <string.h>
#include <allegro5\allegro.h>
#include<allegro5\allegro5.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_color.h>
#include<allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

#define SCREEN_W  800
#define SCREEN_H  600


//SET THE FPS FOR THE TIMER
#define FPS 0.1

//make a new type 
typedef char*  string;
//Set global values
int level = 10, DestroyedBlock = 0, TotalScore = 0, BombCome = 0, cntrI, cntrII, IsBomb = 0, BombTime = 170, IntSize = sizeof(int);
int border = 60;
int x = SCREEN_W - 60;
int y = SCREEN_H - 60;
int HighScore = 0;


void NewCol(int W, int BackE[11][6]);
void DESTROY(int i, int j, int BE[11][6], int block);
void FourCol(int BE[11][6]);
void LevelUp(int BEA[11][6]);
void gameover(int Col);



int main(int argc, char **argv)
{

	FILE *HighScoreFile;


	//This codes is for check the high score file and protect it from unwanted big sizes
	HighScoreFile = fopen("GameFiles/HighScore.bin", "r");
	while (fread(&HighScore, sizeof(int), 1, HighScoreFile) != 0)
	{
		cntrI++;
	}
	fclose(HighScoreFile);
	if (cntrI > 25)
	{
		HighScoreFile = fopen("GameFiles/HighScore.bin", "w");
		fwrite(&HighScore, sizeof(int), 1, HighScoreFile);
		fclose(HighScoreFile);
	}
	///////////////////////////////////////////////////////////////////////////////////////

	HighScoreFile = fopen("HighScore.bin", "ab+");



	//initialize the library
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_SAMPLE *BackgroundMusic = NULL;
	ALLEGRO_SAMPLE *BreakBlock = NULL;
	ALLEGRO_SAMPLE *Button = NULL;


	if (!al_init()) {
		fprintf(stderr, "Couldn't initialize allegro!\n");
		return -1;
	}
	if (!al_init_primitives_addon()) {
		fprintf(stderr, "Couldn't initialize primitives addon!\n");
		return -1;
	}
	if (!al_init_image_addon()) {
		fprintf(stderr, "Couldn't initialize image addon!\n");
		return -1;
	}
	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display) {
		fprintf(stderr, "Couldn't create allegro display!\n");
		return -1;
	}

	if (!al_install_audio()) {
		fprintf(stderr, "failed to initialize audio!\n");
		return -1;
	}

	if (!al_init_acodec_addon()) {
		fprintf(stderr, "failed to initialize audio codecs!\n");
		return -1;
	}

	if (!al_reserve_samples(16)) {
		fprintf(stderr, "failed to reserve samples!\n");
		return -1;
	}

	//Open the sound files
	BackgroundMusic = al_load_sample("GameFiles/BackgroundMusic.wav");
	BreakBlock = al_load_sample("GameFiles/Break.wav");
	Button = al_load_sample("GameFiles/Button.wav");
	/////////////////////


	if (!BackgroundMusic) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}
	if (!BreakBlock) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}
	if (!Button) {
		printf("Audio clip sample not loaded!\n");
		return -1;
	}




	al_install_mouse();
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_FONT *Capture_it72 = al_load_ttf_font("Capture_it.ttf", 72, 0);
	ALLEGRO_FONT *font72 = al_load_ttf_font("GameFiles/ASMAN.ttf", 72, 0);
	ALLEGRO_FONT *font24 = al_load_ttf_font("GameFiles/ASMAN.ttf", 24, 0);
	ALLEGRO_FONT *font48 = al_load_ttf_font("GameFiles/ASMAN.ttf", 48, 0);



	//START THE MUSIC
	al_play_sample(BackgroundMusic, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

	//Display color
	al_clear_to_color(al_map_rgb(128, 128, 128));






	//start the wait for the mouse events AND start the timer
	ALLEGRO_EVENT_QUEUE *queue = NULL;
	ALLEGRO_TIMER *timer;
	timer = al_create_timer(FPS);


	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));



	//the values
	int  counter, H, W = 0, swp, dstryx, dstryy, cntr, End = 0, blck = 0, score = 0, FillCol, start = 0, counterII, xpos, ypos, motion = 0, CheckHighScore = 0;
	char text[30];
	char lvl[30];
	char HighScoreText[100];
	string textp;
	string lvlp;
	string HighSp;
	textp = text;
	lvlp = lvl;
	HighSp = HighScoreText;
	int BackEnd[11][6];
	for (counter = 0; counter < 11; counter++) { for (counterII = 0; counterII < 6; counterII++) { BackEnd[counter][counterII] = 11; } }



	border = 60;
	x = SCREEN_W - border;
	y = SCREEN_H - border;

	counter = 0;



	//Load the PNG's

	ALLEGRO_BITMAP  *Start = al_load_bitmap("GameFiles/Start.png");
	ALLEGRO_BITMAP  *PlayNow = al_load_bitmap("GameFiles/PlayNow1.png");
	ALLEGRO_BITMAP  *red = al_load_bitmap("GameFiles/red.png");
	ALLEGRO_BITMAP  *yellow = al_load_bitmap("GameFiles/yellow.png");
	ALLEGRO_BITMAP  *blue = al_load_bitmap("GameFiles/blue.png");
	ALLEGRO_BITMAP  *green = al_load_bitmap("GameFiles/green.png");
	ALLEGRO_BITMAP  *RedBomb = al_load_bitmap("GameFiles/RedBomb.png");
	ALLEGRO_BITMAP  *YellowBomb = al_load_bitmap("GameFiles/YellowBomb.png");
	ALLEGRO_BITMAP  *BlueBomb = al_load_bitmap("GameFiles/BlueBomb.png");
	ALLEGRO_BITMAP  *GreenBomb = al_load_bitmap("GameFiles/GreenBomb.png");
	ALLEGRO_BITMAP  *background = al_load_bitmap("GameFiles/background.png");
	ALLEGRO_BITMAP  *Arrow = al_load_bitmap("GameFiles/Arrow.png");
	ALLEGRO_BITMAP  *DBlock = al_load_bitmap("GameFiles/DBlock.png");
	ALLEGRO_BITMAP  *HelpPage = al_load_bitmap("GameFiles/HelpPage.png");
	ALLEGRO_BITMAP  *HighScoreImage = al_load_bitmap("GameFiles/HighScoreImage.png");

	ALLEGRO_EVENT events;


	start = clock();



	al_start_timer(timer);
	al_wait_for_event(queue, &events);




	//The StartPage
StartPage: while (1)
{
	if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { goto AllThingDone; }
	fseek(HighScoreFile, -1 * IntSize, SEEK_END);
	fread(&HighScore, sizeof(int), 1, HighScoreFile);
	al_draw_bitmap(Start, 0, 0, 0);
	al_draw_text(font72, al_map_rgb(250, 255, 255), 400, 120, ALLEGRO_ALIGN_CENTRE, "THE VALLEY");
	al_draw_bitmap(PlayNow, 300, 240, 0);
	snprintf(HighSp, 50, "High Score: %d.", HighScore);
	al_draw_text(font24, al_map_rgb(255, 255, 255), 400, 410, ALLEGRO_ALIGN_CENTRE, HighSp);

	al_flip_display();
	al_wait_for_event(queue, &events);
	if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && events.mouse.button & 1)
	{

		if (events.mouse.x > 300 && events.mouse.x < 500)
		{	//Play Now button
			if (events.mouse.y > 240 && events.mouse.y < 300)
			{
				al_play_sample(Button, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				FourCol(BackEnd);  break;
			}

		}

	}
}


		   //start the game loop

		   while (events.type != ALLEGRO_EVENT_DISPLAY_CLOSE)
		   {
			   al_wait_for_event(queue, &events);


			   //score to text
			   snprintf(textp, 30, "Score: %d.", TotalScore);



			   //fill the empty row's 
			   for (cntr = 0; cntr < 6; cntr++)
			   {
				   for (swp = 0; swp < 11; swp++)
				   {
					   for (counter = 0; counter < 5; counter++)
					   {
						   if (BackEnd[swp][counter] == 11 && BackEnd[swp][counter + 1] != 11)
						   {
							   BackEnd[swp][counter] = BackEnd[swp][counter + 1];
							   BackEnd[swp][counter + 1] = 11;
						   }
					   }

				   }
			   }
			   //////////////////////////////////////////////////////////


			   //fill the empty col's
			   for (FillCol = 0; FillCol < 11; FillCol++)
			   {
				   H = 0;
				   if (FillCol != 0)
				   {
					   for (counter = 0; counter < 6; counter++)
					   {
						   if (BackEnd[FillCol - 1][counter] == 11) { H++; }
					   }
				   }
				   if (H == 6 && FillCol != 0)
				   {
					   FillCol--;
					   for (FillCol; FillCol < 11; FillCol++)
					   {
						   for (counter = 0; counter < 6; counter++)
						   {
							   if (FillCol == 10) { BackEnd[FillCol][counter] = 11; }
							   else { BackEnd[FillCol][counter] = BackEnd[FillCol + 1][counter]; }
						   }
					   }

				   }
			   }
			   //////////////////////////////////////


			   //measure the non-empty col's
			   End = 0;
			   for (cntr = 0; cntr < 11; cntr++)
			   {
				   if (BackEnd[cntr][0] != 11)
				   {
					   End++;
				   }
			   }
			   ////////////////////////////////////////////////




			   //draw the blocks

		   Draw: al_draw_bitmap(background, 0, 0, 0);
			   al_draw_bitmap(Arrow, SCREEN_W - 60, SCREEN_H - 60 + 15, 0);
			   al_draw_text(font24, al_map_rgb(255, 255, 255), 730, 10, ALLEGRO_ALIGN_CENTRE, textp);
			   xpos = 0;
			   for (swp = 10; swp >= 0; swp--)
			   {
				   ypos = 1;
				   for (counter = 0; counter < 6; counter++)
				   {
					   if (BackEnd[swp][counter] != 11)
					   {
						   switch (BackEnd[swp][counter])
						   {
							   //Draw the blocks
						   case 0:	al_draw_bitmap(red, x - xpos*border, y - ypos *border, 0); break;
						   case 1:	al_draw_bitmap(yellow, x - xpos*border, y - ypos * border, 0); break;
						   case 2:	al_draw_bitmap(blue, x - xpos*border, y - ypos * border, 0); break;
						   case 3:	al_draw_bitmap(green, x - xpos*border, y - ypos * border, 0); break;
							   //Draw the broken block
						   case 15:	al_draw_bitmap(DBlock, x - xpos*border, y - ypos * border, 0); break;
							   //Draw the bomb
						   case 25:	al_draw_bitmap(RedBomb, x - xpos*border, y - ypos *border, 0); break;
						   case 26:	al_draw_bitmap(YellowBomb, x - xpos*border, y - ypos * border, 0); break;
						   case 27:	al_draw_bitmap(BlueBomb, x - xpos*border, y - ypos * border, 0); break;
						   case 28:	al_draw_bitmap(GreenBomb, x - xpos*border, y - ypos * border, 0); break;
						   }
						   ypos++;
					   }
					   else { motion++; }
				   }
				   if (motion != 6) { xpos++; }
				   motion = 0;
			   }
			   al_flip_display();

			   for (counter = 0; counter < 11; counter++)
			   {
				   for (counterII = 0; counterII < 6; counterII++)
				   {
					   if (BackEnd[counter][counterII] == 15) { BackEnd[counter][counterII] = 11; }
				   }
			   }



			   ////////////////////////////////////////////////


			   //follow the mouse clicks and distroy the blocks

			   if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && events.mouse.button & 1)
			   {

				   {
					   if (events.mouse.x > 200 && events.mouse.x < SCREEN_W && events.mouse.y>180 && events.mouse.y < SCREEN_H - 60)
					   {
						   dstryx = ((events.mouse.x - 200) / 60) - (10 - End);
						   dstryy = (-1 * ((events.mouse.y) / 60)) + 8;
						   blck = BackEnd[dstryx][dstryy];

						   if (blck != 11 && blck != 15 && dstryx >= 0 && dstryy >= 0) { DESTROY(dstryx, dstryy, BackEnd, blck); dstryx = -1; dstryy = -1; }


					   }
					   else if (events.mouse.x > SCREEN_W - 60 && events.mouse.x < SCREEN_W - 30 && events.mouse.y>SCREEN_H - 45 && events.mouse.y < SCREEN_H - 15)
					   {
						   al_play_sample(Button, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL); NewCol(End, BackEnd);
					   }
				   }
			   }
			   if (DestroyedBlock > 1)
			   {
				   al_play_sample(BreakBlock, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			   }


			   ////////////////////////////////////////////////////





			   //save the score
			   score += DestroyedBlock;
			   BombCome += DestroyedBlock;
			   TotalScore += DestroyedBlock;
			   DestroyedBlock = 0;



			   //New Level
			   if (score>75)
			   {

				   for (counter = 0; counter < 11; counter++) { for (counterII = 0; counterII < 6; counterII++) { BackEnd[counter][counterII] = 11; } }
				   level--;
				   score = 0;
				   if (level == 0) { goto EndTheGame; }




				   al_draw_bitmap(background, 0, 0, 0);
				   snprintf(lvlp, 30, "LEVEL %d.", (11 - level));
				   al_draw_text(Capture_it72, al_map_rgb(255, 255, 255), 400, 260, ALLEGRO_ALIGN_CENTRE, lvlp);
				   al_flip_display();

				   dstryx = -1;
				   dstryy = -1;
				   LevelUp(BackEnd);
				   start = clock();

				   goto Draw;
			   }
			   //Move by time
			   else if (clock() >= start + 500 + (300 * level))
			   {
				   NewCol(End, BackEnd);
				   start = clock();

			   }





			   //Add blocks if the screen is empty
			   if (End == 0)
			   {
				   NewCol(End, BackEnd);
			   }




			   //Set the bomb time for level +3
			   if (level < 8) { BombTime = 50; }





			   //check if the Game over
		   EndTheGame:	if (End == 11 || level == 0)
		   {
			   //Check the High Score
			   if (TotalScore > HighScore)
			   {
				   fseek(HighScoreFile, 0, SEEK_END);
				   fwrite(&TotalScore, sizeof(int), 1, HighScoreFile);
				   CheckHighScore = 1;
			   }
			   if (CheckHighScore == 1)
			   {
				   CheckHighScore = 0;
				   al_clear_to_color(al_map_rgb(0, 0, 0));
				   al_draw_bitmap(HighScoreImage, 0, 0, 0);
				   snprintf(HighSp, 50, "The New High Score is  : %d.", TotalScore);
				   al_draw_text(font48, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTRE, HighScoreText);
				   al_flip_display();
				   for (counter = 0; counter < 11; counter++) { for (counterII = 0; counterII < 6; counterII++) { BackEnd[counter][counterII] = 11; } }
				   al_rest(5);
				   TotalScore = 0;
				   BombTime = 230;
				   score = 0;
				   level = 10;
				   BombCome = 0;
				   goto StartPage;
			   }


			   else
			   {
				   for (counter = 0; counter < 11; counter++)
				   {
					   for (counterII = 0; counterII < 6; counterII++) { BackEnd[counter][counterII] = 11; }
				   }
				   gameover(End);
				   TotalScore = 0;
				   BombTime = 230;
				   score = 0;
				   level = 10;
				   BombCome = 0;
				   goto StartPage;
			   }


		   }






		   }
		   //end of game loop



	   AllThingDone:	fclose(HighScoreFile);
		   al_destroy_font(font24);
		   al_destroy_font(font48);
		   al_destroy_font(font72);
		   al_destroy_font(Capture_it72);
		   al_destroy_display(display);

}




//Add blocks or bomb
void NewCol(int W, int BackE[11][6])
{
	int counter;
	srand(time(NULL));

	for (counter = 0; counter < 6; counter++)
	{
		BackE[W][counter] = rand() % 4; al_rest(0.2);
	}
	if (BombCome>BombTime) { BackE[W][rand() % 6] = (rand() % 4) + 25; BombCome = 0; }

}
/////////////////////



// Add 4 col's
void FourCol(int BE[11][6])
{
	for (int x = 0; x < 4; x++) { NewCol(x, BE); }
}
/////////////



//Level up
void LevelUp(int BEA[11][6])
{
	al_rest(2);
	FourCol(BEA);
}
////////////


//Block destroy function////////////////////////////////////////////////////////////
void DESTROY(int i, int j, int BE[11][6], int block)
{

BombIsThere: if (IsBomb == 1)  // if the code see the bomb will come to here again
{
	IsBomb = 0;
	for (cntrI = 0; cntrI < 11; cntrI++)
	{
		for (cntrII = 0; cntrII < 6; cntrII++)
		{
			if (BE[cntrI][cntrII] == block || BE[cntrI][cntrII] == block - 25 || BE[cntrI][cntrII] == block + 25) { BE[cntrI][cntrII] = 15; };

		}
	}
	DestroyedBlock += 5;
}


			 else
			 {
				 //The first "if" check if there is bomb
				 //The second "if" check the blocks
				 if (i != 11 && BE[i + 1][j] == block - 25 || BE[i + 1][j] == block + 25)
				 {
					 IsBomb = 1; goto BombIsThere;
				 }

				 if (i != 11 && BE[i + 1][j] == block) { BE[i][j] = 15;  DestroyedBlock += 1; DESTROY(i + 1, j, BE, block);	BE[i + 1][j] = 15; DestroyedBlock += 1; }

				 if (j != 11 && BE[i][j + 1] == block - 25 || BE[i][j + 1] == block + 25)
				 {
					 IsBomb = 1; goto BombIsThere;
				 }

				 if (j != 11 && BE[i][j + 1] == block) { BE[i][j] = 15;  DestroyedBlock += 1; DESTROY(i, j + 1, BE, block); BE[i][j + 1] = 15; DestroyedBlock += 1; }

				 if (i != 0 && BE[i - 1][j] == block - 25 || BE[i - 1][j] == block + 25)
				 {
					 IsBomb = 1; goto BombIsThere;
				 }

				 if (i != 0 && BE[i - 1][j] == block) { BE[i][j] = 15;  DestroyedBlock += 1; DESTROY(i - 1, j, BE, block); BE[i - 1][j] = 15; DestroyedBlock += 1; }

				 if (j != 0 && BE[i][j - 1] == block - 25 || BE[i][j - 1] == block + 25)
				 {
					 IsBomb = 1; goto BombIsThere;
				 }

				 if (j != 0 && BE[i][j - 1] == block) { BE[i][j] = 15; DestroyedBlock += 1; DESTROY(i, j - 1, BE, block); BE[i][j - 1] = 15; DestroyedBlock += 1; }
			 }

}
/////////////////////////////////////////////////////////////////////////





// "End the game" function
void gameover(int Col)
{
	ALLEGRO_BITMAP  *GameOver = al_load_bitmap("GameOver.png");

	al_draw_bitmap(GameOver, 0, 0, 0);
	al_flip_display();
	al_rest(5);

}
//////////////////////////