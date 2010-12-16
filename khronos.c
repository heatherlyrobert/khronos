/*===========================[[ start-of-code ]]==============================*/
#include      "khronos.h"


struct cACCESSOR my;
char     *args[20];
char   version;
int    failed;

char      debug_top    = 'n';
char      debug_args   = 'n';
char      debug_input  = 'n';
char      debug_proc   = 'n';
char      testing      = 'n';


/*====================------------------------------------====================*/
/*===----                          utilities                           ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITIES_______________o (void) {;}

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
   CHATTY   strftime(my.pulse_time , 50, "%y.%m.%d.%H.%M.%S.%U   %s", curr);
   CHATTY   if (my.silent != 'y') {
   CHATTY      /*---(open)------------------------*/
   CHATTY      pulser = fopen(PULSER, "w");
   CHATTY      if (pulser != NULL) {
   CHATTY         fprintf(pulser, "%s\n", my.pulse_time);
   CHATTY         fclose(pulser);
   CHATTY      } else {
   CHATTY         yLOG_warn  ("PULSER", "cronpulse file can not be openned");
   CHATTY      }
   CHATTY      yLOG_info  ("time",   my.pulse_time);
   CHATTY   }
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

char       /*lf--: PURPOSE : get the logging up and going --------------------*/
initialize         (const char a_quiet)
{
   /*---(defense)------------------------*/
   if (getuid() != 0) {
      printf("FATAL : khronos can only be daemonized by root\n");
      exit (-1);
   }
   /*---(set noisiness)------------------*//*===fat=beg===*/
   CHATTY   if      (a_quiet == 0)  { my.quiet = 0; my.updates = 0; }
   CHATTY   else if (a_quiet == 1)  { my.quiet = 0; my.updates = 1; }
   CHATTY   else                    { my.quiet = 1; my.updates = 1; }
   /*---(begin)--------------------------*/
   CHATTY   my.logger = yLOG_begin("khronos", my.quiet);
   CHATTY   if (my.logger < 1) {
   CHATTY      printf("khronos : can not start logger, FATAL\n");
   CHATTY      exit(1);
   CHATTY   }
   CHATTY   yLOG_info  ("purpose",  "consistent, reliable time-based job scheduling");
   CHATTY   yLOG_info  ("ver_num",  VER_NUM);
   CHATTY   yLOG_info  ("ver_txt",  VER_TXT);
   CHATTY   yLOG_info  ("cli args", "none");
   /*---(init)---------------------------*//*===fat=end===*/
   CHATTY   yLOG_enter (__FUNCTION__);
   my.uid = getuid();
   CHATTY   yLOG_value ("uid num", my.uid);
   my.silent  = 'n';
   /*---(variables)----------------------*/
   nfile  = 0;
   nentry = 0;
   nfast  = 0;
   nproc  = 0;
   /*---(end)----------------------------*/
   CHATTY   yLOG_exit  (__FUNCTION__);
   /*---(complete)-----------------------*/
   return 0;
}

char       /*lf--: PURPOSE : exit on termintation/signal ---------------------*/
terminate          (const char *a_func, const int a_exit)
{
   /*---(log)----------------------------*//*===fat=beg===*/
   CHATTY   if (strncmp(a_func, "", 1) != 0) yLOG_exit  (a_func);
   CHATTY   yLOG_end   ();
   /*---(check for harsh exit)-----------*//*===fat=end===*/
   if (a_exit > 0) exit(a_exit);
   /*---(complete)-----------------------*/
   return 0;
}

void       /*----: PURPOSE : handle signals ----------------------------------*/
communicate        (int a_signal)
{
   /*---(catch)--------------------------*/
   switch (a_signal) {
   case  SIGHUP:
      CHATTY   yLOG_info  ("SIGNAL", "SIGHUP MEANS REFRESH CRONTABS BEFORE NEXT RUN");
      my.resync = '-';
      break;
   case  SIGTERM:
      CHATTY   yLOG_info  ("SIGNAL", "SIGTERM means terminate daemon");
      CHATTY   watch_end();
      terminate("EXITING", 99);
      break;
   case  SIGSEGV:
      CHATTY   yLOG_info  ("SIGNAL", "SIGSEGV means daemon blew up");
      CHATTY   watch_end();
      terminate("EXITING", 99);
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
   /*---(complete)-----------------------*/
   return 0;
}

char               /* PURPOSE : daemonize the program ------------------------*/
daemonize          (void)
{
   CHATTY   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   int       i    = 0;                       /* loop iterator                 */
   int       fd   = 0;                       /* file descriptor               */
   int      status  = 0;
   /*---(fork off and die)----------------------*/
   CHATTY   yLOG_info("foad", "fork off and die");
   my.pid = fork();
   if (my.pid < 0) {         /* error               */
      CHATTY   yLOG_info  ("fork",   "creation of child FAILED");
      terminate(__FUNCTION__, 1);
   }
   if (my.pid > 0) {         /* parent process      */
      CHATTY   yLOG_info  ("PARENT", "exiting parent");
      exit(0);
   }
   wait4(my.pid, &status, 0, NULL);
   /*---(fix the umask)-------------------------*/
   CHATTY   yLOG_info  ("umask",  "reset the default file permissions");
   umask(0);
   /*---(close off all descriptors)-------------*/
   CHATTY   yLOG_info  ("fds",    "close all inherited file descriptors");
   for (i = 0; i < 256; ++i) {
      CHATTY   if (i == my.logger) continue;
      close(i);
   }
   /*---(tie std fds to the bitbucket)----------*/
   CHATTY   yLOG_info  ("std fds",   "redirect stdin, stdout, and stderr to /dev/null");
   fd = open("/dev/null", O_RDWR);
   if (fd < 0) {
      CHATTY   yLOG_info  ("fds",    "creation of safe fd FAILED");
      terminate(__FUNCTION__, 2);
   }
   dup2(fd, 0);
   dup2(fd, 1);
   dup2(fd, 2);
   /*---(obtain a new process group)------------*/
   CHATTY   yLOG_info  ("session", "create a new process/session");
   my.sid = setsid();
   if (my.sid < 0) {
      CHATTY   yLOG_info  ("sid",    "creation FAILED");
      terminate(__FUNCTION__, 3);
   }
   /*---(check security on crontabs)------------*/
   struct    stat s;
   int       rc = 0;
   rc = lstat(CRONTABS, &s);
   if  (rc < 0) {
      CHATTY   yLOG_info  ("crondir", "directory does not exist");
      terminate(__FUNCTION__, 6);
   }
   if  (!S_ISDIR(s.st_mode))  {
      CHATTY   yLOG_info  ("crondir", "not a directory");
      terminate(__FUNCTION__, 6);
   }
   if  (s.st_uid  != 0)  {
      CHATTY   yLOG_info  ("crondir", "directory not owned by root");
      terminate(__FUNCTION__, 6);
   }
   if  (s.st_gid  != 0)  {
      CHATTY   yLOG_info  ("crondir", "directory group is not root");
      terminate(__FUNCTION__, 6);
   }
   if  ((s.st_mode & 00777) != 00700)  {
      CHATTY   yLOG_info  ("crondir", "directory permissions not 0700");
      terminate(__FUNCTION__, 6);
   }
   CHATTY   yLOG_info  ("crondir", "directory owned by root:root and 0700");
   /*---(change to safe location)---------------*/
   CHATTY   yLOG_info  ("location", "change current dir to a safe place");
   if (chdir(CRONTABS) < 0) {
      CHATTY   yLOG_info  ("cd",    "change directory FAILED");
      terminate(__FUNCTION__, 4);
   }
   /*---(record process info)-------------------*/
   my.pid  = getpid();
   my.ppid = getppid();
   CHATTY   yLOG_value ("new pid",  my.pid);
   CHATTY   yLOG_value ("new ppid", my.ppid);
   if (my.ppid != 1) {
      /*---(fork off and die)----------------------*/
      CHATTY   yLOG_info("foad", "fork off and die again");
      my.pid = fork();
      if (my.pid < 0) {         /* error               */
         CHATTY   yLOG_info  ("fork",   "creation of child FAILED");
         terminate(__FUNCTION__, 1);
      }
      if (my.pid > 0) {         /* parent process      */
         exit(0);
      }
      my.pid  = getpid();
      my.ppid = getppid();
      CHATTY   yLOG_value ("and pid",  my.pid);
      CHATTY   yLOG_value ("and ppid", my.ppid);
      if (my.ppid != 1) {
         CHATTY   yLOG_info  ("ppid",  "not owned by init, FAILED");
         terminate(__FUNCTION__, 5);
      }
   }
   CHATTY   yLOG_info  ("ppid",  "owned by init, success");
   /*---(signals)-------------------------------*/
   CHATTY   yLOG_info  ("signals",  "setup signal handlers");
   signals();
   /*---(run file)------------------------------*/
   CHATTY   yLOG_info  ("lockfile", "test for single crond instance");
   lock();
   /*---(complete)------------------------------*/
   CHATTY   yLOG_exit  (__FUNCTION__);
   return 0;
}

char        /* PURPOSE : ensure single threading                       */
lock          (void)
{
   int        rc = 0;
   struct flock lk;
   my.locker = open(LOCKFILE, O_RDWR | O_CREAT, 0644);
   if (my.locker < 0) {
      CHATTY   yLOG_info  ("FAILURE", "crond already running (can not open file)");
      terminate("FAILURE", 7);
   }
   lk.l_type   = F_WRLCK;           /* exclusive lock            */
   lk.l_whence = 0;                 /* zero offset               */
   lk.l_start  = 0L;                /* start at BOF              */
   lk.l_len    = 0L;                /* whole file                */
   rc = fcntl(my.locker, F_SETLK, &lk);    /* non-blocking              */
   if (rc != 0) {
      CHATTY   yLOG_info  ("FAILURE", "crond already running (can not lock file)");
      terminate("FAILURE", 8);
   }
   CHATTY   yLOG_info  ("lock",     "write final pid to /var/run/crond.pid");
   char _msg[50];
   snprintf(_msg, 50, "%d\n", my.pid);
   write(my.locker, _msg, strlen(_msg));
   fsync(my.locker);
   return 0;
}

char        /* PURPOSE : final preparation for run                     */
prepare       (void)
{
   CHATTY   yLOG_info("watcher", "update watcher to /var/log/yLOG/cronwatch");
   CHATTY   watch_beg();
   search('a');
   CHATTY   yLOG_value ("read in", nfile);
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
   CHATTY   FILE      *watcher;
   /*---(get the last stop time)-----------------*/
   CHATTY   watcher = fopen(WATCHER, "a");
   /*---(write the last end time)----------------*/
   CHATTY   timestamp();
   CHATTY   snprintf(my.pulse_end  , 50, "%s   end", my.pulse_time);
   CHATTY   if   (watcher != NULL) {
   CHATTY      fprintf(watcher, "%s\n", my.pulse_end);
   CHATTY      fclose(watcher);
   CHATTY   }
   CHATTY   else yLOG_warn  ("WATCHER", "cronwatch file can not be openned");
   /*---(complete)-------------------------------*/
   return 0;
}

char               /* PURPOSE : update cronwatch interrun monitoring file     */
watch_beg          (void)
{
   if (my.updates) return 0;
   /*---(locals)---------------------------------*/
   CHATTY   FILE      *watcher;
   CHATTY   int        len;
   CHATTY   char       trash[50];
   CHATTY   char       xtime[50];
   CHATTY   struct tm *curr_time;
   CHATTY   time_t     time_date;
   /*---(get the last stop time)-----------------*/
   /*> lastrun();                                                                     <*/
   CHATTY   watcher = fopen(WATCHER, "a");
   /*---(get a start time)--------------------*/
   CHATTY   timestamp();
   CHATTY   yLOG_info  ("this beg", my.pulse_time);
   /*---(get missed time)---------------------*/
   /*> sscanf(my.pulse_time , "%s %ld", trash, &my.this_start);                       <* 
    *> my.fast_beg = my.this_start;                                                   <* 
    *> yLOG_value ("missed secs", my.this_start - my.last_end);                       <*/
   /*---(write the time)-------------------------*/
   snprintf(my.pulse_begin, 50, "%s   BEGINNING", my.pulse_time);
   CHATTY   if (watcher != NULL) {
   CHATTY      fprintf(watcher, "%s\n", my.pulse_begin);
   CHATTY      fclose(watcher);
   CHATTY   }
   CHATTY   else yLOG_warn  ("WATCHER", "cronwatch file can not be openned");
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
   CHATTY   } else {
   CHATTY      yLOG_warn  ("WATCHER", "cronwatch file can not be openned");
   }
   /*---(save if off)----------------------------*/
   curr  = localtime(&my.last_end);
   strftime(my.pulse_time , 50, "%y.%m.%d.%H.%M.%S.%U   %s", curr);
   CHATTY   yLOG_info  ("last end", my.pulse_time);
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
            fprintf(status, "   %3d : %-20.20s : r=%3d, t=%10ld, a=%d, f=%3d, p=%6d : %s\n", line->recd, line->title, line->attempts, line->lasttime, line->active, line->failures, line->rpid, line->cmd);
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
   CHATTY   yLOG_enter (__FUNCTION__);
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
      CHATTY   yLOG_info("crondir", "FATAL, not found");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return 0;
   }
   /*---(report scope)--------------------------*/
   CHATTY   if (my.resync == 'a') yLOG_info("scope", "clear and load all files");
   CHATTY   else                  yLOG_info("scope", "look only for updated files");
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
            CHATTY   yLOG_info ("DEL",   x_file);
            x_file[len - 4] = '\0';
            strncpy(my.action, "DEL", 5);
            assimilate(x_file);
         }
         if (strncmp(x_file + len - 3, "NEW", 5) == 0) {
            CHATTY   yLOG_info ("NEW",   x_file);
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
   CHATTY   yLOG_value ("count",  count);
   /*---(delete)--------------------------------*/
   system("rm -f *.DEL");
   /*---(complete)------------------------------*/
   my.resync = 'n';
   CHATTY   yLOG_exit  (__FUNCTION__);
   /*---(complete)------------------------------*/
   return 0;
}

char        /* PURPOSE : validate a single crontab name and process it        */
assimilate    (cchar *a_name)
{
   CHATTY   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   char      rc   = 0;                            /* simple return code       */
   FILE     *f    = NULL;                         /* crontab file decriptor   */
   tCFILE   *curr = NULL;                         /* current crontab struct   */
   /*---(check file name)-----------------------*/
   rc      = name (a_name, '-');                  /* vaildate name            */
   if (rc <  0) {
      TEST  printf("   crontab is misnamed\n");
      CHATTY   yLOG_warn  ("name",    "file name is not valid");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -1;
   }
   TEST  printf("   name is good = <<%s>>\n", a_name);
   TEST  strcpy(my.user, my.who);
   /*---(check on crontab file)-----------------*/
   struct    stat s;
   rc = lstat(a_name, &s);
   if  (rc < 0) {
      TEST  printf("   crontab could not be found\n");
      CHATTY   yLOG_info  ("FAILED",  "crontab file does not exist");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -2;
   }
   TEST  printf("   crontab found\n");
   if  (!S_ISREG(s.st_mode))  {
      TEST  printf("   crontab is not a regular file\n");
      CHATTY   yLOG_info  ("FAILED",  "crontab is not a regular file");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -2;
   }
   TEST  printf("   crontab is a normal file\n");
   CHATTY   yLOG_info  ("file stat", "crontab is a normal file");
   /*---(elimintate exiting copy)---------------*/
   rc      = retire (my.name);
   if (strncmp(my.action, "DEL", 5) == 0) {
      CHATTY   yLOG_exit  (__FUNCTION__);
      return 0;
   }
   /*---(process new one)-----------------------*/
   f = fopen(a_name, "r");
   if (f == NULL) {
      TEST  printf("   crontab could not be openned\n");
      CHATTY   yLOG_info  ("FAILED",  "can not open the file");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -2;
   }
   TEST  printf("   crontab openned successfully\n");
   /*> TEST  return 0;                                                                <*/
   rc = create (my.name, my.user, &curr);
   if (rc >= 0) rc = inventory (curr, f);
   /*---(complete)------------------------------*/
   fclose(f);
   CHATTY   yLOG_exit  (__FUNCTION__);
   return rc;
}

char        /* PURPOSE : validate the crontab name                            */
name          (cchar *a_name, cchar a_loc)
{
   if (a_name == NULL) return -1;
   CHATTY   yLOG_enter (__FUNCTION__);
   CHATTY   yLOG_info  ("CRONTAB",   a_name);
   /*---(locals)--------------------------------*/
   char     *p;                                   /* pointer  for strtok      */
   int       len;                                 /* string's length          */
   int       i    = 0;                            /* loop iterator -- char    */
   char      legal[100] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
   char      x_name[NAME];        /* name of the current crontab               */
   /*---(defense)-------------------------------*/
   if (a_name[0] == '.') {
      CHATTY   yLOG_warn  ("name",  "can not process hidden crontab names");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -8;
   }
   len = strlen(a_name);
   if (len >= NAME) {
      CHATTY   yLOG_warn  ("name",  "crontab name too long to be processed");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -1;
   }
   strncpy(x_name, a_name, NAME);
   /*---(get user name)-------------------------*/
   p = strtok(x_name, ".");
   if (p == NULL) {
      CHATTY   yLOG_warn  ("user",  "crontab file does not have a user name");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -2;
   }
   len = strlen(p);
   if (len >= USER) {
      CHATTY   yLOG_warn  ("user",  "user name too long to be processed");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -3;
   }
   for (i = 0; i < len; ++i) {
      if (strchr(legal, p[i]) != NULL) continue;
      CHATTY   yLOG_warn  ("user",  "user name has illegal characters");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -4;
   }
   strncpy(my.user, p, USER);
   /*---(get crontab description)---------------*/
   p = strtok(NULL,   ".");
   if (p == NULL) {
      CHATTY   yLOG_warn  ("desc",  "crontab file does not have a description");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -5;
   }
   len = strlen(p);
   if (len >= DESC) {
      CHATTY   yLOG_warn  ("desc",  "decription too long to be processed");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -9;
   }
   for (i = 0; i < len; ++i) {
      if (strchr(legal, p[i]) != NULL) continue;
      CHATTY   yLOG_warn  ("desc",  "crontab description has illegal characters");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -6;
   }
   strncpy(my.desc, p, DESC);
   /*---(defense, improper format)--------------*/
   p = strtok(NULL,   ".");
   if (a_loc == 'c') {
      if (p != NULL && strncmp(p, "NEW", 4) != 0 && strncmp(p, "DEL", 4) != 0) {
         CHATTY   yLOG_info  ("name",  "crontab name is illegally suffixed");
         CHATTY   yLOG_exit  (__FUNCTION__);
         return -7;
      }
   } else {
      if (p != NULL) {
         CHATTY   yLOG_info  ("name",  "crontab name is illegally suffixed");
         CHATTY   yLOG_exit  (__FUNCTION__);
         return -8;
      }
   }
   /*---(complete)------------------------------*/
   strncpy(my.name, a_name, NAME);
   CHATTY   yLOG_info  ("success", "crontab name is legal");
   CHATTY   yLOG_exit  (__FUNCTION__);
   return 0;
}

char
retire        (cchar *a_name)
{
   CHATTY   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   tCFILE   *file;
   tCLINE   *line;
   tCFILE   *save;
   int       found = 0;
   /*---(process)-------------------------------*/
   for (file = cronhead; file != NULL; file = file->next) {
      CHATTY   yLOG_info ("compare", file->name);
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
            CHATTY   yLOG_bullet (line->recd, "running, but marking for deletion");
            line = line->next;
         }
         /*---(non-running line)----------------*/
         else {
            /*> printf("   inactive job, delete\n");                                  <*/
            CHATTY   yLOG_bullet (line->recd, "freeing now");
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
      CHATTY   yLOG_info  ("crontab",  "existing not found, continuing");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -1;
   }
   CHATTY   yLOG_info  ("crontab",  "found and retired existing crontab");
   /*---(complete)------------------------------*/
   CHATTY   yLOG_exit  (__FUNCTION__);
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
   CHATTY   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   tPASSWD  *pass;
   tCFILE   *file = NULL;
   /*---(create the space)----------------------*/
   file          = malloc(sizeof(tCFILE));
   CHATTY   yLOG_point ("malloc",     file);
   if (file == NULL) {
      TEST  printf("   malloc could not create space\n");
      CHATTY   yLOG_warn  ("cronfile",  "malloc could not create space");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -2;
   }
   TEST  printf("   file space malloc'd\n");
   /*---(base data)-----------------------------*/
   strncpy(file->name, a_name, NAME);
   strncpy(file->user, a_user, USER);
   CHATTY   yLOG_info ("user is", file->user);
   /*---(get uid)-------------------------------*/
   pass = getpwnam(a_user);                       /* get password entry       */
   if (pass == NULL) {
      TEST  printf("   user not found = <<%s>>\n", a_user);
      CHATTY   yLOG_warn  ("user",  "user name not a valid account on the system");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return -1;
   }
   file->uid       = pass->pw_uid;
   TEST  printf("   user uid = %d\n", file->uid);
   CHATTY   yLOG_value ("UID",   file->uid);
   /*---(initialize the rest)-------------------*/
   file->retire    = 'n';
   /*---(link into file dll)--------------------*/
   cronfile_add (file);
   TEST  printf("   added to file linked list\n");
   /*---(finalize)------------------------------*/
   (*a_curr) = file;
   /*---(complete)------------------------------*/
   TEST  printf("   done\n\n");
   CHATTY   yLOG_exit  (__FUNCTION__);
   return 0;
}

char
inventory     (tCFILE *a_cfile, FILE *a_source)
{
   CHATTY   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   char      buffer[LINE];
   char      backup[LINE];
   int       len   = 0;
   int       recds = 0;
   char     *xmin, *xhrs, *xday, *xmon, *xdow;
   char     *xtmp, *xcmd;
   int       rc;
   tCLINE   *cline;
   /*---(run input lines)-----------------------*/
   CHATTY   yLOG_point ("cfile",    a_cfile);
   while (1) {
      /*> printf("good to go ONE\n");                                                 <*/
      /*> printf("a_source = %p\n", a_source);                                        <*/
      /*---(get the line)-----------------------*/
      fgets(buffer, LINE - 5, a_source);
      /*> printf("good to go ONE POINT FIVE\n");                                      <*/
      if (feof(a_source))    break;
      /*> printf("good to go TWO\n");                                                 <*/
      len = strlen(buffer) - 1;
      buffer[len] = '\0';
      ++recds;
      /*---(defense)----------------------------*/
      if (buffer[0] == '#') {
         context (recds, buffer);
         continue;
      }
      /*> printf("good to go THREE\n");                                               <*/
      if (len       <  10 )  continue;
      /*> printf("good to go FOUR\n");                                                <*/
      /*---(read)-------------------------------*/
      TEST  printf("inventory line (%2d)...\n", recds);
      TEST  printf("   input recd : <<%-.65s>>\n", buffer);
      strncpy(backup, buffer, LINE);
      xmin = strtok(buffer, " ");
      xhrs = strtok(NULL  , " ");
      xday = strtok(NULL  , " ");
      xmon = strtok(NULL  , " ");
      xdow = strtok(NULL  , " ");
      xtmp = strtok(NULL  , " ");
      xcmd = backup + (xtmp - buffer);
      CHATTY   yLOG_break();
      /*---(create a line)----------------------*/
      cline = malloc(sizeof(tCLINE));
      if (cline == NULL) {
         TEST  printf("couldn't malloc\n\n");
         CHATTY   yLOG_info  ("malloc", "failed");
         CHATTY   yLOG_exit  (__FUNCTION__);
         return -1;
      }
      CHATTY   yLOG_info  ("malloc", "good to go");
      /*---(link into line dll)-----------------*/
      cronline_add (a_cfile, cline);
      /*---(fill in master data)----------------*/
      cline->recd         = recds;
      if (recds == my.for_line + 1) {
         strncpy(cline->title, my.title, 20);
         cline->duration = my.duration;
         cline->recovery = my.recovery;
         cline->priority = my.priority;
         cline->alerting = my.alerting;
         CHATTY   yLOG_delim ("title",    cline->title);
         CHATTY   yLOG_char  ("duration", cline->duration);
         CHATTY   yLOG_char  ("recovery", cline->recovery);
         CHATTY   yLOG_char  ("priority", cline->priority);
         CHATTY   yLOG_char  ("alerting", cline->alerting);
      }
      /*---(parse)------------------------------*/
      rc = 0;
      if (rc == 0)  rc = parse (xmin, cline->min, 59, 0, "minutes");
      if (rc == 0)  rc = parse (xhrs, cline->hrs, 23, 0, "hours");
      if (rc == 0)  rc = parse (xday, cline->day, 31, 0, "days");
      if (rc == 0)  rc = parse (xmon, cline->mon, 12, 0, "months");
      if (rc == 0)  rc = parse (xdow, cline->dow,  6, 0, "weekdays");
      if (rc != 0) {
         TEST  printf("couldn't parse\n\n");
         CHATTY   yLOG_error ("parsing" , "line failed and is deleted");
         free(cline);
         continue;
      }
      /*---(final data)-------------------------*/
      CHATTY   yLOG_info ("cmd",  xcmd);
      strncpy(cline->cmd, xcmd, CMD);
      TEST  printf("   command    : <<%-.65s>>\n", xcmd);
      TEST  printf("   GOOD TO GO\n\n");
   }
   /*---(complete)------------------------------*/
   CHATTY   yLOG_exit  (__FUNCTION__);
   return  0;
}

char               /* PURPOSE : read job context lines for additional data ---*/
context            (int a_recd, cchar *a_data)
{
   /*---(design notes)-------------------*/
   /*
    *   this information makes crond more resilent, recoverable, and
    *   informative while maintaining backwards compatibility with
    *   previous crons as the additions would appear as comments to them.
    *
    *   the additional fields are...
    *      - name      : summary text to give the operator a description
    *      - duration  : guidance on how long is the job expected to take
    *      - recovery  : what to do if execution falls behind
    *      - priority  : relative importance of the job
    *      - alerting  : how to notify the operator of the failure/issue
    */
   /*---(locals)--------------------------------*/
   int       len      = 0;
   int       i        = 0;                   /* loop iterator                 */
   char      legal[100] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";
   char     *p;
   /*---(basic formatting)----------------------*/
   if (a_data[0]   != '#')            return 0;
   len = strlen(a_data);
   if (len !=  80)                    return 0;
   /*---(formatting clues)----------------------*/
   if (a_data[47]  != '[')            return 0;
   if (a_data[68]  != ']')            return 0;
   if (a_data[69]  != '[')            return 0;
   if (a_data[79]  != ']')            return 0;
   /*---(parse)---------------------------------*/
   my.for_line = a_recd;
   my.duration = '-';
   my.recovery = '-';
   my.priority = '-';
   my.alerting = '-';
   if (strchr("-tsmLX#" , a_data[71]) != NULL)     my.duration = a_data[71];
   if (strchr("-x1B"    , a_data[73]) != NULL)     my.recovery = a_data[73];
   if (strchr("-"       , a_data[75]) != NULL)     my.priority = a_data[75];
   if (strchr("-"       , a_data[77]) != NULL)     my.alerting = a_data[75];
   strncpy(my.title, a_data + 48, 21);
   my.title[20] = '\0';
   for (i = 19; i >= 0; --i) {
      if ((p = strchr(legal, my.title[i])) != NULL) break;
      my.title[i] = '\0';
   }
   /*---(complete)------------------------------*/
   return 0;
}

char
parse         (char *a_input, char *a_array, int a_max, int a_off, char *a_name)
{
   int       i    = 0;                       /* loop iterator                 */
   char     *list  = NULL;
   char     *step  = NULL;
   char     *range = NULL;
   char     *beg   = NULL;
   char     *end   = NULL;
   char     *weird = NULL;
   char     *s1    = NULL;
   char     *s2    = NULL;
   char     *s3    = NULL;
   int       step_inc = 0;
   int       beg_val  = 0;
   int       end_val  = 0;
   int       len      = 0;
   int       rc       = 0;
   for (i = 0; i <= a_max; ++i) {
      a_array[i]   = 0;
      my.parsed[i] = '_';
   }
   list  = strtok_r(a_input, ",", &s1);
   while (list != NULL) {
      /*> yLOG_info("list",    list);                                                 <*/
      range = strtok_r(list,  "/", &s2);
      /*> yLOG_info("range",   range);                                                <*/
      /*---(handle step)------------------------*/
      step_inc = 1;
      step     = strtok_r(NULL,  "/", &s2);
      /*> yLOG_info("step",     step);                                                <*/
      if (step != NULL) {
         rc = convert("step", step, 1, a_max);
         if (rc >= 1) step_inc = rc;
         else         return -10 + rc;
      }
      /*---(handle range)-----------------------*/
      if (range[0] == '-')  return -24;
      beg   = strtok_r(range, "-", &s3);
      /*> yLOG_info("beg",      beg);                                                 <*/
      len   = strlen(a_input);
      if (len == 1 && beg[0] == '*') {
         beg_val = a_off;
         end_val = a_max;
      } else {
         rc = convert("beg", beg, a_off, a_max);
         if (rc >= 0) {
            beg_val = rc;
            end_val = rc;
         } else         return -20 + rc;
         end   = strtok_r(NULL,  "-", &s3);
         /*> yLOG_info("end",      end);                                              <*/
         if (end != NULL) {
            rc = convert("end", end, beg_val, a_max);
            if (rc >= 0) end_val = rc;
            else         return -30 + rc;
         }
      }
      weird    = strtok_r(NULL,  "/", &s2);
      if (weird != NULL)  return -10;
      weird    = strtok_r(NULL,  "-", &s3);
      if (weird != NULL)  return -20;
      if (step_inc > 1 && (step_inc > (end_val - beg_val))) return -15;
      /*---(update)-----------------------------*/
      for (i = beg_val; i <= end_val; i += step_inc) {
         a_array[i] = 1;
      }
      /*---(next in list)-----------------------*/
      list  = strtok_r(NULL,  ",", &s1);
   }
   /*---(display)-------------------------------*/
   for (i = a_off; i <= a_max; ++i) {
      if (a_array[i] == 1) my.parsed[i] = '1';
      else                 my.parsed[i] = '_';
   }
   my.parsed[a_max + 1] = '\0';
   CHATTY   yLOG_info (a_name,   my.parsed);
   TEST  printf ("   %-10.10s : %-60.60s\n", a_name, my.parsed);
   /*---(complete)------------------------------*/
   return  0;
}

int
convert       (const char *a_field, const char *a_input, const int a_min, const int a_max)
{
   int       i        = 0;
   int       len      = 0;
   int       value    = 0;
   if (a_input == NULL) return -1;
   len   = strlen(a_input);
   if (len > 2) {
      TEST  printf("   %s (%s) were tool long (>2 digits)\n", a_field, a_input);
      ++failed;
      CHATTY   yLOG_info ("focus",    a_field);
      CHATTY   yLOG_info ("issue",    "value too long");
      CHATTY   yLOG_info ("input",    a_input);
      CHATTY   yLOG_info ("FAILED",   "whole line was rejected");
      return -2;
   }
   for (i = 0; i < len; ++i) {
      if (!isdigit(a_input[i])) {
         TEST  printf("   %s (%s) contains non numeric characters\n", a_field, a_input);
         ++failed;
         CHATTY   yLOG_info ("focus",    a_field);
         CHATTY   yLOG_info ("issue",    "value contains non-numeric values");
         CHATTY   yLOG_info ("input",    a_input);
         CHATTY   yLOG_info ("FAILED",   "whole line was rejected");
         return -3;
      }
   }
   value = atoi(a_input);
   if (value < a_min) {
      TEST  printf("   %s (%s) is less than min of %d\n", a_field, a_input, a_min);
      ++failed;
      CHATTY   yLOG_info ("focus",    a_field);
      CHATTY   yLOG_info ("issue",    "value is less than 1");
      CHATTY   yLOG_info ("input",    a_input);
      CHATTY   yLOG_value("value",    value);
      CHATTY   yLOG_info ("FAILED",   "whole line was rejected");
      return -4;
   }
   if (value > a_max) {
      TEST  printf("   %s (%s) is greater than max of %d\n", a_field, a_input, a_max);
      ++failed;
      CHATTY   yLOG_info ("focus",    a_field);
      CHATTY   yLOG_info ("issue",    "value is tool large");
      CHATTY   yLOG_info ("input",    a_input);
      CHATTY   yLOG_value("value",    value);
      CHATTY   yLOG_info ("FAILED",   "whole line was rejected");
      return -5;
   }
   return value;
}



/*====================------------------------------------====================*/
/*===----                         job handling                         ----===*/
/*====================------------------------------------====================*/
static void      o___EXECUTION_______________o (void) {;};

char        /* PURPOSE : create list of jobs to run in a particular period    */
fast          (clong a_start)                /* time_t of starting hour       */
{
   if (my.silent == 'n') {
      CHATTY   yLOG_enter (__FUNCTION__);
      CHATTY   yLOG_note  ("determine jobs that will run in the next hour");
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
   /*---(format the time)------------------------*/
   strftime(msg, 50, "---- %Hh %dd %mm  %ww", curr_time);
   CHATTY   yLOG_info ("fast for", msg);
   /*---(work out the current time)----------*/
   chrs = curr_time->tm_hour;
   cday = curr_time->tm_mday;
   cmon = curr_time->tm_mon;
   cdow = curr_time->tm_wday;
   /*---(prepare)-------------------------------*/
   nfast     = 0;
   fasthead  = NULL;
   fasttail  = NULL;
   /*---(scan every file)--------------------*/
   for (x_file = cronhead; x_file != NULL; x_file = x_file->next) {
      /*---(scan every line)-----------------*/
      for (x_line = x_file->head; x_line != NULL; x_line = x_line->next) {
         /*---(initialize)----------------------*/
         x_line->active  = 0;
         x_line->fnext   = NULL;
         x_line->fprev   = NULL;
         /*---(filter for active)---------------*/
         if (x_file->retire  == 'y')     continue;
         if (x_line->deleted == 'y')     continue;
         /*---(filter for time)-----------------*/
         if (x_line->hrs[chrs]  == 0)                               continue;
         if (x_line->mon[cmon]  == 0)                               continue;
         if (x_line->day[cday]  == 0 && x_line->dow[cdow]  == 0  )  continue;
         /*---(output)--------------------------*/
         snprintf(msg, 100, "%3d) %2dh %2dd %2dm %2dw %.30s (%d)", nfast,
               x_line->hrs[chrs], x_line->day[cday], x_line->mon[cmon],
               x_line->dow[cdow], x_file->name,      x_line->recd);
         CHATTY   yLOG_info  ("add",     msg);
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
   CHATTY   if (my.silent  == 'n') {
      CHATTY   yLOG_value ("count", nfast);
      CHATTY   yLOG_exit  (__FUNCTION__);
   CHATTY   }
   /*---(complete)------------------------------*/
   return 0;
}

char        /* PURPOSE : run through the fast list to find what should run    */
dispatch      (cint a_min)
{
   CHATTY   if (my.silent == 'n') yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   int       njobs = 0;                      /* number of jobs ready to run   */
   int       ntest = 0;                      /* number of jobs ready to run   */
   tCLINE   *x_line;                         /* current line                  */
   char      msg[200];
   /*---(process all time periods)--------------*/
   CHATTY   if (my.silent == 'n') yLOG_value ("potential",  nfast);
   for (x_line = fasthead; x_line != NULL; x_line = x_line->fnext) {
      if (my.silent == 'y') {
         switch (x_line->recovery) {
         case '-' : continue;  break;
         case 'x' : continue;  break;
         }
      }
      ++ntest;
      snprintf(msg, 200, "%-16.16s,%3d", x_line->file->name, x_line->recd);
      if (x_line->min[a_min]  == 0) {
         if (my.silent == 'n') {
            CHATTY   yLOG_senter ("not");
            CHATTY   yLOG_snote(msg);
            CHATTY   yLOG_snote  ("not sched for min");
            CHATTY   yLOG_sexit  ();
         }
         continue;
      }
      if (x_line->rpid        != 0) {
         if (my.silent == 'n') {
            CHATTY   yLOG_senter ("not");
            CHATTY   yLOG_snote(msg);
            CHATTY   yLOG_snote  ("already running");
            CHATTY   yLOG_sexit  ();
         }
         continue;
      }
      run (x_line->file, x_line);
      ++njobs;
   }
   if (my.silent == 'n') {
      CHATTY   yLOG_value ("tested", ntest);
      CHATTY   yLOG_value ("ran",    njobs);
      CHATTY   yLOG_exit  (__FUNCTION__);
   }
   /*---(complete)------------------------------*/
   return 0;
}

char
run        (tCFILE *a_file, tCLINE *a_line)
{
   CHATTY   yLOG_senter ("RUN");
   /*---(locals)--------------------------------*/
   int       rc        = 0;                       /* simple return code       */
   int       rpid      = 0;                       /* child pid for execution  */
   tPASSWD  *pass;
   FILE     *output = NULL;
   long      now;                                 /* present datetime         */
   tTIME    *curr_time = NULL;
   char      msg[200];
   char      envp[10][200];
   /*---(prepare)-------------------------------*/
   a_line->rpid   = 0;
   /*---(fork off to execute)-------------------*/
   snprintf(msg, 200, "%-16.16s,%3d", a_line->file->name, a_line->recd);
   CHATTY   yLOG_snote(msg);
   CHATTY   yLOG_snote("fork");
   rpid = fork();
   if (rpid < 0) {                        /* error                            */
      CHATTY   yLOG_snote ("FAILURE");
      CHATTY   yLOG_sexit ();
      return -1;
   }
   if (rpid > 0) {
      CHATTY   yLOG_svalue ("pid", rpid);
      /*---(link into proc dll)-----------------*/
      a_line->rpid       = rpid;
      a_line->lasttime = time(NULL);
      ++a_line->attempts;
      proclist_add (a_line);
      /*---(log and exit)-----------------------*/
      CHATTY   yLOG_snote  ("SUCCESS");
      CHATTY   yLOG_sexit  ();
      return 0;              /* parent moves on to next task     */
   }
   output = fopen(STUFF, "a");
   /*---(display header for debugging)----------*/
   fprintf(output, "=== crond : the heatherly cron system ==================================begin===\n");
   /*---(get the date)-----------------------*/
   now = time(NULL);
   curr_time = localtime(&now);
   strftime(msg, 50, "%Ss %Mm %Hh %dd %mm  %ww", curr_time);
   fprintf(output, "start     : %s\n",   msg);
   /*---(get user information)------------------*/
   pass = getpwnam(a_file->user);
   if (pass == NULL) {
      exit (-2);
   }
   /*---(set execution environment)-------------*/
   rc = setenv("USER",                 pass->pw_name, 1);
   snprintf(envp[0], 200, "USER=%s",   pass->pw_name);
   fprintf(output, "USER set  : %s\n", pass->pw_name);
   if (rc <  0) {
      exit (-3);
   }
   rc = setenv("HOME",                 pass->pw_dir,  1);
   snprintf(envp[1], 200, "HOME=%s",   pass->pw_dir);
   fprintf(output, "HOME set  : %s\n", pass->pw_dir);
   if (rc <  0) {
      exit (-3);
   }
   rc = setenv("SHELL",                SHELL, 1);
   snprintf(envp[2], 200, "SHELL=%s",  SHELL);
   fprintf(output, "SHELL set : %s\n", SHELL);
   if (rc <  0) {
      exit (-3);
   }
   rc = setenv("PATH",                 PATH, 1);
   snprintf(envp[3], 200, "PATH=%s",   PATH);
   fprintf(output, "PATH set  : %s\n", PATH);
   if (rc <  0) {
      exit (-3);
   }
   /*---(set permossions)-----------------------*/
   rc = initgroups(a_file->user, pass->pw_gid);
   if (rc <  0) {
      exit (-3);
   }
   rc = setregid(pass->pw_gid,         pass->pw_gid);
   snprintf(envp[4], 200, "GID=%d",    pass->pw_gid);
   fprintf(output, "GID set   : %d\n", pass->pw_gid);
   if (rc <  0) {
      return (-3);
   }
   rc = setreuid(pass->pw_uid, pass->pw_uid);
   snprintf(envp[5], 200, "UID=%d",    pass->pw_uid);
   fprintf(output, "UID set   : %d\n", pass->pw_uid);
   if (rc <  0) {
      return (-3);
   }
   /*---(set current dir)-----------------------*/
   rc = chdir(pass->pw_dir);
   fprintf(output, "chdir     : %s\n", pass->pw_dir);
   if (rc <  0) {
      return (-3);
   }
   /*---(try direct execution)------------------*/
   fprintf(output, "execvp    : %.50s\n", a_line->cmd);
   fprintf(output, "==========================================================================end===\n");
   fflush (output);
   fclose (output);
   char    backup2[CMD];
   strncpy(backup2, a_line->cmd, CMD);
   str_parse (backup2);
   rc = execvp(*args, args);
   /*---(close log)-----------------------------*/
   envp[6][0] = '\0';
   output = fopen(STUFF, "a");
   fprintf(output, "FAILED execvp, fallback...\n");
   fprintf(output, "execl     : %.50s\n", a_line->cmd);
   fprintf(output, "==========================================================================end===\n");
   fflush (output);
   fclose (output);
   rc = execl(SHELL, SHELL, "-c", a_line->cmd, NULL, NULL);
   /*---(this should never come back)-----------*/
   output = fopen(STUFF, "a");
   fprintf(output, "FAILED execl, just won't run\n");
   fprintf(output, "==========================================================================end===\n");
   fflush (output);
   fclose (output);
   _exit (-3);    /* must use _exit to get out properly                       */
}

char        /* PURPOSE : verify status of running jobs and mark completions   */
check         (void)
{
   CHATTY   yLOG_enter (__FUNCTION__);
   /*---(defense)-------------------------------*/
   if (nproc == 0) {
      CHATTY   yLOG_note  ("no currently running jobs");
      CHATTY   yLOG_exit  (__FUNCTION__);
      return 0;
   }
   /*---(locals)--------------------------------*/
   int       rc        = 0;                       /* return code              */
   tCLINE   *x_line    = NULL;                    /* current line             */
   tCFILE   *x_file    = NULL;                    /* the line's file          */
   tCLINE   *x_pnext   = NULL;                    /* the next line to process */
   int       x_status  = 0;                       /* the line's job status    */
   char      msg[200];
   /*---(prepare)-------------------------------*/
   CHATTY   yLOG_value ("before", nproc);
   x_line    = prochead;
   /*---(loop)----------------------------------*/
   while (x_line != NULL) {
      /*---(save the next)----------------------*/
      x_pnext   = x_line->pnext;
      x_file    = x_line->file;
      /*---(output header)----------------------*/
      CHATTY   yLOG_senter("chk");
      snprintf(msg, 200, "%-15.15s,%3d", x_line->file->name, x_line->recd);
      CHATTY   yLOG_snote (msg);
      CHATTY   yLOG_sint  (x_line->rpid);
      /*---(check status)-----------------------*/
      rc = wait4(x_line->rpid, &x_status, WNOHANG, NULL);
      /*---(handle running line)----------------*/
      if (rc ==  0) {
         CHATTY   yLOG_snote ("still running");
      }
      /*---(handle completed line)--------------*/
      else {
         x_line->lastexit = -1;
         /*---(log status code)-----------------*/
         if (WIFEXITED(x_status)) {
            x_line->lastexit = WEXITSTATUS(x_status);
            CHATTY   yLOG_snote ("exited");
            if      (WEXITSTATUS(x_status) == 0) {
               CHATTY   yLOG_snote ("normal");
            } else if (WEXITSTATUS(x_status) >  0) {
               CHATTY   yLOG_snote ("positive");
            } else {
               ++x_line->failures;
               CHATTY   yLOG_snote ("FAILURE");
            }
         } else {
            ++x_line->failures;
            CHATTY   yLOG_snote ("TERMINATED");
         }
         /*---(clear from proc list)------------*/
         CHATTY   yLOG_snote ("clearing");
         proclist_del (x_line);
         /*---(clear from proc list)------------*/
         if (x_line->deleted == 'y') {
            CHATTY   yLOG_snote ("deleted");
            cronline_del (x_line);
            if (x_file->nlines == 0) cronfile_del (x_file);
         }
      }
      CHATTY   yLOG_sexit();
      x_line = x_pnext;
      if (x_line == NULL)    break;
   }
   CHATTY   yLOG_value ("after", nproc);
   CHATTY   yLOG_exit  (__FUNCTION__);
   return 0;
}


/*=============================[[ end-of-code ]]==============================*/
