// gcc asgn1.c -std=gnu99 -IZDK -LZDK -lzdk -lncurses -o Casgn1
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

#define DELAY 10
#define SCREEN_WIDTH (screen_width() - 1)
#define SCREEN_HEIGHT (screen_height() - 1)

// Game stuff
bool game_over = false;
bool update_screen = true;

// Game levels
bool level_one_start = false;
bool level_one_fin = false;

// Hero movement
double hero_dx = 0;
double hero_dy = 0;
bool on_platform = false;

// Stats
int delay = 0;
int minutes = 0;
int seconds = 0;
int lives = 10;
int level = 1;
int score = 0;

// Time
timer_id timer;

// Sprites
sprite_id hero;
sprite_id exit_door;
sprite_id zombie;
sprite_id platform;

// Sprite vectors
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

char * platform_sprite =
"====================";

void level_one(void);

bool process_collision(sprite_id obj_1, sprite_id obj_2) {
	// Get platform and bird screen locations.
	int hx = round(sprite_x(obj_1)), hy = round(sprite_y(obj_1));
	int ox = round(sprite_x(obj_2)), oy = round(sprite_y(obj_2));

	// Check for collision
	bool collided = true;

	if ( hx >= ox + sprite_width(obj_2)) collided = false;
	if ( hy >= oy + sprite_height(obj_2)) collided = false;
	if ( ox >= hx + sprite_width(obj_1)) collided = false;
	if ( oy >= hy + sprite_height(obj_1)) collided = false;

	if ( collided ) {
		// (e) If bird hit platform
		if (obj_1 == hero) {
			// (e.a) declare floating point variables dx and dy and
			// initialise them with the velocity components of the bird.
			double dx = sprite_dx(hero), dy = sprite_dy(hero);

			// (e.b) If bird has struck the platform from below
			// negate dy but leave dx alone. The bird is bumping from
			// below if the top of the bird is level with the platform
			// and the bird is moving upwards.
			if ( hy == oy + sprite_height(hero) - 1 && dy < 0 ) {
				dy = -dy;
			}

			// (e.c) Otherwise, if the bird has bumped into the left end
			// then quench horizontal velocity. The bird bumps the left edge if
			// it is moving to the right and the right-most edge overlaps
			// the left edge of the platform.
			else if ( hx + sprite_width(hero) - 1 == ox && dx > 0 ) {
				dx = 0;
			}

			// (e.d) Otherwise, if the bird has bumped into the right end
			// then quench horizontal velocity.The bird bumps the right edge if
			// it is moving to the left and the left-most edge overlaps
			// the right edge of the platform.
			else if ( hx == ox + sprite_width(obj_2) - 1 && dx < 0 ) {
				dx = 0;
			}

			// (e.e) Otherwise, we landed on the top, so we quench the
			// vertical velocity, so the birdcan slide along the top of the
			// platform.
			else {
				dy = 0;
        on_platform = true;
			}

			// (e.f) Make bird take one step backward, then turn bird
			// to move in new (dx,dy) direction.
			sprite_back(hero);
			sprite_turn_to(hero, dx, dy);
		}
		// (f) Otherwise, if platform moved, push bird down until
		// not overlapping the platform.
		else {
			sprite_move_to(hero, sprite_x(hero), sprite_y(obj_2) + sprite_height(obj_2));
		}
	}

  return collided;
}

void setup(void) {

    int i = 0;
    int length = (int)SCREEN_WIDTH * 0.66 - (int)SCREEN_HEIGHT * 0.33;
    const char * box = "=";

    // Initialise sprites
    hero = sprite_create(2, SCREEN_HEIGHT - 3, 3, 3, hero_sprite);
    exit_door = sprite_create(SCREEN_WIDTH - 6, SCREEN_HEIGHT - 4, 4, 4, exit_sprite);
    zombie = sprite_create(SCREEN_WIDTH - 6, SCREEN_HEIGHT - 4, 4, 4, zombie_sprite);
    platform = sprite_create(SCREEN_WIDTH * 0.43, SCREEN_HEIGHT * 0.77, 20, 1, platform_sprite);

    // Initialise timer
    timer = create_timer(10);

    // Set defaults
    sprite_turn_to(hero, 0, 0);

    // Start Level one
    level_one();
}

void gravity(void) {
    int hy = round(sprite_y(platform));

    // Check if the platform
    if (round(sprite_y(hero)) == hy + round(sprite_height(platform)) - 4
      &&  sprite_x(hero) < sprite_x(platform) + sprite_width(platform)
      &&  sprite_x(platform) < sprite_x(hero) + sprite_width(hero)) {
        on_platform = true;
    } else {
        on_platform = false;
    }

    if (sprite_y(hero) < SCREEN_HEIGHT - 3 && !on_platform) {
        sprite_turn_to(hero, hero_dx, hero_dy + 0.1);
    } else {
        sprite_turn_to(hero, hero_dx, 0);
    }
}

void re_update(void) {

    // Draw border lines
    draw_line(0, 1, SCREEN_WIDTH, 1, '-'); // TOP LINE
    draw_line(0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, '='); // BOTTOM LINE
    draw_line(0, 2, 0, SCREEN_HEIGHT - 1, '|'); // LEFT LINE
    draw_line(SCREEN_WIDTH, 2, SCREEN_WIDTH, SCREEN_HEIGHT - 1, '|'); // RIGHT LINE

    // Draw stats

    draw_formatted(1, 0, "Times: %02d:%02d", minutes, seconds);


    hero_dx = sprite_dx(hero);
    hero_dy = sprite_dy(hero);

    gravity();
}

void level_one(void) {

    // Draw the sprites
    sprite_draw(hero);
    sprite_draw(exit_door);
    sprite_draw(zombie);
    sprite_draw(platform);

    // Move the zombine
    sprite_turn_to(zombie, -0.15, 0);

    level_one_start = true;
}

void process_time(void) {
    delay += 1;
    if (delay == 100) {
        delay = 0;
        seconds += 1;
        if (seconds == 0) {
            seconds = 0;
            minutes += 1;
        }
    }
}

void process(void) {
    clear_screen();
    process_time();
    re_update();

    int input = get_char();

    // Within LEVEL ONE
    if (level_one_start && !level_one_fin) {
        //draw_line(SCREEN_WIDTH * 0.33, SCREEN_HEIGHT * 0.77 , SCREEN_WIDTH * 0.66, SCREEN_HEIGHT * 0.77, '=');

        // Check if zombie has hit the wall
        if (sprite_x(zombie) < 1 || sprite_x(zombie) > SCREEN_WIDTH - 3) {
            sprite_turn(zombie, 180);
        }

        // Check if the player has hit the player
        if (process_collision(hero, zombie)) {
            game_over = true;
        }
        process_collision(hero, platform);

        sprite_step(zombie);
        sprite_draw(zombie);
        sprite_draw(platform);
    }


    // Check for hero movements
    // Check if the hero is moving right
    if (input == 'd') {
        if (hero_dx > 0) {
            sprite_turn_to(hero, 0.4, hero_dy);
        } else if (hero_dx < 0) {
            sprite_turn_to(hero, 0, hero_dy);
        } else {
            sprite_turn_to(hero, hero_dx + 0.2, hero_dy);
        }
    }
    // Check if the hero is moving left
    if (input == 'a') {
        if (hero_dx < 0) {
            sprite_turn_to(hero, -0.4, hero_dy);
        } else if (hero_dx > 0) {
            sprite_turn_to(hero, 0, hero_dy);
        } else {
            sprite_turn_to(hero, hero_dx - 0.2, hero_dy);
        }
    }

    if (input == 'w' && sprite_dy(hero) == 0) {
        sprite_turn_to(hero, hero_dx, hero_dy - 2.0);
        on_platform = false;
    }

    // Check if the hero hit a wall (LEFT & RIGHT)
    if (sprite_x(hero) < 1) {
        sprite_move_to(hero, 1, sprite_y(hero));
        sprite_turn_to(hero, 0, 0);
    }
    if (sprite_x(hero) > SCREEN_WIDTH - 3) {
        sprite_move_to(hero, SCREEN_WIDTH - 3, sprite_y(hero));
        sprite_turn_to(hero, 0, 0);
    }

    // Check if the hero hit the floor
    if (sprite_y(hero) > SCREEN_HEIGHT - 3) {
        sprite_move_to(hero, sprite_x(hero), SCREEN_HEIGHT - 3);
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
