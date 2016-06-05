/*====================--------[[ start-of-code ]]---------====================*/
#include   "khronos.h"



/*====================------------------------------------====================*/
/*===----                     directory processing                     ----===*/
/*====================------------------------------------====================*/
static void      o___DIRECTORIES_____________o (void) {;}

char         /*--> process entries in local crontab dir --[ ------ [ ------ ]-*/
TABS_local         (
      /*---(params)-----------+----------*/
      cchar       a_action    )        /* i=insert/process, l=list            */
{  /*---(locals)-----------+-----------+-*/
   int         rc          = 0;             /* generic return code            */
   char        rce         = -10;           /* return code for errors         */
   char        dir_name    [300] = "";      /* holder for directory name      */
   DIR        *dir;                         /* directory pointer              */
   tDIRENT    *den;                         /* directory entry pointer        */
   int         reviewed    = 0;             /* count of entries reviewed      */
   int         processed   = 0;             /* count of entries processed     */
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT   yLOG_char    ("a_action"  , a_action);
   DEBUG_INPT   yLOG_info    ("my.who"    , my.who);
   /*---(build dir name)-----------------*/
   if (strcmp (my.who, "root") == 0) snprintf (dir_name, 300, DIR_ROOT);
   else                              snprintf (dir_name, 300, "/home/%s/%s", my.who, DIR_LOCAL);
   DEBUG_INPT   yLOG_info    ("dir_name"  , dir_name);
   /*---(move to crontab dir)------------*/
   rc = chdir (dir_name);
   DEBUG_INPT   yLOG_value   ("chdir_rc"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("could not move to directory");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return  rce;
   }
   DEBUG_INPT   yLOG_note    ("successfully changed directory");
   /*---(open dir)-----------------------*/
   dir = opendir(".");
   DEBUG_INPT   yLOG_point   ("*dir"      , dir);
   --rce;  if (dir == NULL) {
      DEBUG_INPT   yLOG_note    ("could not open directory for reading");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return  rce;
   }
   DEBUG_INPT   yLOG_note    ("successfully openned directory for reading");
   /*---(process entries)----------------*/
   DEBUG_INPT   yLOG_note    ("processing entries");
   while (1) {
      den = readdir (dir);
      DEBUG_INPT   yLOG_point   ("*entry"    , den);
      if (den == NULL)  break;
      ++reviewed;
      rc = BASE_name (den->d_name, '-');
      DEBUG_INPT   yLOG_value   ("name_rc"   , rc);
      if (rc < 0) {
         DEBUG_INPT   yLOG_note    ("not a valid crontab file, skipping");
         continue;
      }
      DEBUG_INPT   yLOG_info    ("prefix"    , my.user);
      DEBUG_INPT   yLOG_info    ("suffix"    , my.desc);
      if (strcmp (my.user, "crontab") != 0) {
         DEBUG_INPT   yLOG_note    ("not a valid crontab prefix, skipping");
         continue;
      }
      if (rc == 0) {
         /*> DEBUG_INPT   printf("   found <<%s>>\n", den->d_name);                       <*/
         if      (a_action == 'i') {
            DEBUG_INPT   yLOG_note    ("insert action requested");
            crontab_inst  (my.desc);
         }
         else if (a_action == 'l') {
            DEBUG_INPT   yLOG_note    ("list action requested");
            printf ("crontab.%s\n", my.desc);
         }
         ++processed;
      }
      den = readdir (dir);
   }
   DEBUG_INPT   yLOG_note    ("done with entries");
   DEBUG_INPT   yLOG_value   ("reviewed"  , reviewed);
   DEBUG_INPT   yLOG_value   ("processed" , processed);
   /*---(close dir)----------------------*/
   DEBUG_INPT   yLOG_note    ("closing directory");
   rc = closedir (dir);
   DEBUG_INPT   yLOG_value   ("close_rc"  , rc);
   /*---(complete)------------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char        /* PURPOSE : process updates to main crontable directory ---------*/
crontab_proc       (cchar *a_user, cchar a_action)
{
   /*---(locals)--------------------------------*/
   tDIRENT        *den;
   DIR            *dir;
   char            x_user[25] = "";
   int             count     = 0;
   int             rc        = 0;
   char            x_file[LEN_NAME] = "";
   /*---(defense)-------------------------------*/
   DEBUG_INPT   printf("crontab_proc()...\n");
   if (strcmp(a_user, "ALL") == 0 && my.am_root != 'y') {
      DEBUG_INPT   printf("   ");
      printf("must be root to use the \"--all\" option");
      return -1;
   }
   /*---(format user names)---------------------*/
   DEBUG_INPT   printf("   user = %s\n", a_user);
   snprintf(x_user, 22, "%s.",  a_user);
   /*---(open dir)------------------------------*/
   DEBUG_INPT   printf("   dir  = %s\n", CRONTABS);
   dir = opendir(CRONTABS);
   if (dir == NULL) {
      DEBUG_INPT   printf("   ");
      printf("can not open crontab directory\n");
      return -2;
   }
   DEBUG_INPT   printf("   openned directory successfully\n");
   /*> printf("--crontabs-------------------------\n", a_user);                       <*/
   DEBUG_INPT   printf("   processing records\n\n");
   while ((den = readdir(dir)) != NULL) {
      rc = BASE_name (den->d_name, 'c');
      if (rc != 0)                         continue;
      /*---(filter files)-----------------------*/
      /*> DEBUG_INPT   printf("   found <<%s>>\n", den->d_name);                          <*/
      if (strcmp(a_user, "ALL") == 0 || strcmp(my.user, a_user) == 0) {
         snprintf(x_file, LEN_NAME, "%s.%s", my.user, my.desc);
         /*> DEBUG_INPT   printf("   processing <<%s>>\n", x_file);                       <*/
         if      (a_action == 'c') crontab_cat (den->d_name);
         else if (a_action == 'p') crontab_del (x_file);
         else                      printf("%s\n", den->d_name);
         ++count;
      }
   }
   DEBUG_INPT   printf("back to crontab_proc()...\n");
   DEBUG_INPT   printf("   processed %d installed crontabs\n", count);
   /*> printf("---%03d found-----------------------\n", count);                       <*/
   closedir(dir);
   DEBUG_INPT   printf("   closing directory\n");
   /*---(complete)------------------------------*/
   DEBUG_INPT   printf("   done\n\n");
   return 0;
}

char       /* PURPOSE : search for and process crontab updates               */
crontab_verify     (cchar *a_name, char a_loc)
{
   /*---(locals)--------------------------------*/
   char      dir_name [LEN_DIR] = "";
   tDIRENT  *den;
   DIR      *dir;
   /*---(create dir name)-----------------------*/
   if (a_loc == 'l') {
      if (strcmp(my.who, "root") == 0) snprintf(dir_name, 300, "/home/machine/crontabs/");
      else                             snprintf(dir_name, 300, "/home/%s/c_quani/crontabs/", my.who);
   } else {
      strlcpy (dir_name, CRONTABS, LEN_DIR);
   }
   /*---(open dir)------------------------------*/
   dir = opendir(dir_name);
   if (dir == NULL) {
      printf("can not open crontab directory\n");
      return -1;
   }
   while ((den = readdir(dir)) != NULL) {
      if (strncmp(a_name, den->d_name, LEN_NAME)  == 0) {
         return 1;
      }
   }
   closedir(dir);
   /*---(complete)------------------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       crontab actions                        ----===*/
/*====================------------------------------------====================*/
static void      o___ACTIONS_________________o (void) {;}

char       /* PURPOSE : install a local crontab file -------------------------*/
crontab_inst       (cchar *a_source)
{
   /*---(locals)--------------------------------*/
   char      dir_name[300] = "";
   int       rc        = 0;
   char      x_file[500]= "";          /* file name                           */
   /*---(begin)---------------------------------*/
   DEBUG_INPT   printf("crontab_inst()...\n");
   /*---(set source directory)------------------*/
   if (strcmp(my.who, "root") == 0) snprintf(dir_name, 300, "/home/machine/crontabs/");
   else                             snprintf(dir_name, 300, "/home/%s/c_quani/crontabs/", my.who);
   /*---(move to local directory)--------*/
   DEBUG_INPT   printf("   moved to local crontab directory\n");
   if (chdir(dir_name) < 0) {
      DEBUG_INPT   printf("   ");
      printf("could not move to crontab directory\n\n");
      return -4;
   }
   /*---(break up the file name)-------------*/
   snprintf (x_file, 500, "%s.%s", "crontab", a_source);
   DEBUG_INPT   printf("   starting with <<%s>>\n", x_file);
   rc = BASE_name (x_file, '-');
   if (rc <  0) {
      DEBUG_INPT   printf("   ");
      printf("crontab name is found, but format is not valid [%d]\n\n", rc);
      return -2;
   }
   DEBUG_INPT   printf("   found properly named file\n");
   /*---(verify source file)-----------------*/
   rc = crontab_verify (x_file, 'l');
   if (rc != 1) {
      DEBUG_INPT   printf("   ");
      printf("crontab source file (%s) not found [%d]\n\n", x_file, rc);
      return -1;
   }
   /*---(make sure to clear NEW)-------------*/
   snprintf (x_file, 500, "%s/%s.%s.NEW", CRONTABS, my.who, my.desc);
   rc = remove   (x_file);
   DEBUG_INPT   printf("   deleting <<%s>> : %s\n", x_file, (rc != 0) ? "(none)" : "(FOUND)");
   /*---(make sure to clear DEL)-------------*/
   snprintf (x_file, 500, "%s/%s.%s.DEL", CRONTABS, my.who, my.desc);
   rc = remove   (x_file);
   DEBUG_INPT   printf("   deleting <<%s>> : %s\n", x_file, (rc != 0) ? "(none)" : "(FOUND)");
   /*---(delete previous copy)---------------*/
   snprintf (x_file, 500, "%s/%s.%s", CRONTABS, my.who, my.desc);
   rc = remove   (x_file);
   DEBUG_INPT   printf("   deleting <<%s>>     : %s\n", x_file, (rc != 0) ? "(none)" : "(FOUND)");
   /*---(copy file to crontab dir)-----------*/
   snprintf (x_file, 500, "cp %s.%s %s/%s.%s.NEW", "crontab", my.desc, CRONTABS, my.who, my.desc);
   DEBUG_INPT   printf("   run <<%s>>\n", x_file);
   rc = system   (x_file);
   if (WIFEXITED(rc) <  0) {
      DEBUG_INPT   printf("   ");
      printf("can not copy file (%s.%s.NEW) to crondir [%d]\n\n", "crontab", my.desc, rc);
      return -3;
   }
   /*---(change to root ownership)-----------*/
   snprintf (x_file, 500, "chown root:root %s/%s.%s.NEW", CRONTABS, my.who, my.desc);
   DEBUG_INPT   printf("   run <<%s>>\n", x_file);
   rc = system   (x_file);
   if (WIFEXITED(rc) <  0) {
      DEBUG_INPT   printf("   ");
      printf("can not change to root ownership (%s.%s.NEW) [%d]\n\n", "crontab", my.desc, rc);
      return -4;
   }
   /*---(change to strict permissions)-------*/
   snprintf (x_file, 500, "chmod 0700 %s/%s.%s.NEW", CRONTABS, my.who, my.desc);
   DEBUG_INPT   printf("   run <<%s>>\n", x_file);
   rc = system   (x_file);
   if (WIFEXITED(rc) <  0) {
      DEBUG_INPT   printf("   ");
      printf("can not change to strict permissions (%s.%s.NEW) [%d]\n\n", "crontab", my.desc, rc);
      return -5;
   }
   /*---(send update)------------------------*/
   DEBUG_INPT   printf("   send HUP signal to khronos\n");
   crontab_hup();
   /*---(complete)---------------------------*/
   DEBUG_INPT   printf("   done\n\n");
   return 0;
}

char       /* PURPOSE : delete a crontab from system directory ---------------*/
crontab_del        (cchar *a_source)
{
   /*---(locals)-------------------------*/
   int       rc        = 0;
   char      file[LEN_NAME]= "";           /* file name                           */
   char      x_full[LEN_LONG]= "";          /* full source file name               */
   char      x_dest[LEN_LONG]= "";          /* destination file name               */
   /*---(begin)--------------------------*/
   DEBUG_INPT   printf("crontab_del()...\n");
   /*---(verify name)--------------------*/
   snprintf(file, LEN_NAME, "%s", a_source);
   DEBUG_INPT   printf("   crontab  = <<%s>> (vanilla name)", file);
   rc = BASE_name (file, 'c');
   if (rc < 0) {
      DEBUG_INPT   printf("   FAILED\n");
      snprintf(file, LEN_NAME, "%s.%s", my.who, a_source);
      DEBUG_INPT   printf("   crontab  = <<%s>> (contructed name)", file);
      rc = BASE_name (file, 'c');
      if (rc <  0) { 
         DEBUG_INPT   printf("   FAILED\n");
         printf("crontab name format is not valid (%d)\n\n", rc);
         return -1;
      }
   }
   DEBUG_INPT   printf("   PASSED\n");
   /*---(verify existing file)-----------*/
   strlcpy (x_full, file, LEN_LONG);
   DEBUG_INPT   printf("    searching (%s)", x_full);
   rc = crontab_verify (x_full, 's');
   if (rc != 1) {
      DEBUG_INPT   printf("    : (none)\n");
      /*---(if not, try the NEW version)-*/
      snprintf (x_full, 500, "%s.%s.NEW", my.who, my.desc);
      DEBUG_INPT   printf("   searching (%s)", x_full);
      rc = crontab_verify (x_full, 's');
      if (rc != 1) {
         DEBUG_INPT   printf(" : (none)\n");
         /*---(if not, try DEL version)--*/
         snprintf (x_full, 500, "%s.%s.DEL", my.who, my.desc);
         DEBUG_INPT   printf("   searching (%s)", x_full);
         rc = crontab_verify (x_full, 's');
         if (rc == 1) {
            DEBUG_INPT   printf(" : (FOUND)\n   ");
            printf("crontab already deleted, nothing to do\n\n");
            return -2;
         } else {
            DEBUG_INPT   printf(" : (none)\n   ");
            printf("crontab not presently installed, nothing to do\n\n");
            return -3;
         }
      } else {
         DEBUG_INPT   printf(" : (FOUND)\n");
      }
   } else {
      DEBUG_INPT   printf("    : (FOUND)\n");
   }
   /*---(remove existing file)-----------*/
   DEBUG_INPT   printf("   moved to crontab spool directory\n");
   if (chdir(CRONTABS) < 0) {
      DEBUG_INPT   printf("   ");
      printf("could not move to crontab directory\n\n");
      return -4;
   }
   snprintf(x_dest, 500, "%s.DEL", file);
   DEBUG_INPT   printf("   rename to <<%s>>\n", x_dest);
   rc = rename(x_full, x_dest);
   if (rc <  0) {
      DEBUG_INPT   printf("   ");
      printf("can not rename file (%s) in crondir\n\n", file);
      return -5;
   }
   /*---(send update)--------------------*/
   DEBUG_INPT   printf("   send HUP signal to khronos\n");
   crontab_hup();
   /*---(complete)-----------------------*/
   DEBUG_INPT   printf("   done\n\n");
   return 0;
}

char       /* PURPOSE : list contents of installed crontab -------------------*/
crontab_cat   (cchar *a_name)
{
   /*---(locals)--------------------------------*/
   int       rc        = 0;
   char      x_file[500]= "";          /* file name                           */
   /*---(verify existing file)------------------*/
   printf ("# ===================================================================\n");
   printf ("# crontab file name = %s\n", a_name);
   printf ("# ===================================================================\n");
   snprintf (x_file, 500, "cat %s/%s", CRONTABS, a_name);
   rc = system   (x_file);
   if (WIFEXITED(rc) <  0) {
      printf("# can not open file (%s) to dislay\n", a_name);
      return -2;
   }
   /*---(complete)------------------------------*/
   return 0;
}

char       /* PURPOSE : test the contents of a crontab file ------------------*/
crontab_test       (cchar *a_source)
{
   /*---(locals)--------------------------------*/
   char      dir_name[300] = "";
   int       rc        = 0;
   char      x_file[500]= "";          /* file name                           */
   /*---(begin)---------------------------------*/
   DEBUG_INPT   printf("crontab_test()...\n");
   testing = 'y';
   failed  = 0;
   /*---(set source directory)------------------*/
   if (strcmp(my.who, "root") == 0) snprintf(dir_name, 300, "/home/machine/crontabs/");
   else                             snprintf(dir_name, 300, "/home/%s/c_quani/crontabs/", my.who);
   /*---(move to local directory)--------*/
   DEBUG_INPT   printf("   moved to local crontab directory\n");
   if (chdir(dir_name) < 0) {
      DEBUG_INPT   printf("   ");
      printf("could not move to crontab directory\n\n");
      return -4;
   }
   /*---(break up the file name)-------------*/
   snprintf (x_file, 500, "%s.%s", "crontab", a_source);
   DEBUG_INPT   printf("   starting with <<%s>>\n", x_file);
   rc = BASE_name (x_file, '-');
   if (rc <  0) {
      DEBUG_INPT   printf("   ");
      printf("crontab name is found, but format is not valid [%d]\n\n", rc);
      return -2;
   }
   DEBUG_INPT   printf("   found properly named file\n");
   /*---(verify source file)-----------------*/
   rc = crontab_verify (x_file, 'l');
   if (rc != 1) {
      DEBUG_INPT   printf("   ");
      printf("crontab source file (%s) not found [%d]\n\n", x_file, rc);
      return -1;
   }
   /*---(assimilate)-------------------------*/
   assimilate(x_file);
   /*---(complete)------------------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                          specialty                           ----===*/
/*====================------------------------------------====================*/
static void      o___SPECIALTY_______________o (void) {;}

char
crontab_user (cchar *a_user)
{
   /*---(locals)--------------------------------*/
   tPASSWD  *x_pass    = NULL;         /* passwd data structure               */
   uint      ulen      = 0;               /* length of the user name             */
   /*---(begin)---------------------------------*/
   DEBUG_INPT   printf("crontab_user()...\n");
   /*---(check for root)------------------------*/
   if (my.am_root != 'y') {
      DEBUG_INPT   printf("   ");
      printf("must be root to use user change option\n");
      exit (-10);
   }
   DEBUG_INPT   printf("   verified root status\n");
   /*---(defense)-------------------------------*/
   ulen       = strllen (a_user, LEN_USER);
   DEBUG_INPT   printf("   user requested = <<%s>>\n", a_user);
   if (ulen < 1) {
      DEBUG_INPT   printf("   ");
      printf("user name is null\n");
      return -2;
   }
   if (ulen > 20) {
      DEBUG_INPT   printf("   ");
      printf("user name is too long\n");
      return -3;
   }
   /*---(user name)-----------------------------*/
   x_pass    = getpwnam(a_user);
   if (x_pass == NULL) {
      DEBUG_INPT   printf("   ");
      printf("can not retreive user information from the system\n");
      exit (-1);
   }
   DEBUG_INPT   printf("   verified user account\n");
   my.uid      = x_pass->pw_uid;
   DEBUG_INPT   printf("   user id num    = %d\n", my.uid);
   strlcpy(my.who , a_user, 20);
   /*---(complete)------------------------------*/
   DEBUG_INPT   printf("   done\n\n");
   return 0;
}

char
crontab_hup   (void)
{
   /*---(locals)--------------------------------*/
   int       rc        = 0;
   FILE      *f = NULL;
   int        crond_pid = 0;
   /*---(check for run file)--------------------*/
   f = fopen (FILE_LOCK, "r");
   if (f == NULL)   return -1;
   /*---(read pid)------------------------------*/
   rc = fscanf(f, "%d", &crond_pid);
   if (rc < 1) {
      printf("can not locate pid for crond\n");
      return -1;
   }
   /*---(close file)----------------------------*/
   fclose(f);
   /*---(send signal)---------------------------*/
   kill(crond_pid, SIGHUP);
   /*---(complete)------------------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         stub functions                       ----===*/
/*====================------------------------------------====================*/
static void      o___STUBS___________________o (void) {;}

char
crontab_dir   (void)
{
   printf("will NEVER allow anyone to change the crontab directory (security risk)\n");
   /*---(complete)------------------------------*/
   return 0;
}

char
crontab_stdin (void)
{
   printf("will NEVER allow reading from stdin (no traceability)\n");
   /*---(complete)------------------------------*/
   return 0;
}

char
crontab_edit  (void)
{
   printf("will NEVER allow editing installed crontabs (no traceability)\n");
   /*---(complete)------------------------------*/
   return 0;
}

/*====================---------[[ end-of-code ]]----------====================*/
