#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

int main() {

	ALLEGRO_DISPLAY *display = NULL;

	al_init();
	al_init_primitives_addon();

	display = al_create_display(640, 480);

	al_clear_to_color(al_map_rgb(0, 50, 0));
	
	al_draw_filled_triangle(480,350, 145, 350, 320,100, al_map_rgb(255, 255, 0));

	al_draw_filled_triangle(398, 220, 230, 220, 320, 350, al_map_rgb(0, 0, 0));

	

	al_flip_display();

	al_rest(40.0);


}