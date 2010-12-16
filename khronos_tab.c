/*====================--------[[ start-of-code ]]---------====================*/
#include   "khronos.h"



/*====================------------------------------------====================*/
/*===----                     directory processing                     ----===*/
/*====================------------------------------------====================*/
static void      o___DIRECTORIES_____________o (void) {;}

CHATTY   char        /* PURPOSE : process local crontab directory ---------------------*/
CHATTY   crontab_local (cchar a_action)
CHATTY   {
CHATTY      /*---(locals)--------------------------------*/
CHATTY      char            dir_name[300] = "";
CHATTY      tDIRENT        *den;
CHATTY      DIR            *dir;
CHATTY      int             count     = 0;
CHATTY      int             rc = 0;
CHATTY      /*---(locals)--------------------------------*/
CHATTY      DEBUG_P  printf("crontab_local()...\n");
CHATTY      /*---(create dir name)-----------------------*/
CHATTY      if (strcmp(my.who, "root") == 0) snprintf(dir_name, 300, "/home/system/c_quani/crontabs/", my.who);
CHATTY      else                             snprintf(dir_name, 300, "/home/%s/c_quani/crontabs/", my.who);
CHATTY      DEBUG_P  printf("   dir name = %s\n", dir_name);
CHATTY      /*---(move to crontab  dir)------------------*/
CHATTY      if (chdir(dir_name) < 0) {
CHATTY         DEBUG_P  printf("   ");
CHATTY         CHATTY   printf("could not move to user crontab directory\n");
CHATTY         return -1;
CHATTY      }
CHATTY      DEBUG_P  printf("   successfully changed to directory\n");
CHATTY      /*---(open dir)------------------------------*/
CHATTY      dir = opendir(".");
CHATTY      if (dir == NULL) {
CHATTY         DEBUG_P  printf("   ");
CHATTY         CHATTY   printf("can not open local directory\n");
CHATTY         return -2;
CHATTY      }
CHATTY      DEBUG_P  printf("   openned directory\n");
CHATTY      /*> printf("--crontabs-------------------------\n", a_user);                       <*/
CHATTY      DEBUG_P  printf("   processing records\n\n");
CHATTY      while ((den = readdir(dir)) != NULL) {
CHATTY         rc = name (den->d_name, '-');
CHATTY         if (strcmp(my.user, "crontab") != 0) continue;
CHATTY         if (rc == 0) {
CHATTY            /*> DEBUG_P  printf("   found <<%s>>\n", den->d_name);                       <*/
CHATTY            if      (a_action == 'i') crontab_inst  (my.desc);
CHATTY            else if (a_action == 'l') printf("crontab.%s\n", my.desc);
CHATTY            ++count;
CHATTY         }
CHATTY      }
CHATTY      DEBUG_P  printf("back to crontab_local()...\n");
CHATTY      DEBUG_P  printf("   processed %d local crontabs\n", count);
CHATTY      /*> printf("---%03d found-----------------------\n", count);                       <*/
CHATTY      closedir(dir);
CHATTY      DEBUG_P  printf("   closing directory\n");
CHATTY      /*---(complete)------------------------------*/
CHATTY      DEBUG_P  printf("   done\n\n");
CHATTY      return 0;
CHATTY   }

CHATTY   char        /* PURPOSE : process updates to main crontable directory ---------*/
CHATTY   crontab_proc       (cchar *a_user, cchar a_action)
CHATTY   {
CHATTY      /*---(locals)--------------------------------*/
CHATTY      tDIRENT        *den;
CHATTY      DIR            *dir;
CHATTY      char            x_user[25] = "";
CHATTY      int             count     = 0;
CHATTY      int             rc        = 0;
CHATTY      char            x_file[NAME] = "";
CHATTY      /*---(defense)-------------------------------*/
CHATTY      DEBUG_P  printf("crontab_proc()...\n");
CHATTY      if (strcmp(a_user, "ALL") == 0 && my.am_root != 'y') {
CHATTY         DEBUG_P  printf("   ");
CHATTY         CHATTY   printf("must be root to use the \"--all\" option");
CHATTY         return -1;
CHATTY      }
CHATTY      /*---(format user names)---------------------*/
CHATTY      DEBUG_P  printf("   user = %s\n", a_user);
CHATTY      snprintf(x_user, 22, "%s.",  a_user);
CHATTY      /*---(open dir)------------------------------*/
CHATTY      DEBUG_P  printf("   dir  = %s\n", CRONTABS);
CHATTY      dir = opendir(CRONTABS);
CHATTY      if (dir == NULL) {
CHATTY         DEBUG_P  printf("   ");
CHATTY         CHATTY   printf("can not open crontab directory\n");
CHATTY         return -2;
CHATTY      }
CHATTY      DEBUG_P  printf("   openned directory successfully\n");
CHATTY      /*> printf("--crontabs-------------------------\n", a_user);                       <*/
CHATTY      DEBUG_P  printf("   processing records\n\n");
CHATTY      while ((den = readdir(dir)) != NULL) {
CHATTY         rc = name (den->d_name, 'c');
CHATTY         if (rc != 0)                         continue;
CHATTY         /*---(filter files)-----------------------*/
CHATTY         /*> DEBUG_P  printf("   found <<%s>>\n", den->d_name);                          <*/
CHATTY         if (strcmp(a_user, "ALL") == 0 || strcmp(my.user, a_user) == 0) {
CHATTY            snprintf(x_file, NAME, "%s.%s", my.user, my.desc);
CHATTY            /*> DEBUG_P  printf("   processing <<%s>>\n", x_file);                       <*/
CHATTY            if      (a_action == 'c') crontab_cat (den->d_name);
CHATTY            else if (a_action == 'p') crontab_del (x_file);
CHATTY            else                      printf("%s\n", den->d_name);
CHATTY            ++count;
CHATTY         }
CHATTY      }
CHATTY      DEBUG_P  printf("back to crontab_proc()...\n");
CHATTY      DEBUG_P  printf("   processed %d installed crontabs\n", count);
CHATTY      /*> printf("---%03d found-----------------------\n", count);                       <*/
CHATTY      closedir(dir);
CHATTY      DEBUG_P  printf("   closing directory\n");
CHATTY      /*---(complete)------------------------------*/
CHATTY      DEBUG_P  printf("   done\n\n");
CHATTY      return 0;
CHATTY   }

char       /* PURPOSE : search for and process crontab updates               */
crontab_verify     (cchar *a_name, char a_loc)
{
   /*---(locals)--------------------------------*/
   char      dir_name[300] = "";
   tDIRENT  *den;
   DIR      *dir;
   /*---(create dir name)-----------------------*/
   if (a_loc == 'l') {
      if (strcmp(my.who, "root") == 0) snprintf(dir_name, 300, "/home/system/c_quani/crontabs/", my.who);
      else                             snprintf(dir_name, 300, "/home/%s/c_quani/crontabs/", my.who);
   } else {
      strcpy(dir_name, CRONTABS);
   }
   /*---(open dir)------------------------------*/
   dir = opendir(dir_name);
   if (dir == NULL) {
      CHATTY   printf("can not open crontab directory\n");
      return -1;
   }
   while ((den = readdir(dir)) != NULL) {
      if (strncmp(a_name, den->d_name, NAME)  == 0) {
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
   DEBUG_P  printf("crontab_inst()...\n");
   /*---(set source directory)------------------*/
   if (strcmp(my.who, "root") == 0) snprintf(dir_name, 300, "/home/system/c_quani/crontabs/", my.who);
   else                             snprintf(dir_name, 300, "/home/%s/c_quani/crontabs/", my.who);
   /*---(move to local directory)--------*/
   DEBUG_P  printf("   moved to local crontab directory\n");
   if (chdir(dir_name) < 0) {
      DEBUG_P  printf("   ");
      CHATTY   printf("could not move to crontab directory\n\n");
      return -4;
   }
   /*---(break up the file name)-------------*/
   snprintf (x_file, 500, "%s.%s", "crontab", a_source);
   DEBUG_P  printf("   starting with <<%s>>\n", x_file);
   rc = name (x_file, '-');
   if (rc <  0) {
      DEBUG_P  printf("   ");
      CHATTY   printf("crontab name is found, but format is not valid [%d]\n\n", rc);
      return -2;
   }
   DEBUG_P  printf("   found properly named file\n");
   /*---(verify source file)-----------------*/
   rc = crontab_verify (x_file, 'l');
   if (rc != 1) {
      DEBUG_P  printf("   ");
      CHATTY   printf("crontab source file (%s) not found [%d]\n\n", x_file, rc);
      return -1;
   }
   /*---(make sure to clear NEW)-------------*/
   snprintf (x_file, 500, "%s/%s.%s.NEW", CRONTABS, my.who, my.desc);
   rc = remove   (x_file);
   DEBUG_P  printf("   deleting <<%s>> : %s\n", x_file, (rc != 0) ? "(none)" : "(FOUND)");
   /*---(make sure to clear DEL)-------------*/
   snprintf (x_file, 500, "%s/%s.%s.DEL", CRONTABS, my.who, my.desc);
   rc = remove   (x_file);
   DEBUG_P  printf("   deleting <<%s>> : %s\n", x_file, (rc != 0) ? "(none)" : "(FOUND)");
   /*---(delete previous copy)---------------*/
   snprintf (x_file, 500, "%s/%s.%s", CRONTABS, my.who, my.desc);
   rc = remove   (x_file);
   DEBUG_P  printf("   deleting <<%s>>     : %s\n", x_file, (rc != 0) ? "(none)" : "(FOUND)");
   /*---(copy file to crontab dir)-----------*/
   snprintf (x_file, 500, "cp %s.%s %s/%s.%s.NEW", "crontab", my.desc, CRONTABS, my.who, my.desc);
   DEBUG_P  printf("   run <<%s>>\n", x_file);
   rc = system   (x_file);
   if (WIFEXITED(rc) <  0) {
      DEBUG_P  printf("   ");
      CHATTY   printf("can not copy file (%s.%s.NEW) to crondir [%d]\n\n", "crontab", my.desc, rc);
      return -3;
   }
   /*---(change to root ownership)-----------*/
   snprintf (x_file, 500, "chown root:root %s/%s.%s.NEW", CRONTABS, my.who, my.desc);
   DEBUG_P  printf("   run <<%s>>\n", x_file);
   rc = system   (x_file);
   if (WIFEXITED(rc) <  0) {
      DEBUG_P  printf("   ");
      CHATTY   printf("can not change to root ownership (%s.%s.NEW) [%d]\n\n", "crontab", my.desc, rc);
      return -4;
   }
   /*---(change to strict permissions)-------*/
   snprintf (x_file, 500, "chmod 0700 %s/%s.%s.NEW", CRONTABS, my.who, my.desc);
   DEBUG_P  printf("   run <<%s>>\n", x_file);
   rc = system   (x_file);
   if (WIFEXITED(rc) <  0) {
      DEBUG_P  printf("   ");
      CHATTY   printf("can not change to strict permissions (%s.%s.NEW) [%d]\n\n", "crontab", my.desc, rc);
      return -5;
   }
   /*---(send update)------------------------*/
   DEBUG_P  printf("   send HUP signal to khronos\n");
   crontab_hup();
   /*---(complete)---------------------------*/
   DEBUG_P  printf("   done\n\n");
   return 0;
}

char       /* PURPOSE : delete a crontab from system directory ---------------*/
crontab_del        (cchar *a_source)
{
   /*---(locals)-------------------------*/
   int       rc        = 0;
   char      file[NAME]= "";           /* file name                           */
   char      x_full[500]= "";          /* full source file name               */
   char      x_dest[500]= "";          /* destination file name               */
   /*---(begin)--------------------------*/
   DEBUG_P  printf("crontab_del()...\n");
   /*---(verify name)--------------------*/
   snprintf(file, NAME, "%s", a_source);
   DEBUG_P  printf("   crontab  = <<%s>> (vanilla name)", file);
   rc = name (file, 'c');
   if (rc < 0) {
      DEBUG_P  printf("   FAILED\n");
      snprintf(file, NAME, "%s.%s", my.who, a_source);
      DEBUG_P  printf("   crontab  = <<%s>> (contructed name)", file);
      rc = name (file, 'c');
      if (rc <  0) { 
         DEBUG_P  printf("   FAILED\n");
         CHATTY   printf("crontab name format is not valid (%d)\n\n", rc);
         return -1;
      }
   }
   DEBUG_P  printf("   PASSED\n");
   /*---(verify existing file)-----------*/
   strcpy(x_full, file);
   DEBUG_P  printf("    searching (%s)", x_full);
   rc = crontab_verify (x_full, 's');
   if (rc != 1) {
      DEBUG_P  printf("    : (none)\n");
      /*---(if not, try the NEW version)-*/
      snprintf (x_full, 500, "%s.%s.NEW", my.who, my.desc);
      DEBUG_P  printf("   searching (%s)", x_full);
      rc = crontab_verify (x_full, 's');
      if (rc != 1) {
         DEBUG_P  printf(" : (none)\n");
         /*---(if not, try DEL version)--*/
         snprintf (x_full, 500, "%s.%s.DEL", my.who, my.desc);
         DEBUG_P  printf("   searching (%s)", x_full);
         rc = crontab_verify (x_full, 's');
         if (rc == 1) {
            DEBUG_P  printf(" : (FOUND)\n   ");
            CHATTY   printf("crontab already deleted, nothing to do\n\n", x_full);
            return -2;
         } else {
            DEBUG_P  printf(" : (none)\n   ");
            CHATTY   printf("crontab not presently installed, nothing to do\n\n", x_full);
            return -3;
         }
      } else {
         DEBUG_P  printf(" : (FOUND)\n");
      }
   } else {
      DEBUG_P  printf("    : (FOUND)\n");
   }
   /*---(remove existing file)-----------*/
   DEBUG_P  printf("   moved to crontab spool directory\n");
   if (chdir(CRONTABS) < 0) {
      DEBUG_P  printf("   ");
      CHATTY   printf("could not move to crontab directory\n\n");
      return -4;
   }
   snprintf(x_dest, 500, "%s.DEL", file);
   DEBUG_P  printf("   rename to <<%s>>\n", x_dest);
   rc = rename(x_full, x_dest);
   if (rc <  0) {
      DEBUG_P  printf("   ");
      CHATTY   printf("can not rename file (%s) in crondir\n\n", file);
      return -5;
   }
   /*---(send update)--------------------*/
   DEBUG_P  printf("   send HUP signal to khronos\n");
   crontab_hup();
   /*---(complete)-----------------------*/
   DEBUG_P  printf("   done\n\n");
   return 0;
}

char       /* PURPOSE : list contents of installed crontab -------------------*/
crontab_cat   (cchar *a_name)
{
   /*---(locals)--------------------------------*/
   int       rc        = 0;
   char      x_file[500]= "";          /* file name                           */
   /*---(verify existing file)------------------*/
   CHATTY   printf ("# ===================================================================\n");
   CHATTY   printf ("# crontab file name = %s\n", a_name);
   CHATTY   printf ("# ===================================================================\n");
   snprintf (x_file, 500, "cat %s/%s", CRONTABS, a_name);
   rc = system   (x_file);
   if (WIFEXITED(rc) <  0) {
      CHATTY   printf("# can not open file (%s) to dislay\n", a_name);
      return -2;
   }
   /*---(complete)------------------------------*/
   return 0;
}

CHATTY   char       /* PURPOSE : test the contents of a crontab file ------------------*/
CHATTY   crontab_test       (cchar *a_source)
CHATTY   {
CHATTY      /*---(locals)--------------------------------*/
CHATTY      char      dir_name[300] = "";
CHATTY      int       rc        = 0;
CHATTY      char      x_file[500]= "";          /* file name                           */
CHATTY      /*---(begin)---------------------------------*/
CHATTY      DEBUG_P  printf("crontab_test()...\n");
CHATTY      testing = 'y';
CHATTY      failed  = 0;
CHATTY      /*---(set source directory)------------------*/
CHATTY      if (strcmp(my.who, "root") == 0) snprintf(dir_name, 300, "/home/system/c_quani/crontabs/", my.who);
CHATTY      else                             snprintf(dir_name, 300, "/home/%s/c_quani/crontabs/", my.who);
CHATTY      /*---(move to local directory)--------*/
CHATTY      DEBUG_P  printf("   moved to local crontab directory\n");
CHATTY      if (chdir(dir_name) < 0) {
CHATTY         DEBUG_P  printf("   ");
CHATTY         CHATTY   printf("could not move to crontab directory\n\n");
CHATTY         return -4;
CHATTY      }
CHATTY      /*---(break up the file name)-------------*/
CHATTY      snprintf (x_file, 500, "%s.%s", "crontab", a_source);
CHATTY      DEBUG_P  printf("   starting with <<%s>>\n", x_file);
CHATTY      rc = name (x_file, '-');
CHATTY      if (rc <  0) {
CHATTY         DEBUG_P  printf("   ");
CHATTY         CHATTY   printf("crontab name is found, but format is not valid [%d]\n\n", rc);
CHATTY         return -2;
CHATTY      }
CHATTY      DEBUG_P  printf("   found properly named file\n");
CHATTY      /*---(verify source file)-----------------*/
CHATTY      rc = crontab_verify (x_file, 'l');
CHATTY      if (rc != 1) {
CHATTY         DEBUG_P  printf("   ");
CHATTY         CHATTY   printf("crontab source file (%s) not found [%d]\n\n", x_file, rc);
CHATTY         return -1;
CHATTY      }
CHATTY      /*---(assimilate)-------------------------*/
CHATTY      assimilate(x_file);
CHATTY      /*---(complete)------------------------------*/
CHATTY      return 0;
CHATTY   }



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
   DEBUG_P  printf("crontab_user()...\n");
   /*---(check for root)------------------------*/
   if (my.am_root != 'y') {
      DEBUG_P  printf("   ");
      CHATTY   printf("must be root to use user change option\n");
      exit (-10);
   }
   DEBUG_P  printf("   verified root status\n");
   /*---(defense)-------------------------------*/
   ulen       = strlen(a_user);
   DEBUG_P  printf("   user requested = <<%s>>\n", a_user);
   if (ulen < 1) {
      DEBUG_P  printf("   ");
      CHATTY   printf("user name is null\n");
      return -2;
   }
   if (ulen > 20) {
      DEBUG_P  printf("   ");
      CHATTY   printf("user name is too long\n");
      return -3;
   }
   /*---(user name)-----------------------------*/
   x_pass    = getpwnam(a_user);
   if (x_pass == NULL) {
      DEBUG_P  printf("   ");
      CHATTY   printf("can not retreive user information from the system\n");
      exit (-1);
   }
   DEBUG_P  printf("   verified user account\n");
   my.uid      = x_pass->pw_uid;
   DEBUG_P  printf("   user id num    = %d\n", my.uid);
   strncpy(my.who , a_user, 20);
   /*---(complete)------------------------------*/
   DEBUG_P  printf("   done\n\n");
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
   f = fopen(LOCKFILE, "r");
   if (f == NULL)   return -1;
   /*---(read pid)------------------------------*/
   rc = fscanf(f, "%d", &crond_pid);
   if (rc < 1) {
      CHATTY   printf("can not locate pid for crond\n");
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

CHATTY   char
CHATTY   crontab_dir   (void)
CHATTY   {
CHATTY      printf("will never be able to change crontab directory in this version (security risk)\n");
CHATTY      /*---(complete)------------------------------*/
CHATTY      return 0;
CHATTY   }

CHATTY   char
CHATTY   crontab_stdin (void)
CHATTY   {
CHATTY      printf("will never be able to read from stdin in this version (no traceability)\n");
CHATTY      /*---(complete)------------------------------*/
CHATTY      return 0;
CHATTY   }

CHATTY   char
CHATTY   crontab_edit  (void)
CHATTY   {
CHATTY      printf("will never be able to edit installed crontabs in this version (no traceability)\n");
CHATTY      /*---(complete)------------------------------*/
CHATTY      return 0;
CHATTY   }

/*====================---------[[ end-of-code ]]----------====================*/
