#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "gifenc.h"
#include "palette.h"

static const int image_width = 320;
static const int image_height = 240;

static const int loop_forever = 0;

static const int depth = 6;

void draw_igniter(uint8_t *pixels) {
	const int start_last_row = (image_width * (image_height - 1));
	const int margin = 10;
	for(int w = margin; w < image_width - margin; w++) {
		pixels[start_last_row + w] = 36;
	}
}

void stop_igniter(uint8_t *pixels) {
	const int start_last_row = (image_width * (image_height - 1));
	for(int w = 0; w < image_width; w++) {
		pixels[start_last_row + w] = 0;
	}
}

int main(int argc, char **argv) {
	ge_GIF* anim = ge_new_gif("doom-fire.gif", image_width, image_height, fire_palette, depth, loop_forever);

	uint8_t pixels[image_width * image_height];
	memset(pixels, 0, image_width * image_height);
	draw_igniter(pixels);

	int framerate = 12;
	// First frame, only igniter
	memcpy(anim->frame, pixels, sizeof(pixels));
	ge_add_frame(anim, framerate);

	srand(time(NULL));
	int height_max = 3;
	int upper = 3; int lower = -3;
	for(int i = 0; i < 250; i++) {
		for(int row = image_height - 1 - 1; row > 0; row--) {
			for(int column = 0; column < image_width; column++) {
				int current_row = (row * image_width);
				int next_row = ((row + 1) * image_width);
				int target_pixel = current_row + column;
				int decay = (rand() % height_max) & 1;
				int movement = (rand() % (upper - lower + 1)) + lower;
				int from_pixel = next_row + column + movement;
				if(from_pixel >= 0 && from_pixel < image_width * image_height) {
					int new_pixel = (pixels[from_pixel]) - decay;
					if(new_pixel >= 0 && new_pixel <= 36) {
						pixels[target_pixel] = new_pixel;
					}
				}
			}
		}
		memcpy(anim->frame, pixels, sizeof(pixels));
		ge_add_frame(anim, framerate);
		if(i == 80) {
			stop_igniter(pixels);
			height_max = 5;
		}
	}
	
	// Last frame, black screen
	memset(pixels, 0, image_width * image_height);
	memcpy(anim->frame, pixels, sizeof(pixels));
	ge_add_frame(anim, 150);

	ge_close_gif(anim);
	return EXIT_SUCCESS;
}

