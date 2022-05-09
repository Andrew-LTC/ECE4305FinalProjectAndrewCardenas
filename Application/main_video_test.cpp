/*****************************************************************//**
 * @file main_video_test.cpp
 *
 * @brief Basic test of 4 basic i/o cores
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "vga_core.h"
#include "sseg_core.h"
#include "spi_core.h"
#include "ps2_core.h"


void test_start(GpoCore *led_p) {
   int i;

   for (i = 0; i < 20; i++) {
      led_p->write(0xff00);
      sleep_ms(50);
      led_p->write(0x0000);
      sleep_ms(50);
   }
}

/**
 * check bar generator core
 * @param bar_p pointer to Gpv instance
 */
void bar_check(GpvCore *bar_p) {
   bar_p->bypass(0);
   sleep_ms(3000);
}

/**
 * check color-to-grayscale core
 * @param gray_p pointer to Gpv instance
 */
void gray_check(GpvCore *gray_p) {
   gray_p->bypass(0);
   sleep_ms(3000);
   gray_p->bypass(1);
}

/**
 * check osd core
 * @param osd_p pointer to osd instance
 */
void osd_check(OsdCore *osd_p) {
   osd_p->set_color(0x0f0, 0x001); // dark gray/green
   osd_p->bypass(0);
   osd_p->clr_screen();
   for (int i = 0; i < 64; i++) {
      osd_p->wr_char(8 + i, 20, i);
      osd_p->wr_char(8 + i, 21, 64 + i, 1);
      sleep_ms(100);
   }
   sleep_ms(3000);
}

/**
 * test frame buffer core
 * @param frame_p pointer to frame buffer instance
 */
void frame_check(FrameCore *frame_p) {
   int x, y, color;

   frame_p->bypass(0);
   for (int i = 0; i < 10; i++) {
      frame_p->clr_screen(0x008);  // dark green
      for (int j = 0; j < 20; j++) {
         x = rand() % 640;
         y = rand() % 480;
         color = rand() % 512;
         frame_p->plot_line(400, 200, x, y, color);
      }
      sleep_ms(300);
   }
   sleep_ms(3000);
}

/**
 * test ghost sprite
 * @param ghost_p pointer to mouse sprite instance
 */
void mouse_check(SpriteCore *mouse_p) {
   int x, y;

   mouse_p->bypass(0);
   // clear top and bottom lines
   for (int i = 0; i < 32; i++) {
      mouse_p->wr_mem(i, 0);
      mouse_p->wr_mem(31 * 32 + i, 0);
   }

   // slowly move mouse pointer
   x = 0;
   y = 0;
   for (int i = 0; i < 80; i++) {
      mouse_p->move_xy(x, y);
      sleep_ms(50);
      x = x + 4;
      y = y + 3;
   }
   sleep_ms(3000);
   // load top and bottom rows
   for (int i = 0; i < 32; i++) {
      sleep_ms(20);
      mouse_p->wr_mem(i, 0x00f);
      mouse_p->wr_mem(31 * 32 + i, 0xf00);
   }
   sleep_ms(3000);
}

/**
 * test ghost sprite
 * @param ghost_p pointer to ghost sprite instance
 */
void ghost_check(SpriteCore *ghost_p) {
   int x, y;

   // slowly move mouse pointer
   ghost_p->bypass(0);
   ghost_p->wr_ctrl(0x1c);  //animation; blue ghost
   x = 0;
   y = 100;
   for (int i = 0; i < 156; i++) {
      ghost_p->move_xy(x, y);
      sleep_ms(100);
      x = x + 4;
      if (i == 80) {
         // change to red ghost half way
         ghost_p->wr_ctrl(0x04);
      }
   }
   sleep_ms(3000);
}

void obstacle1_check(SpriteCore *obstacle_p){
	int x, y;

	obstacle_p->bypass(0);
	obstacle_p->wr_ctrl(0x1C);
	x = 0;
	y = 100;
	for (int i = 0; i < 156; i++) {
		obstacle_p->move_xy(x, y);
		sleep_ms(100);
		x = x + 4;
		if (i == 80) {
			// change to red ghost half way
			obstacle_p->wr_ctrl(0x04);
		}
	}
	sleep_ms(3000);
}

void obstacle2_check(SpriteCore *obstacle_p){
	int x, y;

	obstacle_p->bypass(0);
	obstacle_p->wr_ctrl(0x1C);
	x = 0;
	y = 100;
	for (int i = 0; i < 156; i++) {
		obstacle_p->move_xy(x, y);
		sleep_ms(100);
		x = x + 4;
		if (i == 80) {
			// change to red ghost half way
			obstacle_p->wr_ctrl(0x04);
		}
	}
	sleep_ms(3000);
}

void obstacle3_check(SpriteCore *obstacle_p){
	int x, y;

	obstacle_p->bypass(0);
	obstacle_p->wr_ctrl(0x1C);
	x = 0;
	y = 100;
	for (int i = 0; i < 156; i++) {
		obstacle_p->move_xy(x, y);
		sleep_ms(100);
		x = x + 4;
		if (i == 80) {
			// change to red ghost half way
			obstacle_p->wr_ctrl(0x04);
		}
	}
	sleep_ms(3000);
}

void car_check(SpriteCore *car_p){
	int x, y;

	// slowly move mouse pointer
	car_p->bypass(0);
	car_p->wr_ctrl(0x1c);  //animation; blue ghost
	x = 0;
	y = 50;
	for (int i = 0; i < 156; i++) {
		car_p->move_xy(x, y);
		sleep_ms(100);
		x = x + 4;
		if (i == 80) {
			// change to red ghost half way
			car_p->wr_ctrl(0x04);
		}
	}
	sleep_ms(3000);
}

void start_screen(FrameCore *frame_p, OsdCore *osd_p){
	frame_p->bypass(0);
	//set screen color background
	frame_p->clr_screen(0x000);
	//draw title screen
	osd_p->set_color(0xf00, 0x000);
	osd_p->bypass(0);
	osd_p->clr_screen();

	char GameTxt[] = {'S','T','R','E','E','T',' ','R','A','C','E','R','S'};
	char StartTxt[] = {'P','R','E','S','S',' ','A','N','Y',' ','K','E','Y'};
	for(int i = 0; i < 13; i++){
		osd_p->wr_char(i + 34, 15, StartTxt[i]);
		osd_p->wr_char(i + 34, 10, GameTxt[i]);
	}
}

void game_screen(FrameCore *frame_p, OsdCore *osd_p){
	//set screen color background
	frame_p->clr_screen(0x000);
	//fill sides of screen
	frame_p->fill_screen(0,0,160,480,0x0f0);
	frame_p->fill_screen(480,0,640,480,0x0f0);
	//draw lines to mimic road from top down view
	//divide screen into 4  playable sections
	frame_p->plot_line(160, 0, 160, 480, 0xfff);
	frame_p->plot_line(240, 0, 240, 480, 0xfff);
	frame_p->plot_line(320, 0, 320, 480, 0xfff);
	frame_p->plot_line(400, 0, 400, 480, 0xfff);
	frame_p->plot_line(480, 0, 480, 480, 0xfff);
	//draw score tracker in top left of screen
	osd_p->set_color(0xf00, 0x000);
	osd_p->clr_screen();
	char ScoreTxt[] = {'S','C','O','R','E',':'};
	char BoostTxt[] = {'B','O','O','S','T',':'};
	char SpeedTxt[] = {'S','P','E','E','D',':'};
	osd_p->wr_char(3, 11, '%');
	for(int i = 0; i < 6; i++){
		osd_p->wr_char(i, 0, ScoreTxt[i]);
		osd_p->wr_char(i, 10, BoostTxt[i]);
		osd_p->wr_char(i, 20, SpeedTxt[i]);
	}
}

void game_over_screen(FrameCore *frame_p, OsdCore *osd_p){
	frame_p->clr_screen(0x000);

	osd_p->set_color(0xf00, 0x000);
	osd_p->clr_screen();
	char OverTxt[] = {'G','A','M','E',' ','O','V','E','R'};
	for(int i = 0; i < 9; i++){
		osd_p->wr_char(i + 38, 10, OverTxt[i]);
	}
	char RestTxt[] = {'P','L','A','Y',' ','A','G','A','I','N','?'};
	for(int i = 0; i < 11; i++){
		osd_p->wr_char(i + 37, 15, RestTxt[i]);
	}
}

void update_screen(FrameCore *frame_p, OsdCore *osd_p, int score, int boost, int speed){
	char chArr[] = {'0','1','2','3','4','5','6','7','8','9'};
	osd_p->clr_screen();

	//draw score tracker in top left of screen
	osd_p->set_color(0xf00, 0x000);
	osd_p->clr_screen();
	char ScoreTxt[] = {'S','C','O','R','E',':'};
	char BoostTxt[] = {'B','O','O','S','T',':'};
	char SpeedTxt[] = {'S','P','E','E','D',':'};
	osd_p->wr_char(3, 11, '%');
	for(int i = 0; i < 6; i++){
		osd_p->wr_char(i, 0, ScoreTxt[i]);
		osd_p->wr_char(i, 10, BoostTxt[i]);
		osd_p->wr_char(i, 20, SpeedTxt[i]);
	}
	//write score
	osd_p->wr_char(0, 1, chArr[(score / 100000000)]);
	osd_p->wr_char(1, 1, chArr[(score / 10000000) % 10]);
	osd_p->wr_char(2, 1, chArr[(score / 1000000) % 100 % 10]);
	osd_p->wr_char(3, 1, chArr[(score / 100000) % 1000 % 100 % 10]);
	osd_p->wr_char(4, 1, chArr[(score / 10000) % 10000 % 1000 % 100 % 10]);
	osd_p->wr_char(5, 1, chArr[(score / 1000) % 100000 % 10000 % 1000 % 100 % 10]);
	osd_p->wr_char(6, 1, chArr[(score / 100) % 1000000 % 100000 % 10000 % 1000 % 100 % 10]);
	osd_p->wr_char(7, 1, chArr[(score / 10) % 10000000 % 1000000 % 100000 % 10000 % 1000 % 100 % 10]);
	osd_p->wr_char(8, 1, chArr[score % 100000000 % 10000000 % 1000000 % 100000 % 10000 % 1000 % 100 % 10]);
	//write boost
	osd_p->wr_char(0, 11, chArr[(boost / 100)]);
	osd_p->wr_char(1, 11, chArr[(boost / 10) % 10]);
	osd_p->wr_char(2, 11, chArr[(boost % 100) % 10]);
	//write speed
	//	osd_p->wr_char(0, 21, chArr[(speed / 10) % 10]);
	//	osd_p->wr_char(1, 21, chArr[(speed % 100) % 10]);
	//show speed
	osd_p->wr_char(2,22,30);
	if(speed == 15){
		osd_p->wr_char(2,21,30);
	}
}

void dev_chk(Ps2Core *ps2_p){
	int id;
	uart.disp("\n\rPS2 device (1-keyboard / 2-mouse): ");
	id = ps2_p->init();
	uart.disp(id);
	uart.disp("\n\r");
}

int key_press(Ps2Core *ps2_p){
	char ch;
	while(!ps2_p->get_kb_ch(&ch)){};
	return 1;
}

int key_chk(Ps2Core *ps2_p){
	char ch;
	return(ps2_p->get_kb_ch(&ch));
}

int key_chk_ch(Ps2Core *ps2_p){
	char ch;
	ps2_p->get_kb_ch(&ch);
	return ch;
}

//one of 4 valid locations for car to be located
int draw_car(SpriteCore *car_p, int loc){
	int x;
	int y = 460;
	car_p->bypass(0);
	car_p->wr_ctrl(0x04);
	switch(loc){
	case 0:
		x = 192;
		break;
	case 1:
		x = 272;
		break;;
	case 2:
		x = 352;
		break;
	case 3:
		x = 432;
		break;
	default:
		x = 192;
		break;
	}
	car_p->move_xy(x,y);
	return loc;
}

void color_car(SpriteCore *car_p){
	car_p->wr_ctrl(0x1c);
}

int car_position(SpiCore *spi_p){
	const uint8_t RD_CMD = 0x0b;
	const uint8_t DATA_REG = 0x08;
	const float raw_max = 127.0/2.0;

	int8_t Xraw, Yraw, Zraw;
	float x,y,z;

	spi_p ->set_freq(400000);
	spi_p->set_mode(0,0);

	//read x/y/z g values
	spi_p->assert_ss(0);
	spi_p->transfer(RD_CMD);
	spi_p->transfer(DATA_REG);
	Xraw = spi_p->transfer(0x00);
	Yraw = spi_p->transfer(0x00);
	Zraw = spi_p->transfer(0x00);
	spi_p->deassert_ss(0);
	x = (float) Xraw/raw_max;
	y = (float) Yraw/raw_max;
	z = (float) Zraw/raw_max;
	float Mag = abs(x) + abs(y) + abs(z);

//	uart.disp("MAG:");
//	uart.disp(Mag);
//	uart.disp("\n\r");
//	uart.disp(x);
//	uart.disp(" ");
//	uart.disp(y);
//	uart.disp(" ");
//	uart.disp(z);
//	uart.disp("\n\r");

	static int loc = 0;
	//large enough shift to indicate attempt at changing lanes
	if(Mag > 1.25){
		//if turning to the right
		if(y < 0)
			if(loc < 3)	//can't go past far right position
				loc++;
		//if turning to the left
		if(y > 0)
			if(loc > 0)	//can't go past far left position
				loc--;
	}
	return loc;
}

//start at the top of screen and move down one of 4 locations
void draw_obstacle(SpriteCore *obstacle_p, int Hloc, int Vloc){
	int x;
	obstacle_p->bypass(0);
	obstacle_p->wr_ctrl(0x04);

	switch(Hloc){
		case 0:
			x = 192;
			break;
		case 1:
			x = 272;
			break;
		case 2:
			x = 352;
			break;
		case 3:
			x = 432;
			break;
		default:
			x = 192;
			break;
		}
	obstacle_p->move_xy(x,Vloc);
}

// external core instantiation
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
FrameCore frame(FRAME_BASE);
//GpvCore bar(get_sprite_addr(BRIDGE_BASE, V7_BAR));
GpvCore gray(get_sprite_addr(BRIDGE_BASE, V6_GRAY));
//SpriteCore ghost(get_sprite_addr(BRIDGE_BASE, V3_GHOST), 1024);
//SpriteCore mouse(get_sprite_addr(BRIDGE_BASE, V1_MOUSE), 1024);
OsdCore osd(get_sprite_addr(BRIDGE_BASE, V2_OSD));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));

SpriteCore car(get_sprite_addr(BRIDGE_BASE, V5_USER5), 1024);
SpriteCore obstacle4(get_sprite_addr(BRIDGE_BASE, V7_BAR), 1024);
SpriteCore obstacle3(get_sprite_addr(BRIDGE_BASE, V4_USER4), 1024);
SpriteCore obstacle2(get_sprite_addr(BRIDGE_BASE, V3_GHOST), 1024);
SpriteCore obstacle1(get_sprite_addr(BRIDGE_BASE, V1_MOUSE), 1024);

SpiCore spi(get_slot_addr(BRIDGE_BASE, S9_SPI));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));

int main() {
	//check for keyboard
	dev_chk(&ps2);
	while (1) {
		// bypass all cores
		frame.bypass(1);
		gray.bypass(1);
		osd.bypass(1);
		obstacle1.bypass(1);
		obstacle2.bypass(1);
		obstacle3.bypass(1);
		obstacle4.bypass(1);
		car.bypass(1);
		sleep_ms(3000);

		start_screen(&frame, &osd);
		//wait for user to hit a key to start
		key_press(&ps2);
		//draw game screen
		game_screen(&frame, &osd);

		//START GAME
		//game state VARs
		bool GameOver = false;
		int count0 = 0, count1 = 0, count2 = 0, count3 = 0;
		int carLoc = 0;
		int speed = 10;
		int boost = 100;
		bool invincible = false;
		int score = 0;
		int variance0, variance1, variance2, variance3;

		while(!GameOver){
			//while a key isn't pressed
			while(!key_chk(&ps2)){
				//CAR CONTROL
				//move car and check where it is
				carLoc = draw_car(&car,car_position(&spi));
				uart.disp("Car Position: ");
				uart.disp(carLoc);
				uart.disp("\n\r");

				//OBSTACLE CONTROL
				//next draw obstacle at updated position
				draw_obstacle(&obstacle1, 0, count0);
				draw_obstacle(&obstacle2, 1, count1);
				draw_obstacle(&obstacle3, 2, count2);
				draw_obstacle(&obstacle4, 3, count3);

				//regulate boost
				if(invincible){
					if(boost > 0){
						boost = boost - 5;
						color_car(&car);
					}
					else
						invincible = false;
				}
				else
					if(boost < 100)
						boost++;
				uart.disp("INVIN: ");
				uart.disp(invincible);
				uart.disp("\n\r");
				uart.disp("BOOST: ");
				uart.disp(boost);
				uart.disp("\n\r");

				//COLLISION CONTROL
				//cannot collide while invincible
				if(!invincible){
					//check if first obstacle at collision location
					if(carLoc == 0){
						if(count0 >= 460){
							uart.disp("Game Over");
							uart.disp("\n\r");
							GameOver = true;
							break;
						}
					}
					if(carLoc == 1){
						if(count1 >= 460){
							uart.disp("Game Over");
							uart.disp("\n\r");
							GameOver = true;
							break;
						}
					}
					if(carLoc == 2){
						if(count2 >= 460){
							uart.disp("Game Over");
							uart.disp("\n\r");
							GameOver = true;
							break;
						}
					}
					if(carLoc == 3){
						if(count3 >= 460){
							uart.disp("Game Over");
							uart.disp("\n\r");
							GameOver = true;
							break;
						}
					}
				}

				//GAME STATE CONTROL
				//increment game state
				score = score + ((speed/5) - 1);

				count0 = (count0 + speed + variance0);
				if(count0 > 480){
					count0 = 0;
					variance0 = rand() % 10 + 1;
				}

				count1 = (count1 + speed + variance1);
				if(count1 > 480){
					count1 = 0;
					variance1 = rand() % 10 + 1;
				}

				count2 = (count2 + speed + variance2);
				if(count2 > 480){
					count2 = 0;
					variance2 = rand() % 10 + 1;
				}

				count3 = (count3 + speed + variance3);
				if(count3 > 480){
					count3 = 0;
					variance3 = rand() % 10 + 1;
				}

				uart.disp("score: ");
				uart.disp(score);
				uart.disp("\n\r");
				//update screen with new values
				update_screen(&frame, &osd, score, boost, speed);
				sleep_ms(150);
			}
			uart.disp("key pressed");
			uart.disp("\n\r");

			//BOOST CONTROL
			//check for boost input
			if(key_chk_ch(&ps2) == ' ') {
				uart.disp("space bar press");
				uart.disp("\n\r");
				if(boost == 100)
					invincible = true;
			}

			//SPEED CONTROL
			//check for speed input
			//speed up
			if(key_chk_ch(&ps2) == 'w'){
				uart.disp("w press");
				uart.disp("\n\r");
				if(speed < 15)
					speed = speed + 5;
				uart.disp("speed: ");
				uart.disp(speed);
				uart.disp("\n\r");
			}
			//slow down
			if(key_chk_ch(&ps2) == 's'){
				uart.disp("s press");
				uart.disp("\n\r");
				if(speed > 10)
					speed = speed - 5;
				uart.disp("speed: ");
				uart.disp(speed);
				uart.disp("\n\r");
			}
		}
		obstacle1.bypass(1);
		obstacle2.bypass(1);
		obstacle3.bypass(1);
		obstacle4.bypass(1);
		car.bypass(1);
		game_over_screen(&frame,&osd);
		key_press(&ps2);
		GameOver = false;
	}
}
