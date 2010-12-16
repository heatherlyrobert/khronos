/*====================--------[[ start-of-code ]]---------====================*/

/*===[[ SUMMARY ]]============================================================#

 *   application   : khronos
 *   module        : khronos_main
 *   size          : less than 100 slocL
 *
 */
/*===[[ SUMMARY ]]============================================================#

 *   khronos_main is the entry point for khronos and is compilied separately
 *   so that unit testing have be separately linked
 *
 */
/*============================================================================*/


#include  "khronos.h"

#define   MIN         60


/*====================------------------------------------====================*/
/*===----                            driver                            ----===*/
/*====================------------------------------------====================*/

char        /* PURPOSE : reliably wait until the next crond trigger moment    */
wait_minute   (void)
{
   /* this function is impemented to perform even when signals interrupt      */
   /*---(locals)--------------------------------*/
   long      now       = 0;                       /* present time             */
   long      inc       = 0;                       /* seconds to sleep         */
   long      targ      = 0;                       /* time to reach            */
   /*---(process)-------------------------------*/
   now       = time(NULL);
   while (1) {
      inc   = MIN - (now % MIN) + 1;
      if (inc > MIN)         break;               /* solve SIGHUP after sleep */
      targ  = now + inc;
      CHATTY   if (my.silent == 'n') yLOG_value ("sleep (s)", inc);
      sleep(inc);
      now       = time(NULL);
      if (now >= targ)       break;               /* solve SIGHUP during sleep*/
   }
   /*---(complete)------------------------------*/
   return 0;
}

long        /* PURPOSE : return the time at the beginning of the current hour */
curr_hours    (void)
{
   long      now       = time(NULL);
   long      curr      = now - (now % 3600);
   return curr;
}

char
catchup       (void)
{
   CHATTY   yLOG_enter (__FUNCTION__);
   /*---(locals)--------------------------------*/
   long      curr      = 0;                       /* curr hour                */
   int       min       = 0;                       /* curr minute              */
   /*---(init)----------------------------------*/
   my.silent = 'y';
   /*---(main loop)-----------------------------*/
   CHATTY   yLOG_note ("catchup specially flagged missed jobs");
   min       = ((my.last_end / 60) % 60) + 1;     /* start right after last   */
   CHATTY   yLOG_value ("first min", min);
   /*> curr      = my.last_end - (my.last_end % (60 * 60));                           <*/
   curr      = my.last_end;
   while (curr < my.this_start) {
      fast (curr);                                /* id jobs for this hour    */
      /*---(cycle minutes)----------------------*/
      while (min < 60 && curr <  my.this_start) {
         /*> yLOG_value ("minute", min);                                              <*/
         dispatch(min);
         min      += 1;
         curr     += 60;
      }
      if (curr >=  my.this_start) break;
      min       = 0;
   }
   CHATTY   yLOG_value ("last min", min - 1);
   /*---(reset)---------------------------------*/
   my.silent = 'n';
   /*---(complete)------------------------------*/
   CHATTY   yLOG_exit  (__FUNCTION__);
   return 0;
}

int        /* PURPOSE : main driver ------------------------------------------*/
main (int argc, char *argv[])
{
   /*---(locals)--------------------------------*/
   long      save      = 0;                       /* last hour ran            */
   long      curr      = 0;                       /* curr hour                */
   int       min       = 0;                       /* curr minute              */
   int       rc        = 0;
   /*---(init)----------------------------------*/
   rc = prog_urgent (argc, argv);
   rc = prog_whoami ();
   rc = prog_args   (argc, argv);
   if (rc == 1)  return 0;
   rc = initialize(0);
   rc = daemonize();
   CHATTY   yLOG_break();
   rc = prepare();
   /*---(catchup)-------------------------------*/
   CHATTY   yLOG_break();
   catchup();
   /*---(main loop)-----------------------------*/
   CHATTY   yLOG_break();
   CHATTY   yLOG_enter("main_loop");
   /*---(get the date)--------------------------*/
   min       = timestamp();
   wait_minute();
   curr      = curr_hours();
   while (1) {
      CHATTY   yLOG_break();
      CHATTY   yLOG_sync ();
      CHATTY   yLOG_note ("hourly break -- check crontabs and reset fast list");
      search('n');                                /* force update once an hour*/
      save      = curr;
      fast (curr);                                /* id jobs for this hour    */
      /*---(cycle minutes)----------------------*/
      while (curr == save) {
         CHATTY   yLOG_break();
         min       = timestamp();
         CHATTY   yLOG_value ("minute", min);
         search('?');                             /* update only if SIGHUP    */
         check();
         dispatch(min);
         status();
         wait_minute();
         curr      = curr_hours();
      }
   }
   CHATTY   yLOG_exit("main_loop");
   CHATTY   yLOG_break();
   /*---(complete)------------------------------*/
   terminate("", 0);
   return 0;
}

char       /* PURPOSE : process the urgent command line arguments ------------*/
prog_urgent        (int argc, char *argv[])
{
   /*---(locals)-------------------------*//*===fat=beg===*/
   char     *a         = NULL;         /* current argument                    */
   int       i         = 0;            /* loop iterator -- arguments          */
   int       len       = 0;            /* argument length                     */
   /*---(process)------------------------*/
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      if      (strcmp(a, "@a"        ) == 0)    debug_top   = debug_args  = 'y';
      else if (strcmp(a, "@p"        ) == 0)    debug_top   = debug_proc  = 'y';
   }
   DEBUG_T {
      printf("\n");
      printf("khronos -- heatherly time-based job scheduling (crond/crontab)\n\n");
      printf("prog_urgent()...\n");
      printf("   args   = %c\n", debug_args);
      printf("   proc   = %c\n", debug_proc);
      printf("\n");
   }
   /*---(complete)-----------------------*//*===fat=end===*/
   return 0;
}

char       /* PURPOSE : figure out who launched us and how -------------------*/
prog_whoami        (void)
{
   /*---(locals)-------------------------*/
   tPASSWD  *x_pass    = NULL;         /* passwd data structure               */
   int       x_len     = 0;            /* user name length                    */
   int       rc        = 0;            /* generic return code                 */
   /*---(begin)--------------------------*/
   DEBUG_A  printf("prog_whoami()...\n");
   /*---(get real uid)-------------------*/
   my.uid    = getuid();
   DEBUG_A  printf("   caller uid   = %d\n",    my.uid);
   /*---(pull user name)-----------------*/
   x_pass    = getpwuid(my.uid);
   if (x_pass == NULL) {
      printf("can not retreive your user information from the system\n");
      prog_term();
   }
   DEBUG_A  printf("   caller name  = <<%s>>\n",    x_pass->pw_name);
   /*---(check user name)----------------*/
   x_len      = strlen(x_pass->pw_name);
   if (x_len < 1) {
      printf("your user name can not be empty\n");
      prog_term();
   }
   if (x_len > 20) {
      printf("user name is too long\n");
      prog_term();
   }
   strncpy(my.who, x_pass->pw_name, 20);
   /*---(check for root user)--------------*/
   my.am_root = 'n';
   if (my.uid == 0)   my.am_root = 'y';
   DEBUG_A  printf("   am i root    = %c\n",    my.am_root);
   /*---(change uid/permissions)-----------*/
   if (my.am_root != 'y') {
      rc = setuid(0);
      if (rc != 0) {
         printf("could not gain root authority\n");
         prog_term();
      }
      DEBUG_A  printf("   successfully changed to root\n");
   }
   /*---(log pid info)---------------------*/
   my.pid  = getpid();
   DEBUG_A  printf("   current pid  = %d\n",    my.pid);
   my.ppid = getppid();
   DEBUG_A  printf("   current ppid = %d\n",    my.ppid);
   /*---(complete)-----------------------*/
   DEBUG_A  printf("\n");
   return 0;
}

#define    TWOARG      if (two_arg == 1)

char       /* PURPOSE : process the command line arguments -------------------*/
prog_args          (int argc, char *argv[])
{
   /*---(locals)-------------------------*/
   char     *a         = NULL;         /* current argument                    */
   int       i         = 0;            /* loop iterator -- arguments          */
   int       len       = 0;            /* argument length                     */
   char      two_arg   = 0;
   /*---(begin)--------------------------*/
   DEBUG_A  printf("prog_args()...\n");
   DEBUG_A  printf("   processing %d args\n", argc);
   if (argc == 1) {
      DEBUG_A  printf("   calling without args invokes the daemon\n", argc);
      return 0;
   }
   /*---(no logger)-----------------------------*/
   DEBUG_A  printf("   start a null logger\n");
   CHATTY   my.logger = yLOG_begin("khronos", 2);
   /*---(program name)--------------------------*/
   strncpy(my.prog, argv[0], USER);
   DEBUG_A  printf("   prog name = <<%s>>\n\n", my.prog);
   /*---(process)------------------------*/
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      if (i < argc - 1) two_arg = 1; else two_arg = 0;
      len = strlen(a);
      /*---(skip debugging)--------------*/
      if      (a[0] == '@')                     continue;
      /*---(usage/help)------------------*/
      else if (strcmp(a, "-h"        ) == 0)    prog_usage();
      else if (strcmp(a, "--help"    ) == 0)    prog_usage();
      /*---(lists)-----------------------*/
      else if (strcmp(a, "-l"        ) == 0)    crontab_proc  (my.who, 'c');
      else if (strcmp(a, "--list"    ) == 0)    crontab_proc  (my.who, 'l');
      else if (strcmp(a, "--all"     ) == 0)    crontab_proc  ("ALL" , 'l');
      else if (strcmp(a, "--here"    ) == 0)    crontab_local ('l');
      /*---(installing)------------------*/
      else if (a[0] != '-'           )          crontab_inst  (a);
      else if (strcmp(a, "--test"    ) == 0)  { TWOARG  crontab_test  (argv[++i]); }
      else if (strcmp(a, "--reload"  ) == 0)    crontab_local ('i');
      /*---(removing)--------------------*/
      else if (strcmp(a, "-d"        ) == 0)  { TWOARG  crontab_del   (argv[++i]); }
      else if (strcmp(a, "-r"        ) == 0)  { TWOARG  crontab_del   (argv[++i]); }
      else if (strcmp(a, "--purge"   ) == 0)    crontab_proc  (my.who, 'p');
      else if (strcmp(a, "--cleanse" ) == 0)    crontab_proc  ("ALL" , 'p');
      /*---(user switches)---------------*/
      else if (strcmp(a, "-u"        ) == 0)  { TWOARG  crontab_user  (argv[++i]); }
      else if (strcmp(a, "--user"    ) == 0)  { TWOARG  crontab_user  (argv[++i]); }
      /*---(warnings)--------------------*/
      else if (strcmp(a, "-c"        ) == 0)    crontab_dir   ();
      else if (strcmp(a, "-e"        ) == 0)    crontab_edit  ();
      else if (strcmp(a, "-"         ) == 0)    crontab_stdin ();
      /*---(unknown)---------------------*/
      else    printf("requested action not understood or incomplete\n");
   }
   /*> if      (argc == 1)                                   crontab_stdin ();                          <* 
    *> else if (argc >= 2 && argv[1][0] != '-')              crontab_inst  (argv[1]);                   <* 
    *> else if (argc >= 2 && strncmp(a, "--system",10) == 0) printf("feature not implemented yet\n");   <* 
    *> else if (argc >= 2 && strncmp(a, "--test",  10) == 0) printf("feature not implemented yet\n");   <* 
    *> else    printf("requested action not understood or incomplete\n");                               <*/
   /*---(complete)-----------------------*/
   return 1;
}

char       /* PURPOSE : display usage help information -----------------------*/
prog_usage         (void)
{
   printf("\n");
   printf("usage: khronos [OPTION] [<FILE>]\n");
   printf("\n");
   printf("khronos is a fast, simplified, modernized, and technical version of the\n");
   printf("classic posix-defined crond time-based process scheduler which combines\n");
   printf("crond and crontab to allow deeper verification, verbosity, and traceability\n");
   printf("\n");
   printf("posix/standard crontab options implemented:\n");
   printf("   <file>           install/replace a crontab file\n");
   printf("   -l               list all your installed crontabs to stdout\n");
   printf("   -d <desc>        delete one of your crontabs by name\n");
   printf("   -r <desc>        delete one of your crontabs by name\n");
   printf("   -u <user>        act as another user (root only)\n");
   printf("\n");
   printf("posix/standard crontab options rejected:\n");
   printf("   - or null        install from stdin (no traceability, over-write risk)\n");
   printf("   -e               edit your crontab (no traceability or backup)\n");
   printf("   -c <dir>         change the crontab dir (no way, security risk)\n");
   printf("\n");
   printf("extended crontab options:\n");
   printf("   --all            list all installed crontabs (root only)\n");
   printf("   --list           list all your installed crontabs\n");
   printf("   --here           list all local crontabs that could be installed\n");
   printf("   --test <desc>    test a crontab file for formatting correctness\n");
   printf("   --system <desc>  install system crontab (root only)\n");
   printf("   --purge          delete all your installed crontabs\n");
   printf("   --cleanse        delete all installed crontabs (root only)\n");
   printf("   --reload         delete all then install your crontabs\n");
   printf("   --help, -h       print usage information\n");
   printf("\n");
   printf("extended crond options rejected:\n");
   printf("   -n               foreground operation (using logging instead)\n");
   printf("   -x <option>      debugging options (using @ urgents instead)\n");
   printf("   -p               allows weaker security on crontabs (no, no)\n");
   printf("   -m <script>      special email handling (not useful anyway)\n");
   printf("\n");
   printf("heatherly debugging \"urgents\"\n");
   printf("   @a               verbosely traces argument parsing\n");
   printf("   @p               verbosely traces interactive/crontab logic\n");
   printf("\n");
   printf("changes to crontab file naming rules\n");
   printf("   - must be formatted as \"crontab.<description>\"\n");
   printf("   - our version allows multiple crontabs per user (different descritions)\n");
   printf("   - description is '.' plus 1 to 50 characters\n");
   printf("   - valid characters in the description are [A-Za-z0-9_] only\n");
   printf("   - if you have only one crontab, likely just call it \"crontab.base\"\n");
   printf("   - options above only require <desc> part of crontab name\n");
   printf("\n");
   printf("NOTE : all arguments will be processed in the order supplied\n");
   printf("\n");
   exit (0);
}

char
prog_term          (void)
{
   int   rc = 0;
   rc = setuid(my.uid);
   printf("; fatal, program terminated\n");
   exit (-1);
}

char
prog_end           (void)
{
   int   rc = 0;
   rc = setuid(my.uid);
   printf("\n");
   exit (0);
}

/*====================---------[[ end-of-code ]]----------====================*/
