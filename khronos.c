/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"


struct cACCESSOR my;
char     *args[20];
char      version      = 'd';
int       failed;

char      debug_top    = 'n';
char      debug_args   = 'n';
char      debug_input  = 'n';
char      debug_proc   = 'n';
char      testing      = 'n';


/*====================------------------------------------====================*/
/*===----                          utilities                           ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITIES_______________o (void) {;}

void      prog_signal       (int a_signal, siginfo_t *a_info, void *a_nada) {return;};

char       /*lf--: PURPOSE : write the current time to cronpulse -------------*/
timestamp          (void)
{
   /*---(locals)-------------------------*/
   FILE     *pulser    = NULL;
   long      now       = 0;
   tTIME    *curr      = NULL;
   /*---(set time)-----------------------*/
   now       = time(NULL);
   curr      = localtime(&now);
   /*---(log to pulser)------------------*//*===fat=beg===*/
   strftime(my.pulse_time , 50, "%y.%m.%d.%H.%M.%S.%U   %s", curr);
   if (my.silent != 'y') {
      /*---(open)------------------------*/
      pulser = fopen(PULSER, "w");
      if (pulser != NULL) {
         fprintf(pulser, "%s\n", my.pulse_time);
         fclose(pulser);
      } else {
         yLOG_warn  ("PULSER", "cronpulse file can not be openned");
      }
      yLOG_info  ("time",   my.pulse_time);
   }
   /*---(complete)-----------------------*//*===fat=end===*/
   return curr->tm_min;
}

char       /*l---: PURPOSE : break string into an argv[] structure -----------*/
str_parse          (char *a_cstring)
{
   /*---(defenses)-----------------------*/
   if (a_cstring == NULL) return -1;
   /*---(locals)-------------------------*/
   int i, j, k;
   int len = strlen(a_cstring);
   int pos = 0;
   /*---(clear parsed array)-------------*/
   for (i = 0; i < 20; ++i) args[i] = NULL;
   /*---(assign executable)--------------*/
   args[0] = a_cstring;
   /*---(arguments)----------------------*/
   i    = 1;
   pos  = 0;
   while (pos < len) {
      /*---(get though argument)---------*/
      for (k = pos; k <= len; ++k) {
         if (  a_cstring[k] != '\0' &&
               a_cstring[k] != ' ' )     continue;
         pos = k;
         break;
      }
      if (pos + 1 >= len)              break;
      /*---(get though whitespace)-------*/
      for (k = pos; k < len; ++k) {
         if (a_cstring[k] == ' ') {
            a_cstring[k]  =  '\0';
            continue;
         }
         pos = k;
         break;
      }
      if (pos + 1 >= len)              break;
      /*---(assign next argument)--------*/
      args[i] = a_cstring + pos;
      /*---(prepare for next loop)-------*/
      ++i;
      if (i >= 20) break;
   }
   /*---(complete)-------------------------*/
   return 0;
}

char*      /*l---: PURPOSE : clean whitespace from both sides of a string ----*/
str_trim           (char *a_cstring)
{
   /*---(defenses)-------------------------*/
   if (a_cstring == NULL) return NULL;
   /*---(locals)---------------------------*/
   int i, j;
   int len = 0;
   /*---(leading whitespace)---------------*/
   len = strlen(a_cstring);
   for (i = 0; i <= len; ++i) {
      if (a_cstring[0] != ' ') break;
      for (j = 0; j <= len; ++j)
         a_cstring[j] = a_cstring[j + 1];
      --len;
   }
   /*---(trailing whitespace)--------------*/
   len = strlen(a_cstring);
   for (i = len - 1; i >= 0; --i) {
      if (a_cstring[i] != ' ') break;
      a_cstring[i] = '\0';
   }
   /*---(complete)-------------------------*/
   return a_cstring;
}


/*====================------------------------------------====================*/
/*===----                         housekeeping                         ----===*/
/*====================------------------------------------====================*/
static void      o___HOUSEKEEPING____________o (void) {;}

char      verstring    [500];

char*      /* ---- : return library versioning information -------------------*/
about              (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc  ]", 15);
#elif  __CBANG__  > 0
   strncpy (t, "[cbang    ]", 15);
#else
   strncpy (t, "[unknown  ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, VER_NUM, VER_TXT);
   return verstring;
}

char       /*lf--: PURPOSE : get the logging up and going --------------------*/
initialize         (const char a_quiet)
{
   /*---(defense)------------------------*/
   if (getuid() != 0) {
      printf("FATAL : khronos can only be daemonized by root\n");
      exit (-1);
   }
   /*---(set noisiness)------------------*//*===fat=beg===*/
   if      (a_quiet == 0)  { my.quiet = 0; my.updates = 0; }
   else if (a_quiet == 1)  { my.quiet = 0; my.updates = 1; }
   else                    { my.quiet = 1; my.updates = 1; }
   /*---(begin)--------------------------*/
   my.logger = yLOG_begin("khronos", yLOG_SYSTEM, my.quiet);
   if (my.logger < 1) {
      printf("khronos : can not start logger, FATAL\n");
      exit(1);
   }
   yLOG_enter (__FUNCTION__);
   yLOG_info  ("purpose",  "consistent, reliable time-based job scheduling");
   yLOG_info  ("cli args", "none");
   yLOG_info  ("khronos",  about());
   yLOG_info  ("yLOG"   ,  yLOG_version());
   yLOG_info  ("ySCHED" ,  ySCHED_version());
   /*---(init)---------------------------*//*===fat=end===*/
   my.uid = getuid();
   yLOG_value ("uid num", my.uid);
   my.silent  = 'n';
   /*---(variables)----------------------*/
   nfile  = 0;
   nentry = 0;
   nfast  = 0;
   nproc  = 0;
   /*---(end)----------------------------*/
   yLOG_exit  (__FUNCTION__);
   /*---(complete)-----------------------*/
   return 0;
}

void       /*----: PURPOSE : handle signals ----------------------------------*/
communicate        (int a_signal)
{
   /*---(catch)--------------------------*/
   switch (a_signal) {
   case  SIGHUP:
      yLOG_info  ("SIGNAL", "SIGHUP MEANS REFRESH CRONTABS BEFORE NEXT RUN");
      my.resync = '-';
      break;
   case  SIGTERM:
      yLOG_info  ("SIGNAL", "SIGTERM means terminate daemon");
      watch_end();
      yEXEC_term ("EXITING", 99);
      break;
   case  SIGSEGV:
      yLOG_info  ("SIGNAL", "SIGSEGV means daemon blew up");
      watch_end();
      yEXEC_term ("EXITING", 99);
      break;
   case  SIGABRT:
      yLOG_info  ("SIGNAL", "SIGABRT means daemon blew up");
      watch_end();
      yEXEC_term ("EXITING", 99);
      break;
   }
   /*---(reset)--------------------------*/
   signals();
   /*---(complete)-----------------------*/
   return;
}

char       /*l---: PURPOSE : setup signal handling ---------------------------*/
signals            (void)
{
   /*---(set)----------------------------*/
   signal(SIGCHLD,  SIG_IGN);        /* ignore children       */
   signal(SIGTSTP,  SIG_IGN);        /* ignore tty signals    */
   signal(SIGTTOU,  SIG_IGN);        /* ignore tty signals    */
   signal(SIGTTIN,  SIG_IGN);        /* ignore tty signals    */
   signal(SIGHUP,   communicate);    /* hangup means refresh  */
   signal(SIGTERM,  communicate);    /* catch a kill          */
   signal(SIGSEGV,  communicate);    /* catch a segfault      */
   signal(SIGABRT,  communicate);    /* catch a abort         */
   /*---(complete)-----------------------*/
   return 0;
}

char               /* PURPOSE : daemonize the program ------------------------*/
daemonize          (void)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   int       i    = 0;                       /* loop iterator                 */
   int       fd   = 0;                       /* file descriptor               */
   int      status  = 0;
   /*---(fork off and die)----------------------*/
   DAEMON {
      yLOG_info  ("mode", "daemon mode requested");
      yEXEC_daemon (my.logger, &my.pid);
   } else {
      yLOG_info  ("mode", "foreground mode requested");
   }
   /*---(check security on crontabs)------------*/
   struct    stat s;
   int       rc = 0;
   rc = lstat(CRONTABS, &s);
   if  (rc < 0) {
      yLOG_info  ("crondir", "directory does not exist");
      yEXEC_term (__FUNCTION__, 6);
   }
   if  (!S_ISDIR(s.st_mode))  {
      yLOG_info  ("crondir", "not a directory");
      yEXEC_term (__FUNCTION__, 6);
   }
   yLOG_info  ("crondir", "directory existance verified");
   if  (s.st_uid  != 0)  {
      yLOG_info  ("crondir", "directory not owned by root");
      yEXEC_term (__FUNCTION__, 6);
   }
   if  (s.st_gid  != 0)  {
      yLOG_info  ("crondir", "directory group is not root");
      yEXEC_term (__FUNCTION__, 6);
   }
   if  ((s.st_mode & 00777) != 00700)  {
      yLOG_info  ("crondir", "directory permissions not 0700");
      yEXEC_term (__FUNCTION__, 6);
   }
   yLOG_info  ("security", "directory owned by root:root and 0700");
   /*---(change to safe location)---------------*/
   yLOG_info  ("location", "change current dir to a safe place");
   if (chdir(CRONTABS) < 0) {
      yLOG_info  ("cd",    "change directory FAILED");
      yEXEC_term (__FUNCTION__, 4);
   }
   /*---(signals)-------------------------------*/
   yLOG_info  ("signals",  "setup signal handlers");
   yEXEC_signal (yEXEC_SOFT, 'n', yEXEC_CNO, 'n');
   /*---(run file)------------------------------*/
   yLOG_info  ("unique"  , "test for single instance");
   lock();
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char       /* ---- : ensure single threading of the daemon -------------------*/
lock               (void)
{
   if (yEXEC_find ("khronos", NULL) > 1) {
      yLOG_info  ("FAILURE", "khronos already running");
      yEXEC_term ("FAILURE", 7);
   }
   return 0;
}

char        /* PURPOSE : final preparation for run                     */
prepare       (void)
{
   yLOG_info("watcher", "update watcher to /var/log/yLOG/cronwatch");
   watch_beg();
   search('a');
   yLOG_value ("read in", nfile);
   my.resync  = 'n';
   /*---(complete)-------------------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        cronpulse file                        ----===*/
/*====================------------------------------------====================*/
static void      o___CRONPULSE_______________o (void) {;};

char               /* PURPOSE : update cronwatch interrun monitoring file     */
watch_end          (void)
{
   if (my.updates) return 0;
   /*---(locals)---------------------------------*/
   FILE      *watcher;
   /*---(get the last stop time)-----------------*/
   watcher = fopen(WATCHER, "a");
   /*---(write the last end time)----------------*/
   timestamp();
   snprintf(my.pulse_end  , 50, "%s   end", my.pulse_time);
   if   (watcher != NULL) {
      fprintf(watcher, "%s\n", my.pulse_end);
      fclose(watcher);
   }
   else yLOG_warn  ("WATCHER", "cronwatch file can not be openned");
   /*---(complete)-------------------------------*/
   return 0;
}

char               /* PURPOSE : update cronwatch interrun monitoring file     */
watch_beg          (void)
{
   if (my.updates) return 0;
   /*---(locals)---------------------------------*/
   FILE      *watcher;
   int        len;
   char       trash[50];
   char       xtime[50];
   struct tm *curr_time;
   time_t     time_date;
   /*---(get the last stop time)-----------------*/
   /*> lastrun();                                                                     <*/
   watcher = fopen(WATCHER, "a");
   /*---(get a start time)--------------------*/
   timestamp();
   yLOG_info  ("this beg", my.pulse_time);
   /*---(get missed time)---------------------*/
   /*> sscanf(my.pulse_time , "%s %ld", trash, &my.this_start);                       <* 
    *> my.fast_beg = my.this_start;                                                   <* 
    *> yLOG_value ("missed secs", my.this_start - my.last_end);                       <*/
   /*---(write the time)-------------------------*/
   snprintf(my.pulse_begin, 50, "%s   BEGINNING", my.pulse_time);
   if (watcher != NULL) {
      fprintf(watcher, "%s\n", my.pulse_begin);
      fclose(watcher);
   }
   else yLOG_warn  ("WATCHER", "cronwatch file can not be openned");
   /*---(complete)-------------------------------*/
   return 0;
}

long               /* PURPOSE : read the last time from cronpulse ------------*/
pulse_last    (void)
{
   /*---(locals)--------------------------------*/
   FILE     *pulser    = NULL;
   long      xtime     = 0;
   long      final     = 0;
   char      xbuf[55]  = "";
   int       len       = 0;
   char      trash[50] = "";
   int       rc        = 0;
   /*---(default to now)-------------------------*/
   final = time(NULL);
   /*---(open)-----------------------------------*/
   pulser  = fopen(PULSER, "r");
   if (pulser  != NULL) {
      /*---(get the last stop time)--------------*/
      fgets(xbuf, 50 , pulser);
      /*---(clean it up)-------------------------*/
      len    = strlen(xbuf) - 1;
      if (xbuf[len] == '\n') xbuf[len] = '\0';
      /*---(pull out the epoch)------------------*/
      rc = sscanf(xbuf, "%s %ld", trash, &xtime);
      if (rc == 2 && final > xtime && xtime > 0)  final = xtime;
      /*---(close)-------------------------------*/
      fclose(pulser);
   }
   /*---(complete)-------------------------------*/
   return final;
}

long               /* PURPOSE : read the last time from cronpulse ------------*/
lastrun       (void)
{
   /*---(locals)--------------------------------*/
   FILE     *watcher   = NULL;
   long      xtime     = 0;
   char      xbuf[55]  = "";
   int       len       = 0;
   char      trash[50] = "";
   int       rc        = 0;
   tTIME    *curr      = NULL;
   /*---(default to now)-------------------------*/
   my.last_end = time(NULL);
   /*---(open)-----------------------------------*/
   watcher = fopen(WATCHER, "r");
   if (watcher != NULL) {
      /*---(get the last stop time)--------------*/
      fgets(xbuf, 50 , watcher);
      /*---(clean it up)-------------------------*/
      len    = strlen(xbuf) - 1;
      if (xbuf[len] == '\n') xbuf[len] = '\0';
      /*---(pull out the epoch)------------------*/
      rc = sscanf(xbuf, "%s %ld", trash, &xtime);
      if (rc == 2 && xtime < my.last_end && xtime > 0)  my.last_end = xtime;
      /*---(close)-------------------------------*/
      fclose(watcher);
   } else {
      yLOG_warn  ("WATCHER", "cronwatch file can not be openned");
   }
   /*---(save if off)----------------------------*/
   curr  = localtime(&my.last_end);
   strftime(my.pulse_time , 50, "%y.%m.%d.%H.%M.%S.%U   %s", curr);
   yLOG_info  ("last end", my.pulse_time);
   /*---(complete)-------------------------------*/
   return my.last_end;
}

char       /* PURPOSE : generate a current status file -----------------------*/
status             (void)
{
   /*---(locals)---------------------------------*/
   tCFILE   *file;                           /* current file                  */
   tCLINE   *line;                           /* current line                  */
   FILE     *status    = NULL;
   /*---(open)-----------------------------------*/
   status  = fopen(STATUS, "w");
   if (status  != NULL) {
      fprintf(status, "khronos status reporting\n");
      fprintf(status, "updated : %ld\n", (long) time(NULL));
      fprintf(status, "\n");
      /*---(scan every file)--------------------*/
      for (file = cronhead; file != NULL; file = file->next) {
         fprintf(status, "FILE (%c) %3d = %-20.20s :: %-40.40s\n",
               file->retire, file->nlines, file->user, file->name);
         /*---(scan every line)-----------------*/
         for (line = file->head; line != NULL; line = line->next) {
            fprintf(status, "   %3d : %-20.20s : r=%4d, t=%10ld, a=%d, f=%4d, p=%6d : %s\n", line->recd, line->title, line->attempts, line->lasttime, line->active, line->failures, line->rpid, line->cmd);
         }
         fprintf(status, "\n");
      }
      /*---(close)-------------------------------*/
      fclose(status);
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        crontab updates                       ----===*/
/*====================------------------------------------====================*/
static void      o___CRONTABS________________o (void) {;}

char        /* PURPOSE : search for and process crontab updates               */
search        (cchar a_scope)
{
   if (a_scope == '?' && my.resync == 'n')  return -1;
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   tDIRENT  *den;
   DIR      *dir;
   int       count     = 0;
   int       len       = 0;
   char      x_file[NAME];
   /*---(save scope)-----------------------------*/
   if (a_scope    != '?')  my.resync   = a_scope;
   /*---(open dir)------------------------------*/
   dir = opendir(CRONTABS);
   if (dir == NULL) {
      yLOG_info("crondir", "FATAL, not found");
      yLOG_exit  (__FUNCTION__);
      return 0;
   }
   /*---(report scope)--------------------------*/
   if (my.resync == 'a') yLOG_info("scope", "clear and load all files");
   else                  yLOG_info("scope", "look only for updated files");
   /*---(loop through files)--------------------*/
   while ((den = readdir(dir)) != NULL) {
      /*---(defense)----------------------------*/
      len       = strlen(den->d_name);
      if (len < 3)                continue;       /* one char each name/desc  */
      if (len >= NAME - 1)        continue;       /* max acceptable length    */
      strncpy(x_file, den->d_name, NAME);
      if (x_file[len - 1] == '~') continue;       /* backup file              */
      /*---(changes)----------------------------*/
      strncpy(my.action, "", 5);
      if (len > 7) {
         if (strncmp(x_file + len - 3, "DEL", 5) == 0) {
            yLOG_info ("DEL",   x_file);
            x_file[len - 4] = '\0';
            strncpy(my.action, "DEL", 5);
            assimilate(x_file);
         }
         if (strncmp(x_file + len - 3, "NEW", 5) == 0) {
            yLOG_info ("NEW",   x_file);
            x_file[len - 4] = '\0';
            rename(den->d_name, x_file);
            strncpy(my.action, "NEW", 5);
            assimilate(x_file);
         }
      }
      ++count;
      /*---(normal files)-----------------------*/
      if (my.resync != 'a')       continue;
      ++count;
      assimilate(x_file);
   }
   closedir(dir);
   yLOG_value ("count",  count);
   /*---(delete)--------------------------------*/
   system("rm -f *.DEL");
   /*---(complete)------------------------------*/
   /*> my.resync = 'n';                                                               <*/
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char        /* PURPOSE : validate a single crontab name and process it        */
assimilate    (cchar *a_name)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   char      rc   = 0;                            /* simple return code       */
   FILE     *f    = NULL;                         /* crontab file decriptor   */
   tCFILE   *curr = NULL;                         /* current crontab struct   */
   /*---(check file name)-----------------------*/
   rc      = name (a_name, '-');                  /* vaildate name            */
   if (rc <  0) {
      TEST  printf("   crontab is misnamed\n");
      yLOG_warn  ("name",    "file name is not valid");
      yLOG_exit  (__FUNCTION__);
      return -1;
   }
   TEST  printf("   name is good = <<%s>>\n", a_name);
   TEST  strcpy(my.user, my.who);
   /*---(check on crontab file)-----------------*/
   struct    stat s;
   rc = lstat(a_name, &s);
   if  (rc < 0) {
      TEST  printf("   crontab could not be found\n");
      yLOG_info  ("FAILED",  "crontab file does not exist");
      yLOG_exit  (__FUNCTION__);
      return -2;
   }
   TEST  printf("   crontab found\n");
   if  (!S_ISREG(s.st_mode))  {
      TEST  printf("   crontab is not a regular file\n");
      yLOG_info  ("FAILED",  "crontab is not a regular file");
      yLOG_exit  (__FUNCTION__);
      return -2;
   }
   TEST  printf("   crontab is a normal file\n");
   yLOG_info  ("file stat", "crontab is a normal file");
   /*---(elimintate exiting copy)---------------*/
   rc      = retire (my.name);
   if (strncmp(my.action, "DEL", 5) == 0) {
      yLOG_exit  (__FUNCTION__);
      return 0;
   }
   /*---(process new one)-----------------------*/
   f = fopen(a_name, "r");
   if (f == NULL) {
      TEST  printf("   crontab could not be openned\n");
      yLOG_info  ("FAILED",  "can not open the file");
      yLOG_exit  (__FUNCTION__);
      return -2;
   }
   TEST  printf("   crontab openned successfully\n");
   ySCHED_reset();
   rc = create (my.name, my.user, &curr);
   if (rc >= 0) rc = inventory (curr, f);
   /*---(complete)------------------------------*/
   fclose(f);
   yLOG_exit  (__FUNCTION__);
   return rc;
}

char        /* PURPOSE : validate the crontab name                            */
name          (cchar *a_name, cchar a_loc)
{
   if (a_name == NULL) return -1;
   yLOG_enter (__FUNCTION__);
   yLOG_info  ("CRONTAB",   a_name);
   /*---(locals)--------------------------------*/
   char     *p;                                   /* pointer  for strtok      */
   int       len;                                 /* string's length          */
   int       i    = 0;                            /* loop iterator -- char    */
   char      legal[100] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
   char      x_name[NAME];        /* name of the current crontab               */
   /*---(defense)-------------------------------*/
   if (a_name[0] == '.') {
      yLOG_warn  ("name",  "can not process hidden crontab names");
      yLOG_exit  (__FUNCTION__);
      return -8;
   }
   len = strlen(a_name);
   if (len >= NAME) {
      yLOG_warn  ("name",  "crontab name too long to be processed");
      yLOG_exit  (__FUNCTION__);
      return -1;
   }
   strncpy(x_name, a_name, NAME);
   /*---(get user name)-------------------------*/
   p = strtok(x_name, ".");
   if (p == NULL) {
      yLOG_warn  ("user",  "crontab file does not have a user name");
      yLOG_exit  (__FUNCTION__);
      return -2;
   }
   len = strlen(p);
   if (len >= USER) {
      yLOG_warn  ("user",  "user name too long to be processed");
      yLOG_exit  (__FUNCTION__);
      return -3;
   }
   for (i = 0; i < len; ++i) {
      if (strchr(legal, p[i]) != NULL) continue;
      yLOG_warn  ("user",  "user name has illegal characters");
      yLOG_exit  (__FUNCTION__);
      return -4;
   }
   strncpy(my.user, p, USER);
   /*---(get crontab description)---------------*/
   p = strtok(NULL,   ".");
   if (p == NULL) {
      yLOG_warn  ("desc",  "crontab file does not have a description");
      yLOG_exit  (__FUNCTION__);
      return -5;
   }
   len = strlen(p);
   if (len >= DESC) {
      yLOG_warn  ("desc",  "decription too long to be processed");
      yLOG_exit  (__FUNCTION__);
      return -9;
   }
   for (i = 0; i < len; ++i) {
      if (strchr(legal, p[i]) != NULL) continue;
      yLOG_warn  ("desc",  "crontab description has illegal characters");
      yLOG_exit  (__FUNCTION__);
      return -6;
   }
   strncpy(my.desc, p, DESC);
   /*---(defense, improper format)--------------*/
   p = strtok(NULL,   ".");
   if (a_loc == 'c') {
      if (p != NULL && strncmp(p, "NEW", 4) != 0 && strncmp(p, "DEL", 4) != 0) {
         yLOG_info  ("name",  "crontab name is illegally suffixed");
         yLOG_exit  (__FUNCTION__);
         return -7;
      }
   } else {
      if (p != NULL) {
         yLOG_info  ("name",  "crontab name is illegally suffixed");
         yLOG_exit  (__FUNCTION__);
         return -8;
      }
   }
   /*---(complete)------------------------------*/
   strncpy(my.name, a_name, NAME);
   yLOG_info  ("success", "crontab name is legal");
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char
retire        (cchar *a_name)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   tCFILE   *file;
   tCLINE   *line;
   tCFILE   *save;
   int       found = 0;
   /*---(process)-------------------------------*/
   for (file = cronhead; file != NULL; file = file->next) {
      yLOG_info ("compare", file->name);
      /*---(defense)-------------------------*/
      if (file->retire   == 'y')                  continue; /* already retired*/
      if (strncmp(a_name, file->name, NAME) != 0) continue; /* name wrong     */
      /*---(mark head)-----------------------*/
      file->retire   = 'y';
      found = 1;
      /*---(process lines)-------------------*/
      /*> printf("   FOR :: file=%s, lines=%d, retire=%c\n", file->name, file->nlines, file->retire);   <*/
      line = file->head;
      while (line != NULL) {
         line->deleted = 'y';
         /*---(running lines)-------------------*/
         if (line->rpid != 0) {
            /*> printf("   active job, mark\n");                                      <*/
            yLOG_bullet (line->recd, "running, but marking for deletion");
            line = line->next;
         }
         /*---(non-running line)----------------*/
         else {
            /*> printf("   inactive job, delete\n");                                  <*/
            yLOG_bullet (line->recd, "freeing now");
            cronline_del (line);
            line = file->head;
         }
         if (line == NULL) break;
      }
      /*> printf("   AFT :: file=%s, lines=%d, retire=%c\n", file->name, file->nlines, file->retire);   <*/
      if (file->nlines == 0 && file->retire == 'y') {
         /*> printf("   now empty file\n");                                           <*/
         save = file->next;
         cronfile_del (file);
         file = save;
      }
      if (file == NULL) break;
   }
   /*---(summarize)-----------------------------*/
   if (found == 0) {
      yLOG_info  ("crontab",  "existing not found, continuing");
      yLOG_exit  (__FUNCTION__);
      return -1;
   }
   yLOG_info  ("crontab",  "found and retired existing crontab");
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return  0;
}

char        /* PURPOSE : delete all cronfiles and cronlines                   */
purge         (void)
{
   /*---(locals)---------------------------------*/
   tCFILE   *file = cronhead;                /* current file                  */
   int       rc   = 0;
   /*---(scan every file)--------------------*/
   while (file != NULL) {
      /*> printf("purging %s\n", file->name);                                         <*/
      rc = retire(file->name);
      if (rc < 0)       break;
      file = cronhead;
      if (file == NULL) break;
   }
   return 0;
}

char
create        (cchar *a_name, cchar *a_user, tCFILE **a_curr)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   tPASSWD  *pass = NULL;
   tCFILE   *file = NULL;
   /*---(create the space)----------------------*/
   file          = malloc(sizeof(tCFILE));
   yLOG_point ("malloc",     file);
   if (file == NULL) {
      TEST  printf("   malloc could not create space\n");
      yLOG_warn  ("cronfile",  "malloc could not create space");
      yLOG_exit  (__FUNCTION__);
      return -2;
   }
   TEST  printf("   file space malloc'd\n");
   /*---(base data)-----------------------------*/
   strncpy(file->name, a_name, NAME);
   strncpy(file->user, a_user, USER);
   yLOG_info ("user is", file->user);
   /*---(get uid)-------------------------------*/
   pass = getpwnam(a_user);                       /* get password entry       */
   if (pass == NULL) {
      TEST  printf("   user not found = <<%s>>\n", a_user);
      yLOG_warn  ("user",  "user name not a valid account on the system");
      yLOG_exit  (__FUNCTION__);
      return -1;
   }
   file->uid       = pass->pw_uid;
   TEST  printf("   user uid = %d\n", file->uid);
   yLOG_value ("UID",   file->uid);
   /*---(initialize the rest)-------------------*/
   file->retire    = 'n';
   /*---(link into file dll)--------------------*/
   cronfile_add (file);
   TEST  printf("   added to file linked list\n");
   /*---(finalize)------------------------------*/
   (*a_curr) = file;
   /*---(complete)------------------------------*/
   TEST  printf("   done\n\n");
   yLOG_exit  (__FUNCTION__);
   return 0;
}

char
inventory     (tCFILE *a_cfile, FILE *a_source)
{
   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   char      buffer[LINE];
   char      backup[LINE];
   int       len   = 0;
   int       recds = 0;
   char     *xmin, *xhrs, *xday, *xmon, *xdow;
   char     *xtmp, *xcmd;
   int       rc;
   tCLINE   *cline;
   char     *p;
   char     *s0        = NULL;
   char      original  = 'n';
   /*---(run input lines)-----------------------*/
   yLOG_point ("cfile",    a_cfile);
   while (1) {
      /*---(get the line)-----------------------*/
      fgets(buffer, LINE - 5, a_source);
      if (feof(a_source))    break;
      len = strlen(buffer) - 1;
      buffer[len] = '\0';
      ++recds;
      /*> if (len       <  10 ) { ySCHED_parse (NULL, ""); continue; }                <* 
       *> if (buffer[0] == '#') { ySCHED_parse (NULL, ""); continue; }                <*/
      /*---(call parsing)----------------*/
      /*> yLOG_break();                                                               <*/
      original = 'n';
      p = strchr   (buffer, '\x1F');
      if (p == NULL)  {
         /*> printf ("%p : %s\n", buffer, buffer);                                    <*/
         if ((p = strchr   (buffer, ' ')) == NULL)   continue;
         /*> printf ("%p : <<%s>>\n", p, p);                                          <*/
         if ((p = strchr   (p + 1 , ' ')) == NULL)   continue;
         /*> printf ("%p : <<%s>>\n", p, p);                                          <*/
         if ((p = strchr   (p + 1 , ' ')) == NULL)   continue;
         /*> printf ("%p : <<%s>>\n", p, p);                                          <*/
         if ((p = strchr   (p + 1 , ' ')) == NULL)   continue;
         /*> printf ("%p : <<%s>>\n", p, p);                                          <*/
         if ((p = strchr   (p + 1 , ' ')) == NULL)   continue;
         /*> printf ("%p : <<%s>>\n", p, p);                                          <*/
         *p = '\x1F';
         /*> printf ("%p : <<%s>>\n", buffer, buffer);                                <*/
         original = 'y';
      }
      p = strtok_r (buffer, "\x1F", &s0);
      /*> printf ("%p : <<%s>>\n", p, p);                                             <*/
      rc = ySCHED_parse (NULL, p);
      /*> printf ("rc = %d\n", rc);                                                   <*/
      if (rc < 0) continue;
      yLOG_value  ("line_no"   , recds);
      /*---(create a line)---------------*/
      /*> cline = malloc(sizeof(tCLINE));                                             <*/
      cline = malloc(3000);
      if (cline == NULL) {
         TEST  printf("couldn't malloc\n\n");
         yLOG_info  ("malloc", "failed");
         yLOG_exit  (__FUNCTION__);
         return -1;
      }
      yLOG_info  ("malloc", "good to go");
      /*---(link into line dll)----------*/
      cronline_add (a_cfile, cline);
      cline->recd         = recds;
      /*---(scheduling info)-------------*/
      ySCHED_save (&cline->sched);
      /*---(title)-----------------------*/
      if (original == 'n') {
         p = strtok_r (NULL , "\x1F", &s0);
         str_trim(p);
         strncpy(cline->title, p, CMD);
         yLOG_info  ("title",    cline->title);
      }
      /*---(chars)-----------------------*/
      if (original == 'n') {
         p = strtok_r (NULL , "\x1F", &s0);
         str_trim(p);
         if (strlen(p) > 3) {
            cline->recovery = p[0];
            cline->priority = p[1];
            cline->alerting = p[2];
            yLOG_char  ("recovery", cline->recovery);
            yLOG_char  ("priority", cline->priority);
            yLOG_char  ("alerting", cline->alerting);
         }
      }
      /*---(final data)-------------------------*/
      p = strtok_r (NULL , "\x1F", &s0);
      str_trim(p);
      yLOG_info ("cmd",  p);
      strncpy(cline->cmd, p, CMD);
      TEST  printf("   command    : <<%-.65s>>\n", p);
      TEST  printf("   GOOD TO GO\n\n");
   }
   /*---(complete)------------------------------*/
   yLOG_exit  (__FUNCTION__);
   return  0;
}



/*====================------------------------------------====================*/
/*===----                         job handling                         ----===*/
/*====================------------------------------------====================*/
static void      o___EXECUTION_______________o (void) {;};

char        /* PURPOSE : create list of jobs to run in a particular period    */
fast          (clong a_start)                /* time_t of starting hour       */
{
   if (my.silent == 'n') {
      yLOG_enter (__FUNCTION__);
      /*> yLOG_note  ("determine jobs that will run in the next hour");               <*/
   }
   /*---(locals)---------------------------------*/
   int       chrs, cday, cmon, cdow;         /* simplifying temp variables    */
   char      msg[100];                       /* display string                */
   tCFILE   *x_file;                         /* current file                  */
   tCLINE   *x_line;                         /* current line                  */
   /*---(save time)------------------------------*/
   if (a_start    != 0)  my.fast_beg   = a_start;
   /*---(get the date)---------------------------*/
   tTIME    *curr_time = localtime(&my.fast_beg);
   /*---(work out the current time)----------*/
   chrs = curr_time->tm_hour;
   cday = curr_time->tm_mday;
   cmon = curr_time->tm_mon + 1;   /* change to 1-12                          */
   cdow = curr_time->tm_wday;
   if (cdow == 0) cdow = 7;        /* put sunday at the end                   */
   /*---(format the time)------------------------*/
   snprintf(msg, 50, "---- %02dh %02dd %02dm %01dw", chrs, cday, cmon, cdow);
   yLOG_info ("fast for", msg);
   /*---(prepare)-------------------------------*/
   nfast     = 0;
   fasthead  = NULL;
   fasttail  = NULL;
   /*---(scan every file)--------------------*/
   for (x_file = cronhead; x_file != NULL; x_file = x_file->next) {
      yLOG_info  ("file"      , x_file->name);
      /*---(scan every line)-----------------*/
      for (x_line = x_file->head; x_line != NULL; x_line = x_line->next) {
         /*---(initialize every line)-----------*/
         x_line->active  = 0;
         x_line->fnext   = NULL;
         x_line->fprev   = NULL;
         /*---(filter for active)---------------*/
         if (x_file->retire  == 'y')     continue;
         if (x_line->deleted == 'y')     continue;
         /*---(filter for time)-----------------*/
         snprintf(msg, 100, "%3d) %s", x_line->recd, x_line->title); 
         if (ySCHED_test (&x_line->sched, chrs, ySCHED_ANY) < 0) {
            yLOG_info  ("not",     msg);
            continue;
         }
         yLOG_info  ("YES",     msg);
         /*---(hook them up)--------------------*/
         if (nfast == 0) {
            fasthead        = x_line;
            fasttail        = x_line;
         } else {
            fasttail->fnext = x_line;
            fasttail        = x_line;
         }
         x_line->active  = 1;
         ++nfast;
      }
   }
   /*---(log results)---------------------------*/
   if (my.silent  == 'n') {
      yLOG_value ("count", nfast);
      yLOG_exit  (__FUNCTION__);
   }
   /*---(complete)------------------------------*/
   return 0;
}

char        /* PURPOSE : run through the fast list to find what should run    */
dispatch      (cint a_min)
{
   if (my.silent == 'n') yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   int       njobs = 0;                      /* number of jobs ready to run   */
   int       ntest = 0;                      /* number of jobs ready to run   */
   tCLINE   *x_line;                         /* current line                  */
   char      msg[200];
   /*---(process all time periods)--------------*/
   /*> if (my.silent == 'n') yLOG_value ("potential",  nfast);                        <*/
   for (x_line = fasthead; x_line != NULL; x_line = x_line->fnext) {
      if (my.silent == 'y') {
         switch (x_line->recovery) {
         case '-' : continue;  break;
         case 'x' : continue;  break;
         }
      }
      ++ntest;
      snprintf(msg, 200, "%-16.16s,%3d", x_line->file->name, x_line->recd);
      if (x_line->sched.min[a_min]  == '_') {
         if (my.silent == 'n') {
            yLOG_senter ("not");
            yLOG_snote(msg);
            yLOG_snote  ("not sched for min");
            yLOG_sexit  ();
         }
         continue;
      }
      if (x_line->rpid        != 0) {
         if (my.silent == 'n') {
            yLOG_senter ("not");
            yLOG_snote(msg);
            yLOG_snote  ("already running");
            yLOG_sexit  ();
         }
         continue;
      }
      run (x_line->file, x_line);
      ++njobs;
   }
   if (my.silent == 'n') {
      /*> yLOG_value ("tested", ntest);                                               <*/
      /*> yLOG_value ("ran",    njobs);                                               <*/
      yLOG_exit  (__FUNCTION__);
   }
   /*---(complete)------------------------------*/
   return 0;
}

char
run        (tCFILE *a_file, tCLINE *a_line)
{
   /*---(locals)--------------------------------*/
   int       rc        = 0;                       /* simple return code       */
   int       rpid      = 0;                       /* child pid for execution  */
   tPASSWD  *pass;
   FILE     *output = NULL;
   long      now;                                 /* present datetime         */
   tTIME    *curr_time = NULL;
   char      msg[200];
   char      envp[10][200];
   /*---(run)----------------------------*/
   snprintf(msg, 200, "%-16.16s,%3d", a_line->file->name, a_line->recd);
   rc = yEXEC_run (STUFF, msg, a_file->user, a_line->cmd, SHELL, PATH);
   if (rc <  0) {
      a_line->rpid       = 0;
      return -1;
   }
   if (rc >= 0) {
      a_line->rpid       = rc;
      a_line->lasttime = time(NULL);
      ++a_line->attempts;
      proclist_add (a_line);
   }
   return 0;
}

char        /* PURPOSE : verify status of running jobs and mark completions   */
check         (void)
{
   yLOG_enter (__FUNCTION__);
   int          rc =0;
   /*---(defense)-------------------------------*/
   if (nproc == 0) {
      yLOG_note  ("no currently running jobs");
      yLOG_exit  (__FUNCTION__);
      return 0;
   }
   /*---(locals)--------------------------------*/
   tCLINE   *x_line    = NULL;                    /* current line             */
   tCFILE   *x_file    = NULL;                    /* the line's file          */
   tCLINE   *x_pnext   = NULL;                    /* the next line to process */
   int       x_status  = 0;                       /* the line's job status    */
   char      msg[200];
   /*---(prepare)-------------------------------*/
   x_line    = prochead;
   /*---(loop)----------------------------------*/
   while (x_line != NULL) {
      /*---(save the next)----------------------*/
      x_pnext   = x_line->pnext;
      x_file    = x_line->file;
      snprintf(msg, 200, "%-15.15s,%3d", x_line->file->name, x_line->recd);
      rc = yEXEC_check (msg, x_line->rpid);
      /*---(handle running line)----------------*/
      if (rc <= 0)  {
         if (rc <  0)  {
            x_line->lastexit = -1;
            ++x_line->failures;
         }
         proclist_del (x_line);
         if (x_line->deleted == 'y') {
            yLOG_snote ("deleted");
            cronline_del (x_line);
            if (x_file->nlines == 0) cronfile_del (x_file);
         }
      }
      x_line = x_pnext;
      if (x_line == NULL)    break;
   }
   yLOG_exit  (__FUNCTION__);
   return 0;
}


/*=============================[[ end-of-code ]]==============================*/
