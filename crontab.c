/*====================--------[[ start-of-code ]]---------====================*/
#include  "cron.h"

/*
 * options....
 *    <desc>           add the file to the spool directory and SIGHUP crond
 *    -t <desc>        test a crontab file
 *    -l               list names of user's crontabs
 *    -d <desc>        delete crontab by name
 *    -a               list all crontabs (root only)
 *    -s               load as system crontab (root only)
 *
 *
 * rejected...
 *    -                bring in crontab from stdin (no traceability)
 *    -u <user>        manipulate crontabs for another user (no traceability)
 *    -e               edit the crontab in spool (no traceability)
 *    -c <dir>         specify the crontab directory (security risk)
 *
 *
 *
 *
 *
 */


int       uid       = 0;               /* user id of the caller               */
char      user[25]  = "";              /* user name of the caller             */
uint      ulen      = 0;               /* length of the user name             */
char      desc[DESC]= "";              /* desc for file                       */
char      file[NAME]= "";              /* file name                           */
char      am_root   = 'n';             /* are we running as root y/n          */

char      crontab_source  (cchar*);
char      crontab_find    (cchar*);
char      crontab_cat     (cchar*);
char      crontab_list    (cchar*, cchar);
char      crontab_local   (cchar);
char      crontab_help    (void);
char      crontab_init    (void);
char      crontab_user    (cchar*);
char      crontab_hup     (void);
char      crontab_inst    (cchar*);
char      crontab_del     (cchar*);
char      crontab_stdin   (void);
char      crontab_edit    (void);
char      crontab_dir     (void);


char        /* PURPOSE : search for and process crontab updates               */
crontab_source(cchar *a_name)
{
   /*---(locals)--------------------------------*/
   tDIRENT  *den;
   DIR      *dir;
   /*---(open dir)------------------------------*/
   dir = opendir(".");
   if (dir == NULL) {
      printf("can not open source directory\n");
      return -1;
   }
   while ((den = readdir(dir)) != NULL) {
      if (strncmp(a_name, den->d_name, NAME)  == 0) {
         return 1;
      }
   }
   /*---(complete)------------------------------*/
   return 0;
}


char        /* PURPOSE : search for and process crontab updates               */
crontab_find  (cchar *a_name)
{
   /*---(locals)--------------------------------*/
   tDIRENT  *den;
   DIR      *dir;
   /*---(open dir)------------------------------*/
   dir = opendir(CRONTABS);
   if (dir == NULL) {
      printf("can not open crontab directory\n");
      return -1;
   }
   while ((den = readdir(dir)) != NULL) {
      if (strncmp(a_name, den->d_name, NAME)  == 0) {
         return 1;
      }
   }
   /*---(complete)------------------------------*/
   return 0;
}

char
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

char        /* PURPOSE : search for and process crontab updates               */
crontab_local (cchar a_action)
{
   /*---(locals)--------------------------------*/
   tDIRENT        *den;
   DIR            *dir;
   int             count     = 0;
   int             rc = 0;
   /*---(open dir)------------------------------*/
   dir = opendir(".");
   if (dir == NULL) {
      printf("can not open local directory\n");
      return -2;
   }
   /*> printf("--crontabs-------------------------\n", a_user);                       <*/
   while ((den = readdir(dir)) != NULL) {
      rc = name (den->d_name, '-');
      if (rc == 0) {
         if      (a_action == 'i') crontab_inst(den->d_name);
         else                      printf("%s\n", den->d_name);
         ++count;
      }
   }
   /*> printf("---%03d found-----------------------\n", count);                       <*/
   closedir(dir);
   /*---(complete)------------------------------*/
   return 0;
}

char        /* PURPOSE : search for and process crontab updates               */
crontab_list  (cchar *a_user, cchar a_cat)
{
   /*---(locals)--------------------------------*/
   tDIRENT        *den;
   DIR            *dir;
   char            x_user[25] = "";
   int             count     = 0;
   int             rc        = 0;
   /*---(defense)-------------------------------*/
   if (strncmp(a_user, "all", 5) == 0 && am_root == 'n') {
      printf("must be root to use all option\n");
      return -1;
   }
   /*---(format user names)---------------------*/
   snprintf(x_user, 22, "%s.",  a_user);
   /*---(open dir)------------------------------*/
   dir = opendir(CRONTABS);
   if (dir == NULL) {
      printf("can not open crontab directory\n");
      return -2;
   }
   /*> printf("--crontabs-------------------------\n", a_user);                       <*/
   while ((den = readdir(dir)) != NULL) {
      rc = name (den->d_name, 'c');
      /*> if (rc != 0)                         continue;                              <*/
      /*---(filter files)-----------------------*/
      if (strncmp(a_user, "all", 5) == 0 && am_root == 'y') {
         if (a_cat == 'y') crontab_cat(den->d_name);
         else              printf("%s\n", den->d_name);
         ++count;
         continue;
      }
      if (strncmp(den->d_name, x_user, ulen + 1) == 0) {
         if      (a_cat == 'y') crontab_cat (den->d_name);
         else if (a_cat == 'p') crontab_del (den->d_name);
         else                   printf("%s\n", den->d_name);
         ++count;
      }
   }
   /*> printf("---%03d found-----------------------\n", count);                       <*/
   closedir(dir);
   /*---(complete)------------------------------*/
   return 0;
}

char
crontab_help (void)
{
   printf("usage: crontab [OPTION] [<FILE>]\n");
   printf("crontab (by the heatherlys) manages crontab files for the cron daemon\n");
   printf("posix/standard options implemented:\n");
   printf("   <file>           install/replace a crontab file\n");
   printf("   -l               list all your installed crontabs to stdout\n");
   printf("   -d <file>        delete one of your crontabs by name\n");
   printf("   -r <file>        delete one of your crontabs by name\n");
   printf("   -u <user>        act as another user (root only)\n");
   printf("posix/standard options rejected:\n");
   printf("   - or null        install from stdin (no traceability, over-write risk)\n");
   printf("   -e               edit your crontab (no traceability or backup)\n");
   printf("   -c <dir>         change the crontab dir (security risk)\n");
   printf("extended options:\n");
   printf("   --all            list all installed crontabs (root only)\n");
   printf("   --list           list all your installed crontabs\n");
   printf("   --here           list all local crontabs that could be installed\n");
   printf("   --test <file>    test a crontab file for correctness\n");
   printf("   --system <file>  install system crontab (root only)\n");
   printf("   --purge          delete all your installed crontabs\n");
   printf("   --reload         delete all then install your crontabs\n");
   printf("   --help           print usage information\n");
   printf("crontab file naming rules\n");
   printf("   - our version allows multiple crontabs per user\n");
   printf("   - must be formatted as \"crontab.<description>\"\n");
   printf("   - description is '.' plus 1 to 50 characters\n");
   printf("   - valid characters in the description are [A-Za-z0-9_] only\n");
   printf("   - if you have only one crontab, just call it \"crontab.base\"\n");
   return 0;
}

char
crontab_init (void)
{
   /*---(locals)--------------------------------*/
   tPASSWD  *x_pass    = NULL;         /* passwd data structure               */
   int       rc        = 0;
   /*---(user name)-----------------------------*/
   uid       = getuid();
   x_pass    = getpwuid(uid);
   if (x_pass == NULL) {
      printf("can not retreive user information from the system\n");
      exit (-1);
   }
   ulen       = strlen(x_pass->pw_name);
   if (ulen < 1) {
      printf("user name is null\n");
      return -2;
   }
   if (ulen > 20) {
      printf("user name is too long\n");
      return -3;
   }
   strncpy(user, x_pass->pw_name, 20);
   /*---(check for root user)--------------*/
   am_root = 'n';
   if (uid == (int) geteuid())   am_root = 'y';
   /*---(change uid/permissions)-----------*/
   rc = setuid(0);
   if (rc != 0) {
      printf("could not get root authority\n");
      return -4;
   }
   /*> printf("caller : uid = %4d, name = %s\n", uid, user);                          <*/
   return 0;
}

char
crontab_user (cchar *a_user)
{
   /*---(locals)--------------------------------*/
   tPASSWD  *x_pass    = NULL;         /* passwd data structure               */
   /*---(check for root)------------------------*/
   if (am_root != 'y') {
      printf("must be root to use user change option\n");
      exit (-10);
   }
   /*---(defense)-------------------------------*/
   ulen       = strlen(a_user);
   if (ulen < 1) {
      printf("user name is null\n");
      return -2;
   }
   if (ulen > 20) {
      printf("user name is too long\n");
      return -3;
   }
   /*---(user name)-----------------------------*/
   x_pass    = getpwnam(a_user);
   if (x_pass == NULL) {
      printf("can not retreive user information from the system\n");
      exit (-1);
   }
   uid        = x_pass->pw_uid;
   strncpy(user, a_user, 20);
   /*---(complete)------------------------------*/
   return 0;
}

char
crontab_hup   (void)
{
   /*---(locals)--------------------------------*/
   int       rc        = 0;
   FILE      *f = NULL;
   int        crond_pid = 0;
   /*---(read pid)------------------------------*/
   f = fopen(LOCKFILE, "r");
   rc = fscanf(f, "%d", &crond_pid);
   if (rc < 1) {
      printf("can not locate pid for crond\n");
      return -1;
   }
   fclose(f);
   /*---(send signal)---------------------------*/
   kill(crond_pid, SIGHUP);
   /*---(complete)------------------------------*/
   return 0;
}

char
crontab_inst  (cchar *a_source)
{
   /*---(locals)--------------------------------*/
   int       rc        = 0;
   char      x_file[500]= "";          /* file name                           */
   /*---(verify source file)-----------------*/
   rc = crontab_source(a_source);
   if (rc != 1) {
      printf("crontab source file (%s) not found\n", a_source);
      return -1;
   }
   /*---(break up the file name)-------------*/
   rc = name (a_source, '-');
   if (rc <  0) {
      printf("crontab name is found, but format is not valid\n");
      return -2;
   }
   /*---(make sure to clear NEW)-------------*/
   snprintf (x_file, 500, "%s/%s.%s.NEW", CRONTABS, user, my.desc);
   rc = remove   (x_file);
   /*---(make sure to clear DEL)-------------*/
   snprintf (x_file, 500, "%s/%s.%s.DEL", CRONTABS, user, my.desc);
   rc = remove   (x_file);
   /*---(delete previous copy)---------------*/
   snprintf (x_file, 500, "%s/%s.%s", CRONTABS, user, my.desc);
   rc = remove   (x_file);
   /*---(copy file to crontab dir)-----------*/
   snprintf (x_file, 500, "cp %s %s/%s.%s.NEW", a_source, CRONTABS, user, my.desc);
   rc = system   (x_file);
   if (WIFEXITED(rc) <  0) {
      printf("can not copy file (%s.%s.NEW) to crondir\n", user, my.desc);
      return -3;
   }
   /*---(send update)------------------------*/
   crontab_hup();
   /*---(complete)---------------------------*/
   return 0;
}

char
crontab_del   (cchar *a_source)
{
   /*---(locals)--------------------------------*/
   int       rc        = 0;
   char      x_file[500]= "";          /* file name                           */
   /*---(break up the file name)----------------*/
   rc = name (a_source, 'c');
   if (rc <  0) {
      printf("crontab name format is not valid\n");
      return -1;
   }
   /*---(verify existing file)------------------*/
   snprintf(file, 100, "%s.%s", user, my.desc);
   rc = crontab_find (file);
   if (rc != 1) {
      /*---(if not, try the NEW version)--------*/
      snprintf (file, 500, "%s.%s.NEW", user, my.desc);
      rc = crontab_find (file);
      if (rc != 1) {
         /*---(if not, try the DEL version)--------*/
         snprintf (file, 500, "%s.%s.DEL", user, my.desc);
         rc = crontab_find (file);
         if (rc == 1) {
            printf("crontab (%s) is already deleted, nothing to do\n", a_source);
            return -2;
         } else {
            printf("crontab (%s) is not presently installed, nothing to do\n", a_source);
            return -2;
         }
      }
   }
   /*---(remove existing file)------------------*/
   if (chdir(CRONTABS) < 0) {
      printf("could not move to crontab directory\n");
      return -3;
   }
   snprintf(x_file, 500, "%s.%s.DEL", user, my.desc);
   rc = rename(file, x_file);
   if (rc <  0) {
      printf("can not delete file (%s.%s) from crondir\n", user, my.desc);
      return -4;
   }
   /*---(send update)------------------------*/
   crontab_hup();
   /*---(complete)------------------------------*/
   return 0;
}

char
crontab_dir   (void)
{
   printf("can not change crontab directory in this version (security risk)\n");
   /*---(complete)------------------------------*/
   return 0;
}

char
crontab_stdin (void)
{
   printf("can not read from stdin in this version (no traceability)\n");
   /*---(complete)------------------------------*/
   return 0;
}

char
crontab_edit  (void)
{
   printf("can not edit installed crontabs in this version (no traceability)\n");
   /*---(complete)------------------------------*/
   return 0;
}

int
main (int argc, char *argv[])
{
   /*---(no logger)-----------------------------*/
   my.logger = yLOG_begin("crontab", 2);
   /*---(init)----------------------------------*/
   crontab_init();
   /*---(process)-------------------------------*/
   if      (argc == 4 && strncmp(argv[2], "-u",     5) == 0) crontab_user (argv[3]);
   else if (argc == 5 && strncmp(argv[3], "-u",     5) == 0) crontab_user (argv[4]);
   /*---(process)-------------------------------*/
   char  *a = argv[1];
   if      (argc == 1)                                   crontab_stdin ();
   else if (argc >= 2 && strncmp(a, "--help",  10) == 0) crontab_help  ();
   else if (argc >= 2 && strncmp(a, "--list",  10) == 0) crontab_list  (user,  'n');
   else if (argc >= 2 && strncmp(a, "--all",   10) == 0) crontab_list  ("all", 'n');
   else if (argc >= 2 && strncmp(a, "--here",  10) == 0) crontab_local ('-');
   else if (argc >= 2 && strncmp(a, "--purge", 10) == 0) crontab_list  (user,  'p');
   else if (argc >= 2 && strncmp(a, "--reload",10) == 0) { crontab_list  (user,  'p'); crontab_local ('i'); }
   else if (argc >= 2 && strncmp(a, "-l",       5) == 0) crontab_list  (user,  'y');
   else if (argc >= 3 && strncmp(a, "-d",       5) == 0) crontab_del   (argv[2]);
   else if (argc >= 3 && strncmp(a, "-r",       5) == 0) crontab_del   (argv[2]);
   else if (argc >= 2 && strncmp(a, "-c",       5) == 0) crontab_dir   ();
   else if (argc >= 2 && strncmp(a, "-e",       5) == 0) crontab_edit  ();
   else if (argc >= 2 && strncmp(a, "-",        5) == 0) crontab_stdin ();
   else if (argc >= 2 && argv[1][0] != '-')              crontab_inst  (argv[1]);
   else    printf("requested action not understood or incomplete\n");
   /*---(complete)------------------------------*/
   yLOG_end   ();
   return 0;
}

/*====================---------[[ end-of-code ]]----------====================*/
