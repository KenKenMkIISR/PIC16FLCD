#ifndef GRAPHLIB_H
#define	GRAPHLIB_H

#include <xc.h>
#include <stdint.h>
#include "mcc_generated_files/mcc.h"

void g_pset(int x,int y,uint16_t c);
void g_gline(int x1,int y1,int x2,int y2,uint16_t c);
void g_hline(int x1,int x2,int y,uint16_t c);
void g_circle(int x0,int y0,unsigned int r,uint16_t c);
void g_boxfill(int x1,int y1,int x2,int y2,uint16_t c);
void g_circlefill(int x0,int y0,unsigned int r,uint16_t c);
void g_putfont(int x,int y,uint16_t c,uint16_t bc,uint8_t n);
void g_printstr(int x,int y,uint16_t c,uint16_t bc,unsigned char *s);
void g_printnum(int x,int y,uint16_t c,uint16_t bc,uint16_t n);
void g_printnum2(int x,int y,uint16_t c,uint16_t bc,uint16_t n,uint8_t e);

#endif	/* GRAPHLIB_H */

