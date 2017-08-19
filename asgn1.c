// gcc asgn1.c -std=gnu99 -IZDK -LZDK -lzdk -lncurses -o Casgn1
#include <stdlib.h>
#include <math.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

#define DELAY 10
#define SCREEN_WIDTH (screen_width() - 1)
#define SCREEN_HEIGHT (screen_height() - 1)

bool game_over;
bool update_screen = true;

sprite_id hero;

char * hero_sprite =
/**/	" o "
/**/	"/|\\"
/**/	"/ \\";


void setup(void) {

    // Initialise sprites


    // Draw border lines
    draw_line(0, 1, SCREEN_WIDTH, 1, '-'); // TOP LINE
    draw_line(0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, '='); // BOTTOM LINE
    draw_line(0, 2, 0, SCREEN_HEIGHT - 1, '|'); // LEFT LINE
    draw_line(SCREEN_WIDTH, 2, SCREEN_WIDTH, SCREEN_HEIGHT - 1, '|'); // RIGHT LINE

}

void process() {

}

void cleanup(void) {

}

int main(void) {
  setup_screen();

  #if defined(AUTO_SAVE_SCREEN)
      auto_save_screen(true);
  #endif

  setup();

  while(!game_over) {
      process();

      if (update_screen) {
          show_screen();
      }

      timer_pause(DELAY);
  }

  cleanup();

  return 0;
}
