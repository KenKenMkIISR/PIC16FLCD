#include "ili9341driver.h"
const unsigned char FontData[]={
//フォントデータ、キャラクタコード順に8バイトずつ、上位ビットが左
//'0'-'Z'
/*
	0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,
	0x18,0x38,0x78,0x18,0x18,0x18,0x7E,0x00,
	0x7C,0xC6,0x06,0x1C,0x70,0xC0,0xFE,0x00,
	0x7C,0xC6,0x06,0x3C,0x06,0xC6,0x7C,0x00,
	0x0C,0x1C,0x3C,0x6C,0xFE,0x0C,0x0C,0x00,
	0xFE,0xC0,0xF8,0x0C,0x06,0xCC,0x78,0x00,
	0x3C,0x60,0xC0,0xFC,0xC6,0xC6,0x7C,0x00,
	0xFE,0xC6,0x0C,0x18,0x30,0x30,0x30,0x00,
	0x7C,0xC6,0xC6,0x7C,0xC6,0xC6,0x7C,0x00,
	0x7C,0xC6,0xC6,0x7E,0x06,0x0C,0x78,0x00,
	0x00,0x30,0x00,0x00,0x00,0x30,0x00,0x00,
	0x00,0x30,0x00,0x00,0x00,0x30,0x30,0x60,
	0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x00,
	0x00,0x00,0xFE,0x00,0xFE,0x00,0x00,0x00,
	0x60,0x30,0x18,0x0C,0x18,0x30,0x60,0x00,
	0x7C,0xC6,0x06,0x1C,0x30,0x00,0x30,0x00,
	0x3C,0x66,0xDE,0xF6,0xDC,0x60,0x3E,0x00,
	0x38,0x6C,0xC6,0xFE,0xC6,0xC6,0xC6,0x00,
	0xFC,0x66,0x66,0x7C,0x66,0x66,0xFC,0x00,
	0x3C,0x66,0xC0,0xC0,0xC0,0x66,0x3C,0x00,
	0xF8,0x6C,0x66,0x66,0x66,0x6C,0xF8,0x00,
	0xFE,0xC0,0xC0,0xF8,0xC0,0xC0,0xFE,0x00,
	0xFE,0xC0,0xC0,0xF8,0xC0,0xC0,0xC0,0x00,
	0x3C,0x66,0xC0,0xCE,0xC6,0x66,0x3C,0x00,
	0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0x00,
	0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,
	0x1E,0x0C,0x0C,0x0C,0x0C,0xCC,0x78,0x00,
	0xC6,0xCC,0xD8,0xF0,0xD8,0xCC,0xC6,0x00,
	0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xFE,0x00,
	0xC6,0xEE,0xFE,0xD6,0xC6,0xC6,0xC6,0x00,
	0xC6,0xE6,0xF6,0xDE,0xCE,0xC6,0xC6,0x00,
	0x38,0x6C,0xC6,0xC6,0xC6,0x6C,0x38,0x00,
	0xFC,0xC6,0xC6,0xFC,0xC0,0xC0,0xC0,0x00,
	0x38,0x6C,0xC6,0xC6,0xDE,0x6C,0x3E,0x00,
	0xFC,0xC6,0xC6,0xFC,0xD8,0xCC,0xC6,0x00,
	0x7C,0xC6,0xC0,0x7C,0x06,0xC6,0x7C,0x00,
	0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00,
	0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,
	0xC6,0xC6,0xC6,0x6C,0x6C,0x38,0x38,0x00,
	0xC6,0xC6,0xC6,0xD6,0xFE,0xEE,0xC6,0x00,
	0xC6,0xC6,0x6C,0x38,0x6C,0xC6,0xC6,0x00,
	0xCC,0xCC,0xCC,0x78,0x30,0x30,0x30,0x00,
	0xFE,0x06,0x0C,0x38,0x60,0xC0,0xFE,0x00
*/
};
void g_pset(int x,int y,uint16_t c)
// (x,y)の位置にカラーcで点を描画
{
	if(x>=0 && x<X_RES && y>=0 && y<Y_RES)
		drawPixel(x,y,c);
}

void g_gline(int x1,int y1,int x2,int y2,uint16_t c)
// (x1,y1)-(x2,y2)にカラーcで線分を描画
{
	int sx,sy,dx,dy,i;
	int e;

	if(x2>x1){
		dx=x2-x1;
		sx=1;
	}
	else{
		dx=x1-x2;
		sx=-1;
	}
	if(y2>y1){
		dy=y2-y1;
		sy=1;
	}
	else{
		dy=y1-y2;
		sy=-1;
	}
	if(dx>=dy){
		e=-dx;
		for(i=0;i<=dx;i++){
			g_pset(x1,y1,c);
			x1+=sx;
			e+=dy*2;
			if(e>=0){
				y1+=sy;
				e-=dx*2;
			}
		}
	}
	else{
		e=-dy;
		for(i=0;i<=dy;i++){
			g_pset(x1,y1,c);
			y1+=sy;
			e+=dx*2;
			if(e>=0){
				x1+=sx;
				e-=dy*2;
			}
		}
	}
}
void g_hline(int x1,int x2,int y,uint16_t c)
// (x1,y)-(x2,y)への水平ラインを高速描画
{
	int temp;

	if(y<0 || y>=Y_RES) return;
	if(x1>x2){
		temp=x1;
		x1=x2;
		x2=temp;
	}
	if(x2<0 || x1>=X_RES) return;
	if(x1<0) x1=0;
	if(x2>=X_RES) x2=X_RES-1;
	LCD_continuous_output(x1,y,c,x2-x1+1);
}

void g_circle(int x0,int y0,unsigned int r,uint16_t c)
// (x0,y0)を中心に、半径r、カラーcの円を描画
{
	int x,y,f;
	x=r;
	y=0;
	f=-r*2+3;
	while(x>=y){
		g_pset(x0-x,y0-y,c);
		g_pset(x0-x,y0+y,c);
		g_pset(x0+x,y0-y,c);
		g_pset(x0+x,y0+y,c);
		g_pset(x0-y,y0-x,c);
		g_pset(x0-y,y0+x,c);
		g_pset(x0+y,y0-x,c);
		g_pset(x0+y,y0+x,c);
		if(f>=0){
			x--;
			f-=x<<2;
		}
		y++;
		f+=(y<<2)+2;
	}
}
void g_boxfill(int x1,int y1,int x2,int y2,uint16_t c)
// (x1,y1),(x2,y2)を対角線とするカラーcで塗られた長方形を描画
{
	int temp;
	if(x1>x2){
		temp=x1;
		x1=x2;
		x2=temp;
	}
	if(x2<0 || x1>=X_RES) return;
	if(y1>y2){
		temp=y1;
		y1=y2;
		y2=temp;
	}
	if(y2<0 || y1>=Y_RES) return;
	if(y1<0) y1=0;
	if(y2>=Y_RES) y2=Y_RES-1;
	while(y1<=y2){
		g_hline(x1,x2,y1++,c);
	}
}
void g_circlefill(int x0,int y0,unsigned int r,uint16_t c)
// (x0,y0)を中心に、半径r、カラーcで塗られた円を描画
{
	int x,y,f;
	x=r;
	y=0;
	f=3-2*r;
	while(x>=y){
		g_hline(x0-x,x0+x,y0-y,c);
		g_hline(x0-x,x0+x,y0+y,c);
		g_hline(x0-y,x0+y,y0-x,c);
		g_hline(x0-y,x0+y,y0+x,c);
		if(f>=0){
			x--;
			f-=x*4;
		}
		y++;
		f+=y*4+2;
	}
}
void g_putfont(int x,int y,uint16_t c,uint16_t bc,uint8_t n)
//8*8ドットのアルファベットフォント表示
//座標(x,y)、カラーc
//bc:バックグランドカラー
//n:文字番号
{
	int i,j,dx,dy;
	unsigned char d,b;
	const unsigned char *p;
	if(x<=-8 || x>=X_RES || y<=-8 || y>=Y_RES) return; //画面外
	n-=0x30;
	if(n>42) return;
	if(y<0){ //画面上部に切れる場合
		i=0;
		p=FontData+n*8-y;
		dy=8+y;
	}
	else{
		i=y;
		p=FontData+n*8;
		if(y+8<=Y_RES) dy=8;
		else dy=Y_RES-y;
	}
	if(x<0){ //画面左に切れる場合
		j=0;
		dx=8+x;
	}
	else{
		j=x;
		if(x+8<=X_RES) dx=8;
		else dx=X_RES-x;
	}
	LCD_setAddrWindow(j,i,dx,dy);
	for(;i<y+8;i++){
		if(i>=Y_RES) break; //画面下部に切れる場合
		d=*p++;
		if(x<0){ //画面左に切れる場合は残る部分のみ描画
			j=0;
			d<<=-x;
		}
		else{
			j=x;
		}
		for(;j<x+8;j++){
			if(j>=X_RES){ //画面右に切れる場合
				break;
			}
			if(d&0x80){
				LCD_WriteData2(c);
			}
			else{
				LCD_WriteData2(bc);
			}
			d<<=1;
		}
	}
}

void g_printstr(int x,int y,uint16_t c,uint16_t bc,unsigned char *s){
	//座標(x,y)からカラーcで文字列sを表示、bc:バックグランドカラー
	while(*s){
		g_putfont(x,y,c,bc,*s++);
		x+=8;
	}
}
void g_printnum(int x,int y,uint16_t c,uint16_t bc,uint16_t n){
	//座標(x,y)にカラーcで数値nを表示、bc:バックグランドカラー
	uint16_t d,e;
	d=10;
	e=0;
	while(n>=d){
		e++;
		if(e==9) break;
		d*=10;
	}
	x+=e*8;
	do{
		g_putfont(x,y,c,bc,'0'+n%10);
		n/=10;
		x-=8;
	}while(n!=0);
}
void g_printnum2(int x,int y,uint16_t c,uint16_t bc,uint16_t n,uint8_t e){
	//座標(x,y)にカラーcで数値nを表示、bc:バックグランドカラー、e桁で表示
	if(e==0) return;
	x+=(e-1)*8;
	do{
		g_putfont(x,y,c,bc,'0'+n%10);
		e--;
		n/=10;
		x-=8;
	}while(e!=0 && n!=0);
	while(e!=0){
		g_putfont(x,y,c,bc,' ');
		x-=8;
		e--;
	}
}
