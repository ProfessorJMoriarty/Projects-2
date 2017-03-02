#include <allegro5\allegro.h>

int collision(int PaddleL_x, int PaddleL_y, int bouncer_x, int bouncer_y);
int main() {
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *PaddleL = NULL;
	ALLEGRO_BITMAP *PaddleR = NULL;
	ALLEGRO_BITMAP *bouncer = NULL;
	//these two variables hold the x and y positions of the PaddleL
	//initalize these variables to where you want your PaddleL to start
	float PaddleL_x = 50;
	float PaddleL_y = 50;

	float PaddleR_x = 550;
	float PaddleR_y = 50;

	float bouncer_x = 150;
	float bouncer_y = 150;

	float bouncer_dx = -4.0, bouncer_dy = 4.0;
	//here's our key states. they're all starting as "false" because nothing has been pressed yet.
	//the first slot represents "up", then "down", "left" and "right"
	bool key[2] = { false, false };

	bool key1[2] = { false, false };

	//don't redraw until an event happens
	bool redraw = true;

	//this controls our game loop
	bool doexit = false;

	al_init();

	//get the keyboard ready to use
	al_install_keyboard();

	timer = al_create_timer(.02);

	display = al_create_display(640, 480);

	PaddleL = al_create_bitmap(32, 70);
	PaddleR = al_create_bitmap(32, 70);
	bouncer = al_create_bitmap(32, 32);
	// Bit Maps
	al_set_target_bitmap(bouncer);

	al_clear_to_color(al_map_rgb(255, 100, 100));

	al_set_target_bitmap(PaddleL);

	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(PaddleR);

	al_clear_to_color(al_map_rgb(100, 255, 255));

	al_set_target_bitmap(al_get_backbuffer(display));

	al_clear_to_color(al_map_rgb(255, 200, 100));

	event_queue = al_create_event_queue();

	//these lines tell teh event source what to look for
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	//new! tell the event queue that it should take keyboard events, too
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	//so the game loop is set to act on "ticks" of the timer OR keyboard presses
	//OR the mouse closing the display
	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			//if the box hits the left wall OR the right wall
			if (bouncer_x < 0 || bouncer_x > 640 - 32) {
				//flip the x direction
				bouncer_dx = -bouncer_dx;
			}
			//if the box hits the top wall OR the bottom wall
			if (bouncer_y < 0 || bouncer_y > 480 - 32) {
				//flip the y direction
				bouncer_dy = -bouncer_dy;
			}

			//really important code!
			//move the box in a diagonal
			bouncer_x += bouncer_dx;
			bouncer_y += bouncer_dy;

			//if the up button is pressed AND we're still below the top wall,
			//move the box "up" by 4 pixels
			if (key[0] && PaddleL_y >= 0) {
				PaddleL_y -= 4.0;
			}

			if (key1[0] && PaddleR_y > 0) {
				PaddleR_y -= 4.0;
			}
			//if the down button is pressed AND we're still above the bottom wall
			//move the box "down" by 4 pixels
			if (key[1] && PaddleL_y <= 480 - 70) {
				PaddleL_y += 4.0;
			}

			if (key1[1] && PaddleR_y <= 480 - 70) {
				PaddleR_y += 4.0;
			}

			//redraw at every tick of the timer
			redraw = true;

			//call again for paddleR 
			if (collision(PaddleL_x, PaddleL_y, bouncer_x, bouncer_y))
				bouncer_dx = -bouncer_dx;

			//if an event happened, you better redraw
			redraw = true;
		}

		//////////////////////////////////////////////////////////////////
		//kill program if the user clicks the exit button
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the algorithm that turns key presses into events
		//a "key down" event is when a key is pushed
		//while a "key up" event is when a key is released

		//has something been pressed on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			//"keycode" holds all the different keys on the keyboard
			switch (ev.keyboard.keycode) {

				//if the up key has been pressed
			case ALLEGRO_KEY_UP:
				key1[0] = true;
				break;

			case ALLEGRO_KEY_W:
				key[0] = true;
				break;

				//if the down key has been pressed
			case ALLEGRO_KEY_DOWN:
				key1[1] = true;
				break;

			case ALLEGRO_KEY_S:
				key[1] = true;
				break;

			}
		}
		//has something been released on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key1[0] = false;
				break;

			case ALLEGRO_KEY_W:
				key[0] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key1[1] = false;
				break;

			case ALLEGRO_KEY_S:
				key[1] = false;
				break;


				//kill the program if someone presses escape
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}
		//RENDER SECTION
		//if the clock ticked but no other events happened, don't bother redrawing
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			//paint black over the old screen, so the old PaddleL dissapears
			al_clear_to_color(al_map_rgb(0, 0, 0));

			//the algorithm above just changes the x and y coordinates
			//here's where the bitmap is actually drawn somewhere else
			al_draw_bitmap(PaddleL, PaddleL_x, PaddleL_y, 0);

			al_draw_bitmap(PaddleR, PaddleR_x, PaddleR_y, 0);

			//at the coordinate position bouncer_x, bouncer_y
			al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);

			al_flip_display();

		}
	}

	al_destroy_bitmap(PaddleL);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;



}
int collision(int PaddleL_x, int PaddleL_y, int bouncer_x, int bouncer_y) {
	if ((PaddleL_x > bouncer_x + 32) ||//ball is to the left
		(PaddleL_x +32< bouncer_x) ||
		(PaddleL_y > bouncer_y + 32) ||
		(PaddleL_y +70 < bouncer_y) 
	
		)
	{
		return 0;//no collision

	}
	else
		return 1;//collision
}
