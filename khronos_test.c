/*====================--------[[ start-of-code ]]---------====================*/
#include   "khronos.h"



char          unit_answer [ LEN_RECD ];



/*====================------------------------------------====================*/
/*===----                   helpers for unit testing                   ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
prog__unit              (char *a_question)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_HUND]  = "";
   /*---(prepare)------------------------*/
   ystrlcpy  (unit_answer, "PROG             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "mode"          )  == 0) {
      ystrlcpy (s, yJOBS_iam ()  , LEN_HUND);
      ystrlcpy (t, yJOBS_mode () , LEN_HUND);
      snprintf (unit_answer, LEN_HUND, "PROG mode        : (%c) %-18.18s, (%c) %-18.18s, å%sæ", my.run_as, s, my.run_mode, t, my.run_file);
   }
   else if (strcmp (a_question, "action"        )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG action      : %c  %c  %2då%sæ", my.run_as, my.run_mode, strlen (my.run_file), my.run_file);
   }
   else if (strcmp (a_question, "n_central"     )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG central     : %2då%sæ", strlen (my.n_central  ), my.n_central);
   }
   else if (strcmp (a_question, "n_home"        )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG home        : %2då%sæ", strlen (my.n_home     ), my.n_home);
   }
   else if (strcmp (a_question, "n_root"        )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG root        : %2då%sæ", strlen (my.n_root     ), my.n_root);
   }
   else if (strcmp (a_question, "n_heartbeat"   )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG heartbeat   : %2då%sæ", strlen (my.n_heartbeat), my.n_heartbeat);
   }
   else if (strcmp (a_question, "n_activity"    )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG activity    : %2då%sæ", strlen (my.n_activity ), my.n_activity);
   }
   else if (strcmp (a_question, "n_status"      )  == 0) {
      snprintf (unit_answer, LEN_HUND, "PROG status      : %2då%sæ", strlen (my.n_status   ), my.n_status);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char
PROG__unit_prepare      (void)
{
   char        x_cmd       [LEN_RECD]  = "";
   char        x_home      [LEN_PATH];
   char        x_dir       [LEN_RECD]  = "";
   PROG__files_unit ();
   yJOBS_mkdirs ();
   /*---(directories)--------------------*/
   /*> getcwd (x_home, LEN_PATH);                                                     <* 
    *> chdir  ("/tmp");                                                               <* 
    *> sprintf (x_dir, "%s", DIR_UNIT);                                               <* 
    *> yURG_mkdir (x_dir, "root"   , "root"  , "0755");                               <* 
    *> sprintf (x_dir, "%skhronos" , DIR_UNIT);                                       <* 
    *> yURG_mkdir (x_dir, "root"   , "root"  , "0700");                               <* 
    *> sprintf (x_dir, "%sroot"    , DIR_UNIT);                                       <* 
    *> yURG_mkdir (x_dir, "root"   , "root"  , "0700");                               <* 
    *> sprintf (x_dir, "%smember"  , DIR_UNIT);                                       <* 
    *> yURG_mkdir (x_dir, "member" , "root"  , "0700");                               <* 
    *> sprintf (x_dir, "%smachine" , DIR_UNIT);                                       <* 
    *> yURG_mkdir (x_dir, "machine", "root"  , "0700");                               <* 
    *> sprintf (x_dir, "%smonkey"  , DIR_UNIT);                                       <* 
    *> yURG_mkdir (x_dir, "monkey" , "root"  , "0700");                               <* 
    *> chdir  (x_home);                                                               <*/
   /*---(complete)-----------------------*/
   return 0;
}

char
PROG__unit_cleanup      (void)
{
   char        x_cmd       [LEN_RECD];
   char        x_home      [LEN_PATH];
   /*---(directories)--------------------*/
   yJOBS_rmdirs ();
   /*> getcwd (x_home, LEN_PATH);                                                     <* 
    *> chdir  ("/tmp");                                                               <* 
    *> yURG_rmdir (DIR_UNIT);                                                         <* 
    *> ystrlcpy (my.n_central, DIR_CENTRAL, LEN_PATH);                                 <* 
    *> chdir  (x_home);                                                               <*/
   /*---(complete)-----------------------*/
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_quiet   (void)
{
   char        rc          =    0;
   int         x_argc      =    1;
   char       *x_argv [1]  = { "khronos" };
   rc = PROG_debugging (x_argc, x_argv);
   PROG__files_unit ();
   rc = PROG_startup   (x_argc, x_argv);
   return rc;
}

char       /*----: set up programgents/debugging -----------------------------*/
prog__unit_loud    (void)
{
   char        rc          =    0;
   int         x_argc      =    7;
   char       *x_argv [7]  = { "khronos_unit", "@@kitchen", "@@yjobs", "@@yparse", "@@ydlst", "@@ysched", "@@yexec"  };
   rc = PROG_debugging (x_argc, x_argv);
   PROG__files_unit ();
   rc = PROG_startup   (x_argc, x_argv);
   return rc;
}

char       /*----: set up program urgents/debugging --------------------------*/
prog__unit_end     (void)
{
   PROG_shutdown  ();
   return 0;
}

