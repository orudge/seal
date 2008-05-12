/******************************************************************
 * SEAL 2.0                                                       *
 * Copyright (c) 1999-2002 SEAL Developers. All Rights Reserved.  *
 *                                                                *
 * Web site: http://sealsystem.sourceforge.net/                   *
 * E-mail (current maintainer): orudge@users.sourceforge.net      *
 ******************************************************************/

/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef _TXTS_SEAL_H_
#define _TXTS_SEAL_H_

#define TXT_YES                  GETTXTID (1)
#define TXT_NO                   GETTXTID (2)
#define TXT_OK                   GETTXTID (3)
#define TXT_CANCEL               GETTXTID (4)
#define TXT_CLOSE                GETTXTID (5)
#define TXT_EXIT                 GETTXTID (6)
#define TXT_HELP                 GETTXTID (7)
#define TXT_ABOUT                GETTXTID (8)
#define TXT_FILE                 GETTXTID (9)
#define TXT_NEW                  GETTXTID (10)
#define TXT_OPEN                 GETTXTID (11)
#define TXT_SAVE                 GETTXTID (12)
#define TXT_SAVE_AS              GETTXTID (13)
#define TXT_EDIT                 GETTXTID (14)
#define TXT_CUT                  GETTXTID (15)
#define TXT_COPY                 GETTXTID (16)
#define TXT_PASTE                GETTXTID (17)
#define TXT_SEARCH               GETTXTID (18)
#define TXT_FIND                 GETTXTID (19)
#define TXT_FIND_NEXT            GETTXTID (20)
#define TXT_FIND_AND_REPLACE     GETTXTID (21)
#define TXT_PROGRAMS             GETTXTID (22)
#define TXT_SETTINGS             GETTXTID (23)
#define TXT_DOCUMENTS            GETTXTID (24)
#define TXT_SHUT_DOWN            GETTXTID (25)
#define TXT_MAXIMIZE             GETTXTID (26)
#define TXT_MINIMIZE             GETTXTID (27)
#define TXT_SHOW                 GETTXTID (28)
#define TXT_HIDE                 GETTXTID (29)
#define TXT_NO_TO_ALL            GETTXTID (30)
#define TXT_YES_TO_ALL           GETTXTID (31)
#define TXT_WARNING              GETTXTID (32)
#define TXT_ERROR                GETTXTID (33)
#define TXT_INFO                 GETTXTID (34)
#define TXT_QUESTION             GETTXTID (35)
#define TXT_UNAVAIBLE            GETTXTID (36)
#define TXT_JANUARY              GETTXTID (37)
#define TXT_FEBRUARY             GETTXTID (38)
#define TXT_MARCH                GETTXTID (39)
#define TXT_APRIL                GETTXTID (40)
#define TXT_MAY                  GETTXTID (41)
#define TXT_JUNE                 GETTXTID (42)
#define TXT_JULY                 GETTXTID (43)
#define TXT_AUGUST               GETTXTID (44)
#define TXT_SEPTEMBER            GETTXTID (45)
#define TXT_OCTOBER              GETTXTID (46)
#define TXT_NOVEMBER             GETTXTID (47)
#define TXT_DECEMBER             GETTXTID (48)
#define TXT_SUNDAY               GETTXTID (49)
#define TXT_MONDAY               GETTXTID (50)
#define TXT_THUESDAY             GETTXTID (51)
#define TXT_WEDNESDAY            GETTXTID (52)
#define TXT_THURSDAY             GETTXTID (53)
#define TXT_FRYDAY               GETTXTID (54)
#define TXT_SATURDAY             GETTXTID (55)
#define TXT_WELCOME              GETTXTID (56)
#define TXT_DIRECTORY                   GETTXTID (57)
#define TXT_FILETYPE                    GETTXTID (58)
#define TXT_CHANGENAME                  GETTXTID (59)
#define TXT_DELETE                      GETTXTID (60)
#define TXT_PROPERTIES                  GETTXTID (61)
#define TXT_NEWDIRECTORY                GETTXTID (62)
#define TXT_FILENAME                    GETTXTID (63)
#define TXT_DIRNAME                     GETTXTID (64)
#define TXT_DATE                        GETTXTID (65)
#define TXT_TIME                        GETTXTID (66)
#define TXT_SIZE                        GETTXTID (67)
#define TXT_PATH                        GETTXTID (68)
#define TXT_NOTFOUND                    GETTXTID (69)
#define TXT_ATTRIB                      GETTXTID (70)
#define TXT_NUMBEROFFILES               GETTXTID (71)
#define TXT_NUMBEROFDIRECTORIES         GETTXTID (72)
#define TXT_PROCESSSTATUS               GETTXTID (73)
#define TXT_DONE                        GETTXTID (74)
#define TXT_HALTED                      GETTXTID (75)
#define TXT_PLEASEWAIT                  GETTXTID (76)
#define TXT_COPYTOFILE                  GETTXTID (77)
#define TXT_COPYTODIRECTORY             GETTXTID (78)
#define TXT_AREYOUSUREYOUWANTTODELETE   GETTXTID (79)
#define TXT_THESEFILES                  GETTXTID (80)
#define TXT_AREYOUSUREYOUWANTTOCOPY     GETTXTID (81)
#define TXT_INTO                        GETTXTID (82)
#define TXT_COPYINGFILES                GETTXTID (83)
#define TXT_DELETINGFILES               GETTXTID (84)
#define TXT_COPYINGFILESTO              GETTXTID (85)
#define TXT_DELETINGFILESFROM           GETTXTID (86)
#define TXT_FILECOULDNTBERENAME         GETTXTID (87)
#define TXT_AREYOUSTUPID                GETTXTID (88)
#define TXT_CHECKTHEFILENAMEANDTRYAGAIN GETTXTID (89)
#define TXT_EXIST                       GETTXTID (90)
#define TXT_OPENFILE                    GETTXTID (91)
#define TXT_SAVEFILEAS                  GETTXTID (92)
#define TXT_WOULDYOULIKETOOVERWRITEIT   GETTXTID (93)
#define TXT_REFERENCETODIRECTORY        GETTXTID (94)
#define TXT_NOTENOUGHMEMORY             GETTXTID (95)
#define TXT_NOTENOUGHMEMEMORYFOROPERATION GETTXTID (96)
#define TXT_PLEASESTOREYOURDATAANDEXITSOMEAPPLICATION GETTXTID (97)
#define TXT_CANTRUNFILE                 GETTXTID (98)
#define TXT_CONVERTTOIMAGE              GETTXTID (99)
#define TXT_VIEWFILE                    GETTXTID (100)
#define TXT_INFOVIEWFILE                GETTXTID (101)
#define TXT_INFOCONVERTTOIMAGE          GETTXTID (102)
#define TXT_CONVERTNOW                  GETTXTID (103)
#define TXT_CLOSEBYALTF4                GETTXTID (104)
#define TXT_IMAGEISCONVRTINGINTOFILE    GETTXTID (105)
#define TXT_AREYOUSURETOQUITSEAL        GETTXTID (106)
// New ( from 4/11/20001 )
#define TXT_DISPLAY        				GETTXTID(107)
#define TXT_DISPLAYSETTINGS        		GETTXTID(108)
#define TXT_DISPLAYINFO        			GETTXTID(109)
#define TXT_HARDWARE        			GETTXTID(110)
#define TXT_HARDWARESETTINGS        	GETTXTID(111)
#define TXT_HARDWAREINFO        		GETTXTID(112)
#define TXT_CONTROL_PANEL               GETTXTID(113)
#define TXT_COLOR                 		GETTXTID(114)
#define TXT_CHANGE               		GETTXTID(115)
#define TXT_OTHER               		GETTXTID(116)
#define TXT__OTHER_               		GETTXTID(117)
#define TXT__NONE_               		GETTXTID(118)
#define TXT_SYSTEM_COLORS               GETTXTID(119)
#define TXT_PATERN               		GETTXTID(120)
#define TXT_STRETCH               		GETTXTID(121)
#define TXT_CENTER               		GETTXTID(122)
#define TXT_WALLPAPER               	GETTXTID(123)
#define TXT_WALLPAPER_ALIGN             GETTXTID(124)
#define TXT_DESKTOP_WALLPAPER           GETTXTID(125)
#define TXT_SCREEN_SAVER               	GETTXTID(126)
#define TXT_TEST               			GETTXTID(127)
#define TXT_WAIT__SEC_               	GETTXTID(128)
#define TXT_GRAPHICS_EFFECTS_AND_SKINS  GETTXTID(129)
#define TXT_SKIN_SELECTROR              GETTXTID(130)
#define TXT_TRANSLUCIDE_LEVEL           GETTXTID(131)
#define TXT_PREVIEW                     GETTXTID(132)
#define TXT_COMPUTER                    GETTXTID(133)

#define TXT_SCREEN_DEFINITION           GETTXTID(134)
#define TXT_SCREEN_SIZE                 GETTXTID(135)
#define TXT_SCREEN_DEPTH                GETTXTID(136)
#define TXT_SCREEN_DBL_BUFFER_INFO      GETTXTID(137)
#define TXT_RESTART      				GETTXTID(138)

#define MAX_TXTS 139

l_text GETTXTTXT ( l_text text );
l_text GETTXT ( l_text name );
l_text GETTXTID ( l_int id );

extern l_text lngfile;
extern l_text lng;

#define  INI_TEXT(x) GETTXTTXT(x)

#endif

