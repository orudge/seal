#include"allegro.h"

#define _INLINE_
#define fix     fixed


#ifdef __cplusplus
extern "C" {
#endif
  extern int _textmode;
  extern BLENDER_MAP _trans_blender15;
  extern BLENDER_MAP _trans_blender16;
  extern BLENDER_MAP _trans_blender24;
  extern int _blender_alpha;
  extern int _color_conv;
  int _color_load_depth(int _depth);
  BITMAP  *_fixup_loaded_bitmap ( BITMAP *bmp, PALETTE pal, int bpp );
  _INLINE_ int bitmap_color_depth(BITMAP *bmp);
  _INLINE_ int bitmap_mask_color(BITMAP *bmp);
  _INLINE_ int is_same_bitmap(BITMAP *bmp1, BITMAP *bmp2);
  _INLINE_ int is_linear_bitmap(BITMAP *bmp);
  _INLINE_ int is_planar_bitmap(BITMAP *bmp);
  _INLINE_ int is_memory_bitmap(BITMAP *bmp);
  _INLINE_ int is_screen_bitmap(BITMAP *bmp);
  _INLINE_ int is_sub_bitmap(BITMAP *bmp);
  _INLINE_ void vsync();
  _INLINE_ void _set_color(int index, RGB *p);
  _INLINE_ int makecol15(int r, int g, int b);
  _INLINE_ int makecol16(int r, int g, int b);
  _INLINE_ int makecol24(int r, int g, int b);
  _INLINE_ int makecol32(int r, int g, int b);
  _INLINE_ int getr8(int c);
  _INLINE_ int getg8(int c);
  _INLINE_ int getb8(int c);
  _INLINE_ int getr15(int c);
  _INLINE_ int getg15(int c);
  _INLINE_ int getb15(int c);
  _INLINE_ int getr16(int c);
  _INLINE_ int getg16(int c);
  _INLINE_ int getb16(int c);
  _INLINE_ int getr24(int c);
  _INLINE_ int getg24(int c);
  _INLINE_ int getb24(int c);
  _INLINE_ int getr32(int c);
  _INLINE_ int getg32(int c);
  _INLINE_ int getb32(int c);
  _INLINE_ int getpixel(BITMAP *bmp, int x, int y);
  _INLINE_ void putpixel(BITMAP *bmp, int x, int y, int color);
  _INLINE_ void vline(BITMAP *bmp, int x, int y1, int y2, int color);
  _INLINE_ void hline(BITMAP *bmp, int x1, int y, int x2, int color);
  _INLINE_ void line(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
  _INLINE_ void rectfill(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
  _INLINE_ void draw_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y);
  _INLINE_ void draw_sprite_v_flip(BITMAP *bmp, BITMAP *sprite, int x, int y);
  _INLINE_ void draw_sprite_h_flip(BITMAP *bmp, BITMAP *sprite, int x, int y);
  _INLINE_ void draw_sprite_vh_flip(BITMAP *bmp, BITMAP *sprite, int x, int y);
  _INLINE_ void draw_trans_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y);
  _INLINE_ void draw_lit_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int color);
  _INLINE_ void draw_character(BITMAP *bmp, BITMAP *sprite, int x, int y, int color);
  _INLINE_ void draw_rle_sprite(BITMAP *bmp, struct RLE_SPRITE *sprite, int x, int y);
  _INLINE_ void draw_trans_rle_sprite(BITMAP *bmp, struct RLE_SPRITE *sprite, int x, int y);
  _INLINE_ void draw_lit_rle_sprite(BITMAP *bmp, struct RLE_SPRITE *sprite, int x, int y, int color);
  _INLINE_ void clear_to_color(BITMAP *bitmap, int color);

  _INLINE_ unsigned long bmp_write_line(BITMAP *bmp, int line);
  _INLINE_ unsigned long bmp_read_line(BITMAP *bmp, int line);
  _INLINE_ void _putpixel(BITMAP *bmp, int x, int y, unsigned char color);
  _INLINE_ unsigned char _getpixel(BITMAP *bmp, int x, int y);
  _INLINE_ int pack_getc(PACKFILE *f);
  _INLINE_ int pack_putc(int c, PACKFILE *f);
  _INLINE_ fixed dot_product(fixed x1, fixed y1, fixed z1, fixed x2, fixed y2, fixed z2);
  _INLINE_ float dot_product_f(float x1, float y1, float z1, float x2, float y2, float z2);
  _INLINE_ void apply_matrix(MATRIX *m, fixed x, fixed y, fixed z, fixed *xout, fixed *yout, fixed *zout);
  _INLINE_ void persp_project_f(float x, float y, float z, float *xout, float *yout);

#if !defined alleg_math_unused
  _INLINE_ fixed itofix(int x);
  _INLINE_ int fixtoi(fixed x);
  _INLINE_ fixed ftofix(double x);
  _INLINE_ double fixtof(fixed x);

  _INLINE_ fixed fcos(fixed x);
  _INLINE_ fixed fsin(fixed x);
  _INLINE_ fixed ftan(fixed x);
  _INLINE_ fixed facos(fixed x);
  _INLINE_ fixed fasin(fixed x);
  _INLINE_ fixed fadd(fixed x, fixed y);
  _INLINE_ fixed fsub(fixed x, fixed y);
  _INLINE_ fixed fmul(fixed x, fixed y);
  _INLINE_ fixed fdiv(fixed x, fixed y);
#endif

#ifndef ALLEG3UP
  _INLINE_ void get_y_rotate_matrix(MATRIX *m, fix r);
  _INLINE_ void qtranslate_matrix(MATRIX *m, fix x, fix y, fix z);
  _INLINE_ void persp_project(fixed x, fixed y, fixed z, fixed *xout, fixed *yout);
  _INLINE_ void get_translation_matrix(MATRIX *m, fix x, fix y, fix z);
  _INLINE_ void get_x_rotate_matrix(MATRIX *m, fix r);
  _INLINE_ void get_rotation_matrix(MATRIX *m, fix x, fix y, fix z);
  _INLINE_ void get_camera_matrix(MATRIX *m, fix x, fix y, fix z, fix xfront, fix yfront, fix zfront, fix xup, fix yup, fix zup, fix fov, fix aspect);
  _INLINE_ void get_transformation_matrix(MATRIX *m, fix scale, fix xrot, fix yrot, fix zrot, fix x, fix y, fix z);
  _INLINE_ void get_vector_rotation_matrix(MATRIX *m, fix x, fix y, fix z, fix a);
  _INLINE_ void get_scaling_matrix(MATRIX *m, fix x, fix y, fix z);
  _INLINE_ void get_z_rotate_matrix(MATRIX *m, fix r);
  _INLINE_ void normalize_vector(fix *x, fix *y, fix *z);
  _INLINE_ void get_align_matrix(MATRIX *m, fix xfront, fix yfront, fix zfront, fix xup, fix yup, fix zup);
  _INLINE_ void qscale_matrix(MATRIX *m, fix scale);
  _INLINE_ fix dot_product(fix x1, fix y1, fix z1, fix x2, fix y2, fix z2);
  _INLINE_ void apply_matrix(MATRIX *m, fix x, fix y, fix z, fix *xout, fix *yout, fix *zout);
  _INLINE_ fix vector_length(fix x, fix y, fix z);
  _INLINE_ void cross_product(fix x1, fix y1, fix z1, fix x2, fix y2, fix z2, fix *xout, fix *yout, fix *zout);
#endif

#ifdef __cplusplus
};
#endif

#define ALLEG3UP 1


void  __DZCOMM_H__call_all_function ( void ) {
};


void  __ALLEGRO_H__call_all_functions ( void ) {

GFX_DRIVER gf;
_GFX_DRIVER_INFO gl;
GFX_VTABLE vt;
_VTABLE_INFO inf;
RGB rgb;
DIGI_DRIVER dg;
MATRIX mx;
MATRIX_f mxf;
_DIGI_DRIVER_INFO ddi;
_MIDI_DRIVER_INFO mdi;
MIDI_DRIVER md;
BLENDER_MAP m;
PALETTE pal;

/* begin of not allegro.h functions */


  _cos_tbl[0] = 0;
  _tan_tbl[0] = 0;
  _acos_tbl[0] = 0;
  _color_load_depth(0);
  _textmode = 0;
  _trans_blender15 = m;
  _trans_blender16 = m;
  _trans_blender24 = m;
  _blender_alpha = 0;
  _color_conv = 0;

/* ebd of not allegro.h functions */

allegro_id[0] = 0;
allegro_error[0] = 0;
os_type = 0;
windows_version = 0, windows_sub_version = 0;

allegro_init();
allegro_exit();
check_cpu();
cpu_vendor[0] = 0;
cpu_family = 0;
cpu_model = 0;
cpu_fpu = 0;
cpu_mmx = 0;
#ifdef ALLEG3UP
cpu_3dnow = 0;
#endif
cpu_cpuid = 0;

_fixup_loaded_bitmap(NULL, pal, 0);

lock_bitmap(NULL);
lock_sample(NULL);
lock_midi(NULL);

set_config_file(NULL);
set_config_data(NULL, 0);
override_config_file(NULL);
override_config_data(NULL, 0);

push_config_state();
pop_config_state();

get_config_string(NULL, NULL, NULL);
get_config_int(NULL, NULL, 0);
get_config_hex(NULL, NULL, 0);
get_config_float(NULL, NULL, 0);
get_config_argv(NULL, NULL, NULL);

set_config_string(NULL, NULL, NULL);
set_config_int(NULL, NULL, 0);
set_config_hex(NULL, NULL, 0);
set_config_float(NULL, NULL, 0);

/****************************************/
/************ Mouse routines ************/
/****************************************/

#if !defined alleg_mouse_unused

install_mouse();
remove_mouse();

mouse_x = 0;
mouse_y = 0;
mouse_b = 0;
mouse_pos = 0;

freeze_mouse_flag = 0;

mouse_callback = NULL;

show_mouse(NULL);
position_mouse(0, 0);
set_mouse_range(0, 0, 0, 0);
set_mouse_speed(0, 0);
set_mouse_sprite(NULL);
set_mouse_sprite_focus(0, 0);
get_mouse_mickeys(NULL, NULL);

#endif

/****************************************/
/************ Timer routines ************/
/****************************************/

#if !defined alleg_timer_unused

install_timer();
remove_timer();

install_int_ex(NULL, 0);
install_int(NULL, 0);
remove_int(NULL);

i_love_bill = 0;

retrace_count = 0;
retrace_proc = NULL;

timer_simulate_retrace(0);

rest(0);
rest_callback(0, NULL);

#endif

/*******************************************/
/************ Keyboard routines ************/
/*******************************************/

#if !defined alleg_keyboard_unused

install_keyboard();
remove_keyboard();

keyboard_callback = NULL;

install_keyboard_hooks(NULL, NULL);

key[0] = 0;
key_shifts = 0;

three_finger_flag = 0;
key_led_flag = 0;

keypressed();
readkey();
simulate_keypress(0);
clear_keybuf();
set_leds(0);

key_ascii_table[0] = 0;
key_capslock_table[0] = 0;
key_shift_table[0] = 0;
key_control_table[0] = 0;
key_altgr_table[0] = 0;
key_accent1_lower_table[0] = 0;
key_accent1_upper_table[0] = 0;
key_accent1_shift_lower_table[0] = 0;
key_accent1_shift_upper_table[0] = 0;
key_accent2_lower_table[0] = 0;
key_accent2_upper_table[0] = 0;
key_accent2_shift_lower_table[0] = 0;
key_accent2_shift_upper_table[0] = 0;
key_numlock_table[0] = 0;
key_extended_table[0] = 0;
key_special_table[0] = 0;

#endif

/*******************************************/
/************ Joystick routines ************/
/*******************************************/

#if !defined alleg_joystick_unused

#ifdef ALLEG3UP

joy[0] = joy[0];
num_joysticks = 0;

joystick_none = joystick_standard = joystick_sw = joystick_gpro;
joystick_sp1 = joystick_sp2 = joystick_sp3;

_joystick_driver_list[0] = _joystick_driver_list[0];
joystick_driver = NULL;

install_joystick(0);
remove_joystick();

_joystick_installed = 0;

poll_joystick();

save_joystick_data(NULL);
load_joystick_data(NULL);

calibrate_joystick_name(0);
calibrate_joystick(0);

joy_type = 0;

initialise_joystick();
calibrate_joystick_tl();
calibrate_joystick_br();
calibrate_joystick_throttle_min();
calibrate_joystick_throttle_max();
calibrate_joystick_hat(0);

#else

joy_type = 0;

/* joystick status variables */
joy_x = 0, joy_y = 0;
joy_left = 0, joy_right = 0, joy_up = 0, joy_down = 0;
joy_b1 = 0, joy_b2 = 0, joy_b3 = 0, joy_b4 = 0, joy_b5 = 0, joy_b6 = 0;
#ifdef ALLEG30
  joy_hat = 0;
#endif
joy_throttle = 0;
joy2_x = 0, joy2_y = 0;
joy2_left = 0, joy2_right = 0, joy2_up = 0, joy2_down = 0;
joy2_b1 = 0, joy2_b2 = 0;

initialise_joystick();
calibrate_joystick_tl();
calibrate_joystick_br();
calibrate_joystick_throttle_min();
calibrate_joystick_throttle_max();
calibrate_joystick_hat(0);

poll_joystick();

save_joystick_data(NULL);
load_joystick_data(NULL);

#endif

#endif

/************************************************/
/************ Screen/bitmap routines ************/
/************************************************/

#if !defined alleg_gfx_driver_unused || \
    !defined alleg_graphics_unused   || \
    !defined alleg_vidmem_unused     || \
    !defined alleg_palette_unused

gfx_vga = gf, gfx_modex = gf;

#ifdef DJGPP 

/* for djgpp */
gfx_vesa_1 = gf, gfx_vesa_2b = gf, gfx_vesa_2l = gf, gfx_vbeaf = gf,
		  gfx_xtended = gf
        #ifdef ALLEG30
  		    ,gfx_ati = gf, gfx_mach64 = gf, gfx_cirrus64 = gf,
		    gfx_cirrus54 = gf, gfx_realtek = gf, gfx_s3 = gf, gfx_trident = gf,
		    gfx_et3000 = gf, gfx_et4000 = gf, gfx_paradise = gf, gfx_video7 = gf
        #endif
        ;

#else 

/* for linux */
gfx_svgalib = gf;

#endif 

/* driver table for autodetection */

_gfx_driver_list[0] = gl;

gfx_driver = NULL;   /* the driver currently in use */

#ifdef ALLEG3UP
gfx_capabilities = 0;     /* current driver capabilities */
#endif

__linear_vtable8 = vt, __linear_vtable15 = vt, __linear_vtable16 = vt,
		  __linear_vtable24 = vt, __linear_vtable32 = vt, __modex_vtable = vt;


_vtable_list[0] = inf;

screen = NULL;

set_color_depth(0);
set_color_conversion(0);
set_gfx_mode(0, 0, 0, 0, 0);
scroll_screen(0, 0);

#ifdef ALLEG3UP
request_scroll(0, 0);
poll_scroll();
show_video_bitmap(NULL);
request_video_bitmap(NULL);
#endif

request_modex_scroll(0, 0);
poll_modex_scroll();
split_modex_screen(0);

create_bitmap(0, 0);
create_bitmap_ex(0, 0, 0);
create_sub_bitmap(NULL, 0, 0, 0, 0);
destroy_bitmap(NULL);


_INLINE_ bitmap_color_depth(NULL);
_INLINE_ bitmap_mask_color(NULL);
_INLINE_ is_same_bitmap(NULL, NULL);
_INLINE_ is_linear_bitmap(NULL);
_INLINE_ is_planar_bitmap(NULL);
_INLINE_ is_memory_bitmap(NULL);
_INLINE_ is_screen_bitmap(NULL);
_INLINE_ is_sub_bitmap(NULL);


#endif

/************************************************/
/************ Color/Pallete routines ************/
/************************************************/

#if !defined alleg_palette_unused || !defined alleg_flic_unused

black_rgb = rgb;
black_pallete[0] = rgb, desktop_pallete[0] = rgb, _current_pallete[0] = rgb;

rgb_map = NULL;
color_map = NULL;

_color_depth = 0;

_rgb_r_shift_15 = 0, _rgb_g_shift_15 = 0, _rgb_b_shift_15 = 0,
	   _rgb_r_shift_16 = 0, _rgb_g_shift_16 = 0, _rgb_b_shift_16 = 0,
	   _rgb_r_shift_24 = 0, _rgb_g_shift_24 = 0, _rgb_b_shift_24 = 0,
	   _rgb_r_shift_32 = 0, _rgb_g_shift_32 = 0, _rgb_b_shift_32 = 0;

_rgb_scale_5[0] = 0, _rgb_scale_6[0] = 0;

pallete_color[0] = 0;

set_color(0, NULL);
set_pallete(NULL);
set_pallete_range(NULL, 0, 0, 0);

get_color(0, NULL);
get_pallete(NULL);
get_pallete_range(NULL, 0, 0);

fade_interpolate(NULL, NULL, NULL, 0, 0, 0);
fade_from_range(NULL, NULL, 0, 0, 0);
fade_in_range(NULL, 0, 0, 0);
fade_out_range(0, 0, 0);
fade_from(NULL, NULL, 0);
fade_in(NULL, 0);
fade_out(0);

select_pallete(NULL);
unselect_pallete();

generate_332_palette(NULL);
generate_optimized_palette(NULL, NULL, NULL);

create_rgb_table(NULL, NULL, NULL);
create_light_table(NULL, NULL, 0, 0, 0, NULL);
create_trans_table(NULL, NULL, 0, 0, 0, NULL);
create_color_table(NULL, NULL, NULL, NULL);

set_blender_mode(NULL, NULL, NULL, 0, 0, 0, 0);
set_trans_blender(0, 0, 0, 0);

hsv_to_rgb(0, 0, 0, NULL, NULL, NULL);
rgb_to_hsv(0, 0, 0, NULL, NULL, NULL);

bestfit_color(NULL, 0, 0, 0);

makecol(0, 0, 0);
makecol8(0, 0, 0);
makecol_depth(0, 0, 0, 0);

getr(0);
getg(0);
getb(0);

getr_depth(0, 0);
getg_depth(0, 0);
getb_depth(0, 0);

_INLINE_ vsync();
_INLINE_ _set_color(0, NULL);
_INLINE_ makecol15(0, 0, 0);
_INLINE_ makecol16(0, 0, 0);
_INLINE_ makecol24(0, 0, 0);
_INLINE_ makecol32(0, 0, 0);
_INLINE_ getr8(0);
_INLINE_ getg8(0);
_INLINE_ getb8(0);
_INLINE_ getr15(0);
_INLINE_ getg15(0);
_INLINE_ getb15(0);
_INLINE_ getr16(0);
_INLINE_ getg16(0);
_INLINE_ getb16(0);
_INLINE_ getr24(0);
_INLINE_ getg24(0);
_INLINE_ getb24(0);
_INLINE_ getr32(0);
_INLINE_ getg32(0);
_INLINE_ getb32(0);


#endif

/******************************************************/
/************ Graphics and sprite routines ************/
/******************************************************/

#if !defined alleg_graphics_unused

set_clip(NULL, 0, 0, 0, 0);

drawing_mode(0, NULL, 0, 0);
xor_mode(0);
solid_mode();

_INLINE_ getpixel(NULL, 0, 0);
_INLINE_ putpixel(NULL, 0, 0, 0);
_INLINE_ vline(NULL, 0, 0, 0, 0);
_INLINE_ hline(NULL, 0, 0, 0, 0);
_INLINE_ line(NULL, 0, 0, 0, 0, 0);
_INLINE_ rectfill(NULL, 0, 0, 0, 0, 0);
_INLINE_ draw_sprite(NULL, NULL, 0, 0);
_INLINE_ draw_sprite_v_flip(NULL, NULL, 0, 0);
_INLINE_ draw_sprite_h_flip(NULL, NULL, 0, 0);
_INLINE_ draw_sprite_vh_flip(NULL, NULL, 0, 0);
_INLINE_ draw_trans_sprite(NULL, NULL, 0, 0);
_INLINE_ draw_lit_sprite(NULL, NULL, 0, 0, 0);
_INLINE_ draw_character(NULL, NULL, 0, 0, 0);
_INLINE_ draw_rle_sprite(NULL, NULL, 0, 0);
_INLINE_ draw_trans_rle_sprite(NULL, NULL, 0, 0);
_INLINE_ draw_lit_rle_sprite(NULL, NULL, 0, 0, 0);
_INLINE_ clear_to_color(NULL, 0);


do_line(NULL, 0, 0, 0, 0, 0, NULL);
triangle(NULL, 0, 0, 0, 0, 0, 0, 0);
polygon(NULL, 0, NULL, 0);
rect(NULL, 0, 0, 0, 0, 0);
do_circle(NULL, 0, 0, 0, 0, NULL);
circle(NULL, 0, 0, 0, 0);
circlefill(NULL, 0, 0, 0, 0);
do_ellipse(NULL, 0, 0, 0, 0, 0, NULL);
ellipse(NULL, 0, 0, 0, 0, 0);
ellipsefill(NULL, 0, 0, 0, 0, 0);
calc_spline(NULL, 0, NULL, NULL);
spline(NULL, NULL, 0);
floodfill(NULL, 0, 0, 0);
blit(NULL, NULL, 0, 0, 0, 0, 0, 0);
masked_blit(NULL, NULL, 0, 0, 0, 0, 0, 0);
stretch_blit(NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0);
stretch_sprite(NULL, NULL, 0, 0, 0, 0);
rotate_sprite(NULL, NULL, 0, 0, 0);
rotate_scaled_sprite(NULL, NULL, 0, 0, 0, 0);
clear(NULL);

get_rle_sprite(NULL);
destroy_rle_sprite(NULL);

get_compiled_sprite(NULL, 0);
destroy_compiled_sprite(NULL);
draw_compiled_sprite(NULL, NULL, 0, 0);
font = NULL;

text_mode(0);
textprintf(NULL, NULL, 0, 0, 0, NULL);
textprintf_centre(NULL, NULL, 0, 0, 0, NULL);

_INLINE_ textout(NULL, NULL, NULL, 0, 0, 0);
_INLINE_ textout_centre(NULL, NULL, NULL, 0, 0, 0);
_INLINE_ textout_justify(NULL, NULL, NULL, 0, 0, 0, 0, 0);
_INLINE_ text_length(NULL, NULL);

text_height(NULL);
destroy_font(NULL);

polygon3d(NULL, 0, NULL, 0, NULL);
polygon3d_f(NULL, 0, NULL, 0, NULL);
triangle3d(NULL, 0, NULL, NULL, NULL, NULL);
triangle3d_f(NULL, 0, NULL, NULL, NULL, NULL);
quad3d(NULL, 0, NULL, NULL, NULL, NULL, NULL);
quad3d_f(NULL, 0, NULL, NULL, NULL, NULL, NULL);

/*********************************************/
/************ Video memory access ************/
/*********************************************/

#if !defined alleg_vidmem_unused

_INLINE_ bmp_write_line(NULL, 0);
_INLINE_ bmp_read_line(NULL, 0);
_INLINE_ _putpixel(NULL, 0, 0, 0);
_INLINE_ _getpixel(NULL, 0, 0);

#endif

/******************************************/
/************ FLI/FLC routines ************/
/******************************************/

#if !defined alleg_flic_unused

play_fli(NULL, NULL, 0, NULL);
play_memory_fli(NULL, NULL, 0, NULL);

open_fli(NULL);
open_memory_fli(NULL);
close_fli();
next_fli_frame(0);
reset_fli_variables();

fli_bitmap = NULL;             /* current frame of the FLI */
fli_pallete[0] = rgb;            /* current FLI pallete */

fli_bmp_dirty_from = 0;         /* what part of fli_bitmap is dirty */
fli_bmp_dirty_to = 0;
fli_pal_dirty_from = 0;         /* what part of fli_pallete is dirty */
fli_pal_dirty_to = 0;

fli_frame = 0;                  /* current frame number */

fli_timer = 0;         /* for timing FLI playback */

#endif

/****************************************/
/************ Sound routines ************/
/****************************************/

#if !defined alleg_sound_unused

digi_none = dg;

#ifdef DJGPP
/* for djgpp */
digi_sb = dg;
#else
/* for linux */
digi_linux_sound_not_implemented_yet = dg;
#endif

midi_none = md;

#ifdef DJGPP
/* for djgpp */
midi_adlib = md, midi_sb_out = md, midi_mpu401 = md, midi_digmid = md, midi_awe32 = md;
#else
/* for linux */
midi_linux_sound_not_implemented_yet = md;
#endif

/* driver tables for autodetection */

_digi_driver_list[0] = ddi;
_midi_driver_list[0] = mdi;

digi_driver = NULL;       /* the drivers currently in use */
midi_driver = NULL;

digi_card = 0, midi_card = 0;

midi_pos = 0;         /* current position in the midi file */

midi_loop_start = 0;           /* where to loop back to at EOF */
midi_loop_end = 0;             /* loop when we hit this position */

detect_digi_driver(0);
detect_midi_driver(0);

reserve_voices(0, 0);
install_sound(0, 0, NULL);
remove_sound();
set_volume(0, 0);

load_ibk(NULL, 0);

load_sample(NULL);
load_wav(NULL);
load_voc(NULL);
destroy_sample(NULL);

play_sample(NULL, 0, 0, 0, 0);
stop_sample(NULL);
adjust_sample(NULL, 0, 0, 0, 0);

allocate_voice(NULL);
deallocate_voice(0);
reallocate_voice(0, NULL);
release_voice(0);
voice_start(0);
voice_stop(0);
voice_set_priority(0, 0);
voice_check(0);

voice_set_playmode(0, 0);

voice_get_position(0);
voice_set_position(0, 0);

voice_get_volume(0);
voice_set_volume(0, 0);
voice_ramp_volume(0, 0, 0);
voice_stop_volumeramp(0);

voice_get_frequency(0);
voice_set_frequency(0, 0);
voice_sweep_frequency(0, 0, 0);
voice_stop_frequency_sweep(0);

voice_get_pan(0);
voice_set_pan(0, 0);
voice_sweep_pan(0, 0, 0);
voice_stop_pan_sweep(0);

voice_set_echo(0, 0, 0);
voice_set_tremolo(0, 0, 0);
voice_set_vibrato(0, 0, 0);

load_midi(NULL);
destroy_midi(NULL);
play_midi(NULL, 0);
play_looped_midi(NULL, 0, 0);
stop_midi();
midi_pause();
midi_resume();
midi_seek(0);
midi_out(NULL, 0);
load_midi_patches();

midi_msg_callback = NULL;
midi_meta_callback = NULL;
midi_sysex_callback = NULL;

#ifdef ALLEG3UP
  play_audio_stream(0, 0, 0, 0, 0, 0);
#else
  play_audio_stream(0, 0, 0, 0, 0);
#endif


stop_audio_stream(NULL);
get_audio_stream_buffer(NULL);
free_audio_stream_buffer(NULL);

#endif

/***********************************************************/
/************ File I/O and compression routines ************/
/***********************************************************/

#if !defined alleg_file_unused

#ifdef ALLEG3UP
fix_filename_case(NULL);
fix_filename_slashes(NULL);
fix_filename_path(NULL, NULL, 0);
replace_filename(NULL, NULL, NULL, 0);
replace_extension(NULL, NULL, NULL, 0);
append_filename(NULL, NULL, NULL, 0);
#endif

get_filename(NULL);
get_extension(NULL);
put_backslash(NULL);
file_exists(NULL, 0, NULL);
exists(NULL);
file_size(NULL);
file_time(NULL);
delete_file(NULL);
for_each_file(NULL, 0, NULL, 0);

packfile_password(NULL);
pack_fopen(NULL, NULL);
pack_fclose(NULL);
pack_fseek(NULL, 0);
pack_fopen_chunk(NULL, 0);
pack_fclose_chunk(NULL);
pack_igetw(NULL);
pack_igetl(NULL);
pack_iputw(0, NULL);
pack_iputl(0, NULL);
pack_mgetw(NULL);
pack_mgetl(NULL);
pack_mputw(0, NULL);
pack_mputl(0, NULL);
pack_fread(NULL, 0, NULL);
pack_fwrite(NULL, 0, NULL);
pack_fgets(NULL, 0, NULL);
pack_fputs(NULL, NULL);

_sort_out_getc(NULL);
_sort_out_putc(0, NULL);

_INLINE_ pack_getc(NULL);
_INLINE_ pack_putc(0, NULL);

#endif

/*******************************************/
/************ Datafile routines ************/
/*******************************************/

#if !defined alleg_datafile_unused

load_datafile(NULL);
unload_datafile(NULL);

load_datafile_object(NULL, NULL);
unload_datafile_object(NULL);

get_datafile_property(NULL, 0);
register_datafile_object(0, NULL, NULL);

fixup_datafile(NULL);

load_bitmap(NULL, NULL);
load_bmp(NULL, NULL);
load_lbm(NULL, NULL);
load_pcx(NULL, NULL);
load_tga(NULL, NULL);

save_bitmap(NULL, NULL, NULL);
save_bmp(NULL, NULL, NULL);
save_pcx(NULL, NULL, NULL);
save_tga(NULL, NULL, NULL);

register_bitmap_file_type(NULL, NULL, NULL);

#endif

/***************************************/
/************ Math routines ************/
/***************************************/

#if !defined alleg_math_unused

_INLINE_ itofix(0);
_INLINE_ fixtoi(0);
_INLINE_ ftofix(0);
_INLINE_ fixtof(0);
fsqrt(0);
fatan(0);
fatan2(0, 0);

  _cos_tbl[0] = 0;
  _tan_tbl[0] = 0;
  _acos_tbl[0] = 0;

_INLINE_ dot_product_f(0, 0, 0, 0, 0, 0);

_INLINE_ fcos(0);
_INLINE_ fsin(0);
_INLINE_ ftan(0);
_INLINE_ facos(0);
_INLINE_ fasin(0);
_INLINE_ fadd(0, 0);
_INLINE_ fsub(0, 0);
_INLINE_ fmul(0, 0);
_INLINE_ fdiv(0, 0);

#endif   /* ifdef __cplusplus */

identity_matrix = mx;
identity_matrix_f = mxf;


#ifndef ALLEG3UP
get_translation_matrix(NULL, 0, 0, 0);
#endif
get_translation_matrix_f(NULL, 0, 0, 0);

#ifndef ALLEG3UP
get_scaling_matrix(NULL, 0, 0, 0);
create_sample(0, 0, 0, 0);
#endif
get_scaling_matrix_f(NULL, 0, 0, 0);

#ifndef ALLEG3UP
get_x_rotate_matrix(NULL, 0);
#endif
get_x_rotate_matrix_f(NULL, 0);

#ifndef ALLEG3UP
get_y_rotate_matrix(NULL, 0);
#endif
get_y_rotate_matrix_f(NULL, 0);

#ifndef ALLEG3UP
get_z_rotate_matrix(NULL, 0);
#endif
get_z_rotate_matrix_f(NULL, 0);

#ifndef ALLEG3UP
get_rotation_matrix(NULL, 0, 0, 0);
#endif
get_rotation_matrix_f(NULL, 0, 0, 0);

#ifndef ALLEG3UP
get_align_matrix(NULL, 0, 0, 0, 0, 0, 0);
#endif
get_align_matrix_f(NULL, 0, 0, 0, 0, 0, 0);

#ifndef ALLEG3UP
get_vector_rotation_matrix(NULL, 0, 0, 0, 0);
#endif
get_vector_rotation_matrix_f(NULL, 0, 0, 0, 0);

#ifndef ALLEG3UP
get_transformation_matrix(NULL, 0, 0, 0, 0, 0, 0, 0);
#endif
get_transformation_matrix_f(NULL, 0, 0, 0, 0, 0, 0, 0);

#ifndef ALLEG3UP
get_camera_matrix(NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
#endif
get_camera_matrix_f(NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

#ifndef ALLEG3UP
qtranslate_matrix(NULL, 0, 0, 0);
#endif
qtranslate_matrix_f(NULL, 0, 0, 0);

#ifndef ALLEG3UP
qscale_matrix(NULL, 0);
#endif
qscale_matrix_f(NULL, 0);

#ifndef ALLEG3UP
matrix_mul(NULL, NULL, NULL);
#endif
matrix_mul_f(NULL, NULL, NULL);

#ifndef ALLEG3UP
vector_length(0, 0, 0);
#endif
vector_length_f(0, 0, 0);

#ifndef ALLEG3UP
normalize_vector(NULL, NULL, NULL);
#endif
normalize_vector_f(NULL, NULL, NULL);

#ifndef ALLEG3UP
cross_product(0, 0, 0, 0, 0, 0, NULL, NULL, NULL);
#endif
cross_product_f(0, 0, 0, 0, 0, 0, NULL, NULL, NULL);

#ifndef ALLEG3UP
polygon_z_normal(NULL, NULL, NULL);
#endif
polygon_z_normal_f(NULL, NULL, NULL);

_INLINE_ dot_product_f(0, 0, 0, 0, 0, 0);


apply_matrix_f(NULL, 0, 0, 0, NULL, NULL, NULL);

_persp_xscale = 0, _persp_yscale = 0, _persp_xoffset = 0, _persp_yoffset = 0;
_persp_xscale_f = 0, _persp_yscale_f = 0, _persp_xoffset_f = 0, _persp_yoffset_f = 0;

set_projection_viewport(0, 0, 0, 0);

_INLINE_ persp_project_f(0, 0, 0, NULL, NULL);

//#ifndef ALLEG3UP
  _INLINE_ get_y_rotate_matrix(NULL, 0);
  _INLINE_ qtranslate_matrix(NULL, 0, 0, 0);
  _INLINE_ persp_project(0, 0, 0, NULL, NULL);
  _INLINE_ get_translation_matrix(NULL, 0, 0, 0);
  _INLINE_ get_x_rotate_matrix(NULL, 0);
  _INLINE_ get_rotation_matrix(NULL, 0, 0, 0);
  _INLINE_ get_camera_matrix(NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  _INLINE_ get_transformation_matrix(NULL, 0, 0, 0, 0, 0, 0, 0);
  _INLINE_ get_vector_rotation_matrix(NULL, 0, 0, 0, 0);
  _INLINE_ get_scaling_matrix(NULL, 0, 0, 0);
  _INLINE_ get_z_rotate_matrix(NULL, 0);
  _INLINE_ normalize_vector(NULL, NULL, NULL);
  _INLINE_ get_align_matrix(NULL, 0, 0, 0, 0, 0, 0);
  _INLINE_ qscale_matrix(NULL, 0);
  _INLINE_ dot_product(0, 0, 0, 0, 0, 0);
  _INLINE_ apply_matrix(NULL, 0, 0, 0, NULL, NULL, NULL);
  _INLINE_ vector_length(0, 0, 0);
  _INLINE_ cross_product(0, 0, 0, 0, 0, 0, NULL, NULL, NULL);
//#endif

#endif
/***************************************/
/************ GUI routines  ************/
/***************************************/

#if !defined alleg_gui_unused

/* some dialog procedures */
d_clear_proc(0, NULL, 0);
d_box_proc(0, NULL, 0);
d_shadow_box_proc(0, NULL, 0);
d_bitmap_proc(0, NULL, 0);
d_text_proc(0, NULL, 0);
d_ctext_proc(0, NULL, 0);
d_button_proc(0, NULL, 0);
d_check_proc(0, NULL, 0);
d_radio_proc(0, NULL, 0);
d_icon_proc(0, NULL, 0);
d_keyboard_proc(0, NULL, 0);
d_edit_proc(0, NULL, 0);
d_list_proc(0, NULL, 0);
d_textbox_proc(0, NULL, 0);
d_slider_proc(0, NULL, 0);
d_menu_proc(0, NULL, 0);

active_dialog = NULL;
active_menu = NULL;

gui_mouse_focus = 0;
gui_fg_color = 0, gui_mg_color = 0, gui_bg_color = 0;
gui_font_baseline = 0;

gui_textout(NULL, NULL, 0, 0, 0, 0);
gui_strlen(NULL);
centre_dialog(NULL);
set_dialog_color(NULL, 0, 0);
find_dialog_focus(NULL);
dialog_message(NULL, 0, 0, NULL);
broadcast_dialog_message(0, 0);
do_dialog(NULL, 0);
popup_dialog(NULL, 0);
init_dialog(NULL, 0);
update_dialog(NULL);
shutdown_dialog(NULL);
do_menu(NULL, 0, 0);
alert(NULL, NULL, NULL, NULL, NULL, 0, 0);
alert3(NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0);
file_select(NULL, NULL, NULL);
gfx_mode_select(NULL, NULL, NULL);
gfx_mode_select_ex(NULL, NULL, NULL, NULL);

#endif

};


