/**************************************************************************
 *
 *  rgb2yuv.c, 24-bit RGB bitmap to YUV converter
 *
 *  Copyright (C) 2001  Project Mayo
 *
 *  Adam Li
 *
 *  DivX Advance Research Center <darc@projectmayo.com>
 *
 **************************************************************************/
 
#include <stdlib.h>
#include "rgb2yuv.h"

static int RGB2YUV_YR[256], RGB2YUV_YG[256], RGB2YUV_YB[256];
static int RGB2YUV_UR[256], RGB2YUV_UG[256], RGB2YUV_UBVR[256];
static int                  RGB2YUV_VG[256], RGB2YUV_VB[256];

void init_lookup_table()
{
    int i;

    for (i = 0; i < 256; i++) RGB2YUV_YR[i] = 65.481 * (i<<8);
    for (i = 0; i < 256; i++) RGB2YUV_YG[i] = 128.553 * (i<<8);
    for (i = 0; i < 256; i++) RGB2YUV_YB[i] = 24.966 * (i<<8);
    for (i = 0; i < 256; i++) RGB2YUV_UR[i] = 37.797 * (i<<8);
    for (i = 0; i < 256; i++) RGB2YUV_UG[i] = 74.203 * (i<<8);
    for (i = 0; i < 256; i++) RGB2YUV_VG[i] = 93.786 * (i<<8);
    for (i = 0; i < 256; i++) RGB2YUV_VB[i] = 8.214 * (i<<8);
    for (i = 0; i < 256; i++) RGB2YUV_UBVR[i] = 112 * (i<<8);
}

/************************************************************************
 *
 *  int RGB2YUV420 (int x_dim, int y_dim,
 *              unsigned char *bmp,
 *              unsigned char *yuv, int flip)
 *
 *  Purpose :   It takes a 24-bit RGB bitmap and convert it into
 *              YUV (4:2:0) format
 *
 *  Input :     x_dim   the x dimension of the bitmap
 *              y_dim   the y dimension of the bitmap
 *              bmp     pointer to the buffer of the bitmap
 *              yuv     pointer to the YUV structure
 *              flip    0: RGB order in bmp; 1: BGR order in bmp
 *
 *
 *
 ************************************************************************/
int rgb24_to_yuv420 (int x_dim, int y_dim,
             unsigned char *bmp,
             unsigned char *yuv, int flip)
{
    int i, j;
    unsigned char *r, *g, *b;
    unsigned char *y, *u, *v;
    unsigned char *uu, *vv;
    unsigned char *pu1, *pu2,*pu3,*pu4;
    unsigned char *pv1, *pv2,*pv3,*pv4;

    if(flip==0){
        r=bmp;
        g=bmp+1;
        b=bmp+2;
    }
    else{
        b=bmp;
        g=bmp+1;
        r=bmp+2;
    }

    y=yuv;
    //uu=new unsigned char[x_dim*y_dim];
    //vv=new unsigned char[x_dim*y_dim];
    uu = (unsigned char *)malloc(sizeof(char) * x_dim * y_dim);
    vv = (unsigned char *)malloc(sizeof(char) * x_dim * y_dim);
    u=uu;
    v=vv;
    for (i=0;i<y_dim;i++){
        for (j=0;j<x_dim;j++){
            *y++=( RGB2YUV_YR[*r]  +RGB2YUV_YG[*g]+RGB2YUV_YB[*b]+1048576)>>16;
            *u++=(-RGB2YUV_UR[*r]  -RGB2YUV_UG[*g]+RGB2YUV_UBVR[*b]+8388608)>>16;
            *v++=( RGB2YUV_UBVR[*r]-RGB2YUV_VG[*g]-RGB2YUV_VB[*b]+8388608)>>16;

            r+=3;
            g+=3;
            b+=3;
        }
    }

    //dimension reduction for U and V components
    u=yuv+x_dim*y_dim;
    v=u+x_dim*y_dim/4;

    pu1=uu;
    pu2=pu1+1;
    pu3=pu1+x_dim;
    pu4=pu3+1;

    pv1=vv;
    pv2=pv1+1;
    pv3=pv1+x_dim;
    pv4=pv3+1;
    for(i=0;i<y_dim;i+=2){
        for(j=0;j<x_dim;j+=2){
            *u++=(*pu1+*pu2+*pu3+*pu4)>>2;
            *v++=(*pv1+*pv2+*pv3+*pv4)>>2;
            pu1+=2;
            pu2+=2;
            pu3+=2;
            pu4+=2;
            pv1+=2;
            pv2+=2;
            pv3+=2;
            pv4+=2;
        }
        pu1+=x_dim;
        pu2+=x_dim;
        pu3+=x_dim;
        pu4+=x_dim;
        pv1+=x_dim;
        pv2+=x_dim;
        pv3+=x_dim;
        pv4+=x_dim;
    }
    free(uu);
    free(vv);
    //delete []uu;
    //delete []vv;
    return 0;
}

#define SCALEBITS            8
#define ONE_HALF             (1 << (SCALEBITS - 1))
#define FIX(x)               ((int) ((x) * (1L<<SCALEBITS) + 0.5))
typedef unsigned char        uint8_t;
void rgb24_to_yuv420p(uint8_t *lum, uint8_t *cb, uint8_t *cr, uint8_t *src, int width, int height)
{
    int wrap, wrap3, x, y;
    int r, g, b, r1, g1, b1;
    uint8_t *p;
    wrap = width;
    wrap3 = width * 3;
    p = src;
    for (y = 0; y < height; y += 2)
    {
        for (x = 0; x < width; x += 2)
        {
            r = p[0];
            g = p[1];
            b = p[2];
            r1 = r;
            g1 = g;
            b1 = b;
            lum[0] = (FIX(0.29900) * r + FIX(0.58700) * g +FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
            r = p[3];
            g = p[4];
            b = p[5];
            r1 += r;
            g1 += g;
            b1 += b;
            lum[1] = (FIX(0.29900) * r + FIX(0.58700) * g +FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
            p += wrap3;
            lum += wrap;
            r = p[0];
            g = p[1];
            b = p[2];
            r1 += r;
            g1 += g;
            b1 += b;
            lum[0] = (FIX(0.29900) * r + FIX(0.58700) * g +FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
            r = p[3];
            g = p[4];
            b = p[5];
            r1 += r;
            g1 += g;
            b1 += b;
            lum[1] = (FIX(0.29900) * r + FIX(0.58700) * g +FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;

            cb[0] = (((- FIX(0.16874) * r1 - FIX(0.33126) * g1 +
                FIX(0.50000) * b1 + 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128);
            cr[0] = (((FIX(0.50000) * r1 - FIX(0.41869) * g1 -
                FIX(0.08131) * b1 + 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128);
            cb++;
            cr++;
            p += -wrap3+2 * 3;
            lum += -wrap + 2;
        }
        p += wrap3;
        lum += wrap;
    }
}