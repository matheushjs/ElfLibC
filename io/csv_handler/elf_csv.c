// Following the standard RFC4180
// Available at: https://tools.ietf.org/html/rfc4180
// Should work with either latin1 or UTF8
// CSV may end with a newline

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <elf_string_buf.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)

// Convenient define
#define SEEK(X, Y) fseek(X, Y, SEEK_CUR)

/* static function declarations */
static char *read_field(FILE *fp, char sep);
static char **read_record(FILE *fp, char sep);


// CSV Reader
typedef struct _ElfCsvR {
	FILE *fp;
	char sep;
} ElfCsvR;

// Documented in header file.
ElfCsvR *elfCsvR_new_withSep(const char *filename, char sep){
	if(sep == '"' || sep == '\n' || sep == '\r'){
		fprintf(stderr, "Separator can be anything but double quotes, newline or carriage return.\n");
		return NULL;
	}

	ElfCsvR *elf;

	elf = malloc(sizeof(ElfCsvR));

	// If file does not exist, we still want to return something.
	// The function 'hasNext()' must return false though.
	elf->fp = fopen(filename, "r");
	elf->sep = sep;

	return elf;
}

// Documented in header file.
ElfCsvR *elfCsvR_new(const char *filename){
	return elfCsvR_new_withSep(filename, ',');
}

// Documented in header file.
void elfCsvR_destroy(ElfCsvR **elf_p){
	ElfCsvR *elf = *elf_p;
	if(elf){
		if(elf->fp)
			fclose(elf->fp);
		free(elf);
		*elf_p = NULL;
	}
}

// Reads a CSV field.
// A field ends when a comma or newline is encountered.
// If the field is quoted, it ends when the other quote is found.
// Anything between the last quote and the next comma is considered an ERROR.
// 'fp' must be positioned on first character after previous comma/newline.
// 'fp' is left above the comma, EOF or newline character after the value read.
static
char *read_field(FILE *fp, char sep){
	bool isQuoted;
	char c;
	char *result;
	ElfStringBuf *buf;

	buf = elfStringBuf_new();

	// Decide if it's a quoted value or not.
	c = fgetc(fp);
	if(c == '"'){
		isQuoted = true;
	} else { 
		isQuoted = false;
		SEEK(fp, -1);
	}

	while(true){
		c = fgetc(fp);

		// Check termination condition
		if(isQuoted){
			if(c == EOF){
				ELF_DIE("FATAL: Unexpected EOF encountered.");
			} else if(c == '"') {
				// Check if it is a double quote, case in which it's replaced by a single quote.
				// If it's not another quote, then it must be a comma.
				if(fgetc(fp) != '"'){
					SEEK(fp, -1);
					break;
				}
			}

		} else /* if(!isQuoted) */ {
			if(c == EOF){
				break;
			} else if(c == sep || c == '\n' || c == '\r'){
				SEEK(fp, -1);
				break;
			}
		}

		elfStringBuf_appendChar(buf, c);
	}

	result = elfStringBuf_getString(buf, NULL);
	elfStringBuf_destroy(&buf);

	return result;
}

// Reads a CSV line
// A line ends at EOF, newline or carriage return
// 'fp' is expected to be at the beginning of the line
// 'fp' is left at the beginning of next line
static
char **read_record(FILE *fp, char sep){
	char c;
	char **result;
	int size;

	size = 0;
	result = NULL;

	while(true){
		size++;
		result = realloc(result, sizeof(char *) * size);
		result[size-1] = read_field(fp, sep);
		
		c = fgetc(fp);

		//Check termination condition
		if(c == '\r'){
			c = fgetc(fp);
			
			//Check if there is another newline character
			if(c == EOF || c == '\r' || c == '\n'){
				break;
			} else {
				SEEK(fp, -1);
				break;
			}

		} else if(c == '\n' || c == EOF){
			break;

		} else {
			// There is something to read
			if(c != sep)
				SEEK(fp, -1);
		}
	}

	// Append NULL
	result = realloc(result, sizeof(char *) * (size+1));
	result[size] = NULL;
	return result;
}

// Documented in header file.
bool elfCsvR_hasNext(ElfCsvR *elf){
	char c;

	if(elf->fp == NULL)
		return false;

	c = fgetc(elf->fp);
	if(c == EOF){
		return false;
	} else {
		SEEK(elf->fp, -1);
		return true;
	}
}

// Documented in header file.
char **elfCsvR_nextLine(ElfCsvR *elf){
	if(elfCsvR_hasNext(elf))
		return read_record(elf->fp, elf->sep);
	else return NULL;
}



typedef struct _ElfCsvW {
	FILE *fp;
	char sep;
} ElfCsvW;


