                                     SEAL 2.00.11
                                       README

               Copyright (c) SEAL Developers 1999-2002. All Rights Reserved.

                   Web site: http://sealsystem.sourceforge.net/
                       E-mail: orudge@users.sourceforge.net

Thank you for downloading SEAL 2.0! SEAL is a 32-bit graphical user interface
for DOS. It supports high-resolution display modes, multimedia, and more!
The source code is freely available and you are encouraged to help with SEAL
in whatever way you can - whether you can program or not, every little helps.

SYSTEM REQUIREMENTS
===================

SEAL requires the following:

- A 486 processor or better, with a maths co-processor
- At least 8mb of RAM (you may be able to run SEAL with less, but it is not recommended)
- A video card which supports at least 640x480 resolution in 256 colours, although
  16-bit colour or better is recommended.
- A mouse and mouse driver (CuteMouse is provided if you don't have a driver)
- At least 1.6mb of hard disk space (more recommended)
- MS-DOS 3.0 or higher (or compatible - DR-DOS, PC-DOS and FreeDOS all work with SEAL)

A sound card is recommended, but not required.

You may be able to run SEAL on a system less powerful than this, but it may be slow
and you may have problems.

INSTALLING SEAL
===============

If you downloaded the binary distribution, just run INSTALL.EXE. This will
let you specify a path to install SEAL to, and will copy all the files required
and get you a system that is ready-to-run.

If you downloaded the source distribution, you must build SEAL yourself. See
docs/build.txt for information on how to do this.

RUNNING SEAL
============

To run SEAL, simply run the SEAL.EXE program in the directory you installed SEAL to.

IF YOU GET A DISPLAY ERROR
--------------------------

If you get an error along the lines of:

"ERROR: Selected resolution (640x480 @ 16 bpp) is not supported!"

or similar, then your video card does not support 640x480 in 16-bit colour. You can
make SEAL use 640x480 in 256 colours by doing the following:

1. Change directory to your installation directory (or SEAL2\BIN if you downloaded
   the sources).

2. Type:

      REGEDIT

   at the command line.

3. Navigate to SYSTEM/GRAPHICS. Highlight DEPTH and press F4. Type 8 in the New Key
   Value box and press ENTER. Then press ESCAPE to quit the Registry Editor.

Then try running SEAL again. If it still doesn't work, then your video card does not
support SEAL.

SETTING UP SOUND
================

SEAL supports a number of sound cards, including the following:

- Sound Blaster 1.0
- Sound Blaster 1.5
- Sound Blaster 2.0
- Sound Blaster Pro
- Sound Blaster 16
- ESS AudioDrive
- Ensoniq Soundscape
- Windows Sound System

The following MIDI devices are also supported:

- Adlib (autodetect OPL version)
- OPL2 FM synth
- Dual OPL2 (SB Pro-1)
- OPL3 FM synth
- raw SB MIDI interface
- MPU-401 MIDI interface
- DIGMID software wavetable
- AWE32

DIGMID uses a PATCHES.DAT file to provide software MIDI. See http://alleg.sourceforge.net/
for a list of various PATCHES.DAT files you can use.

A tool is being written that will let you set sound up from SEAL. However, this is not
currently complete, so the Allegro Setup program, available from the SEAL web site, should be
used to select your sound card and set it up. This also lets you set up keyboard and joystick
information.

LICENSE
=======

SEAL is licensed under the GNU General Public License. Please see docs/license.txt for more
information.

CREDITS
=======

Please see the docs/authors.txt file for a list of people who have helped make SEAL what it is
today.

CONTACT INFORMATION
===================

I hope you find SEAL useful. If you have any comments, questions or problems, please visit
the forums at http://sealsystem.sourceforge.net/ and post a message.

You can e-mail me, the maintainer of SEAL, at orudge@users.sourceforge.net.

Enjoy!

Owen Rudge, 2nd April 2002
http://www.owenrudge.co.uk/

MSN Messenger: orudge@freeuk.com   ICQ: 125455765
AIM: OwenCrealRudge                Yahoo: orudge