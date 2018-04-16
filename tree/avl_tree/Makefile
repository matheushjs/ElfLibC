# MIT License
# 
# Copyright (c) 2018 Matheus H. J. Saldanha <mhjsaldanha@gmail.com>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#Compiler
CC=gcc
CFLAGS=-Wall -O3 -Wno-unused-result
LIBS=

#files
SRCFILES=avltree.c
INCLUDEDIR=.
MAIN1=main.c
BIN1=prog

#tools
RM=rm -v


all:
	make $(BIN1)

$(BIN1): $(MAIN1) $(SRCFILES)
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) $(MAIN1) $(SRCFILES) -o $(BIN1)

clean:
	find -type f -name "*~" -exec $(RM) '{}' \;
