/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"



char        g_seq   =  0;
static      char        s_print     [LEN_RECD] = "";



/*====================------------------------------------====================*/
/*===----                     supporting functions                     ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
FILE__wipe              (tFILE *a_cur)
{
   /*---(defense)--------------*/
   if (a_cur == NULL)  return -1;
   /*---(master)-------------------------*/
   a_cur->seq     =   -1;
   strlcpy (a_cur->title  , "" , LEN_HUND);
   strlcpy (a_cur->user   , "" , LEN_USER);
   a_cur->uid     =   -1;
   strlcpy (a_cur->note   , "-", LEN_TERSE);
   a_cur->lines   =    0;
   a_cur->retired =  '-';
   /*---(complete)-------------*/
   return 1;
}

char*
FILE__memory            (tFILE *a_cur)
{
   int         n           =    0;
   strlcpy (s_print, "å____.__._æ", LEN_RECD);
   ++n;  if (a_cur->seq         >= 0)           s_print [n] = 'X';
   ++n;  if (a_cur->title   [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->user    [0] != '\0')        s_print [n] = 'X';
   ++n;  if (a_cur->uid         >= 0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->note    [0] != '-')         s_print [n] = 'X';
   ++n;  if (a_cur->lines       >  0)           s_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->retired     != '-')         s_print [n] = 'X';
   return s_print;
}



/*====================------------------------------------====================*/
/*===----                     allocating and freeing                   ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
FILE__new               (tFILE **a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   tFILE      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_senter  (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_INPT  yLOG_spoint  (a_new);
   --rce;  if (a_new == NULL) {
      DEBUG_INPT  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_spoint  (*a_new);
   --rce;  if (*a_new != NULL) {
      DEBUG_INPT  yLOG_snote   ("already set");
      DEBUG_INPT  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *a_new = NULL;
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tFILE *) malloc (sizeof (tFILE));
      if (x_tries > 3)   break;
   }
   DEBUG_INPT  yLOG_sint    (x_tries);
   DEBUG_INPT  yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_INPT  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }

   /*---(master)-------------------------*/
   DEBUG_INPT   yLOG_snote   ("wipe");
   rc = FILE__wipe (x_new);
   DEBUG_INPT   yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save back)----------------------*/
   *a_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
FILE__free              (tFILE **a_old)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   tFILE      *x_old       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_senter  (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_INPT  yLOG_spoint  (a_old);
   --rce;  if (a_old == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_spoint  (*a_old);
   --rce;  if (*a_old == NULL) {
      DEBUG_INPT   yLOG_snote   ("never set");
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(simplify)-----------------------*/
   x_old = *a_old;
   /*---(clear and return)---------------*/
   FILE__wipe (x_old);
   free (x_old);
   *a_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        major actions                         ----===*/
/*====================------------------------------------====================*/
static void  o___ACTIONS_________o () { return; }

char
FILE_create             (char *a_name, char *a_user, int a_uid)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_INPT   yLOG_char    ("f_ready"   , my.f_ready);
   --rce;  if (my.f_ready != 'y') {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   DEBUG_INPT   yLOG_point   ("a_user"    , a_user);
   --rce;  if (a_user == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_user"    , a_user);
   /*---(create data)--------------------*/
   rc = FILE__new (&x_file);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   DEBUG_INPT   yLOG_info    ("file"      , a_name);
   strlcpy (x_file->title, a_name, LEN_HUND);
   DEBUG_INPT   yLOG_info    ("user"      , a_user);
   strlcpy (x_file->user , a_user, LEN_USER);
   DEBUG_INPT   yLOG_value   ("uid"       , a_uid);
   x_file->uid   = a_uid;
   /*---(create list)--------------------*/
   rc = yDLST_list_create (x_file->title, x_file);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(assign sequence)----------------*/
   x_file->seq   = g_seq++;
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FILE_delete             (char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(defenses)-----------------------*/
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   /*---(find list)----------------------*/
   rc = yDLST_list_by_name (a_name, NULL, &x_file);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check still in use)-------------*/
   DEBUG_INPT   yLOG_value   ("lines"     , x_file->lines);
   --rce;  if (x_file->lines > 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(destroy)------------------------*/
   rc = yDLST_list_destroy (a_name);
   DEBUG_INPT   yLOG_value   ("destroy"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    contirm good crontabs                     ----===*/
/*====================------------------------------------====================*/
static void      o___CONFIRM_________________o (void) {;}

FILE__defaults          (void)
{
   yURG_err_clear ();
   my.f_ready = '-';
   strlcpy (my.f_name, "", LEN_HUND);
   strlcpy (my.f_user, "", LEN_USER);
   my.f_uid = -1;
   strlcpy (my.f_desc, "", LEN_DESC);
   strlcpy (my.f_full, "", LEN_DESC);
   strlcpy (my.f_new , "", LEN_DESC);
   return 0;
}

char
FILE_assimilate        (cchar a_loc, cchar *a_name, char *r_user, char *r_desc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tFILE      *x_file      = NULL;
   int         c           =    0;
   char        x_dir       [LEN_PATH]  = "";
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   DEBUG_INPT  yLOG_char    ("a_loc"     , a_loc);
   /*---(default)------------------------*/
   if (r_user != NULL)  strlcpy (r_user, ""       , LEN_USER);
   if (r_desc != NULL)  strlcpy (r_desc, ""       , LEN_DESC);
   /*---(parse file)---------------------*/
   --rce;  if (a_loc == YEXEC_CENTRAL)   rc = yEXEC_central    (my.run_as, a_name, &(my.f_user), &(my.f_uid), &(my.f_desc), x_dir);
   else if    (a_loc == YEXEC_LOCAL  )   rc = yEXEC_acceptable (my.run_as, a_name, &(my.f_user), &(my.f_uid), &(my.f_desc), x_dir);
   else {
      DEBUG_INPT  yLOG_note    ("location is illegal");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_value   ("parse"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save back)----------------------*/
   if (r_user != NULL)  strlcpy (r_user, my.f_user, LEN_USER);
   if (r_desc != NULL)  strlcpy (r_desc, my.f_desc, LEN_DESC);
   /*---(create file)--------------------*/
   yURG_msg ('>', "assimilate file lines...");
   /*> rc = FILE_create (my.f_name, my.f_user, my.f_uid);                             <*/
   rc = FILE_create (a_name, my.f_user, my.f_uid);
   DEBUG_INPT   yLOG_value   ("file"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "created a list to house the lines");
   /*---(get current file)---------------*/
   yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
   DEBUG_INPT  yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      yURG_msg ('-', "file failed");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ySCHED_newfile ();
   DEBUG_INPT   yLOG_value   ("ySCHED"    , rc);
   --rce;  if (rc < 0) {
      yURG_msg ('-', "schedule failed");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "initialized the valid date for scheduling");
   yURG_msg ('-', "calling auto-reader");
   yURG_msg (' ', "");
   /*---(pull all lines)-----------------*/
   sprintf (my.f_full, "%s%s", x_dir, a_name);
   DEBUG_INPT  yLOG_info    ("f_full"     , my.f_full);
   rc = yPARSE_autoread (my.f_full, NULL, LINE_handler);
   DEBUG_PROG  yLOG_value   ("read"      , rc);
   --rce;  if (rc <  0) {
      strlcpy (x_file->note, "NO FILE" , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   c  = yDLST_line_count (YDLST_LOCAL);
   --rce;  if (c != x_file->lines) {
      yURG_msg ('>', "all lines read, ERRORS, reviewed %d, accepted %d", x_file->lines, c);
      yURG_msg (' ', "");
      strlcpy (x_file->note, "ERRORS"  , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   } else if (c == 0) {
      yURG_msg ('>', "all lines read, EMPTY, reviewed %d, accepted %d", x_file->lines, c);
      yURG_msg (' ', "");
      strlcpy (x_file->note, "EMPTY"   , LEN_TERSE);
      DEBUG_PROG  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('>', "all lines read, SUCCESS, reviewed %d, accepted %d", x_file->lines, c);
   yURG_msg (' ', "");
   strlcpy (x_file->note, "success"  , LEN_TERSE);
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     retirement of files                      ----===*/
/*====================------------------------------------====================*/
static void      o___RETIRE__________________o (void) {;}

char
FILE_init               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(create retired)-----------------*/
   my.f_ready = 'y';
   rc = FILE_create ("RETIRED", "n/a", -1);
   DEBUG_INPT   yLOG_value   ("create"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get current file)---------------*/
   yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
   DEBUG_INPT  yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update retired flag)------------*/
   x_file->retired = 'y';
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FILE_prune              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         x_running   =    0;
   tLINE      *x_line      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   /*---(check all lines)----------------*/
   rc = yDLST_list_by_name ("RETIRED", NULL, NULL);
   rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);
   DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
   while (x_line != NULL) {
      /*---(mark retired)----------------*/
      if (x_line->retire == 'y' && x_line->rpid <= 0) {
         /*---(destroy inactive)------------*/
         rc = yDLST_line_remove  (x_line->tracker);
         DEBUG_INPT   yLOG_value   ("destroy"   , rc);
         if (rc < 0) {
            DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
         LINE__free (&x_line);
         rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);
         x_running = 0;
         continue;
      }
      ++x_running;
      rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DNEXT, NULL, &x_line);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return x_running;
}

char
FILE_retire             (char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   char        x_tracker   [LEN_TITLE] = "";
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   yURG_msg ('>', "retire crontab lines...");
   /*---(defenses)-----------------------*/
   DEBUG_INPT   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      yURG_err ('f', "crontab name is null/empty");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_name"    , a_name);
   yURG_msg ('-', "crontab name requested is å%sæ", a_name);
   /*---(reject retired)-----------------*/
   if (strcmp (a_name, "RETIRED") == 0) {
      DEBUG_INPT   yLOG_note    ("can not retire the RETIRED list");
      DEBUG_INPT  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(find list)----------------------*/
   rc = yDLST_list_by_name (a_name, NULL, &x_file);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   --rce;  if (x_file == NULL) {
      yURG_msg ('-', "crontab is not assimilated, nothing to retire");
      yURG_msg (' ', "");
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_file->retired = 'y';
   yURG_msg ('-', "marking file as retired as precaution");
   /*---(check still in use)-------------*/
   DEBUG_INPT   yLOG_value   ("lines"     , x_file->lines);
   rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);
   DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
   while (rc >= 0 && x_line != NULL) {
      /*---(report out)------------------*/
      DEBUG_INPT   yLOG_value   ("recdno"    , x_line->recdno);
      DEBUG_INPT   yLOG_info    ("tracker"   , x_line->tracker);
      DEBUG_INPT   yLOG_info    ("command"   , x_line->command);
      rc = yDLST_line_remove (x_line->tracker);
      DEBUG_INPT   yLOG_value   ("remove" , rc);
      if (rc < 0) {
         yURG_err ('w', "could not remove line å%sæ (%d)", x_line->tracker, rc);
         continue;
      }
      /*---(inactive)--------------------*/
      if (x_line->rpid <= 0) {
         DEBUG_INPT   yLOG_note    ("found inactive process, no worries");
         LINE__free (&x_line);
         yURG_msg ('-', "line successfully retired");
         rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);
         continue;
      }
      /*---(mark retired)----------------*/
      DEBUG_INPT   yLOG_note    ("found running process, move to retired");
      x_line->retire = 'y';
      /*---(find retired list)--------------*/
      rc = yDLST_list_by_name ("RETIRED", NULL, &x_file);
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      --rce;  if (x_file == NULL) {
         yURG_err ('f', "retired list could not be found");
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(add line to retired)------------*/
      rc = yDLST_line_create (x_line->tracker, x_line);
      DEBUG_INPT   yLOG_value   ("create"    , rc);
      --rce;  if (rc < 0) {
         yURG_err ('w', "line could not be created å%sæ (%d)", x_line->tracker, rc);
         continue;
      }
      /*---(back to original list)----------*/
      rc = yDLST_list_by_name (a_name, NULL, &x_file);
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      --rce;  if (x_file == NULL) {
         yURG_err ('f', "original list could not be found å%sæ (%d)", a_name, rc);
         DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(next line)----------------------*/
      yURG_msg ('-', "line å%sæ running, but successfully moved to retired list");
      rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_DHEAD, NULL, &x_line);
      DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
      /*---(done)---------------------------*/
   }
   /*---(destroy)------------------------*/
   rc = yDLST_list_destroy (a_name);
   DEBUG_INPT   yLOG_value   ("destroy"   , rc);
   --rce;  if (rc < 0) {
      yURG_err ('f', "original list could not be destroyed å%sæ (%d)", a_name, rc);
      DEBUG_INPT  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('-', "original list destroyed å%sæ ");
   yURG_msg (' ', "");
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
FILE_flush              (void)
{
   char        rc          =    0;
   tFILE      *x_file      = NULL;
   DEBUG_INPT  yLOG_enter   (__FUNCTION__);
   rc = yDLST_list_by_cursor (YDLST_DHEAD, NULL, &x_file);
   DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
   while (rc >= 0 && x_file != NULL) {
      /*---(retire)-------------------------*/
      rc = FILE_retire (x_file->title);
      /*---(next line)----------------------*/
      yURG_msg ('-', "file å%sæ successfully moved to retired list", x_file->title);
      rc = yDLST_list_by_cursor (YDLST_DHEAD, NULL, &x_file);
      if (rc >= 0 && x_file != NULL && strcmp (x_file->title, "RETIRED") == 0) {
         rc = yDLST_list_by_cursor (YDLST_DNEXT, NULL, &x_file);
      }
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      /*---(done)---------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char          unit_answer [ LEN_RECD ];

char*            /*--> unit test accessor ------------------------------*/
file__unit              (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        t           [LEN_RECD]  = "åæ";
   char        s           [LEN_RECD]  = "åæ";
   int         c           =    0;
   tFILE      *x_file      = NULL;
   /*---(prepare)------------------------*/
   strlcpy  (unit_answer, "FILE             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   if      (strcmp (a_question, "file"    )        == 0) {
      sprintf (t, "å%sæ", my.f_name);
      snprintf (unit_answer, LEN_HUND, "FILE name        : %2d%-37.37s  %c", strlen (my.f_name), t, my.f_ready);
   }
   else if (strcmp (a_question, "user"    )        == 0) {
      sprintf (t, "å%sæ", my.f_user);
      snprintf (unit_answer, LEN_HUND, "FILE user        : %2d%-22.22s  %4d", strlen (my.f_user), t, my.f_uid);
   }
   else if (strcmp (a_question, "desc"    )        == 0) {
      snprintf (unit_answer, LEN_HUND, "FILE desc        : %2då%.35sæ", strlen (my.f_desc), my.f_desc);
   }
   else if (strcmp (a_question, "new"     )        == 0) {
      snprintf (unit_answer, LEN_HUND, "FILE new         : %2då%.35sæ", strlen (my.f_new), my.f_new);
   }
   else if (strcmp (a_question, "full"          )  == 0) {
      snprintf (unit_answer, LEN_HUND, "FILE full        : %2då%.60sæ", strlen (my.f_full), my.f_full);
   }
   else if (strcmp (a_question, "count"   )        == 0) {
      snprintf (unit_answer, LEN_HUND, "FILE count       : %d", yDLST_list_count ());
   }
   else if (strcmp (a_question, "entry"   )        == 0) {
      rc = yDLST_list_by_index (a_num, NULL, &x_file);
      c  = yDLST_line_count (YDLST_LOCAL);
      if (x_file != NULL) {
         sprintf (t, "%2då%.30sæ", strlen (x_file->title), x_file->title);
         sprintf (s, "%2då%.10sæ", strlen (x_file->user) , x_file->user);
         snprintf (unit_answer, LEN_HUND, "FILE entry  (%2d) : %2d  %-34.34s  %-14.14s  %4d  %2d %2d  %s", a_num, x_file->seq, t, s, x_file->uid, c, x_file->lines, x_file->note);
      } else {
         snprintf (unit_answer, LEN_HUND, "FILE entry  (%2d) :  -   -åæ                                 -åæ               -   -  -  -", a_num);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}






