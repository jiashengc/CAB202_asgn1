// gcc ex41.c -std=gnu99 -IZDK -LZDK -lzdk -lncurses -o ex41
#include <stdlib.h>
#include <math.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

#define DELAY 10;

bool game_over;
bool update_screen = true;

sprite_id hero;

void setup(void) {

  

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
