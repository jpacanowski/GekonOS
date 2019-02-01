#ifndef x86_H
#define x86_H

/* Sends the given data to the given I/O port */
void outportb(unsigned short port, unsigned char data);
void outportw(unsigned short port, unsigned short data);
void outportl(unsigned short port, unsigned int data);

/* Reads a byte from an I/O port */
unsigned char inportb(unsigned short port);
unsigned short inportw(unsigned short port);
unsigned int inportl(unsigned short port);

#endif
