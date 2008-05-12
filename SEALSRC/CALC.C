/*
 * Calculator
 *
 *
*/

#include<math.h>

#include"allegro.h"
#include"seal.h"
#include"app.h"
#include"button.h"
#include"dialogs.h"


#define  MSG_0          100001
#define  MSG_1          100002
#define  MSG_2          100003
#define  MSG_3          100004
#define  MSG_4          100005
#define  MSG_5          100006
#define  MSG_6          100007
#define  MSG_7          100008
#define  MSG_8          100009
#define  MSG_9          100010
#define  MSG_MINUS      100011
#define  MSG_PLUS       100012
#define  MSG_DELENO     100013
#define  MSG_KRAT       100014
#define  MSG_PERCENT    100015
#define  MSG_SQRT       100016
#define  MSG_ROVNO      100017
#define  MSG_C          100018
#define  MSG_PLUSMINUS  100019
#define  MSG_BODKA      100020
#define  MSG_PI         100021
#define  MSG_SQR        100022

#define  RECT           30
#define  RECT1          40

p_textline main_text = NULL;

l_bool     main_c = true;
l_dword    main_operacia = 0;
l_bool     main_bodka = false;
double     main_vysledok = 0;
l_bool     main_minus = false;

static p_appwin  main_form;


/*
 *
 *  Form translate_event function
 *
*/

l_text del_minus ( l_text txt )
{
  l_text old = txt;

  if ( !txt ) return NULL;

  txt = strchr(txt, '-');

  if ( txt ) {
      while ( *txt ) {
        *txt = *++txt;
      };

  };

  return _strdup(old);

};


l_text ignore_zeros ( l_text txt )
{
   l_text old = txt;
   l_text last = NULL;

   if ( !txt ) return NULL;

   txt = strchr(txt, '.');

   if ( txt )
   while ( *txt ) {

      if ( !last && *txt == '0' ) last = txt;
      else last = NULL;

      txt++;

   };

   if ( last ) *last = 0;

   return old;
};

void  make_operation ( l_dword oper )
{

   l_text text = NULL;

   l_bool is_0 = (l_bool)(main_text->text && !strcmp(main_text->text,"0"));
   l_bool err = false;

   if ( !main_c || is_0 ) {

         if ( main_operacia == 0 ) main_vysledok = atof(main_text->text);

         if ( main_operacia == MSG_PLUS ) main_vysledok += atof(main_text->text);
         if ( main_operacia == MSG_MINUS ) main_vysledok -= atof(main_text->text);
         if ( main_operacia == MSG_KRAT ) main_vysledok *= atof(main_text->text);
         if ( main_operacia == MSG_PERCENT ) main_vysledok = main_vysledok*(atof(main_text->text)/100.0);
         /* nedelit nulou */
         if ( main_operacia == MSG_DELENO )

            if ( !is_0 ) main_vysledok /= atof(main_text->text);

            else {

               err = true;

               text = _strdup(INI_TEXT("Error : Dividing by ZERO"));
            };

         if ( !text ) {

               text = set_format_text(NULL, "%G", main_vysledok);

               text = ignore_zeros(text);

         };

         main_text->set_text(main_text, text);

   };

   main_c = true;
   main_bodka = false;
   main_minus = false;

   if ( oper != MSG_ROVNO && !err ) main_operacia = oper;
   else main_operacia = 0;

   _free(text);

};

void  form_translate_event ( p_object o, p_event e )
{
  // TODO : translate events

  RETVIEW(o, e);

  if ( o->phase == PH_PREPROCESS ) {

    if ( e->type == EV_KEYBOARD ) {

        if ( OBJECT(keyb)->state & KB_SF_KEYDOWN ) {

            switch ( KEY_TO(keyb->code) ) {

                case KB_DEL : set_event(e, EV_MESSAGE, MSG_C, o); break;
                case KB_ENTER : set_event(e, EV_MESSAGE, MSG_ROVNO, o); break;

            };

            if ( e->type == EV_KEYBOARD )

            switch ( (l_byte)TO_CHAR(keyb->code) ) {

                case '1' : set_event(e, EV_MESSAGE, MSG_1, o); break;
                case '2' : set_event(e, EV_MESSAGE, MSG_2, o); break;
                case '3' : set_event(e, EV_MESSAGE, MSG_3, o); break;
                case '4' : set_event(e, EV_MESSAGE, MSG_4, o); break;
                case '5' : set_event(e, EV_MESSAGE, MSG_5, o); break;
                case '6' : set_event(e, EV_MESSAGE, MSG_6, o); break;
                case '7' : set_event(e, EV_MESSAGE, MSG_7, o); break;
                case '8' : set_event(e, EV_MESSAGE, MSG_8, o); break;
                case '9' : set_event(e, EV_MESSAGE, MSG_9, o); break;
                case '0' : set_event(e, EV_MESSAGE, MSG_0, o); break;
                case '+' : set_event(e, EV_MESSAGE, MSG_PLUS, o); break;
                case '-' : set_event(e, EV_MESSAGE, MSG_MINUS, o); break;
                case '.' : set_event(e, EV_MESSAGE, MSG_BODKA, o); break;
                case '/' : set_event(e, EV_MESSAGE, MSG_DELENO, o); break;
                case '*' : set_event(e, EV_MESSAGE, MSG_KRAT, o); break;

            };

        };

    };

  };

  if ( o->phase == PH_POSTPROCESS ) {

    if ( e->type == EV_MESSAGE ) {

        switch ( e->message ) {

             case MSG_1 : {

                l_text text = main_c?_strdup("1"):set_format_text(NULL, "%s%c", main_text->text, '1');

                main_text->set_text(main_text, text);

                _free(text);

                main_c = false;

                clear_event(e);

             }; break;

             case MSG_2 : {

                l_text text = main_c?_strdup("2"):set_format_text(NULL, "%s%c", main_text->text, '2');

                main_text->set_text(main_text, text);

                _free(text);

                main_c = false;

                clear_event(e);

             }; break;

             case MSG_3 : {

                l_text text = main_c?_strdup("3"):set_format_text(NULL, "%s%c", main_text->text, '3');

                main_text->set_text(main_text, text);

                _free(text);

                main_c = false;

                clear_event(e);

             }; break;

             case MSG_4 : {

                l_text text = main_c?_strdup("4"):set_format_text(NULL, "%s%c", main_text->text, '4');

                main_text->set_text(main_text, text);

                _free(text);

                main_c = false;

                clear_event(e);

             }; break;

             case MSG_5 : {

                l_text text = main_c?_strdup("5"):set_format_text(NULL, "%s%c", main_text->text, '5');

                main_text->set_text(main_text, text);

                _free(text);

                main_c = false;

                clear_event(e);

             }; break;

             case MSG_6 : {

                l_text text = main_c?_strdup("6"):set_format_text(NULL, "%s%c", main_text->text, '6');

                main_text->set_text(main_text, text);

                _free(text);

                main_c = false;

                clear_event(e);

             }; break;

             case MSG_7 : {

                l_text text = main_c?_strdup("7"):set_format_text(NULL, "%s%c", main_text->text, '7');

                main_text->set_text(main_text, text);

                _free(text);

                main_c = false;

                clear_event(e);

             }; break;

             case MSG_8 : {

                l_text text = main_c?_strdup("8"):set_format_text(NULL, "%s%c", main_text->text, '8');

                main_text->set_text(main_text, text);

                _free(text);

                main_c = false;

                clear_event(e);

             }; break;

             case MSG_9 : {

                l_text text = main_c?_strdup("9"):set_format_text(NULL, "%s%c", main_text->text, '9');

                main_text->set_text(main_text, text);

                _free(text);

                main_c = false;

                clear_event(e);

             }; break;

             case MSG_0 : {

                l_text text = main_c?_strdup("0"):set_format_text(NULL, "%s%c", main_text->text, '0');

                main_text->set_text(main_text, text);

                _free(text);

                clear_event(e);

             }; break;

             case MSG_PLUS  : {

                make_operation(e->message);

                clear_event(e);

             }; break;

             case MSG_MINUS : {

                make_operation(e->message);

                clear_event(e);

             }; break;

             case MSG_PERCENT : {

                make_operation(e->message);

                clear_event(e);

             }; break;

             case MSG_DELENO : {

                make_operation(e->message);

                clear_event(e);

             }; break;

             case MSG_KRAT : {

                make_operation(e->message);

                clear_event(e);

             }; break;

             case MSG_SQRT : {

                make_operation(e->message);

                clear_event(e);

             }; break;

             case MSG_SQR : {

                make_operation(e->message);

                clear_event(e);

             }; break;

             case MSG_ROVNO : {

                make_operation(e->message);

                clear_event(e);

             }; break;

             case MSG_C : {

                main_text->set_text(main_text, "0");

                main_c = true;

                main_bodka = false;

                main_minus = false;

                main_vysledok = 0.0;

                main_operacia = 0;

                clear_event(e);

             }; break;

             case MSG_PLUSMINUS : if ( !main_c ) {

                l_text text = main_minus?del_minus(main_text->text):set_format_text(NULL, "-%s", main_text->text);

                main_text->set_text(main_text, text);

                _free(text);

                main_minus = !main_minus;

                clear_event(e);

             }; break;

             case MSG_BODKA : if ( !main_bodka ) {

                l_text text = main_c?_strdup("0."):set_format_text(NULL, "%s%c", main_text->text, '.');

                main_text->set_text(main_text, text);

                _free(text);

                main_c = false;

                main_bodka = true;

                clear_event(e);

             }; break;

             case MSG_PI : {

                l_text text = set_format_text(NULL, "%f", M_PI);

                main_text->set_text(main_text, text);

                _free(text);

                main_c = false;

                main_bodka = false;

                clear_event(e);

             }; break;

        };

    };

  };

};


/*
 * Init application window
 *
*/

void init_form ( void )
{

  // create new application window

  t_rect r = rect_assign(100, 100, 355, 330);
  t_rect s;

  p_textline t;
  p_button   b;

  main_form = new_application(r, "Calculator", WF_MINIMIZE, &form_translate_event);


  // insert application window to the desktop

  OBJECT(desktop)->insert(OBJECT(desktop), OBJECT(main_form));

  // TODO : insert other objects to the form

  s = VIEW(main_form)->size_limits(VIEW(main_form));

  s = rect_assign(s.a.x+10, s.a.y+10, s.b.x-10, s.b.y-10);


  r = rect_assign(s.a.x, s.a.y, s.b.x, s.a.y+20);

  main_text = textline_init(_malloc(sizeof(t_textline)), r, 40, 1+2);

  s = rect_assign(r.a.x, r.b.y+10, r.b.x, s.b.y);


  r = rect_assign(r.a.x, r.b.y+10, r.a.x+RECT, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "7", MSG_7, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(main_text));

  OBJECT(main_text)->set_options(OBJECT(main_text), OB_OF_SELECTABLE, false);

  main_text->set_text(main_text, "0");


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "4", MSG_4, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "1", MSG_1, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "0", MSG_0, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.b.x+10, s.a.y, r.b.x+RECT1, s.a.y+RECT);

  b = button_init(_malloc(sizeof(t_button)), r, "8", MSG_8, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "5", MSG_5, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "2", MSG_2, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "+/-", MSG_PLUSMINUS, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.b.x+10, s.a.y, r.b.x+RECT1, s.a.y+RECT);

  b = button_init(_malloc(sizeof(t_button)), r, "9", MSG_9, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "6", MSG_6, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "3", MSG_3, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, ".", MSG_BODKA, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.b.x+10, s.a.y, r.b.x+RECT1, s.a.y+RECT);

  b = button_init(_malloc(sizeof(t_button)), r, "+", MSG_PLUS, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "-", MSG_MINUS, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "*", MSG_KRAT, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "/", MSG_DELENO, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.b.x+10, s.a.y, r.b.x+RECT+RECT1+10, s.a.y+RECT);

  b = button_init(_malloc(sizeof(t_button)), r, "C", MSG_C, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x-RECT1, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "sqrt", MSG_SQRT, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "pi", MSG_PI, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x+RECT1, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "=", MSG_ROVNO, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.b.x-RECT, s.a.y+RECT1, r.b.x, s.a.y+RECT1+RECT);

  b = button_init(_malloc(sizeof(t_button)), r, "sqr", MSG_SQR, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));


  r = rect_assign(r.a.x, r.b.y+10, r.b.x, r.b.y+RECT1);

  b = button_init(_malloc(sizeof(t_button)), r, "%", MSG_PERCENT, BF_NORMAL);

  OBJECT(main_form)->insert(OBJECT(main_form), OBJECT(b));

};


/*
 * Application main function
 *
*/
app_begin ( void ) {

  // begin of application

  if ( ap_process == AP_INIT ) {

     // maximum number of calls

     AP_SETNUMOFCALLS(1);

     // TODO : your functions

     init_form();

  };

  // end of application

  if ( ap_process == AP_DONE ) {

  };

} app_end;
