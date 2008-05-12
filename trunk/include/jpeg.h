/******************************************************************/
/*                  libjpeg for Allegro                           */
/*                  by Eric Vannier                               */
/*                                                                */
/*    Version : 1.03                                              */
/*    Date    : 02/03/98                                          */
/*                                                                */
/*   If you need some information read the readme.1st             */
/*                                                                */
/******************************************************************/


#ifndef JPEG_H
#define JPEG_H
#define JPEG_VERSION 1
#define JPEG_SUB_VERSION 0
#define JPEG_REVISION 3
#define JPEG_DESC "jpeg library v1.03 based on IJG v6.0a"
#define JPEG_AUTHOR "Eric Vannier as known as Fweibuli"

#ifndef ALLEGRO_H
/* make sure that the Allegro header is included      */
#include <allegro.h>
#endif /* ALLEGRO_H */

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_QUALITY 75

typedef enum {
 CT_RGB,
 CT_GRAYSCALE
} color_type_t;

typedef enum {
 DS_SLOW,
 DS_FAST
} decoding_speed_t;



typedef struct {
int quality; /* quality factor : from 0 to 100. Lowest value result in a   */
             /* poor quality/good compression image                        */
color_type_t color_type; /* type of output : color or grayscale output.    */
                         /* The input should be always RGB encoded with    */
                         /* Grayscale Y value used                         */
} jpeg_code_param ;

typedef struct {
color_type_t color_type; /* type of the result : color or grayscale output.*/
                         /* this will be always RGB coded                  */
int scale_factor;        /* scale factor of the output. It must be 1 , 2,  */
                         /* 4 ,8. 1 is default. The main interest is to    */
                         /* have thumbnail images and therefore fast       */
                         /* decompression                                  */
decoding_speed_t decoding_speed;
                         /* Unless you want to save for thumbnail output   */
                         /* for example, this should be DS_SLOW, since a   */
                         /* fast output may result in a lower quality      */
unsigned int first_color; /* first color to be altered in the palette.     */
                          /* Only used in 8 bits mode. It is faster if it  */
                          /* equals zero.                                  */
unsigned int nbr_colors; /* number of colors to be altered in the palette. */
                         /* Only used in 8 bits mode. This should be used  */
                         /* with the previous option.                      */
                         /* first_color+nbr_colors must be less than 256   */
} jpeg_decode_param;




/* Desc : Loads a jpeg image specified in filename    */
/*        according to the params specified in *jpeg  */
/* Params :                                           */
/*   filename : name of the input jpeg file           */
/*   pal      : pointer for the palette in 8bits mode */
/*   jpeg     : pointer to a jpeg_decode_param struct */
/* Return Value : the bitmap loaded. It is left to the*/
/*   user to free the memory with destroy_bitmap      */
/*   returns NULL on error                            */
BITMAP *load_jpeg_ex(char *filename, RGB *pal,jpeg_decode_param *jpeg);


/* Desc : Loads a jpeg image specified in filename    */
/* Params :                                           */
/*   filename : name of the input jpeg file           */
/*   pal      : pointer for the palette in 8bits mode */
/* Return Value : the bitmap loaded. It is left to the*/
/*   user to free the memory with destroy_bitmap      */
/*   returns NULL on error                            */
BITMAP *load_jpeg(char *filename, RGB *pal);

/* Desc : saves a bitmap according to the *jpeg struct*/
/* Params :                                           */
/*   filename : name of the output jpeg file          */
/*   pal      : palette to be used in 8bits mode      */
/*   jpeg     : pointer to a jpeg_decode_param struct */
/*              which specifies the parameters of the */
/*              operation.                            */
/* Return Value : zero on success, non zero otherwise */
int save_jpeg_ex(char *filename, BITMAP *bmp, RGB *pal,jpeg_code_param *jpeg);

/* Desc : saves a bitmap according to the jpeg format */
/* Params :                                           */
/*   filename : name of the output jpeg file          */
/*   pal      : palette to be used in 8bits mode      */
/* Return Value : zero on success, non zero otherwise */
int save_jpeg(char *filename, BITMAP *bmp, RGB *pal);


#ifdef __cplusplus
}
#endif



#endif /*JPEG_H */




