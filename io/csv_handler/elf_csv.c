// Following the standard RFC4180
// Available at: https://tools.ietf.org/html/rfc4180
// Should work with either latin1 or UTF8
// CSV may end with a newline

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <elf_string_buf.h>
#include <elf_string.h>

#define ELF_DIE(X) fprintf(stdout, "%s:%s:%d - %s", __FILE__, __func__, __LINE__, X), exit(EXIT_FAILURE)

// Convenient define
#define SEEK(X, Y) fseek(X, Y, SEEK_CUR)

/* static function declarations */
static char *read_field(FILE *fp, char sep);
static char **read_record(FILE *fp, char sep);
static char *normalize_value(const char *string, char sep);


// CSV Reader
typedef struct _ElfCsvR {
	FILE *fp;
	char sep;
	char **last_read;
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
	elf->last_read = NULL;

	return elf;
}

// Documented in header file.
ElfCsvR *elfCsvR_new(const char *filename){
	return elfCsvR_new_withSep(filename, ',');
}

// Documented in header file.
void elfCsv_freeStrings(char **strings){
	char **aux = strings;
	while(*aux){
		free(*aux);
		aux++;
	}
	free(strings);
}

// Documented in header file.
void elfCsvR_destroy(ElfCsvR **elf_p){
	ElfCsvR *elf = *elf_p;
	if(elf){
		if(elf->fp){
			fclose(elf->fp);
		}
		if(elf->last_read){
			elfCsv_freeStrings(elf->last_read);
			elf->last_read = NULL; // why not?
		}
		free(elf);
		*elf_p = NULL;
	}
}

// Returns 'true' if 'fp' is positioned above 2 quotes in sequence.
// 'fp' position is preserved.
static
int count_quotes(FILE *fp){
	int tell = ftell(fp);
	int count;
	
	count = 0;
	while(fgetc(fp) == '"')
		count++;

	fseek(fp, tell, SEEK_SET);
	return count;
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
	int count;
	char c;
	char *result;
	ElfStringBuf *buf;

	buf = elfStringBuf_new();

	// Decide if it's a quoted value or not.
	count = count_quotes(fp);
	if(count%2 == 1){
		isQuoted = true;
	} else {
		isQuoted = false;
	}
	
	fseek(fp, count, SEEK_CUR); //advances all quotes

	// Adds to the buffer remaining counted quotes
	for(count /= 2; count > 0; count--)
		elfStringBuf_appendChar(buf, '"');

	while(true){
		c = fgetc(fp);

		// Check termination condition and double quotes
		if(isQuoted){
			if(c == EOF){
				ELF_DIE("FATAL: Unexpected EOF encountered.");
			} else if(c == '"') {
				// Check if it is a double quote, case in which it's replaced by a single quote.
				// If it's not another quote, then it must be a comma, which we have to unget.
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
			} else if(c == '"') {
				// It has to be a double quote, so we just fgetc()
				fgetc(fp);
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

// Documented in header file.
const char **elfCsvR_nextLine_managed(ElfCsvR *elf){
	if(elf->last_read){
		elfCsv_freeStrings(elf->last_read);
	}
	elf->last_read = elfCsvR_nextLine(elf);
	return (const char **) elf->last_read;
}



// CSV Writer
typedef struct _ElfCsvW {
	FILE *fp;
	char sep;
} ElfCsvW;

// Documented in header file.
ElfCsvW *elfCsvW_new_withArgs(const char *filename, char sep, bool truncate){
	if(sep == '"' || sep == '\n' || sep == '\r'){
		fprintf(stderr, "Separator can be anything but double quotes, newline or carriage return.\n");
		return NULL;
	}

	ElfCsvW *elf;
	elf = malloc(sizeof(ElfCsvW));

	// If the file does not exist, we create it.
	if(truncate)
		elf->fp = fopen(filename, "w");
	else
		elf->fp = fopen(filename, "a");

	elf->sep = sep;

	return elf;
}

// Documented in header file.
ElfCsvW *elfCsvW_new(const char *filename){
	return elfCsvW_new_withArgs(filename, ',', true);
}

// Documented in header file.
void elfCsvW_destroy(ElfCsvW **elf_p){
	ElfCsvW *elf = *elf_p;
	if(elf){
		fclose(elf->fp);
		free(elf);
		*elf_p = NULL;
	}
}

// Normalizes a string value to be suitable for writing as a CSV value.
// If there are commas or newlines, the value is quoted.
// All inner quotes are replaced by double quotes.
static
char *normalize_value(const char *string, char sep){
	bool shouldQuote;
	bool hasQuote;
	char sep_string[2];
	char *result, *aux;

	sep_string[0] = sep;
	sep_string[1] = '\0';

	shouldQuote = false;
	if(elfString_count(string, sep_string) > 0)
		shouldQuote = true;
	else if(elfString_count(string, "\n") > 0)
		shouldQuote = true;
	else if(elfString_count(string, "\r") > 0)
		shouldQuote = true;

	if(elfString_count(string, "\"") > 0)
		hasQuote = true;

	if(hasQuote)
		aux = elfString_replace(string, "\"", "\"\"");
	else
		aux = elfString_dup(string);

	if(shouldQuote){
		result = elfString_format("\"%s\"", aux);
		free(aux);
	} else {
		result = aux;
	}

	return result;
}

// Documented in header file.
void elfCsvW_writeLine(ElfCsvW *elf, const char **strings){
	bool lock_comma;
	char *string;

	// If there are no values, no line is written.
	// Because an empty line is a line with a single empty value.
	if(*strings == NULL)
		return;

	lock_comma = true;
	while(*strings){
		string = normalize_value(*strings, elf->sep);
		
		if(!lock_comma)
			fputc(elf->sep, elf->fp);
		fputs(string, elf->fp);

		free(string);
		lock_comma = false;
		strings++;
	}

	// Jump a line
	fputc('\n', elf->fp);
}
