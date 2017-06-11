#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int read_numeric_file(char *path) {
	FILE *f = fopen(path, "rt");
	if (!f) {
		return -1;
	}
	int max_bri = -1;
	int res = fscanf(f, "%i", &max_bri);
	fclose(f);
	if (res <= 0) {
		return -1;
	}
	return max_bri;
}

int write_numeric_file(char *path, int value) {
	FILE *f = fopen(path, "wt");
	if (!f) {
		printf("Can't open '%s' for writing. Make sure the file is writeable to the current user.\nTry this: sudo chmod ugo+w /sys/class/backlight/intel_backlight/brightness\n", path);
		return -1;
	}
	int res = fprintf(f, "%i", value);
	fclose(f);
	return res;
}

int get_max_brightness() {
	return read_numeric_file("/sys/class/backlight/intel_backlight/max_brightness");
}

int get_cur_brightness() {
	return read_numeric_file("/sys/class/backlight/intel_backlight/brightness");
}

int set_brightness(int val) {
	return write_numeric_file("/sys/class/backlight/intel_backlight/brightness", val);
}

int inc_bri(int min_bri, int max_bri, int cur_bri) {
	float f_cur_bri = (float)cur_bri;
	float f_max_bri = (float)max_bri;

	f_cur_bri += (f_max_bri * 0.2);
	if (f_cur_bri > max_bri) {
		f_cur_bri = max_bri;
	}
	return (int)f_cur_bri;
}
	
int dec_bri(int min_bri, int max_bri, int cur_bri) {
	float f_cur_bri = (float)cur_bri;
	float f_max_bri = (float)max_bri;
	f_cur_bri -= (f_max_bri * 0.2);
	if (f_cur_bri <= min_bri) {
		f_cur_bri = min_bri;
	}
	return (int)f_cur_bri;
}

void dump_info() {
		printf("Quick and dirty backlight setter for thinkpad x220 for when xbacklight won't work.\n(c) js@szpilewski.com, licensed under Affero GPL 3\n");
		printf("usage: bl <switch>\n");
		printf("\t-d\tdecrease brightness by 10%%\n");
		printf("\t-i\tincrease brightness by 10%%\n");
		printf("\t-f\tset brightness to FULL\n");
		printf("\t-l\tset brightness to LOW\n");
		printf("\t-0\tset brightness to 0 (warning: will turn off screen!)\n");
}	

int main(int argc, char *argv[]) {
	int max_bri = get_max_brightness();
	int min_bri = (int)((float)max_bri * 0.05f);
	int cur_bri = get_cur_brightness();

	if (argc == 1) {
		dump_info();
		return 0;
	}

	/* poor man's getopts */
	if (argc == 2) {
		if (!strcmp(argv[1], "-d")) {
			int new_bri = dec_bri(min_bri, max_bri, cur_bri);
			set_brightness(new_bri);
			return 0;
		}

		if (!strcmp(argv[1], "-i")) {
			int new_bri = inc_bri(min_bri, max_bri, cur_bri);
			set_brightness(new_bri);
			return 0;
		}
	
		if (!strcmp(argv[1], "-f")) {
			set_brightness(max_bri);
			return 0;
		}

		if (!strcmp(argv[1], "-l")) {
			int new_bri = (int)((float)max_bri * 0.1f);
			set_brightness(new_bri);
			return 0;
		}

		if (!strcmp(argv[1], "-0")) {
			set_brightness(0);
			return 0;
		}
	}

		dump_info();
//	printf("max_bri: %i\n", max_bri);
//	printf("cur_bri: %i\n", cur_bri);
//	printf("dec: %i\n", new_bri);
	return 0;
}

