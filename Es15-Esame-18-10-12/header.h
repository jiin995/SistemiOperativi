#ifndef HEADER_H
#define HEADER_H


#define P1 1
#define P2 2
#define P3 3
#define P4 4
#define P5 5
#define P6 6

#define FTOK_PATH "."

#define FTOK_CHAR_Q_OP_1  'a'
#define FTOK_CHAR_Q_RIS_1 'b'
#define FTOK_CHAR_Q_OP_2  'c'
#define FTOK_CHAR_Q_RIS_2 'd'

typedef struct{
	long processo;
	int operandi[4];
}msg_operandi;

typedef struct{
	long processo;
	int risposta;
}msg_risposta;

#endif
