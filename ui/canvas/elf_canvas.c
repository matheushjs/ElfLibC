/* The idea is to provide the user with a structure that holds a matrix of characters
 *   that will be shown on terminal.
 * Upon construction, only the width of the canvas should be specified.
 *   The height will be decided on-the-fly.
 */

typedef struct _ElfCanvas {
	char **matrix;
	int w;
	int h;
	bool hasMinHeight;
} ElfCanvas;

/* Rules for the canvas
 *
 * 1) The user won't be able to specify a maximum height; just a minimal one.
 * 2) Pixels not specified by the user will be shown as a whitespace.
 * 3) The canvas is always left-justified within the terminal.
 */
