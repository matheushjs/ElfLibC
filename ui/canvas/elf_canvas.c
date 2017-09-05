#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include <elf_string_buf.h>
#include <elf_utf_buf.h>
#include <elf_encodings.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)
#define ELF_MAX(X,Y) ((X)>(Y)?X:Y)
#define ELF_MIN(X,Y) ((X)<(Y)?X:Y)

typedef struct _ElfCanvas {
	ElfUtfBuf **canvas;
	int w;  // Width
	int h;  // Height
} ElfCanvas;

/* Static function declarations */
static inline ElfUtfBuf *new_buffer(int size);
/* */

// Returns a new Utf buffer filled with 'size' blank spaces.
static inline
ElfUtfBuf *new_buffer(int size){
	ElfUtfBuf *buf = elfUtfBuf_new();
	for(; size > 0; size--)
		elfUtfBuf_appendChar(buf, " ");
	return buf;
}

// Documented in header file.
ElfCanvas *elfCanvas_new(int width, int height){
	ElfCanvas *elf;

	elf = malloc(sizeof(ElfCanvas));
	
	elf->w = width  > 0 ? width  : 1;
	elf->h = height > 0 ? height : 1;
	elf->canvas = malloc(sizeof(ElfUtfBuf *) * elf->h);

	int i;
	for(i = 0; i < elf->h; i++)
		elf->canvas[i] = new_buffer(elf->w);

	return elf;
}

// Documented in header file.
void elfCanvas_destroy(ElfCanvas **elf_p){
	ElfCanvas *elf = *elf_p;
	if(elf){
		int i;
		for(i = 0; i < elf->h; i++)
			elfUtfBuf_destroy(&elf->canvas[i]);
		free(elf->canvas);
		free(elf);
		*elf_p = NULL;
	}
}

// Documented in header file.
int elfCanvas_getWidth(const ElfCanvas *elf){
	return elf->w;
}

// Documented in header file.
int elfCanvas_getHeight(const ElfCanvas *elf){
	return elf->h;
}

// Documented in header file.
void elfCanvas_fprint(const ElfCanvas *elf, FILE *fp){
	int i;
	const char *str;

	for(i = 0; i < elf->h; i++){
		str = elfUtfBuf_getString(elf->canvas[i]);
		fprintf(fp, "%s", str);
		fputc('\n', fp);
	}
}

// Documented in header file.
void elfCanvas_print(const ElfCanvas *elf){
	elfCanvas_fprint(elf, stdout);
}

// Documented in header file.
void elfCanvas_drawChar(const ElfCanvas *elf, int w, int h, const char *c){
	if(w < 0 || w >= elf->w || h < 0 || h >= elf->h) ELF_DIE("Invalid arguments");
	elfUtfBuf_setChar(elf->canvas[h], w, c);
}

// Documented in header file.
void elfCanvas_drawText(ElfCanvas *elf, int w, int h, const char *str){
	if(w < 0 || w >= elf->w || h < 0 || h >= elf->h) ELF_DIE("Invalid arguments");

	int charCount, charSize;

	charCount = 0;
	while(*str != '\0' && (charCount + w) < elf->w){
		charSize = elfEncodings_charLength_utf8(*str);
		elfUtfBuf_setChar(elf->canvas[h], w+charCount, str);
		charCount += 1;
		str += charSize;
	}
}

// Documented in header file.
void elfCanvas_drawText_v(ElfCanvas *elf, int w, int h, const char *str){
	if(w < 0 || w >= elf->w || h < 0 || h >= elf->h) ELF_DIE("Invalid arguments");

	int charCount, charSize;

	charCount = 0;
	while(*str != '\0' && (charCount + h) < elf->h){
		charSize = elfEncodings_charLength_utf8(*str);
		elfUtfBuf_setChar(elf->canvas[h+charCount], w, str);
		charCount += 1;
		str += charSize;
	}
}

// Documented in header file.
void elfCanvas_fillRow_span(ElfCanvas *elf, int w1, int w2, int h, const char *c){
	if(w1 < 0 || w2 < 0
	|| w1 > w2
	|| h < 0 || h >= elf->h) ELF_DIE("Invalid arguments");

	w1 = ELF_MIN(w1, elf->w);
	w2 = ELF_MIN(w2, elf->w-1);

	while(w1 <= w2){
		elfUtfBuf_setChar(elf->canvas[h], w1, c);
		w1++;
	}
}

// Documented in header file.
void elfCanvas_fillRow(ElfCanvas *elf, int h, const char *c){
	elfCanvas_fillRow_span(elf, 0, elf->w-1, h, c);
}

// Documented in header file.
void elfCanvas_fillCol_span(ElfCanvas *elf, int h1, int h2, int w, const char *c){
	if(h1 < 0 || h2 < 0
	|| h1 > h2
	|| w < 0 || w >= elf->w) ELF_DIE("Invalid arguments");

	h1 = ELF_MIN(h1, elf->h);
	h2 = ELF_MIN(h2, elf->h-1);

	while(h1 <= h2){
		elfUtfBuf_setChar(elf->canvas[h1], w, c);
		h1++;
	}
}

// Documented in header file.
void elfCanvas_fillCol(ElfCanvas *elf, int w, const char *c){
	elfCanvas_fillCol_span(elf, 0, elf->h-1, w, c);
}

// Documented in header file.
char *elfCanvas_buildString(ElfCanvas *elf){
	ElfStringBuf *buf;
	int i, n;

	buf = elfStringBuf_new();

	// Append each line in the canvas to the string buffer, delimited by newlines
	for(i = 0, n = elf->h; i < n; i++){
		const char *toAppend = elfUtfBuf_getString(elf->canvas[i]);
		elfStringBuf_appendString(buf, toAppend);
		elfStringBuf_appendChar(buf, '\n');
	}

	char *retval = elfStringBuf_makeString(buf, NULL);
	elfStringBuf_destroy(&buf);
	
	return retval;
}
