/*
  input sizes
*/
#define HTML_INPUT_SPACE                 10
#define HTML_INPUT_SPACEH                0
#define HTML_INPUT_BUTTON_WIDTH(x)       (HTML_FUNC_FONT_GETSTRWIDTH(font_system, x, strlen(x))+30+HTML_INPUT_SPACE)
#define HTML_INPUT_BUTTON_HEIGHT(x)      (HTML_FUNC_FONT_GETHEIGHT(font_system)+5+HTML_INPUT_SPACEH)
#define HTML_INPUT_CHECKBOX_WIDTH(x)     (15+HTML_INPUT_SPACE)
#define HTML_INPUT_CHECKBOX_HEIGHT(x)    (15+HTML_INPUT_SPACEH)
#define HTML_INPUT_FILE_WIDTH(x)         HTML_INPUT_BUTTON_WIDTH(x)
#define HTML_INPUT_FILE_HEIGHT(x)        HTML_INPUT_BUTTON_HEIGHT(x)
#define HTML_INPUT_HIDDEN_WIDTH(x)       (x)
#define HTML_INPUT_HIDDEN_HEIGHT(x)      (x)
#define HTML_INPUT_IMAGE_WIDTH(x)        (x->w)
#define HTML_INPUT_IMAGE_HEIGHT(x)       (x->h)
#define HTML_INPUT_PASSWORD_WIDTH(x)     ((HTML_FUNC_FONT_GETCHARWIDTH(font_system, '*')*x)+30+HTML_INPUT_SPACE)
#define HTML_INPUT_PASSWORD_HEIGHT(x)    (HTML_FUNC_FONT_GETHEIGHT(font_system)+HTML_INPUT_SPACEH+5)
#define HTML_INPUT_RADIO_WIDTH(x)        (15+HTML_INPUT_SPACE)
#define HTML_INPUT_RADIO_HEIGHT(x)       (15+HTML_INPUT_SPACEH)
#define HTML_INPUT_RESET_WIDTH(x)        HTML_INPUT_BUTTON_WIDTH(x)
#define HTML_INPUT_RESET_HEIGHT(x)       HTML_INPUT_BUTTON_HEIGHT(x)
#define HTML_INPUT_SUBMIT_WIDTH(x)       HTML_INPUT_BUTTON_WIDTH(x)
#define HTML_INPUT_SUBMIT_HEIGHT(x)      HTML_INPUT_BUTTON_HEIGHT(x)
#define HTML_INPUT_TEXT_WIDTH(x)         ((HTML_FUNC_FONT_GETCHARWIDTH(font_system, 'w')*x)+30+HTML_INPUT_SPACE)
#define HTML_INPUT_TEXT_HEIGHT(x)        (HTML_FUNC_FONT_GETHEIGHT(font_system)+HTML_INPUT_SPACEH+5)


/*
  icons for htmloader
*/

#define HI_ICONS        1

#define HI_NOIMAGE      0

#define HLOADIMG(i)     ((i<HI_ICONS)?(BITMAP*)GET_DATA(datfile, i):NULL)



/*
  html redeclaration
*/

#define HTML_PATH_MAX            MAXPATH
#define HTML_BLOCKLEVEL_SPACE    20
#define HTML_LISTS_SPACE         20
#define HTML_LINE_SPACE          10
#define HTML_BRLINE_SPACE        HTML_LINE_SPACE



/*
  html bitmap redeclaration
*/

static BITMAP *html_bitmap_stretch ( BITMAP *b, l_int w, l_int h )
{
  BITMAP *newb = b;

  if ( b ) {

    newb = create_bitmap_ex(get_depth(b), w, h);

    if ( newb )

      stretch_sprite(newb, b, 0, 0, w, h);

  };

  return newb;
};


#define HTML_FUNC_BITMAP_DESTROY(b)        destroy_bitmap(b)
#define HTML_FUNC_BITMAP_LOAD(f)           load_image(f)
#define HTML_FUNC_BITMAP_GETWIDTH(b)       IMAGE_WIDTH(b)
#define HTML_FUNC_BITMAP_GETHEIGHT(b)      IMAGE_HEIGHT(b)
#define HTML_FUNC_BITMAP_STRETCH(b,w,h)    html_bitmap_stretch(b,w,h)
#define HTML_BITMAP_NOSIZEX                (HLOADIMG(HI_NOIMAGE)?(IMAGE_WIDTH(((BITMAP*)(datfile[HI_NOIMAGE].dat)))+8):0)
#define HTML_BITMAP_NOSIZEY                (HLOADIMG(HI_NOIMAGE)?(IMAGE_HEIGHT(((BITMAP*)(datfile[HI_NOIMAGE].dat)))+8):0)
#define HTML_BITMAP_MINIMUMX               10
#define HTML_BITMAP_MINIMUMY               10

/*
  html color redeclaration
*/

#define HTML_FUNC_COLOR_MAKERGB(r,g,b)     makecol(r, g, b)
#define HTML_COLOR_ANCHOR                  pal_htmloader[2]
#define HTML_COLOR_VLINK                   pal_htmloader[3]
#define HTML_COLOR_ALINK                   pal_htmloader[3]


static l_font *load_f ( l_text filename, l_int w, l_int h ) {
  l_font *f = get_font_in_size(filename, w, h);
  if ( !f ) return font_system;
};

/*
  html font redeclaration
*/
#define HTML_FUNC_FONT_LOAD(f,w,h)           load_f(f, w, h)
#define HTML_FUNC_FONT_GETCHARWIDTH(f,c)     FONT_GETWIDTH(f,c)
#define HTML_FUNC_FONT_GETSTRWIDTH(f,str,s)  FONT_GETSTRWIDTH(f,str,s)
#define HTML_FUNC_FONT_GETHEIGHT(f)          FONT_GETHEIGHT(f)

#define HTML_FONT_NAMES
l_text  HTML_FONT_REGULAR_NAME         = "Times";
l_text  HTML_FONT_ITALIC_NAME          = "TimesItalic";
l_text  HTML_FONT_BOLD_NAME            = "TimesBold";
l_text  HTML_FONT_BOLDITALIC_NAME      = "TimesBoldItalic";
l_text  HTML_FONT_FIXED_NAME           = "Courier";
l_text  HTML_FONT_FIXEDBOLD_NAME       = "CourierBold";
l_text  HTML_FONT_FIXEDITALIC_NAME     = "CourierItalic";
l_text  HTML_FONT_FIXEDBOLDITALIC_NAME = "CourierBoldItalic";
l_text  HTML_FONT_DIRECTORY_NAME       = NULL;


/*
  ...end of html redeclaration
*/
