// gcc asgn1.c -std=gnu99 -IZDK -LZDK -lzdk -lncurses -o Casgn1
#include <stdlib.h>
#include <math.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

#define DELAY 10
#define SCREEN_WIDTH (screen_width() - 1)
#define SCREEN_HEIGHT (screen_height() - 1)

bool game_over = false;
bool update_screen = true;

// Game levels
bool level_one_start = false;
bool level_one_fin = false;

// Hero movement
double hero_dx = 0;
double hero_dy = 0;

sprite_id hero;
sprite_id exit_door;
sprite_id zombie;

char * hero_sprite =
/**/	" o "
/**/	"/|\\"
/**/	"/ \\";

char * exit_sprite =
/**/	"EXIT"
/**/	"|  |"
/**/	"| .|"
/**/  "|  |";

char * zombie_sprite =
/**/	"ZZZZ"
/**/	"  Z "
/**/	" Z  "
/**/	"ZZZZ";

void level_one(void);

void setup(void) {

    // Initialise sprites
    hero = sprite_create(2, SCREEN_HEIGHT - 3, 3, 3, hero_sprite);
    exit_door = sprite_create(SCREEN_WIDTH - 6, SCREEN_HEIGHT - 4, 4, 4, exit_sprite);
    zombie = sprite_create(SCREEN_WIDTH - 6, SCREEN_HEIGHT - 4, 4, 4, zombie_sprite);

    // Set defaults
    sprite_turn_to(hero, 0, 0);

    // Start Level one
    level_one();
}

void re_update(void) {
    // Draw border lines
    draw_line(0, 1, SCREEN_WIDTH, 1, '-'); // TOP LINE
    draw_line(0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, '='); // BOTTOM LINE
    draw_line(0, 2, 0, SCREEN_HEIGHT - 1, '|'); // LEFT LINE
    draw_line(SCREEN_WIDTH, 2, SCREEN_WIDTH, SCREEN_HEIGHT - 1, '|'); // RIGHT LINE

    hero_dx = sprite_dx(hero);
    hero_dy = sprite_dy(hero);
}



void level_one(void) {

    // Draw the sprites
    sprite_draw(hero);
    sprite_draw(exit_door);
    sprite_draw(zombie);

    // Draw that one ranom platform line
    draw_line(SCREEN_WIDTH * 0.33, SCREEN_HEIGHT * 0.77 , SCREEN_WIDTH * 0.66, SCREEN_HEIGHT * 0.77, '=');

    // Move the zombine
    sprite_turn_to(zombie, -0.15, 0);

    level_one_start = true;
}

void process(void) {
    clear_screen();
    re_update();

    int input = get_char();

    // Within LEVEL ONE
    if (level_one_start && !level_one_fin) {

        // Check if zombie has hit the wall
        if (sprite_x(zombie) < 1 || sprite_x(zombie) > SCREEN_WIDTH - 3) {
            sprite_turn(zombie, 180);
        }

        // Check if the player has hit the player
        if (round(sprite_x(zombie)) == round(sprite_x(hero))
         && round(sprite_y(zombie)) == round(sprite_y(hero))) {
            game_over = true;
        }

        sprite_step(zombie);
        sprite_draw(zombie);
    }


    // Check for hero movements
    // Check if the hero is moving right
    if (input == 'd') {
        if (hero_dx > 0) {
            sprite_turn_to(hero, hero_dx * 2, hero_dy);
        } else if (hero_dx < 0) {
            sprite_turn_to(hero, 0, hero_dy);
        } else {
            sprite_turn_to(hero, hero_dx + 0.2, hero_dy);
        }
    }
    // Check if the hero is moving left
    if (input == 'a') {
        if (hero_dx < 0) {
            sprite_turn_to(hero, hero_dx * 2, hero_dy);
        } else if (hero_dx > 0) {
            sprite_turn_to(hero, 0, hero_dy);
        } else {
            sprite_turn_to(hero, hero_dx - 0.2, hero_dy);
        }
    }

    if (input == 'w') {
        sprite_turn_to(hero, hero_dx, hero_dy - 0.5);
    }

    // Check if the player hit a wall
    if (sprite_x(hero) < 1 || sprite_x(hero) > SCREEN_WIDTH - 3) {
        sprite_back(hero);
        sprite_turn_to(hero, 0, 0);
    }

    sprite_step(hero);
    sprite_draw(exit_door);
    sprite_draw(hero);
}

void cleanup(void) {
    clear_screen();
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
