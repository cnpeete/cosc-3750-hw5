# Makefile
# Cameron Peete
# COSC 3750 Spring 2025
# Homework 5
# 03/01/2025
#
# Creating a Makefile for Homework 5

CC=gcc
CFLAGS=-Wall -ggdb -I.

.PHONY: clean

wycat: wyls.c
	$(CC) $(CFLAGS) wyls.c -o wyls

clean:
	/bin/rm -f wyls
	/bin/rm -f *.core

