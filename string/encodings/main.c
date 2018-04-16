/*
 * MIT License
 * 
 * Copyright (c) 2018 Matheus H. J. Saldanha <mhjsaldanha@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <elf_encodings.h>

// Defined on the bottom
const static char test_string[];

void test(){
	char u1[] = "は", u2[] = "é";
	char lat1 = elfEncodings_toLatin1_utf8("é", NULL);
	char buf[3] = "\0\0";

	printf("utf1: %s\n", u1);
	printf("utf2: %s\n", u2);
	elfEncodings_toUtf8_latin1(lat1, &buf[0], &buf[1]);
	printf("lat1: %s\n", buf);

	elfEncodings_lower_utf8(u1);
	elfEncodings_lower_utf8(u2);
	lat1 = elfEncodings_lower_latin1(lat1);

	printf("utf1: %s\n", u1);
	printf("utf2: %s\n", u2);
	elfEncodings_toUtf8_latin1(lat1, &buf[0], &buf[1]);
	printf("lat1: %s\n", buf);

	elfEncodings_upper_utf8(u1);
	elfEncodings_upper_utf8(u2);
	lat1 = elfEncodings_upper_latin1(lat1);

	printf("utf1: %s\n", u1);
	printf("utf2: %s\n", u2);
	elfEncodings_toUtf8_latin1(lat1, &buf[0], &buf[1]);
	printf("lat1: %s\n", buf);

	printf("Length %s: %d\n", u1, elfEncodings_charLength_utf8(*u1));
	printf("Length %s: %d\n", u2, elfEncodings_charLength_utf8(*u2));

	char u3[] = "はひふへほ";
	char *u4 = (char *) u3;

	int i;
	for(i = 0; i < 5; i++){
		u4 = (char*) elfEncodings_advanceChar_utf8(u4);
		printf("Advance: %s\n", u4);
	}


	char *u5, *u6;

	u5 = "は";
	u6 = "は";
	printf("%s == %s returns %d\n", u5, u6, elfEncodings_charCmp_utf8(u5, u6));

	u5 = "a";
	u6 = "a";
	printf("%s == %s returns %d\n", u5, u6, elfEncodings_charCmp_utf8(u5, u6));

	u5 = "a";
	u6 = "á";
	printf("%s == %s returns %d\n", u5, u6, elfEncodings_charCmp_utf8(u5, u6));

	u5 = "は";
	u6 = "目";
	printf("%s == %s returns %d\n", u5, u6, elfEncodings_charCmp_utf8(u5, u6));

	u5 = "目";
	u6 = "は";
	printf("%s == %s returns %d\n", u5, u6, elfEncodings_charCmp_utf8(u5, u6));

	elfEncodings_putc_utf8("目の前に");
	elfEncodings_putc_utf8("\n");
	elfEncodings_putc_utf8("a");
	elfEncodings_putc_utf8("\n");
	elfEncodings_putc_utf8("");
	elfEncodings_putc_utf8("\n");
}

/* XXX: Might be a good idea to run the tests on ElfString too, whenever the tests here
 *   are executed.
 */
int main(int argc, char *argv[]){
	test();
	return 0;
}
