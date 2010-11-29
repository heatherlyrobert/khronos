/*====================--------[[ start-of-code ]]---------====================*/

/*===[[ SUMMARY ]]============================================================#

 *   application   : chronos
 *   module        : chronos_test
 *   size          : less than 100 slocL
 *
 */
/*===[[ SUMMARY ]]============================================================#

 *   chronos_list is a set of unit test accessor functions that provide the
 *   necessary data to ensure chronos is operating properly
 *
 */
/*============================================================================*/


#include  "chronos.h"



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;};

#define       LEN_TEXT  2000
char          unit_answer [ LEN_TEXT ];

char*
unit_accessor(char *a_question, int a_num)
{
   if        (strncmp(a_question, "parsed", 20)      == 0) {
      snprintf(unit_answer, LEN_TEXT, "parsed frequency : %.35s", my.parsed);
   } else if (strncmp(a_question, "name", 20)        == 0) {
      snprintf(unit_answer, LEN_TEXT, "crontab name     : %.35s", my.name);
   } else if (strncmp(a_question, "user", 20)        == 0) {
      snprintf(unit_answer, LEN_TEXT, "user name        : %.35s", my.user);
   } else if (strncmp(a_question, "desc", 20)        == 0) {
      snprintf(unit_answer, LEN_TEXT, "description      : %.35s", my.desc);
   } else if (strncmp(a_question, "cron count", 20)  == 0) {
      snprintf(unit_answer, LEN_TEXT, "cron counters    : %4df %5de", nfile, nentry);
   } else if (strncmp(a_question, "counters", 20)    == 0) {
      snprintf(unit_answer, LEN_TEXT, "counters         : %4d line, %4d fast, %4d proc", nentry, nfast, nproc);
   } else if (strncmp(a_question, "file list", 20)   == 0) {
      snprintf(unit_answer, LEN_TEXT, "cronfile list    : %3d %10p %10p", nfile, cronhead, crontail);
   } else if (strncmp(a_question, "cron shape", 20)  == 0) {
      char   xshape[40] = "(empty)";
      shape(xshape);
      snprintf(unit_answer, LEN_TEXT, "cron shape (%3d) : %s", nfile + nentry, xshape);
   } else if (strncmp(a_question, "cronpulse", 20)   == 0) {
      snprintf(unit_answer, LEN_TEXT, "cronpulse        : :%s:", my.pulse_time);
   } else if (strncmp(a_question, "cronbegin", 20)   == 0) {
      snprintf(unit_answer, LEN_TEXT, "cron beginning   : :%s:", my.pulse_begin);
   } else if (strncmp(a_question, "cronend",   20)   == 0) {
      snprintf(unit_answer, LEN_TEXT, "cron ending      : :%s:", my.pulse_end);
   } else {
      snprintf(unit_answer, LEN_TEXT, "unknown          : unit_accessor question not defined");
   }
   return unit_answer;
}

char        /* PURPOSE : specifically built list to aid unit testing          */
shape         (char *a_shape)
{
   /*---(locals)---------------------------------*/
   tCFILE   *file;                           /* current file                  */
   tCLINE   *line;                           /* current line                  */
   int       count = 0;
   /*---(scan every file)--------------------*/
   for (file = cronhead; file != NULL; file = file->next) {
      a_shape[count++] = 'F';
      a_shape[count  ] = '\0';
      if (count >= 35) return 0;
      /*---(scan every line)-----------------*/
      for (line = file->head; line != NULL; line = line->next) {
         a_shape[count++] = '_';
         a_shape[count  ] = '\0';
         if (count >= 35) return 0;
      }
   }
   return 0;
}

char        /* PURPOSE : specifically built list to aid unit testing          */
list_cron     (void)
{
   /*---(locals)---------------------------------*/
   tCFILE   *file;                           /* current file                  */
   tCLINE   *line;                           /* current line                  */
   /*---(scan every file)--------------------*/
   printf("\n      ---cron-list-----------------------------------\n");
   for (file = cronhead; file != NULL; file = file->next) {
      printf("      FILE (%c) %3d = %-20.20s :: %-40.40s\n",
            file->retire, file->nlines, file->user, file->name);
      /*---(scan every line)-----------------*/
      for (line = file->head; line != NULL; line = line->next) {
         printf("           line (%c) <<%.50s>>\n", line->deleted, line->cmd);
      }
   }
   printf("      -----------------------------------------------\n");
   return 0;
}

char        /* PURPOSE : specifically built list to aid unit testing          */
list_fast     (void)
{
   /*---(locals)---------------------------------*/
   tCLINE   *line;                           /* current line                  */
   /*---(scan every file)--------------------*/
   printf("\n      ---fast-list-----------------------------------\n");
   for (line = fasthead; line != NULL; line = line->fnext) {
      printf("      %-20.20s (%3d) %-40.40s\n", line->file->user, line->recd, line->file->name);
   }
   printf("      -----------------------------------------------\n");
   return 0;
}

char        /* PURPOSE : specifically built list to aid unit testing          */
list_proc     (void)
{
   /*---(locals)---------------------------------*/
   tCLINE   *line;                           /* current line                  */
   /*---(scan every file)--------------------*/
   printf("\n      ---proc-list-----------------------------------\n");
   for (line = prochead; line != NULL; line = line->pnext) {
      printf("      (%6d) <<%.50s>>\n", line->rpid, line->cmd);
   }
   printf("      -----------------------------------------------\n");
   return 0;
}


/*====================---------[[ end-of-code ]]----------====================*/
