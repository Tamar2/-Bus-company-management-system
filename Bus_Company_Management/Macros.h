#define _CRTDBG_MAP_ALLOC

#ifndef MACROS_H_
#define MACROS_H_
#include <stdio.h>


#define MSG_CLOSE_FILE_RETURN_ZERO(fp,msg) {fclose(fp); puts(msg);return 0;}
#define	CHECK_0_MSG_CLOSE_FILE(f, value, msg) if ((value) == 0) MSG_CLOSE_FILE_RETURN_ZERO(fp, msg)

#define CHECK_RETURN_ZERO(ptr) {if(!ptr) return 0;}
#define CHECK_RETURN_MSG_ZERO(ptr,msg) {if(!(ptr)) {printf("%s\n", msg); return 0;}}
#define CHECK_COLSE_RETURN_ZERO(ptr,fp) {if(!ptr){fclose(fp); return 0;}}
#define COLSE_RETURN_ZERO(fp) {fclose(fp); return 0;}
#define UNDER_LINE() {printf("\033[4m");}
#define COLOR_TXT_BACKGROUND(){printf("\033[0;45m");}//red background
#define SET_DEAFULT_CONSOLE() {printf("\033[0m");}
#define RED()	    {printf("\x1b[31m");}
#define GREEN()   {printf("\x1b[32m");}
#define YELLOW()  {printf("\x1b[33m");}
#define BLUE()    {printf("\x1b[34m");} 
#define MAGENTA() {printf( "\x1b[35m");}
#define CYAN()   {printf("\x1b[36m");}
#define PINK()	{printf("\x1b[38;5;206m");}
#define RESET()   {printf("\x1b[0m");}

#endif 
