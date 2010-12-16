/*====================--------[[ start-of-code ]]---------====================*/
#include  "khronos.h"



/*====================------------------------------------====================*/
/*===----                     directory processing                     ----===*/
/*====================------------------------------------====================*/
static void      o___DIRECTORIES_____________o (void) {;}

char        /* PURPOSE : process local crontab directory ---------------------*/
crontab_local (cchar a_action)
{
   /*---(locals)--------------------------------*/
   char            dir_name[300] = "";
   tDIRENT        *den;
   DIR            *dir;
   int             count     = 0;
   int             rc = 0;
   /*---(locals)--------------------------------*/
   DEBUG_P  printf("crontab_local()...\n");
   /*---(create dir name)-----------------------*/
   if (strcmp(my.who, "root") == 0) snprintf(dir_name, 300, "/home/system/c_quani/crontabs/", my.who);
   else                             snprintf(dir_name, 300, "/home/%s/c_quani/crontabs/", my.who);
   DEBUG_P  printf("   dir name = %s\n", dir_name);
   /*---(move to crontab  dir)------------------*/
   if (chdir(dir_name) < 0) {
      DEBUG_P  printf("   ");
      printf("could not move to user crontab directory\n");
      return -1;
   }
   DEBUG_P  printf("   successfully changed to directory\n");
   /*---(open dir)------------------------------*/
   dir = opendir(".");
   if (dir == NULL) {
      DEBUG_P  printf("   ");
      printf("can not open local directory\n");
      return -2;
   }
   DEBUG_P  printf("   openned directory\n");
   /*> printf("--crontabs-------------------------\n", a_user);                       <*/
   DEBUG_P  printf("   processing records\n\n");
   while ((den = readdir(dir)) != NULL) {
      rc = name (den->d_name, '-');
      if (strcmp(my.user, "crontab") != 0) continue;
      if (rc == 0) {
         /*> DEBUG_P  printf("   found <<%s>>\n", den->d_name);                       <*/
         if      (a_action == 'i') crontab_inst  (my.desc);
         else if (a_action == 'l') printf("crontab.%s\n", my.desc);
         ++count;
      }
   }
   DEBUG_P  printf("back to crontab_local()...\n");
   DEBUG_P  printf("   processed %d local crontabs\n", count);
   /*> printf("---%03d found-----------------------\n", count);                       <*/
   closedir(dir);
   DEBUG_P  printf("   closing directory\n");
   /*---(complete)------------------------------*/
   DEBUG_P  printf("   done\n\n");
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
   char            x_file[NAME] = "";
   /*---(defense)-------------------------------*/
   DEBUG_P  printf("crontab_list()...\n");
   if (strcmp(a_user, "ALL") == 0 && my.am_root != 'y') {
      DEBUG_P  printf("   ");
      printf("must be root to use the \"--all\" option");
      prog_term();
   }
   /*---(format user names)---------------------*/
   DEBUG_P  printf("   user = %s\n", a_user);
   snprintf(x_user, 22, "%s.",  a_user);
   /*---(open dir)------------------------------*/
   DEBUG_P  printf("   dir  = %s\n", CRONTABS);
   dir = opendir(CRONTABS);
   if (dir == NULL) {
      DEBUG_P  printf("   ");
      printf("can not open crontab directory\n");
      prog_term();
   }
   DEBUG_P  printf("   openned directory successfully\n");
   /*> printf("--crontabs-------------------------\n", a_user);                       <*/
   DEBUG_P  printf("   processing records\n\n");
   while ((den = readdir(dir)) != NULL) {
      rc = name (den->d_name, 'c');
      if (rc != 0)                         continue;
      /*---(filter files)-----------------------*/
      /*> DEBUG_P  printf("   found <<%s>>\n", den->d_name);                          <*/
      if (strcmp(a_user, "ALL") == 0 || strcmp(my.user, a_user) == 0) {
         snprintf(x_file, NAME, "%s.%s", my.user, my.desc);
         /*> DEBUG_P  printf("   processing <<%s>>\n", x_file);                       <*/
         if      (a_action == 'c') crontab_cat (den->d_name);
         else if (a_action == 'p') crontab_del (x_file);
         else                      printf("%s\n", den->d_name);
         ++count;
      }
   }
   /*> printf("---%03d found-----------------------\n", count);                       <*/
   closedir(dir);
   /*---(complete)------------------------------*/
   return 0;
}

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
      printf("can not open crontab directory\n");
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
      printf("could not move to crontab directory\n\n");
      return -4;
   }
   /*---(break up the file name)-------------*/
   snprintf (x_file, 500, "%s.%s", "crontab", a_source);
   DEBUG_P  printf("   starting with <<%s>>\n", x_file);
   rc = name (x_file, '-');
   if (rc <  0) {
      DEBUG_P  printf("   ");
      printf("crontab name is found, but format is not valid [%d]\n\n", rc);
      return -2;
   }
   DEBUG_P  printf("   found properly named file\n");
   /*---(verify source file)-----------------*/
   rc = crontab_verify (x_file, 'l');
   if (rc != 1) {
      DEBUG_P  printf("   ");
      printf("crontab source file (%s) not found [%d]\n\n", x_file, rc);
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
      printf("can not copy file (%s.%s.NEW) to crondir [%d]\n\n", "crontab", my.desc, rc);
      return -3;
   }
   /*---(change to root ownership)-----------*/
   snprintf (x_file, 500, "chown root:root %s/%s.%s.NEW", CRONTABS, my.who, my.desc);
   DEBUG_P  printf("   run <<%s>>\n", x_file);
   rc = system   (x_file);
   if (WIFEXITED(rc) <  0) {
      DEBUG_P  printf("   ");
      printf("can not change to root ownership (%s.%s.NEW) [%d]\n\n", "crontab", my.desc, rc);
      return -4;
   }
   /*---(change to strict permissions)-------*/
   snprintf (x_file, 500, "chmod 0700 %s/%s.%s.NEW", CRONTABS, my.who, my.desc);
   DEBUG_P  printf("   run <<%s>>\n", x_file);
   rc = system   (x_file);
   if (WIFEXITED(rc) <  0) {
      DEBUG_P  printf("   ");
      printf("can not change to strict permissions (%s.%s.NEW) [%d]\n\n", "crontab", my.desc, rc);
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
         printf("crontab name format is not valid (%d)\n\n", rc);
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
            printf("crontab already deleted, nothing to do\n\n", x_full);
            return -2;
         } else {
            DEBUG_P  printf(" : (none)\n   ");
            printf("crontab not presently installed, nothing to do\n\n", x_full);
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
      printf("could not move to crontab directory\n\n");
      return -4;
   }
   snprintf(x_dest, 500, "%s.DEL", file);
   DEBUG_P  printf("   rename to <<%s>>\n", x_dest);
   rc = rename(x_full, x_dest);
   if (rc <  0) {
      DEBUG_P  printf("   ");
      printf("can not rename file (%s) in crondir\n\n", file);
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
   DEBUG_P  printf("crontab_test()...\n");
   testing = 'y';
   /*---(set source directory)------------------*/
   if (strcmp(my.who, "root") == 0) snprintf(dir_name, 300, "/home/system/c_quani/crontabs/", my.who);
   else                             snprintf(dir_name, 300, "/home/%s/c_quani/crontabs/", my.who);
   /*---(move to local directory)--------*/
   DEBUG_P  printf("   moved to local crontab directory\n");
   if (chdir(dir_name) < 0) {
      DEBUG_P  printf("   ");
      printf("could not move to crontab directory\n\n");
      return -4;
   }
   /*---(break up the file name)-------------*/
   snprintf (x_file, 500, "%s.%s", "crontab", a_source);
   DEBUG_P  printf("   starting with <<%s>>\n", x_file);
   rc = name (x_file, '-');
   if (rc <  0) {
      DEBUG_P  printf("   ");
      printf("crontab name is found, but format is not valid [%d]\n\n", rc);
      return -2;
   }
   DEBUG_P  printf("   found properly named file\n");
   /*---(verify source file)-----------------*/
   rc = crontab_verify (x_file, 'l');
   if (rc != 1) {
      DEBUG_P  printf("   ");
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
   DEBUG_P  printf("crontab_user()...\n");
   /*---(check for root)------------------------*/
   if (my.am_root != 'y') {
      DEBUG_P  printf("   ");
      printf("must be root to use user change option\n");
      exit (-10);
   }
   DEBUG_P  printf("   verified root status\n");
   /*---(defense)-------------------------------*/
   ulen       = strlen(a_user);
   DEBUG_P  printf("   user requested = <<%s>>\n", a_user);
   if (ulen < 1) {
      DEBUG_P  printf("   ");
      printf("user name is null\n");
      return -2;
   }
   if (ulen > 20) {
      DEBUG_P  printf("   ");
      printf("user name is too long\n");
      return -3;
   }
   /*---(user name)-----------------------------*/
   x_pass    = getpwnam(a_user);
   if (x_pass == NULL) {
      DEBUG_P  printf("   ");
      printf("can not retreive user information from the system\n");
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
   printf("will never be able to change crontab directory in this version (security risk)\n");
   /*---(complete)------------------------------*/
   return 0;
}

char
crontab_stdin (void)
{
   printf("will never be able to read from stdin in this version (no traceability)\n");
   /*---(complete)------------------------------*/
   return 0;
}

char
crontab_edit  (void)
{
   printf("will never be able to edit installed crontabs in this version (no traceability)\n");
   /*---(complete)------------------------------*/
   return 0;
}

/*====================---------[[ end-of-code ]]----------====================*/
