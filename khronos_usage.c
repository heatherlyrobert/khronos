/*====================--------[[ start-of-code ]]---------====================*/
#include   "khronos.h"




static tTVAL  s_stime;
static tRUSE  s_sruse;
static long   s_euse;

static tTVAL  s_btime;
static tRUSE  s_bruse;

static tTVAL  s_etime;
static tRUSE  s_eruse;

/*> typedef  struct cUSAGE  tUSAGE;                                                   <* 
 *> struct cUSAGE {                                                                   <* 
 *>    short       dur;                                                               <* 
 *>    short       use;                                                               <* 
 *>    float       cpu;                                                               <* 
 *> };                                                                                <*/
static ushort s_usage [LEN_RECD];
static int    s_count     = 0;
static float  s_avg       = 0.0;

static float  s_cpu       = 0.0;
/*> static float  s_lcpu      = 0.0;                                                  <*/

static long   s_ctime     = 0;
static long   s_cruse     = 0;

char
USAGE_purge             (void)
{
   int         i           =    0;
   for (i = 0; i < LEN_RECD; ++i)  s_usage [i]  = 0;
   s_count = 0;
   return 0;
}

char
USAGE_init              (void)
{
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(clear structure)----------------*/
   USAGE_purge  ();
   USAGE_import ();
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
USAGE_prerun     (void)
{
   /*---(get khronos start)--------------*/
   gettimeofday (&s_stime, NULL);
   getrusage (RUSAGE_SELF, &s_sruse);
   s_euse  = (s_sruse.ru_stime.tv_sec + s_sruse.ru_utime.tv_sec) * 1000000;
   s_euse += s_sruse.ru_stime.tv_usec + s_sruse.ru_utime.tv_usec;
   return 0;
}

char
USAGE_beg        (char a_hr, char a_mn)
{
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(beg of minute work)-------------*/
   gettimeofday (&s_btime, NULL);
   getrusage (RUSAGE_SELF, &s_bruse);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
USAGE_end        (char a_hr, char a_mn)
{
   /*---(locals)-----------+-----+-----+-*/
   int         n           =    0;
   long        x_dur       =    0;
   long        x_adur      =    0;
   long        x_use       =    0;
   long        x_rsec      =    0;
   long        x_buse      =    0;
   long        x_euse      =    0;
   float       x_cpu       =  0.0;
   char        s           [LEN_LABEL] = "";
   char        t           [LEN_HUND]  = "";
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(set index)----------------------*/
   n = a_hr * 60 + a_mn;
   /*---(end of minute work)-------------*/
   gettimeofday (&s_etime, NULL);
   x_dur  = (s_etime.tv_sec - s_btime.tv_sec) * 1000000;
   x_dur += s_etime.tv_usec - s_btime.tv_usec;
   s_usage [n] = x_dur;
   /*---(get all uptime)-----------------*/
   x_adur  = (s_etime.tv_sec - s_stime.tv_sec) * 1000000;
   x_adur += s_etime.tv_usec - s_stime.tv_usec;
   /*---(get usage)----------------------*/
   getrusage (RUSAGE_SELF, &s_eruse);
   x_buse  = (s_bruse.ru_stime.tv_sec + s_bruse.ru_utime.tv_sec) * 1000000;
   x_buse += s_bruse.ru_stime.tv_usec + s_bruse.ru_utime.tv_usec;
   x_euse  = (s_eruse.ru_stime.tv_sec + s_eruse.ru_utime.tv_sec) * 1000000;
   x_euse += s_eruse.ru_stime.tv_usec + s_eruse.ru_utime.tv_usec;
   /*> s_usage [n].use = x_euse - x_buse;                                             <*/
   /*---(cpu use)------------------------*/
   /*> x_cpu   = ((float) s_usage [n].use) / ((float) s_usage [n].dur);               <* 
    *> x_cpu  *= 100;                                                                 <* 
    *> s_usage [n].cpu = x_cpu;                                                       <* 
    *> s_lcpu  = x_cpu;                                                               <*/
   /*---(cumulative)---------------------*/
   s_ctime = x_adur;
   s_cruse = x_euse - s_euse;
   /*---(add to elapsed)-----------------*/
   ystrl4comma (s_cpu, s, 6, 'C', '-', LEN_LABEL);
   if (s_count < 5)  sprintf (my.usage, "%4d#  %4.2fms  ··.···'···c", s_count, s_avg);
   else              sprintf (my.usage, "%4d#  %4.2fms  %10.10sc", s_count, s_avg, s);
   /*---(export)-------------------------*/
   USAGE_export ();
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;

}


char
USAGE_import_full       (char  *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   int         x_line      =    0;
   char        x_recd      [LEN_RECD]  = "";
   int         x_len       =    0;
   int         n           =    0;
   short       x_time      =    0;
   char       *p           = NULL;
   float       x_cpu       = 0.0;
   int         i           =   0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("a_name"    , a_name);
   f = fopen (a_name, "rt");
   DEBUG_OUTP   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defaults)-----------------------*/
   USAGE_purge ();
   /*---(read all)-----------------------*/
   while (1) {
      /*---(read)------------------------*/
      fgets (x_recd, LEN_RECD, f);
      DEBUG_YJOBS   yLOG_value   ("length"    , strlen (x_recd));
      if (feof (f)) {
         DEBUG_YJOBS   yLOG_note    ("end of file reached");
         break;
      }
      ++x_line;
      /*---(filter)----------------------*/
      if (x_recd [0] == '\0')  continue;
      if (x_recd [0] == '#')   continue;
      /*---(clean-up)--------------------*/
      ystrltrim (x_recd, ySTR_BOTH, LEN_RECD);
      x_len = strlen (x_recd);
      if (x_len <  5)          continue; if (x_recd [x_len - 1] == '\n')  x_recd [--x_len] = '\0';
      DEBUG_YJOBS   yLOG_complex ("x_recd"    , "%3d, %s", x_line, x_recd);
      /*---(parse)-----------------------*/
      p = strtok (x_recd, " ");
      n = 0;
      while (p != NULL) {
         /*---(parse)-----------------------*/
         switch (n) {
         case  0 : 
            x_time = atoi (p);
            break;
         case  4 : 
            s_usage [x_time] = atoi (p);
            ++s_count;
            break;
         }
         /*---(next)------------------------*/
         p = strtok (NULL, " ");
         ++n;
         /*---(done)------------------------*/
      }
   }
   /*---(close file)---------------------*/
   rc = fclose  (f);
   DEBUG_OUTP   yLOG_point   ("close"     , rc);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char  USAGE_import (void) { return USAGE_import_full (my.n_usage); }

char
USAGE_export_full       (char  *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   long        x_cdur      =    0;
   FILE       *f           = NULL;
   char        s           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defaults)-----------------------*/
   s_count = 0;
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("a_name"    , a_name);
   /*---(open file)----------------------*/
   f = fopen (a_name, "wt");
   DEBUG_OUTP   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write header)-------------------*/
   fprintf (f, "## %s\n", P_ONELINE);
   fprintf (f, "##    rolling system usage data by minute\n");
   fprintf (f, "##    heartbeat to verify update å%sæ\n", my.heartbeat);
   fprintf (f, "\n");
   fprintf (f, "\n");
   /*---(write lines)--------------------*/
   for (i = 0; i < LEN_RECD; ++i) {
      if (s_usage [i] > 0) {
         if (s_count % 5 == 0)  fprintf (f, "##-t  hr  mn  ---n  -usec\n");
         ++s_count;
         x_cdur += s_usage [i];
         fprintf (f, "%-4d  %2d  %2d  %4d  %5d\n", i, i / 60, i % 60, s_count, s_usage [i]);
      }
   }
   /*---(full time stats)----------------*/
   s_cpu  = ((double) s_cruse) / ((double) s_ctime);
   s_cpu *= 100.0;
   ystrl4comma (s_cpu, s, 6, 'C', '-', LEN_LABEL);
   fprintf (f, "\n");
   fprintf (f, "## cum time  %ld\n"  , s_ctime);
   fprintf (f, "## cum ruse  %ld\n"  , s_cruse);
   fprintf (f, "## avg cpu   %s\n", s);
   /*---(run dur stats)------------------*/
   if (s_count > 0 && x_cdur > 0) {
      s_avg = ((float) x_cdur / ((float) s_count)) / 1000;
   }
   /*> s_lcpu  = ((double) s_cruse) / ((double) x_cdur);                              <* 
    *> s_lcpu *= 100.0;                                                               <* 
    *> ystrl4comma (s_lcpu, s, 6, 'C', '-', LEN_LABEL);                                <*/
   fprintf (f, "\n");
   fprintf (f, "## cum run   %ld\n" , x_cdur);
   fprintf (f, "## avg dur   %4.2f\n", s_avg);
   /*> fprintf (f, "## avg cpu   %s\n", s);                                           <*/
   /*---(close file)---------------------*/
   rc = fclose  (f);
   DEBUG_OUTP   yLOG_point   ("close"     , rc);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char  USAGE_export (void) { return USAGE_export_full (my.n_usage); }



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*
USAGE__unit             (char *a_question, int a_num)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         i           =    0;
   int         x_min       =    0;
   int         x_max       =    0;
   /*---(prepare)------------------------*/
   ystrlcpy  (unit_answer, "RPTG             : question not understood", LEN_HUND);
   /*---(simple requests)----------------*/
   if      (strcmp (a_question, "count"         )  == 0) {
      for (i = 0; i < LEN_RECD; ++i)       { if (x_min == 0 && s_usage [i] > 0)  x_min = i; }
      for (i = LEN_RECD - 1; i >= 0; --i)  { if (x_max == 0 && s_usage [i] > 0)  x_max = i; }
      snprintf (unit_answer, LEN_HUND, "USAGE count      : %4da, %4dn, %4dx", s_count, x_min, x_max);
   }
   else if (strcmp (a_question, "entry"         )  == 0) {
      if (a_num < 0 || a_num > LEN_RECD)  snprintf (unit_answer, LEN_HUND, "USAGE entry      :    ·,     ·n");
      snprintf (unit_answer, LEN_HUND, "USAGE entry      : %4d, %5dms", a_num, s_usage [a_num]);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



