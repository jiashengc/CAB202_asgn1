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
bool the_end = false;

// Game levels
bool level_one_start = false;
bool level_one_fin = false;
bool level_two_start = false;
bool level_two_fin = false;
bool level_three_start = false;
bool level_three_fin = false;
bool level_four_start = false;
bool level_four_fin = false;
bool level_five_start = false;
bool level_five_fin = false;

// Hero movement
double hero_dx = 0;
double hero_dy = 0;
bool on_platform = false;

// Mob movement
bool mob_on_platform = false;

// Stats
int delay = 0;
int minutes = 0;
int seconds = 0;
int lives = 10;
int level = 1;
int score = 0;

// Treasure
bool treasure_get = false;
bool key_get = false;

// Time
timer_id timer;

// Sprites
sprite_id hero;
sprite_id exit_door;
sprite_id mob;
sprite_id platform;
sprite_id platform_2;
sprite_id platform_3;
sprite_id treasure;
sprite_id key;
sprite_id amulet;
sprite_id pit;
sprite_id wall;

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

char * spinner_sprite =
/**/  " |"
/**/  "/\\";

char * zombie_sprite =
/**/	"ZZZZ"
/**/	"  Z "
/**/	" Z  "
/**/	"ZZZZ";

char * boulder_sprite =
/**/  "/''\\"
/**/  "\\__//";

char * treasure_sprite =
/**/  "$";

char * key_sprite =
/**/  "o-+";

char * amulet_sprite =
/**/  " _ "
/**/  "/ \\"
/**/  "\\ /"
/**/  " @ ";

char * platform_sprite =
"============================================================================";

char * platform_sprite_2 =
"============================================================================";

char * pit_sprite =
"                                                                            ";

char * wall_sprite =
/**/ "[]"
/**/ "[]"
/**/ "[]"
/**/ "[]"
/**/ "[]"
/**/ "[]"
/**/ "[]"
/**/ "[]"
/**/ "[]"
/**/ "[]"
/**/ "[]";

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
		if (obj_1 == hero) {
			double dx = sprite_dx(hero), dy = sprite_dy(hero);

			if ( hy == oy + sprite_height(hero) - 1 && dy < 0 ) {
				dy = -dy;
			}

			else if ( hx + sprite_width(hero) - 1 == ox && dx > 0 ) {
				dx = 0;
			}

			else if ( hx == ox + sprite_width(obj_2) - 1 && dx < 0 ) {
				dx = 0;
			}

			else {
				dy = 0;
        on_platform = true;
			}

			sprite_back(hero);
			sprite_turn_to(hero, dx, dy);
		}
		else {
			sprite_move_to(hero, sprite_x(hero), sprite_y(obj_2) + sprite_height(obj_2));
		}
	}

  return collided;
}

void setup(void) {
    // Start Level one
    level_one();
}

void level_1_gravity(void) {
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
        sprite_turn_to(hero, hero_dx, hero_dy + 0.05);
    } else {
        sprite_turn_to(hero, hero_dx, 0);
    }
}

void level_2_gravity(void) {
    int by = round(sprite_y(platform));
    int hy = round(sprite_y(platform_2));

    // Check if the platform
    if ((round(sprite_y(hero)) == hy + round(sprite_height(platform_2)) - 4
      &&  sprite_x(hero) < sprite_x(platform_2) + sprite_width(platform_2)
      &&  sprite_x(platform_2) < sprite_x(hero) + sprite_width(hero))
      || (round(sprite_y(hero)) == by + round(sprite_height(platform)) - 4
        &&  sprite_x(hero) < sprite_x(platform) + sprite_width(platform)
        &&  sprite_x(platform) < sprite_x(hero) + sprite_width(hero))) {
        on_platform = true;
    } else {
        on_platform = false;
    }

    if (sprite_y(hero) < SCREEN_HEIGHT - 3 && !on_platform) {
        sprite_turn_to(hero, hero_dx, hero_dy + 0.05);
    } else {
        sprite_turn_to(hero, hero_dx, 0);
    }
}

void level_4_gravity(void) {
    int by = round(sprite_y(platform));
    int hy = round(sprite_y(platform_2));
    int ky = round(sprite_y(platform_3));

    // Check if the platform
    if ((round(sprite_y(hero)) == hy + round(sprite_height(platform_2)) - 4
      &&  sprite_x(hero) < sprite_x(platform_2) + sprite_width(platform_2)
      &&  sprite_x(platform_2) < sprite_x(hero) + sprite_width(hero))
      || (round(sprite_y(hero)) == by + round(sprite_height(platform)) - 4
        &&  sprite_x(hero) < sprite_x(platform) + sprite_width(platform)
        &&  sprite_x(platform) < sprite_x(hero) + sprite_width(hero))
      || (round(sprite_y(hero)) == ky + round(sprite_height(platform_3)) - 4
        &&  sprite_x(hero) < sprite_x(platform_3) + sprite_width(platform_3)
        &&  sprite_x(platform_3) < sprite_x(hero) + sprite_width(hero))) {
        on_platform = true;
    } else {
        on_platform = false;
    }

    if (sprite_y(hero) < SCREEN_HEIGHT - 3 && !on_platform) {
        sprite_turn_to(hero, hero_dx, hero_dy + 0.05);
    } else {
        sprite_turn_to(hero, hero_dx, 0);
    }
}

void level_5_gravity(void) {
		int by = round(sprite_y(platform));
		int hy = round(sprite_y(platform_2));

		double mob_dx = sprite_dx(mob);
		double mob_dy = sprite_dy(mob);
		double mob_x = sprite_x(mob);

		// Check if the platform
		if ((round(sprite_y(hero)) == hy + round(sprite_height(platform_2)) - 4
			&&  sprite_x(hero) < sprite_x(platform_2) + sprite_width(platform_2)
			&&  sprite_x(platform_2) < sprite_x(hero) + sprite_width(hero))
			|| (round(sprite_y(hero)) == by + round(sprite_height(platform)) - 4
				&&  sprite_x(hero) < sprite_x(platform) + sprite_width(platform)
				&&  sprite_x(platform) < sprite_x(hero) + sprite_width(hero))) {
				on_platform = true;
		} else {
				on_platform = false;
		}

		if ((round(sprite_y(mob)) == hy + round(sprite_height(platform_2)) - 3
			&&  sprite_x(mob) < sprite_x(platform_2) + sprite_width(platform_2)
			&&  sprite_x(platform_2) < sprite_x(mob) + sprite_width(mob))
			|| (round(sprite_y(mob)) == by + round(sprite_height(platform)) - 3
				&&  sprite_x(mob) < sprite_x(platform) + sprite_width(platform)
				&&  sprite_x(platform) < sprite_x(mob) + sprite_width(mob))) {
				mob_on_platform = true;
		} else {
				mob_on_platform = false;
		}

		if (sprite_y(hero) < SCREEN_HEIGHT - 3 && !on_platform) {
				sprite_turn_to(hero, hero_dx, hero_dy + 0.05);
		} else {
				sprite_turn_to(hero, hero_dx, 0);
		}

		if ((sprite_y(mob) < SCREEN_HEIGHT - 2 || sprite_x(mob) > SCREEN_WIDTH * 0.25 - 1) && !mob_on_platform) {
				sprite_turn_to(mob, mob_dx, mob_dy + 0.02);
		} else {
				sprite_turn_to(mob, mob_dx, 0);
		}

		if (sprite_y(mob) > SCREEN_HEIGHT) {
				sprite_destroy(mob);
				mob = sprite_create(mob_x, 3, 4, 2, boulder_sprite);
				sprite_turn_to(mob, mob_dx, mob_dy);
		}
}

void re_update(void) {

    // Draw border lines
    draw_line(0, 1, SCREEN_WIDTH, 1, '-'); // TOP LINE

    draw_line(0, 2, 0, SCREEN_HEIGHT - 1, '|'); // LEFT LINE
    draw_line(SCREEN_WIDTH, 2, SCREEN_WIDTH, SCREEN_HEIGHT - 1, '|'); // RIGHT LINE
    if (level == 3) {
        draw_line(0, SCREEN_HEIGHT, SCREEN_WIDTH * 0.25-1, SCREEN_HEIGHT, '=');
        draw_line(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, '=');
    } else if (level == 5) {
        draw_line(0, SCREEN_HEIGHT, SCREEN_WIDTH * 0.25, SCREEN_HEIGHT, '=');
    } else {
        draw_line(0, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT, '='); // BOTTOM LINE
    }

    // Draw stats
    draw_formatted(SCREEN_WIDTH * 0.11, 0, "Times: %02d:%02d", minutes, seconds);
    draw_formatted(SCREEN_WIDTH * 0.33, 0, "Lives: %d", lives);
    draw_formatted(SCREEN_WIDTH * 0.55, 0, "Level: %d", level);
    draw_formatted(SCREEN_WIDTH * 0.77, 0, "Score: %d", score);

    hero_dx = sprite_dx(hero);
    hero_dy = sprite_dy(hero);

    if (level == 1) {
        level_1_gravity();
    } else if (level == 2 || level == 3) {
        level_2_gravity();
    } else if (level == 4) {
        level_4_gravity();
    } else if (level == 5) {
				level_5_gravity();
		}
}

void level_one(void) {
    int platform_length = (int)(SCREEN_WIDTH * 0.3 - 1);
    // Initialise sprite
    hero = sprite_create(2, SCREEN_HEIGHT - 3, 3, 3, hero_sprite);
    exit_door = sprite_create(SCREEN_WIDTH - 6, SCREEN_HEIGHT - 4, 4, 4, exit_sprite);
    mob = sprite_create(SCREEN_WIDTH - 6, SCREEN_HEIGHT - 4, 4, 4, zombie_sprite);
    platform = sprite_create(SCREEN_WIDTH * 0.33, SCREEN_HEIGHT * 0.77, platform_length + 1, 1, platform_sprite);

    // Draw the sprites
    sprite_draw(hero);
    sprite_draw(exit_door);
    sprite_draw(mob);
    sprite_draw(platform);

    // Move the zombine
    sprite_turn_to(mob, -0.15, 0);

    level_one_start = true;
}

void level_two(void) {
    int platform_1_length = (int)(SCREEN_WIDTH * 0.3 - 1);
    int platform_2_length = (int)(SCREEN_WIDTH * 0.1 - 1);

    // Initialize the sprites
    hero = sprite_create(2, SCREEN_HEIGHT - 3, 3, 3, hero_sprite);
    exit_door = sprite_create(SCREEN_WIDTH - 6, SCREEN_HEIGHT - 4, 4, 4, exit_sprite);
    mob = sprite_create(SCREEN_WIDTH - 8, SCREEN_HEIGHT - 3, 2, 2, spinner_sprite);
    platform = sprite_create(SCREEN_WIDTH * 0.33, SCREEN_HEIGHT * 0.77, platform_1_length + 1, 1, platform_sprite);
    platform_2 = sprite_create(SCREEN_WIDTH * 0.43, SCREEN_HEIGHT * 0.55, platform_2_length + 1, 1, platform_sprite);
    treasure = sprite_create(SCREEN_WIDTH * 0.48, SCREEN_HEIGHT * 0.33, 1, 1, treasure_sprite);

    // Draw the sprites
    sprite_draw(hero);
    sprite_draw(exit_door);
    sprite_draw(mob);
    sprite_draw(platform);
    sprite_draw(platform_2);
    sprite_draw(treasure);

    // Move the spinner
    sprite_turn_to(mob, -0.20, 0);

    treasure_get = false;
    level_two_start = true;
}

void level_three(void) {
    int platform_1_length = (int)(SCREEN_WIDTH * 0.3 - 1);
    int platform_2_length = (int)(SCREEN_WIDTH * 0.1 - 1);
		int pit_length = (int)(SCREEN_WIDTH * 0.5 - 1);

    // Initialize the sprites
    hero = sprite_create(2, SCREEN_HEIGHT - 3, 3, 3, hero_sprite);
    platform = sprite_create(SCREEN_WIDTH * 0.33, SCREEN_HEIGHT * 0.77, platform_1_length + 1, 1, platform_sprite);
    platform_2 = sprite_create(SCREEN_WIDTH * 0.43, SCREEN_HEIGHT * 0.55, platform_2_length + 1, 1, platform_sprite);
    exit_door = sprite_create(SCREEN_WIDTH * 0.43 + 3, SCREEN_HEIGHT * 0.55 - 4, 4, 4, exit_sprite);
		treasure = sprite_create(SCREEN_WIDTH - 6, SCREEN_HEIGHT - 2, 1, 1, treasure_sprite);
		pit = sprite_create(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT - 1, pit_length, 1, pit_sprite);

    // Draw the sprites
    sprite_draw(hero);
    sprite_draw(exit_door);
    sprite_draw(platform);
    sprite_draw(platform_2);
		sprite_draw(treasure);
		sprite_draw(pit);

    sprite_turn_to(platform, 0.1, 0);

    treasure_get = false;
    level_three_start = true;
}

void level_four(void) {
		int platform_length = (int)(SCREEN_WIDTH * 0.6 - 1);

		// Initialise the sprites
		hero = sprite_create(2, SCREEN_HEIGHT - 3, 3, 3, hero_sprite);
		exit_door = sprite_create(SCREEN_WIDTH - 6, SCREEN_HEIGHT - 4, 4, 4, exit_sprite);
		platform = sprite_create(SCREEN_WIDTH * 0.4 + 2, SCREEN_HEIGHT * 0.78, platform_length, 1, platform_sprite);
		platform_2 = sprite_create(0, SCREEN_HEIGHT * 0.56, platform_length, 1, platform_sprite);
		platform_3 = sprite_create(SCREEN_WIDTH * 0.4 + 2, SCREEN_HEIGHT * 0.34, platform_length, 1, platform_sprite);
		key = sprite_create(SCREEN_WIDTH - 6, 5, 3, 1, key_sprite);
		wall = sprite_create(SCREEN_WIDTH * 0.65, SCREEN_HEIGHT * 0.79 + 1, 2, SCREEN_HEIGHT - SCREEN_HEIGHT * 0.77 - 1, wall_sprite);

    // Draw the sprites
    sprite_draw(hero);
    sprite_draw(exit_door);
    sprite_draw(platform);
    sprite_draw(platform_2);
    sprite_draw(platform_3);
    sprite_draw(wall);
    sprite_draw(key);

		key_get = false;
    level_four_start = true;
}

void level_five(void) {
    int platform_length = (int)(SCREEN_WIDTH * 0.5);
    int pit_length = (int)(SCREEN_WIDTH * 0.7 - 1);


    // Initialise the sprites
    hero = sprite_create(2, SCREEN_HEIGHT - 3, 3, 3, hero_sprite);
    platform = sprite_create(SCREEN_WIDTH * 0.5 - 1, SCREEN_HEIGHT * 0.75, platform_length, 1, platform_sprite);
    platform_2 = sprite_create(1, SCREEN_HEIGHT * 0.45, platform_length, 1, platform_sprite);
    mob = sprite_create(6, 3, 4, 2, boulder_sprite);
    amulet = sprite_create(SCREEN_WIDTH * 0.43 + 2, SCREEN_HEIGHT * 0.2 - 3, 3, 4, amulet_sprite);
    pit = sprite_create(SCREEN_WIDTH * 0.3 + 1, SCREEN_HEIGHT - 1, pit_length, 1, pit_sprite);


    // Draw the sprites
    sprite_draw(hero);
    sprite_draw(platform);
    sprite_draw(platform_2);
    sprite_draw(mob);
    sprite_draw(pit);
    sprite_draw(amulet);

    sprite_turn_to(platform, -0.2, 0);
    sprite_turn_to(platform_2, 0.2, 0);
    sprite_turn_to(mob, 0.25, 0);

		level_five_start = true;
}

void process_level_one(void) {
    // Check if mob has hit the wall
    if (sprite_x(mob) < 1 || sprite_x(mob) > SCREEN_WIDTH - 3) {
        sprite_turn(mob, 180);
    }

    // Check if the hero hits other sprites within the level
    if (process_collision(hero, exit_door)) {
        score += 100;
        level_one_start = false;
        level_one_fin = true;
        sprite_destroy(hero);
        sprite_destroy(mob);
        sprite_destroy(platform);
        level += 1;
        level_two();
    } else if (process_collision(hero, mob)) {
        lives -= 1;
        level_one();
    }
    process_collision(hero, platform);

    sprite_step(mob);
    sprite_draw(mob);
    sprite_draw(exit_door);
    sprite_draw(platform);
}

void process_level_two(void) {
    // Check if the spinner hit the wall
    if (sprite_x(mob) < 1 || sprite_x(mob) > SCREEN_WIDTH - 2) {
        sprite_turn(mob, 180);
    }

    // Checks if the hero hits the other spirtes within the level
    if (process_collision(hero, exit_door)) {
        score += 100;
        if (treasure_get) {
            score += 50;
        } else {
            sprite_destroy(treasure);
        }
        level_two_start = false;
        level_two_fin = true;
        sprite_destroy(hero);
        sprite_destroy(mob);
        sprite_destroy(platform);
        sprite_destroy(platform_2);
        level += 1;
        level_three();
    } else if (process_collision(hero, mob)) {
        lives -= 1;
        level_two();
    }
    if (process_collision(hero, treasure)) {
        treasure_get = true;
        score += 50;
        sprite_destroy(treasure);
    }

    process_collision(hero, platform);
    process_collision(hero, platform_2);

    sprite_step(mob);
    sprite_draw(hero);
    sprite_draw(exit_door);
    sprite_draw(mob);
    sprite_draw(platform);
    sprite_draw(platform_2);
    if (!treasure_get) {
        sprite_draw(treasure);
    }
}

void process_level_three(void) {
    if (sprite_x(platform) < 1 || sprite_x(platform) + sprite_width(platform) > SCREEN_WIDTH - 1) {
        sprite_turn(platform, 180);
    }

    if (process_collision(hero, exit_door)) {
        score += 100;
        if (treasure_get) {
          score += 50;
        } else {
            sprite_destroy(treasure);
        }
        level_three_start = false;
        level_three_fin = true;
        sprite_destroy(hero);
        sprite_destroy(platform);
        sprite_destroy(platform_2);
        sprite_destroy(pit);
        level += 1;
        level_four();
    } else if (process_collision(hero, pit)) {
        lives -= 1;
        level_three();
    }
    if (process_collision(hero, treasure)) {
        treasure_get = true;
        score += 50;
        sprite_destroy(treasure);
    }
    process_collision(hero, platform);
    process_collision(hero, platform_2);

    sprite_step(platform);
    sprite_draw(hero);
    sprite_draw(exit_door);
    sprite_draw(platform);
    sprite_draw(platform_2);
    sprite_draw(pit);
    if (!treasure_get) {
        sprite_draw(treasure);
    }
}

void process_level_four(void) {
    if (process_collision(hero, exit_door)) {
        score += 100;
        level_four_start = false;
        level_four_fin = true;
        sprite_destroy(hero);
        sprite_destroy(platform);
        sprite_destroy(platform_2);
        sprite_destroy(platform_3);
        sprite_destroy(exit_door);
        level += 1;
        level_five();
    }
    if (process_collision(hero, key)) {
        key_get = true;
        sprite_destroy(key);
        sprite_destroy(wall);
    }
    process_collision(hero, platform);
    process_collision(hero, platform_2);
    process_collision(hero, platform_3);
    if (!key_get) {
        process_collision(hero, wall);
    }


    sprite_draw(hero);
    sprite_draw(exit_door);
    sprite_draw(platform);
    sprite_draw(platform_2);
    sprite_draw(platform_3);
    if (!key_get) {
        sprite_draw(key);
        sprite_draw(wall);
    }

}

void process_level_five(void) {
    if (process_collision(hero, amulet)) {
        score += 1000;
        level_five_start = false;
        level_five_fin = true;
        game_over = true;
    }

    if (sprite_x(mob) < 1 || sprite_x(mob) > SCREEN_WIDTH - 3) {
				sprite_turn(mob, 180);
		}

		if (sprite_x(platform) < 1 || sprite_x(platform) + sprite_width(platform) > SCREEN_WIDTH - 1) {
        sprite_turn(platform, 180);
    }

		if (sprite_x(platform_2) < 1 || sprite_x(platform_2) + sprite_width(platform_2) > SCREEN_WIDTH - 1) {
        sprite_turn(platform_2, 180);
    }

    if (process_collision(hero, pit)) {
        lives -= 1;
        level_five();
    }

    if (process_collision(hero, mob)) {
        lives -= 1;
        level_five();
    }

		process_collision(hero, platform);
		process_collision(hero, platform_2);
		process_collision(mob, platform);
		process_collision(mob, platform_2);

		sprite_step(mob);
    sprite_step(platform);
    sprite_step(platform_2);
    sprite_draw(hero);
    sprite_draw(mob);
    sprite_draw(pit);
    sprite_draw(platform);
    sprite_draw(platform_2);
		sprite_draw(amulet);
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
        process_level_one();
    }

    // Within LEVEL TWO
    else if (level_two_start && !level_two_fin) {
        process_level_two();
    }

    // Within LEVEL THREE
    else if (level_three_start && !level_three_fin) {
        process_level_three();
    }

		// within LEVEL FOUR
		else if (level_four_start && !level_four_fin) {
				process_level_four();
		}

		// within LEVEL FIVE
		else if (level_five_start && !level_five_fin) {
				process_level_five();
		}

    // Check if lives has reached 0
    if (lives <= 0) {
        game_over = true;
    }

    // Check for hero movements
    // Check if the hero is moving right
    if (input == 'd' && sprite_dy(hero) == 0) {
        if (hero_dx > 0) {
            sprite_turn_to(hero, 0.4, hero_dy);
        } else if (hero_dx < 0) {
            sprite_turn_to(hero, 0, hero_dy);
        } else {
            sprite_turn_to(hero, hero_dx + 0.2, hero_dy);
        }
    }
    // Check if the hero is moving left
    if (input == 'a' && sprite_dy(hero) == 0) {
        if (hero_dx < 0) {
            sprite_turn_to(hero, -0.4, hero_dy);
        } else if (hero_dx > 0) {
            sprite_turn_to(hero, 0, hero_dy);
        } else {
            sprite_turn_to(hero, hero_dx - 0.2, hero_dy);
        }
    }
    // Check if the player jumps
    if (input == 'w' && sprite_dy(hero) == 0) {
        sprite_turn_to(hero, hero_dx, hero_dy - 1.5);
        on_platform = false;
    }
    // Check if the player is skipping a level
    if (input == 'l') {
        switch(level) {
            case 1:
                level_one_start = false;
                level_two_start = true;
                level += 1;
                level_two();
                break;
            case 2:
                level_two_start = false;
                level_three_start = true;
                level += 1;
                level_three();
                break;
            case 3:
                level_three_start = false;
                level_four_start = true;
                level += 1;
                level_four();
                break;
            case 4:
                level_four_start = false;
                level_five_start = true;
								level += 1;
                level_five();
                break;
        }
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
    sprite_draw(hero);
}

void cleanup(void) {
    clear_screen();
}

void game_over_screen(void) {
    // Wait till the player does something
    clear_screen();

    while ( get_char() >= 0 ) {}
    draw_formatted(SCREEN_WIDTH * 0.48, SCREEN_HEIGHT * 0.45, "GAME OVER");
    if (level_five_fin) {
        draw_formatted(SCREEN_WIDTH * 0.47, SCREEN_HEIGHT * 0.47, "SCORE: %d", score);
    }
    draw_formatted(SCREEN_WIDTH * 0.42, SCREEN_HEIGHT * 0.50, "Press any key to exit!");

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

  while(!the_end) {
      game_over_screen();

      if (update_screen) {
          show_screen();
      }

      timer_pause(DELAY);

      if (wait_char() >= 0) {
          the_end = true;
      }
  }

  timer_pause(100);

  return 0;
}
