/*====================--------[[ start-of-code ]]---------====================*/
#include   "khronos.h"


/*
 *
 * these are for summary statistics only actual tracking exists on most
 * jobs to research actual numbers
 *
 * stats
 *   #  tot  ---early---  end
 *      r a  s b o k s f  w p  ---history--->>>
 *  "·  · ·  · · · · · ·  · ·  · ´····´····´····´····´····´····´····´····´····´····´····´····´"
 *
 * durs (for warnings and passes only)
 *   #  e l  < -30     -10        0         10      30 >  --history--->>>
 *  "·  · ·  · · · · · ·········· · ·········· · · · · ·  · ´····´····´····´····´····´····´····´····´····´····´····´····´"
 *
 * both of these use the YSTR_COUNT for counting numbers  ·1-9a-zA-Z
 *
 * the # entry is the chosen rolling number, i.e. 1 to 60 runs.  
 *   -- less for new jobs
 *   -- more for stable ones
 *   -- regardless of setting, 60 runs of history will be kept in detail
 *   -- why not the size is allocated anyway ;)  then changes are easy.
 *
 * history for durs uses < n m l k jihgfedcbq 0 ABCDEFGHIJ K L M N >
 *   -- a is exactly  -1
 *   -- j is exactly -10
 *   -- k is -11 to -15
 *   -- l is -16 to -20
 *   -- m is -25 to -21
 *   -- n is -30 to -26
 *   -- < is less than 30
 *
 * keep history line so i can do rolling stats very easily, and also
 * over-ride to keep only 'x' number whenever i wish
 *
 * actual tracking exists on most jobs to research actual numbers
 *
 * NOTE : i am keeping the detail and totals together in the file.  its
 * duplication and larger, but it allows easier reporting later and an
 * small potential element of data corruption checking.
 *
 */



static char     s_name  [LEN_PATH] = "";
static FILE    *f_trks  = NULL;

static tTRKS   *s_head  = NULL;
static tTRKS   *s_tail  = NULL;
static int      s_count = 0;



/*====================------------------------------------====================*/
/*===----                     supporting functions                     ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
TRKS__wipe              (tTRKS *a_cur)
{
   /*---(defense)--------------*/
   if (a_cur == NULL)  return -1;
   /*---(master)-------------------------*/
   ystrlcpy (a_cur->file   , "" , LEN_TITLE);
   ystrlcpy (a_cur->tracker, "" , LEN_TITLE);
   ystrlcpy (a_cur->last   , "" , LEN_HUND);
   ystrlcpy (a_cur->stats  , "" , LEN_FULL);
   ystrlcpy (a_cur->durs   , "" , LEN_FULL);
   ystrlcpy (a_cur->actual , "" , LEN_RECD);
   a_cur->parent  = NULL;
   a_cur->m_prev  = NULL;
   a_cur->m_next  = NULL;
   /*---(complete)-------------*/
   return 1;
}

char*
TRKS__memory            (tTRKS *a_cur)
{
   int         n           =    0;
   ystrlcpy (g_print, "å___.___.___æ", LEN_RECD);
   ++n;  if (a_cur->file    [0] != '\0')        g_print [n] = 'X';
   ++n;  if (a_cur->tracker [0] != '\0')        g_print [n] = 'X';
   ++n;  if (a_cur->last    [0] != '\0')        g_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->stats   [0] != '\0')        g_print [n] = 'X';
   ++n;  if (a_cur->durs    [0] != '\0')        g_print [n] = 'X';
   ++n;  if (a_cur->actual  [0] != '\0')        g_print [n] = 'X';
   ++n;
   ++n;  if (a_cur->parent      != NULL)        g_print [n] = 'X';
   ++n;  if (a_cur->m_prev      != NULL)        g_print [n] = 'X';
   ++n;  if (a_cur->m_next      != NULL)        g_print [n] = 'X';
   return g_print;
}

char
TRKS__tracker           (cchar *a_tracker)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_tracker" , a_tracker);
   --rce;  if (a_tracker == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("a_tracker" , a_tracker);
   /*---(check tracker)------------------*/
   l = strlen (a_tracker);
   DEBUG_OUTP   yLOG_value   ("l"         , l);
   --rce;  if (l < 3) {
      DEBUG_OUTP   yLOG_note    ("must be at least 3 characters");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (l > LEN_TITLE) {
      DEBUG_OUTP   yLOG_note    ("can not be more than 30 characters");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ystrlgood (a_tracker, ySTR_WRITE, LEN_TITLE);
   DEBUG_OUTP   yLOG_value   ("good"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_note    ("found non-writing/special characters (ySTR_WRITE)");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__coded             (char a_type, cchar *a_key, char *a_str)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         i           =    0;
   int         l           =    0;
   int         x_trouble   =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_key"     , a_key);
   --rce;  if (a_key == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_point   ("a_str"     , a_str);
   --rce;  if (a_str == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check tracker)------------------*/
   l = strlen (a_str);
   DEBUG_OUTP   yLOG_value   ("l"         , l);
   --rce;  if (a_type != 'a' && l != strlen (a_key)) {
      DEBUG_OUTP   yLOG_note    ("does not match default length");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   for (i = 0; i < l; ++i) {
      switch (a_key [i]) {
      case ' ' :
         if (a_str [i] != ' ') {
            DEBUG_OUTP   yLOG_complex ("not empty" , "%3d, %c vs %c", i, a_key [i], a_str [i]);
            ++x_trouble;
         }
         break;
      case ')' :
         if (a_str [i] != ')') {
            DEBUG_OUTP   yLOG_complex ("not paren" , "%3d, %c vs %c", i, a_key [i], a_str [i]);
            ++x_trouble;
         }
         break;
      case '¬' :
         break;
      default  :
         if (a_str [i] == ' ') {
            DEBUG_OUTP   yLOG_complex ("not filled", "%3d, %c vs %c", i, a_key [i], a_str [i]);
            ++x_trouble;
         }
         break;
      }
   }
   /*---(check results)------------------*/
   DEBUG_OUTP   yLOG_value   ("x_trouble" , x_trouble);
   --rce;  if (x_trouble > 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(append on actuals)--------------*/
   --rce;  if (a_type == 'a') {
      if (l <  strlen (a_key))  ystrlcat (a_str, KHRONOS_DEFACTUAL + l,  LEN_RECD);
      if (l >  strlen (a_key)) {
         DEBUG_OUTP   yLOG_note    ("actual is longer than default length");
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__typing            (cchar *a_tracker)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_type      =  '·';
   /*---(defense)------------------------*/
   if      (a_tracker == NULL)                     ;
   else if (strcmp (a_tracker, ".valid"    ) == 0) x_type = KHRONOS_VALIDITY;
   else if (strcmp (a_tracker, ".only"     ) == 0) x_type = KHRONOS_VALIDITY;
   else if (strcmp (a_tracker, ".blackout" ) == 0) x_type = KHRONOS_VALIDITY;
   else if (strcmp (a_tracker, ".retire"   ) == 0) x_type = KHRONOS_VALIDITY;
   /*---(complete)-----------------------*/
   return x_type;
}



/*====================------------------------------------====================*/
/*===----                     allocating and freeing                   ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
trks__new               (tTRKS **r_new, char a_force)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   int         x_tries     =    0;
   tTRKS      *x_new       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_senter  (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_INPT  yLOG_spoint  (r_new);
   --rce;  if (r_new == NULL) {
      DEBUG_INPT  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_spoint  (*r_new);
   --rce;  if (a_force == '-' && *r_new != NULL) {
      DEBUG_INPT  yLOG_snote   ("already set");
      DEBUG_INPT  yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *r_new = NULL;
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = (tTRKS *) malloc (sizeof (tTRKS));
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
   rc = TRKS__wipe (x_new);
   DEBUG_INPT   yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save back)----------------------*/
   *r_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char TRKS__new     (tTRKS **r_new) { return trks__new (r_new, '-'); }
char TRKS__force   (tTRKS **r_new) { return trks__new (r_new, 'y'); }

char
TRKS__free              (tTRKS **b_old)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rce         =  -10;
   int         rc          =    0;
   tTRKS      *x_old       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT  yLOG_senter  (__FUNCTION__);
   /*---(check return)-------------------*/
   DEBUG_INPT  yLOG_spoint  (b_old);
   --rce;  if (b_old == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT  yLOG_spoint  (*b_old);
   --rce;  if (*b_old == NULL) {
      DEBUG_INPT   yLOG_snote   ("never set");
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(simplify)-----------------------*/
   x_old = *b_old;
   /*---(clear and return)---------------*/
   TRKS__wipe (x_old);
   free (x_old);
   *b_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_INPT  yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       existance                              ----===*/
/*====================------------------------------------====================*/
static void  o___EXIST___________o () { return; }

char
TRKS__default           (tTRKS *a_cur)
{
   char        i           =    0;
   ystrlcpy (a_cur->last  , KHRONOS_DEFLAST  , LEN_HUND);
   ystrlcpy (a_cur->stats , KHRONOS_DEFSTATS , LEN_FULL);
   ystrlcpy (a_cur->durs  , KHRONOS_DEFDURS  , LEN_FULL);
   ystrlcpy (a_cur->actual, KHRONOS_DEFACTUAL, LEN_RECD);
   return 0;
}

char
TRKS__by_name           (cchar *a_file, cchar *a_tracker, tTRKS **r_trks)
{
   char        rce         =  -10;
   tTRKS      *x_cur       = NULL;
   x_cur = s_head;
   --rce;  if (r_trks == NULL)  return rce;
   *r_trks = NULL;
   while (x_cur != NULL) {
      if (strcmp (a_file, x_cur->file) == 0) {
         if (strcmp (a_tracker, x_cur->tracker) == 0) {
            *r_trks = x_cur;
            return 1;
         }
      }
      x_cur = x_cur->m_next;
   }
   return 0;
}

char
TRKS__by_index          (int n, tTRKS **r_trks)
{
   char        rce         =  -10;
   int         c           =    0;
   tTRKS      *x_cur       = NULL;
   x_cur = s_head;
   --rce;  if (r_trks == NULL)  return rce;
   *r_trks = NULL;
   while (x_cur != NULL) {
      if (c == n) {
         *r_trks = x_cur;
         return 1;
      }
      ++c;
      x_cur = x_cur->m_next;
   }
   return 0;
}

char
TRKS__hook              (tTRKS *a_cur)
{
   if (s_head == NULL) {
      s_head  = s_tail  = a_cur;
      s_count = 1;
   } else {
      a_cur->m_prev  = s_tail;
      s_tail->m_next = a_cur;
      s_tail         = a_cur;
      ++s_count;
   }
   return 0;
}

char
TRKS__replace           (tTRKS *a_old, tTRKS *a_new)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(populate new pointers)----------*/
   a_new->m_prev = a_old->m_prev;
   if (a_old->m_prev != NULL)  a_old->m_prev->m_next = a_new;
   a_new->m_next = a_old->m_next;
   if (a_old->m_next != NULL)  a_old->m_next->m_prev = a_new;
   /*---(ground old pointers)------------*/
   a_old->m_prev = NULL;
   a_old->m_next = NULL;
   /*---(free old)-----------------------*/
   TRKS__free (&a_old);
   /*---(complete)-----------------------*/
   return 0;
}

char
TRKS_purge              (void)
{
   char        rce         =  -10;
   tTRKS      *x_cur       = NULL;
   tTRKS      *x_nxt       = NULL;
   x_cur = s_head;
   while (x_cur != NULL) {
      x_nxt = x_cur->m_next;
      x_cur->m_prev = x_cur->m_next = NULL;
      TRKS__free (&x_cur);
      --s_count;
      x_cur = x_nxt;
   }
   s_head  = s_tail  = NULL;
   return 0;
}

char
TRKS_create             (cchar *a_file, cchar *a_tracker, tLINE *a_line, tTRKS **r_trks)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTRKS      *x_cur       = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_trks != NULL)  *r_trks = NULL;
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file    == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("a_file"    , a_file);
   DEBUG_OUTP   yLOG_point   ("a_tracker" , a_tracker);
   --rce;  if (a_tracker == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_info    ("a_tracker" , a_tracker);
   DEBUG_OUTP   yLOG_point   ("r_trks"    , r_trks);
   --rce;  if (r_trks == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check tracker)------------------*/
   rc = TRKS__tracker (a_tracker);
   DEBUG_OUTP   yLOG_value   ("tracker"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(find)------------------------*/
   rc = TRKS__by_name (a_file, a_tracker, &x_cur);
   if (x_cur != NULL) {
      *r_trks = x_cur;
      DEBUG_OUTP   yLOG_note    ("using existing");
      DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(create)----------------------*/
   rc = TRKS__new (&x_cur);
   DEBUG_YJOBS   yLOG_value   ("new"       , rc);
   if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(add data)--------------------*/
   ystrlcpy (x_cur->file   , a_file   , LEN_TITLE);
   ystrlcpy (x_cur->tracker, a_tracker, LEN_TITLE);
   if (a_line != NULL)  x_cur->parent = a_line;
   TRKS__default (x_cur);
   /*---(hook)------------------------*/
   TRKS__hook (x_cur);
   /*---(save back)-------------------*/
   *r_trks = x_cur;
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       file related                           ----===*/
/*====================------------------------------------====================*/
static void  o___FILE____________o () { return; }

char
TRKS__open              (char *a_name, char *a_perms)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_spoint  (f_trks);
   --rce;  if (f_trks  != NULL) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_spoint  (a_name);
   --rce;  if (a_name  == NULL) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_snote   (a_name);
   DEBUG_OUTP   yLOG_spoint  (a_perms);
   --rce;  if (a_perms == NULL) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP   yLOG_snote   (a_perms);
   --rce;  if (strcmp (a_perms, "rt") != 0 && strcmp (a_perms, "wt") != 0) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   f_trks = fopen (a_name, a_perms);
   DEBUG_OUTP   yLOG_spoint  (f_trks);
   --rce;  if (f_trks == NULL) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save)---------------------------*/
   ystrlcpy (s_name, a_name, LEN_PATH);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
TRKS__close             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_spoint  (f_trks);
   --rce;  if (f_trks == NULL) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = fclose (f_trks);
   DEBUG_OUTP   yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(ground)-------------------------*/
   f_trks = NULL;
   DEBUG_OUTP   yLOG_spoint  (f_trks);
   /*---(save)---------------------------*/
   ystrlcpy (s_name, "", LEN_PATH);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       export/import                          ----===*/
/*====================------------------------------------====================*/
static void  o___EXIM____________o () { return; }

char
TRKS__parse             (cchar *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_HUGE]  = "";
   char        x_file      [LEN_TITLE] = "";
   char        x_tracker   [LEN_TITLE] = "";
   char       *p           = NULL;
   char        n           =    0;
   char       *x_dst       = NULL;
   int         l           =    0;
   tTRKS      *x_cur       = NULL;
   char        t           [LEN_RECD]  = "";
   int         x_len       =    0;
   uchar       x_runs      =    0;
   uchar       x_actv      =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP   yLOG_point   ("a_recd"    , a_recd);
   --rce;  if (a_recd == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   ystrlcpy (x_recd, a_recd, LEN_HUGE);
   p = strtok (x_recd, "§");
   /*---(parse fields)-------------------*/
   --rce;  while (p != NULL) {
      /*---(parse)-----------------------*/
      switch (n) {
      case  0 : x_dst = x_file;          l = LEN_TITLE;  break;
      case  1 : x_dst = x_tracker;       l = LEN_TITLE;  break;
      case  2 : x_dst = x_cur->last;     l = LEN_HUND;   break;
      case  3 : x_dst = x_cur->stats;    l = LEN_FULL;   break;
      case  4 : x_dst = x_cur->durs;     l = LEN_FULL;   break;
      case  5 : x_dst = x_cur->actual;   l = LEN_RECD;   break;
      default : break;  /* too many fields */
      }
      ystrlcpy  (t, p        , LEN_RECD);
      ystrltrim (t, ySTR_BOTH, LEN_RECD);
      x_len = strlen (t);
      DEBUG_OUTP   yLOG_complex ("saving"    , "%d, %p, %d, %d, %s", n, x_dst, l, x_len, t);
      if (x_len >= l) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      ystrlcpy  (x_dst, t, l);
      /*---(enough)----------------------*/
      if (n == 1) {
         rc = TRKS_create (x_file, x_tracker, NULL, &x_cur);
         DEBUG_OUTP   yLOG_value   ("create"    , rc);
         if (rc < 0) {
            DEBUG_OUTP   yLOG_note    ("could not create properly");
            break;
         }
      }
      /*---(next)------------------------*/
      p = strtok (NULL, "§");
      ++n;
      /*---(done)------------------------*/
   }
   /*---(check result)-------------------*/
   DEBUG_OUTP   yLOG_value   ("n"         , n);
   --rce;  if (n < 2)  {
      TRKS__wipe (x_cur);
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (n > 6)  {
      DEBUG_OUTP   yLOG_note    ("too many fields, ignoring trailing");
   }
   /*---(data quality checks)------------*/
   if (TRKS__coded ('l', KHRONOS_DEFLAST  , x_cur->last     ) < 0)
      ystrlcpy (x_cur->last  , KHRONOS_DEFLAST  , LEN_HUND);
   if (TRKS__coded ('s', KHRONOS_DEFSTATS , x_cur->stats    ) < 0)
      ystrlcpy (x_cur->stats , KHRONOS_DEFSTATS , LEN_FULL);
   if (TRKS__coded ('d', KHRONOS_DEFDURS  , x_cur->durs     ) < 0)
      ystrlcpy (x_cur->durs  , KHRONOS_DEFDURS  , LEN_FULL);
   if (TRKS__coded ('a' ,KHRONOS_DEFACTUAL, x_cur->actual   ) < 0)
      ystrlcpy (x_cur->actual, KHRONOS_DEFACTUAL, LEN_RECD);
   if (x_cur->stats [3] == '0')  x_cur->stats [3] == '·';
   x_cur->stats [5] = '·';
   /*---(update rolling)-----------------*/
   if (x_cur->parent != NULL) {
      x_cur->stats [0] = x_cur->parent->rolling;
      TRKS_restat (x_cur->stats);
      x_cur->durs  [0] = x_cur->parent->rolling;
      TRKS_redur  (x_cur->durs, x_cur->parent->est_min / 60, x_cur->parent->est / 60, x_cur->parent->est_max / 60);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS_import_full        (cchar *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_HUGE]  = "";
   int         x_len       =    0;
   tTRKS      *x_cur       = NULL;
   int         n           =    0;
   int         c           =    0;
   int         g           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(open file)----------------------*/
   rc = TRKS__open (a_file, "rt");
   DEBUG_OUTP   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read all)-----------------------*/
   while (1) {
      /*---(read)------------------------*/
      fgets (x_recd, LEN_HUGE, f_trks);
      DEBUG_YJOBS   yLOG_value   ("length"    , strlen (x_recd));
      if (feof (f_trks)) {
         DEBUG_YJOBS   yLOG_note    ("end of file reached");
         break;
      }
      ++n;
      /*---(filter)----------------------*/
      if (x_recd [0] == '\0')  continue;
      if (x_recd [0] == '#')   continue;
      if (x_recd [0] == ' ')   continue;
      /*---(clean-up)--------------------*/
      ystrltrim (x_recd, ySTR_BOTH, LEN_HUGE);
      x_len = strlen (x_recd);
      if (x_len <  5)          continue;
      if (x_recd [x_len - 1] == '\n')  x_recd [--x_len] = '\0';
      DEBUG_YJOBS   yLOG_info    ("x_recd"    , x_recd);
      /*---(parse)-----------------------*/
      rc = TRKS__parse (x_recd);
      if (rc < 0) {
         TRKS__free (&x_cur);
         DEBUG_YJOBS   yLOG_note    ("record could not be parsed");
         continue;
      }
      /*---(next)------------------------*/
      ++g;
      /*---(done)------------------------*/
   }
   /*---(close file)---------------------*/
   rc = TRKS__close ();
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(tracker requests)---------------*/
   DEBUG_OUTP   yLOG_value   ("my.actuals", my.actuals);
   switch (my.actuals) {
   case 'd'  :  TRKS_actual_reset ();  break;
   case 'u'  :  TRKS_unplan  ();       break;
   case 'p'  :  TRKS_plan    ();       break;
   case 'r'  :  TRKS_replan  ();       break;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char TRKS_import   (void) { return TRKS_import_full (my.n_trks); }

char
TRKS__export_check      (char *a_tracker)
{
   int         l           =    0;
   if (a_tracker == NULL)                            return 'y';
   l = strlen (a_tracker);
   /*> if (a_tracker [0] == '.')                         return '-';                  <*/
   if (l <  6)                                       return 'y';
   if (strncmp (a_tracker, "line ", 5) != 0)         return 'y';
   if (strchr (YSTR_NUMBER, a_tracker [5]) == NULL)  return 'y';
   if (l == 6 && a_tracker [6] == '\0')              return '-';
   if (strchr (YSTR_NUMBER, a_tracker [6]) == NULL)  return 'y';
   if (l == 7 && a_tracker [7] == '\0')              return '-';
   if (strchr (YSTR_NUMBER, a_tracker [7]) == NULL)  return 'y';
   if (l == 8 && a_tracker [8] == '\0')              return '-';
   return 'y';
}

char
TRKS_export_full        (cchar *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTRKS      *x_cur       = NULL;
   tTRKS      *x_last      = NULL;
   int         c           =    0;
   int         i           =    0;
   int         j           =    0;
   int         k           =    0;
   int         x_hr        =    8;
   tLINE      *x_save      = NULL;
   char        x_good      =  '-';
   char        t           [LEN_TERSE] = "";
   char        r           [LEN_FULL]  = "";
   char        s           [LEN_RECD]  = "";
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(open file)----------------------*/
   rc = TRKS__open (a_file, "wt");
   DEBUG_OUTP   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write headers)------------------*/
   fprintf (f_trks, "## %s\n", P_ONELINE);
   fprintf (f_trks, "##    full execution tracker inventory (including historical)\n");
   fprintf (f_trks, "##    heartbeat to verify update å%sæ\n", my.heartbeat);
   fprintf (f_trks, "##\n");
   fprintf (f_trks, "#@ x-parse  10åÏ-----------------------------···Ï-----------------------------···Ï-------------------------··Ï---------··Ï-----···Ï--------------------------------··Ï--------------------------------------------------------------···Ï--------------------------------------------------··Ï--------------------------------------------------------------···Ï--------------------------------------------------------------------------------------·æ\n");
   fprintf (f_trks, "#@ titles     åfile·····························tracker··························heartbeat···················epoch·······ppid·····statistics·························shistory··························································durations············································dhistory··························································actuals·················································································æ\n");
   fprintf (f_trks, "##\n");
   /*---(write all)----------------------*/
   DEBUG_OUTP   yLOG_value   ("s_count"   , s_count);
   x_cur = s_head;
   while (x_cur != NULL) {
      x_good = TRKS__export_check (x_cur->tracker);
      DEBUG_OUTP   yLOG_complex ("x_cur"     , "%2di, %2dk, %c, %s, %s", i, k, x_good, x_cur->file, x_cur->tracker);
      if (x_good == 'y') {
         /*---(end of used trackers)-----*/
         if (x_save != NULL && x_cur->parent == NULL && i > 0) {
            DEBUG_OUTP   yLOG_note    ("end of currently used trackers");
            fprintf (f_trks, "\n## %d current lines written\n\n", i);
            i = 0;
            k = 0;
         }
         /*---(break between files)------*/
         if (x_last == NULL || strcmp (x_last->file, x_cur->file) != 0)  {
            DEBUG_OUTP   yLOG_note    ("break between files");
            fprintf (f_trks, "\n");
            k = 0;
         }
         /*---(periodic breaks)----------*/
         if (k % 5 == 0) {
            /*---(all but actual)--------*/
            ystrlcpy (s, "##---file--------------------  ---tracker-------------------  ---heartbeat--------------··--epoch---··--ppid  ---statistics--------------------··---shistory----------------------------------------------------  ---durations---------------------------------------··---dhistory----------------------------------------------------  -pos- ", LEN_RECD);
            /*---(actual header)---------*/
            x_hr = 20;
            for (j = 0; j < 25; ++j) {
               if (x_hr > 23) x_hr = 0;
               if (x_hr < 10)  sprintf (t, "[%d>-", x_hr);
               else            sprintf (t, "[%2d>", x_hr);
               sprintf (r, "%s-´----Á----´----Â----´----Ã----´----Ä----´----Å----´----", t);
               ystrlcat (s, r, LEN_RECD);
               ++x_hr;
            }
            /*---(current marker)--------*/
            if (my.hour >= 20)  x_hr = my.hour - 20;
            else                x_hr = my.hour + 4;
            n = 339 + x_hr * 60 + my.minute;
            s [n] = 'Š';
            /*---(final)-----------------*/
            fprintf (f_trks, "%s \n", s);
         }
         fprintf (f_trks, "%-29.29s  %-29.29s  ", x_cur->file, x_cur->tracker);
         fprintf (f_trks, "%s  %s  " , x_cur->last, x_cur->stats);
         fprintf (f_trks, "%s  %s \n", x_cur->durs, x_cur->actual);
         ++c; ++i; ++k;
         x_save = x_cur->parent;
         x_last = x_cur;
      }
      x_cur  = x_cur->m_next;
   }
   /*---(write footer)-------------------*/
   if (i > 0) {
      if (x_save != NULL)  fprintf (f_trks, "\n## %d current lines written\n", i);
      if (x_save == NULL)  fprintf (f_trks, "\n## %d non-current lines written\n", i);
   }
   fprintf (f_trks, "\n## end-of-file.  %d total lines.  done, finito, completare, whimper [Ï´···\n", c);
   /*---(close file)---------------------*/
   rc = TRKS__close ();
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char TRKS_export   (void) { return TRKS_export_full (my.n_trks); }



/*====================------------------------------------====================*/
/*===----                         data updates                         ----===*/
/*====================------------------------------------====================*/
static void  o___DATA____________o () { return; }

char
TRKS__count2num         (char a_count)
{
   char       *p           = NULL;
   if (a_count ==   0)   return -10;
   if (a_count == '´')   return   0;
   p = strchr (YSTR_COUNT, a_count);
   if (p == NULL)  return -1;
   return p - YSTR_COUNT;
}

char
TRKS__num2count         (char a_num)
{
   int         l           =    0;
   char       *p           = NULL;
   char        n           =    0;
   n = a_num;
   l = strlen (YSTR_COUNT);
   if (a_num <    0)   return -1;
   if (a_num >=   l)   return -1;
   return YSTR_COUNT [n];
}

uchar
TRKS__biggun2num        (uchar a_biggun)
{
   char        rce         =  -10;
   char       *p           = NULL;
   DEBUG_OUTP    yLOG_senter  (__FUNCTION__);
   DEBUG_OUTP    yLOG_schar   (a_biggun);
   -rce;  if (a_biggun ==   0) {
      DEBUG_OUTP    yLOG_snote   ("sent a null char, return error");
      DEBUG_OUTP    yLOG_sexit   (__FUNCTION__);
      return '¢';
   }
   if (a_biggun == (uchar) '·') {
      DEBUG_OUTP    yLOG_snote   ("· defaults to 0");
      DEBUG_OUTP    yLOG_sexit   (__FUNCTION__);
      return   0;
   }
   if (a_biggun == (uchar) '´') {
      DEBUG_OUTP    yLOG_snote   ("´ defaults to 0");
      DEBUG_OUTP    yLOG_sexit   (__FUNCTION__);
      return   0;
   }
   p = strchr ((uchar *) YSTR_BIGGUN, a_biggun);
   if (p == NULL) {
      DEBUG_OUTP    yLOG_snote   ("couldn't find char, return error");
      DEBUG_OUTP    yLOG_sexit   (__FUNCTION__);
      return '¢';
   }
   DEBUG_OUTP    yLOG_sexit   (__FUNCTION__);
   return p - YSTR_BIGGUN;
}

uchar
TRKS__num2biggun        (uchar a_num)
{
   int         l           =    0;
   char       *p           = NULL;
   uchar       n           =    0;
   DEBUG_OUTP    yLOG_senter  (__FUNCTION__);
   DEBUG_OUTP    yLOG_sint    (a_num);
   n = a_num;
   l = strlen (YSTR_BIGGUN);
   DEBUG_OUTP    yLOG_sint    (l);
   if (a_num >=   l)   a_num = l - 1;
   DEBUG_OUTP    yLOG_sexit   (__FUNCTION__);
   return YSTR_BIGGUN [n];
}

/*    r a  s B O K T  g v G V  f w p  ---history--->>>
 * ·  · ·  · · · · ·  · · · ·  · · ·  ·)´····´····´····´····´····´····´····´····´····´····´····´····´"
 * -123456789-123456789-123456789-123456789-123456789-123456789-123456789
 * 0         1         2         3         4         5         6
 */



/*====================------------------------------------====================*/
/*===----                         statistics                           ----===*/
/*====================------------------------------------====================*/
static void  o___STATS___________o () { return; }


#define    KHRONOS_STAT  "[sBOKTgvGVfwp"

char
TRKS__stat_prepare      (char *b_str, char a_code, char *r_roll, char *r_hist)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_roll      =    0;
   char        x_hist      =    0;
   char        i           =    0;
   char        x_check     =    0;
   char        l           =    0;
   /*---(quick-out)----------------------*/
   if (a_code == KHRONOS_ACTV)  return 0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_roll != NULL)  *r_roll = 0;
   if (r_hist != NULL)  *r_hist = 0;
   /*---(defense)------------------------*/
   DEBUG_OUTP    yLOG_char    ("a_code"    , a_code);
   --rce;  if (a_code == 0 || strchr (KHRONOS_STAT, a_code) == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = TRKS__coded ('s', KHRONOS_DEFSTATS , b_str);
   DEBUG_OUTP    yLOG_value   ("coded"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   l  = strlen (b_str);
   /*---(get roll)-----------------------*/
   if      (b_str [0] == '´')  x_roll = 60;
   else if (b_str [0] == '·')  x_roll = 60;
   else if (b_str [0] == '[')  x_roll =  1;
   else if (b_str [0] == '<')  x_roll = 15;
   else if (b_str [0] == '|')  x_roll = 30;
   else if (b_str [0] == '>')  x_roll = 45;
   else if (b_str [0] == ']')  x_roll = 60;
   else                        x_roll  = TRKS__count2num (b_str [ 0]);
   if (x_roll > 60)  x_roll = 60;
   DEBUG_OUTP    yLOG_complex ("x_roll"    , "%c, %d", b_str [ 0], x_roll);
   --rce;  if (x_roll < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get history)--------------------*/
   x_hist  = TRKS__count2num (b_str [35]);
   DEBUG_OUTP    yLOG_complex ("x_hist"    , "%c, %d", b_str [35], x_hist);
   --rce;  if (x_hist < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify history string)----------*/
   for (i = 0; i < 60; ++i) {
      if (strchr (KHRONOS_STAT, b_str [i + 37]) != NULL)  ++x_check;
      else break;
   }
   DEBUG_OUTP    yLOG_value   ("check"     , x_check);
   --rce;  if (x_hist != x_check) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP    yLOG_char    ("tail"      , b_str [l - 1]);
   --rce;  if (b_str [l - 1] != '´') {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (r_roll != NULL)  *r_roll = x_roll;
   if (r_hist != NULL)  *r_hist = x_hist;
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__stat_history      (char *b_str, char a_code, char *b_hist)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_next      =    0;
   char        i           =    0;
   /*---(quick-out)----------------------*/
   if (a_code == KHRONOS_BEG)   return 0;
   if (a_code == KHRONOS_ACTV)  return 0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(get history)--------------------*/
   x_next  = *b_hist + 37;
   DEBUG_OUTP    yLOG_complex ("history"   , "%c, %d, %d", b_str [35], *b_hist, x_next);
   /*---(not full yet)-------------------*/
   if (*b_hist < 60) {
      b_str [x_next] = a_code;
      b_str [35    ] = TRKS__num2count (++(*b_hist));
      DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(full)---------------------------*/
   for (i = 1; i < 60; ++i)  b_str [i + 37 - 1] = b_str [i + 37];
   b_str [59 + 37] = a_code;
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 2;
}

char
TRKS__stat_update       (char *b_str, char a_code, char a_roll)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        b           =    0;
   char        a           =    0;
   char        x_bef       =  '-';
   char        x_aft       =  '-';
   char        x_off       =    0;
   char        x_chr       =  '-';
   char        x_cha       =  '-';
   uchar       x_runs      =    0;
   char        x_actv      =    0;
   /*---(quick-out)----------------------*/
   if (a_code == KHRONOS_ACTV)  return 0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(beginnings)---------------------*/
   --rce;  switch  (a_code) {
   case KHRONOS_BEG  :  x_off =  3;    break;
   }
   /*---(troubles)-----------------------*/
   --rce;  if (x_off == 0)  switch  (a_code) {
   case KHRONOS_SKIP :  x_off =  8;    break;
   case KHRONOS_BADD :  x_off = 10;    break;
   case KHRONOS_BOOM :  x_off = 12;    break;
   case KHRONOS_KILL :  x_off = 14;    break;
   case KHRONOS_TERM :  x_off = 16;    break;
   }
   /*---(interventions)------------------*/
   --rce;  if (x_off == 0)  switch  (a_code) {
   case KHRONOS_LGRA :  x_off = 19;    break;
   case KHRONOS_LVIO :  x_off = 21;    break;
   case KHRONOS_GGRA :  x_off = 23;    break;
   case KHRONOS_GVIO :  x_off = 25;    break;
   }
   /*---(endings)------------------------*/
   --rce;  if (x_off == 0)  switch  (a_code) {
   case KHRONOS_FAIL :  x_off = 28;    break;
   case KHRONOS_WARN :  x_off = 30;    break;
   case KHRONOS_PASS :  x_off = 32;    break;
   }
   /*---(summarize)----------------------*/
   DEBUG_OUTP    yLOG_complex ("values"    , "%c, %d", a_code, x_off);
   --rce;  if (x_off == 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check active)-------------------*/
   x_chr  = b_str [3];
   x_runs = TRKS__count2num (x_chr);
   x_cha  = b_str [5];
   x_actv = TRKS__count2num  (x_cha);
   DEBUG_OUTP    yLOG_complex ("runs/actv" , "%c=%3dR, %c=%3dA", x_chr, x_runs, x_cha, x_actv);
   if (x_actv > 1) {
      DEBUG_OUTP   yLOG_note    ("WARNING: active jobs greater than 1");
   }
   --rce;  if (a_code != KHRONOS_BEG && x_actv == 0) {
      DEBUG_OUTP   yLOG_note    ("FATAL: no active jobs to complete");
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update specific stat)-----------*/
   if (x_off > 5) {
      DEBUG_OUTP    yLOG_note    ("increment specific");
      x_bef = b_str [x_off];
      b     = TRKS__count2num  (b_str [x_off]);
      DEBUG_OUTP    yLOG_complex ("specific"  , "%2do, %c=%2db", x_off, x_bef, b);
      /*> --rce;  if (b > a_roll) {                                                     <* 
       *>    DEBUG_OUTP   yLOG_note    ("update is past roll number, can't do this");   <* 
       *>    DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);                             <* 
       *>    return rce;                                                                <* 
       *> }                                                                             <*/
      a = b + 1;
      x_aft = TRKS__num2count  (a);
      b_str [x_off ] = x_aft;
      DEBUG_OUTP    yLOG_complex ("specific"  , "%2do, %2da=%c", x_off, a, x_aft);
   }
   /*---(update runs)--------------------*/
   if (a_code == KHRONOS_BEG) {
      DEBUG_OUTP    yLOG_note    ("increment runs");
      x_chr     = TRKS__num2count (++x_runs);
      b_str [3] = x_chr;
   }
   /*---(also update active)-------------*/
   if (a_code == KHRONOS_BEG) {
      DEBUG_OUTP    yLOG_note    ("increment actives");
      x_cha     = TRKS__num2count (++x_actv);
      b_str [5] = x_cha;
   } else  {
      DEBUG_OUTP    yLOG_note    ("decrement actives");
      x_cha     = TRKS__num2count (--x_actv);
      b_str [5] = x_cha;
   }
   DEBUG_OUTP    yLOG_complex ("runs/actv" , "%3dR=%c, %3dA=%c", x_runs, x_chr, x_actv, x_cha);
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__stat_restat       (char *b_str, char a_roll, char a_hist)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   char        i           =    0;
   int         x_code      =    0;
   char        t           [LEN_FULL]  = "";
   char        x_actv      =    0;
   char        x_max       =    0;
   char        x_off       =    0;
   uchar       n, m;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(capture history)----------------*/
   ystrlcpy (t, b_str + 35, LEN_FULL);
   l = strlen (t);
   DEBUG_OUTP    yLOG_complex ("t"         , "%2då%sæ", l, t);
   /*---(check active)-------------------*/
   x_actv = b_str [5];
   DEBUG_OUTP    yLOG_char    ("x_actv"    , x_actv);
   /*---(refresh stats)------------------*/
   sprintf (b_str, "%33.33s  %s", KHRONOS_DEFSTATS, t);
   b_str [0] = TRKS__num2count (a_roll);
   if (b_str [0] == '·')  b_str [0] = '´';
   b_str [5] = x_actv;
   DEBUG_OUTP    yLOG_complex ("b_str"     , "%2då%sæ", strlen (b_str), b_str);
   /*---(set offset)---------------------*/
   if (a_roll < a_hist) {
      x_max  = a_roll;
      x_off  = 2 + (a_hist - a_roll);
   } else {
      x_max  = a_hist;
      x_off  = 2;
   }
   /*---(run history)--------------------*/
   --rce;  for (i = 0; i < x_max; ++i) {
      rc = TRKS__stat_update (b_str, KHRONOS_BEG, a_roll);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_note    ("failed to begin a job");
         ystrlcpy (b_str, KHRONOS_DEFSTATS, LEN_FULL);
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      x_code = t [i + x_off];
      rc = TRKS__stat_update (b_str, x_code, a_roll);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_complex ("failed"    , "at pos %d, could not update with (%c), stopping", i, t [i]);
         ystrlcpy (b_str, KHRONOS_DEFSTATS, LEN_FULL);
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(update total runs)--------------*/
   n = TRKS__count2num (b_str [3]);
   m = TRKS__count2num (b_str [5]);
   DEBUG_OUTP    yLOG_complex ("jobs"      , "%c, %3d, %c, %3d, %3d", b_str [3], n, b_str [5], m, n + m);
   b_str [3] = TRKS__num2count (n + m);
   if (b_str [3] == '0')  b_str [3] == '·';
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__stat_handler      (char a_type, char *b_str, char a_code)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_roll      =    0;
   char        x_hist      =    0;
   /*---(quick-out)----------------------*/
   if (a_code == KHRONOS_ACTV)  return 0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = TRKS__stat_prepare (b_str, a_code, &x_roll, &x_hist);
   DEBUG_OUTP    yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(history)------------------------*/
   if (a_type != 'r') {
      rc = TRKS__stat_history (b_str, a_code, &x_hist);
      DEBUG_OUTP    yLOG_value   ("history"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(update one)---------------------*/
   if (a_type != 'r') {
      rc = TRKS__stat_update  (b_str, a_code, x_roll);
      DEBUG_OUTP    yLOG_value   ("update"    , rc);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(update all)---------------------*/
   --rce;  if (x_roll <= x_hist || a_type == 'r') {
      rc = TRKS__stat_restat  (b_str, x_roll, x_hist);
      DEBUG_OUTP    yLOG_value   ("restat"    , rc);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__stat              (char *b_str, char a_code)
{
   return TRKS__stat_handler  ('-', b_str, a_code);
}

char
TRKS_restat             (char *b_str)
{
   return TRKS__stat_handler  ('r', b_str, KHRONOS_BEG);
}



/*====================------------------------------------====================*/
/*===----                         durations                            ----===*/
/*====================------------------------------------====================*/
static void  o___DURS____________o () { return; }

#define    KHRONOS_DURS  "<nmlkjihgfedcba0ABCDEFGHIJKLMN>"

/*> < -30     -10        0         10      30 >  --history--->>>                                                    <* 
 *> · · · · · ·········· · ·········· · · · · ·  · ´····´····´····´····´····´····´····´····´····´····´····´····´"   <* 
 *> -123456789-123456789-123456789-123456789-12                                                                     <* 
 *> 0         1         2         3         4                                                                       <*/

/*> ´  · ·  · · · · · ·········· · ·········· · · · · ·  ·)´····´····´····´····´····´····´····´····´····´····´····´····´   <* 
 *> -123456789-123456789-123456789-123456789-123456789-123456789-123456789                                                 <* 
 *> 0         1         2         3         4         5         6                                                          <*/

char
TRKS__durs_prepare      (char *b_str, int *b_dur, char *r_roll, char *r_hist, int *b_min, int a_est, int *b_max)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_roll      =    0;
   char        x_hist      =    0;
   char        i           =    0;
   char        x_check     =    0;
   char        l           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_roll != NULL)  *r_roll = 0;
   if (r_hist != NULL)  *r_hist = 0;
   /*---(defense)------------------------*/
   rc = TRKS__coded ('d', KHRONOS_DEFDURS  , b_str);
   DEBUG_OUTP    yLOG_value   ("coded"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   l  = strlen (b_str);
   /*---(get roll)-----------------------*/
   if      (b_str [0] == '´')  x_roll = 60;
   else if (b_str [0] == '·')  x_roll = 60;
   else if (b_str [0] == '[')  x_roll =  1;
   else if (b_str [0] == '<')  x_roll = 15;
   else if (b_str [0] == '|')  x_roll = 30;
   else if (b_str [0] == '>')  x_roll = 45;
   else if (b_str [0] == ']')  x_roll = 60;
   else                        x_roll  = TRKS__count2num (b_str [ 0]);
   if (x_roll > 60)  x_roll = 60;
   DEBUG_OUTP    yLOG_complex ("x_roll"    , "%c, %d", b_str [ 0], x_roll);
   --rce;  if (x_roll < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get history)--------------------*/
   x_hist  = TRKS__count2num (b_str [53]);
   DEBUG_OUTP    yLOG_complex ("x_hist"    , "%c, %d", b_str [53], x_hist);
   --rce;  if (x_hist < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify history string)----------*/
   for (i = 0; i < 60; ++i) {
      if (strchr (YSTR_BIGGUN, b_str [i + 55]) != NULL)  ++x_check;
      else break;
   }
   DEBUG_OUTP    yLOG_value   ("check"     , x_check);
   --rce;  if (x_hist != x_check) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP    yLOG_char    ("tail"      , b_str [l - 1]);
   --rce;  if (b_str [l - 1] != '´') {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(verify duration)----------------*/
   DEBUG_OUTP    yLOG_value   ("*b_dur"    , *b_dur);
   --rce;  if (*b_dur < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = 0;
   if (*b_dur >= strlen (YSTR_BIGGUN)) {
      *b_dur = strlen (YSTR_BIGGUN) - 1;
      rc = 1;
   }
   /*---(fix range)----------------------*/
   DEBUG_OUTP    yLOG_value   ("b_min"     , *b_min);
   DEBUG_OUTP    yLOG_value   ("a_est"     ,  a_est);
   DEBUG_OUTP    yLOG_value   ("b_max"     , *b_max);
   *b_min /= 1000;
   *b_max /= 1000;
   if (b_min != NULL && *b_min > a_est)  *b_min = a_est;
   if (b_max != NULL && *b_max < a_est)  *b_max = a_est;
   /*---(save-back)----------------------*/
   if (r_roll != NULL)  *r_roll = x_roll;
   if (r_hist != NULL)  *r_hist = x_hist;
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return rc;
}

char
TRKS__durs_history      (char *b_str, int a_dur, char *b_hist)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_next      =    0;
   char        i           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(get history)--------------------*/
   x_next  = *b_hist + 55;
   DEBUG_OUTP    yLOG_complex ("history"   , "%c, %d, %d", b_str [55], *b_hist, x_next);
   /*---(not full yet)-------------------*/
   if (*b_hist < 60) {
      b_str [x_next] = TRKS__num2biggun (a_dur);
      b_str [53    ] = TRKS__num2count  (++(*b_hist));
      DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(full)---------------------------*/
   for (i = 1; i < 60; ++i)  b_str [i + 55 - 1] = b_str [i + 55];
   b_str [59 + 55] = TRKS__num2biggun (a_dur);
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 2;
}

char
TRKS__durs_update       (char *b_str, int a_dur, int a_min, int a_est, int a_max)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_var       =    0;
   char        x_off       =    0;
   char        n           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   x_var = a_dur - a_est;
   DEBUG_OUTP    yLOG_value   ("x_var"     , x_var);
   /*---(negatives)----------------------*/
   if      (x_var  <  -30)   x_off =  0; 
   else if (x_var  <  -25)   x_off =  2; 
   else if (x_var  <  -20)   x_off =  4; 
   else if (x_var  <  -15)   x_off =  6; 
   else if (x_var  <  -10)   x_off =  8; 
   else if (x_var  <    0)   x_off = 20 + x_var;
   /*---(zero)---------------------------*/
   else if (x_var  ==   0)   x_off = 21;
   /*---(positives)----------------------*/
   else if (x_var  >   30)   x_off = 42;
   else if (x_var  >   25)   x_off = 40;
   else if (x_var  >   20)   x_off = 38;
   else if (x_var  >   15)   x_off = 36;
   else if (x_var  >   10)   x_off = 34;
   else                      x_off = 22 + x_var;
   /*---(update stats)-------------------*/
   DEBUG_OUTP    yLOG_value   ("x_off"     , x_off);
   x_off += 8;
   n = TRKS__count2num (b_str [x_off]);
   DEBUG_OUTP    yLOG_char    ("n"         , n);
   b_str [x_off ] = TRKS__num2count (++n);
   /*---(early/late)---------------------*/
   DEBUG_OUTP    yLOG_complex ("durs"      , "%7dd, %7dn, %7de, %7dx", a_dur, a_min, a_est, a_max);
   if (a_dur < a_min) {
      DEBUG_OUTP    yLOG_note    ("register/update early");
      n = TRKS__count2num (b_str [3]);
      b_str [3] = TRKS__num2count (++n);
   }
   if (a_dur > a_max) {
      DEBUG_OUTP    yLOG_note    ("register/update late");
      n = TRKS__count2num (b_str [5]);
      b_str [5] = TRKS__num2count (++n);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__durs_redur        (char *b_str, char a_roll, char a_hist, int a_min, int a_est, int a_max)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l           =    0;
   char        i           =    0;
   char        t           [LEN_FULL]  = "";
   char        x_max       =    0;
   char        x_off       =    0;
   int         x_dur       =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(capture history)----------------*/
   ystrlcpy (t, b_str + 53, LEN_FULL);
   l = strlen (t);
   DEBUG_OUTP    yLOG_complex ("t"         , "%2då%sæ", l, t);
   /*---(refresh stats)------------------*/
   sprintf (b_str, "%51.51s  %s", KHRONOS_DEFDURS, t);
   b_str [0] = TRKS__num2count (a_roll);
   if (b_str [0] == '·')  b_str [0] = '´';
   DEBUG_OUTP    yLOG_complex ("b_str"     , "%2då%sæ", strlen (b_str), b_str);
   /*---(set offset)---------------------*/
   if (a_roll < a_hist) {
      x_max  = a_roll;
      x_off  = 2 + (a_hist - a_roll);
   } else {
      x_max  = a_hist;
      x_off  = 2;
   }
   /*---(run history)--------------------*/
   --rce;  for (i = 0; i < x_max; ++i) {
      x_dur  = TRKS__biggun2num (t [i + x_off]);
      rc = TRKS__durs_update (b_str, x_dur, a_min, a_est, a_max);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_complex ("failed"    , "at pos %d, could not update with (%c), stopping", i, t [i]);
         ystrlcpy (b_str, KHRONOS_DEFDURS , LEN_FULL);
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__durs_handler      (char a_type, char *b_str, int a_dur, int a_min, int a_est, int a_max)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_roll      =    0;
   char        x_hist      =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   DEBUG_OUTP    yLOG_complex ("durs"      , "%7dd, %7dn, %7de, %7dx", a_dur, a_min, a_est, a_max);
   rc = TRKS__durs_prepare (b_str, &a_dur, &x_roll, &x_hist, &a_min, a_est, &a_max);
   DEBUG_OUTP    yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP    yLOG_complex ("durs"      , "%7dd, %7dn, %7de, %7dx", a_dur, a_min, a_est, a_max);
   /*---(history)------------------------*/
   if (a_type != 'r') {
      rc = TRKS__durs_history (b_str, a_dur, &x_hist);
      DEBUG_OUTP    yLOG_value   ("history"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(update one)---------------------*/
   if (a_type != 'r') {
      rc = TRKS__durs_update  (b_str, a_dur, a_min, a_est, a_max);
      DEBUG_OUTP    yLOG_value   ("update"    , rc);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(update all)---------------------*/
   --rce;  if (x_roll <= x_hist || a_type == 'r') {
      rc = TRKS__durs_redur   (b_str, x_roll, x_hist, a_min, a_est, a_max);
      DEBUG_OUTP    yLOG_value   ("restat"    , rc);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__durs              (char *b_str, int a_dur, int a_min, int a_est, int a_max)
{
   return  TRKS__durs_handler ('-', b_str, a_dur, a_min, a_est, a_max);
}

char
TRKS_redur              (char *b_str, int a_min, int a_est, int a_max)
{
   return  TRKS__durs_handler ('r', b_str, 0, a_min, a_est, a_max);
}



/*====================------------------------------------====================*/
/*===----                      schedule tracking                       ----===*/
/*====================------------------------------------====================*/
static void      o___ACTUAL__________________o (void) {;}

/*>                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       <* 
 *> ´¬¬¬  ¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> -123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              <* 
 *> 0         1         2         3         4         5                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   <* 
 *>                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       <*/

/*>                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       <* 
 *> ´¬¬¬  ¨···· €€€>+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> ´¬¬¬  ¨····º===»+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> ´¬¬¬  ¨····Ï´´´´+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> ´¬¬¬  ¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> ´¬¬¬  ¨····¼€€€>+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> ´¬¬¬  ¨····¼´´´½+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> ´¬¬¬  ¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> ´¬¬¬  ¨····°····+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> ´¬¬¬  ¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> ´¬¬¬  ¨····Ï´´´´´´´´´·····+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> ´¬¬¬  ¨····¼€€€€€€€€½·····+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> ´¬¬¬  ¨····¼€€€½´´´´´·····+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *> ´¬¬¬  ¨····Ï´´¼€€€€€½·····+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨·········+·········+·········|·········+·········+·········¨   <* 
 *>                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       <*/

static int      s_rem  = 0;   /* remaining minutes in a jobs duration         */

char
TRKS__actual_reset      (tTRKS *a_trks)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_trks"    , a_trks);
   --rce;  if (a_trks == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(default actuals)----------------*/
   ystrlcpy (a_trks->actual, KHRONOS_DEFACTUAL, LEN_RECD);
   /*---(reset remaining)----------------*/
   s_rem = 0;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS_actual_reset       (void)
{
   tTRKS      *x_cur       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   x_cur = s_head;
   while (x_cur != NULL) {
      DEBUG_INPT   yLOG_info    ("x_cur"     , x_cur->tracker);
      TRKS__actual_reset (x_cur);
      x_cur = x_cur->m_next;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__planning_hour     (tTRKS *a_trks, char a_vis, char a_yr, char a_mo, char a_dy, char a_hr, char *a_out)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        i           =    0;
   int         n           =    0;
   char        x_hr        =    0;
   char        rc          =    0;
   char        x_type      =  '·';
   char        x_ch        =  '-';
   char        x_yes       =    0;
   tLINE      *x_line      = NULL;
   uchar      *x_hour      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_trks"    , a_trks);
   --rce;  if (a_trks == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_line = a_trks->parent;
   DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
   --rce;  if (x_line == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set date)-----------------------*/
   rc = ySCHED_date (a_yr, a_mo, a_dy);
   DEBUG_INPT   yLOG_value   ("date"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   x_type = TRKS__typing (x_line->tracker);
   /*> if      (strncmp (x_line->tracker, ".graceful", 9) == 0) x_type = '·';         <* 
    *> else if (strncmp (x_line->tracker, ".violent" , 8) == 0) x_type = '·';         <* 
    *> else if (x_line->tracker [0] == '.')                     x_type = 'V';         <* 
    *> else                                                     x_type = '·';         <*/
   if (a_vis == 'y') {
      if      (a_hr == 25) { x_hr = 24;  a_hr = 20; }
      else if (a_hr >= 20)   x_hr = a_hr - 20;
      else                   x_hr = a_hr +  4;
   } else x_hr = 24;
   /*---(populate base a_out)------------*/
   x_hour = a_trks->actual + x_hr * 60 + 6;
   /*---(cycle minutes)------------------*/
   for (i = 0; i < 60; ++i) {
      /*---(test)------------------------*/
      n    = x_hr * 60 + i;
      x_ch = x_hour [i];
      x_yes    = 0;
      if (a_vis == 'y' && strchr ("¨|+·Ï´", x_ch) != NULL)  x_yes = 1;
      rc   = ySCHED_test (x_line->sched, a_hr, i, NULL);
      DEBUG_INPT   yLOG_complex ("test"      , "%2dy, %2do, %2dd, %2dh, %2dm, %4dn, %d", a_yr, a_mo, a_dy, a_hr, i, n, rc);
      /*---(trouble)---------------------*/
      if (rc <  0)  {
         DEBUG_INPT   yLOG_note    ("handle trouble");
         if (x_yes)  x_hour [i] =  '¢';
         continue;
      }
      /*---(validity lines)--------------*/
      if (x_type == KHRONOS_VALIDITY) {
         DEBUG_INPT   yLOG_note    ("handle viability");
         if (x_yes) {
            if (rc >  0) {
               if (strchr ("‡²", x_hour [i - 1]) == NULL)  x_hour [i] =  '‡';
               else                                        x_hour [i] =  '²';
            } else {
               x_hour [i] =  '­';
               if (strchr ("‡²", x_hour [i - 1]) != NULL)  x_hour [i -1 ] =  '†';
            }
         }
         continue;
      }
      /*---(no-match, continue)----------*/
      if (rc == 0) {
         DEBUG_INPT   yLOG_note    ("handle continuence");
         if (s_rem > 0) {
            if (x_yes)  x_hour [i] =  '´';
            s_rem -= 60;
         }
         continue;
      }
      /*---(must skip run)---------------*/
      if (s_rem > 0)  {
         DEBUG_INPT   yLOG_note    ("handle skip");
         if (x_yes)  x_hour [i] =  '¥';
         s_rem -= 60;
         continue;
      }
      /*---(normal)----------------------*/
      DEBUG_INPT   yLOG_note    ("handle normal");
      if (x_yes)  x_hour [i] =  'Ï';
      s_rem = x_line->est - 60;
      /*---(done)------------------------*/
   }
   /*---(save back)----------------------*/
   if (a_out != NULL)  ystrlcpy (a_out, x_hour, 61);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__planning          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTRKS      *x_cur       = NULL;
   int         i           =    0;
   char        x_yr, x_mo, x_dy;
   char        x_hr        =    0;
   char        x_vis       =  '-';
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(get to evening shift day)-------*/
   DEBUG_OUTP    yLOG_value   ("my.hour"   , my.hour);
   if (my.hour >= 20) rc = RPTG_by_min__offset  (0, my.year, my.month, my.day, -1, NULL, &x_yr, &x_mo, &x_dy);
   else               rc = RPTG_by_min__offset  (0, my.year, my.month, my.day,  0, NULL, &x_yr, &x_mo, &x_dy);
   DEBUG_OUTP    yLOG_value   ("offset"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_OUTP    yLOG_complex ("date"      , "%2dy, %2dm, %2dd", x_yr, x_mo, x_dy);
   /*---(walk trackers)---------------*/
   x_cur = s_head;
   while (x_cur != NULL) {
      s_rem = 0;
      DEBUG_OUTP    yLOG_complex ("tracker"   , "%s, %s", x_cur->file, x_cur->tracker);
      /*---(walk hours)---------------------*/
      for (i = 0; i < 26; ++i) {
         /*---(move day at midnight)--------*/
         if (i == 5)   RPTG_by_min__offset  (0, x_yr, x_mo, x_dy,  1, NULL, &x_yr, &x_mo, &x_dy);
         /*---(set hour)--------------------*/
         if      (i    ==  0)  { x_hr = 19;     x_vis = '-'; }
         else if (i    <=  4)  { x_hr = i + 19; x_vis = 'y'; }
         else if (i    <  25)  { x_hr = i -  5; x_vis = 'y'; }
         else                  { x_hr = 25;     x_vis = 'y'; }
         DEBUG_OUTP    yLOG_complex ("x_hour"    , "%2dy, %2dm, %2dd, %2dh, %c", x_yr, x_mo, x_dy, x_hr, x_vis);
         TRKS__planning_hour (x_cur, x_vis, x_yr, x_mo, x_dy, x_hr, NULL);
         /*---(done)--------------------*/
      }
      RPTG_by_min__offset  (0, x_yr, x_mo, x_dy, -1, NULL, &x_yr, &x_mo, &x_dy);
      x_cur = x_cur->m_next;
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS_plan               (void)
{
   TRKS_actual_reset ();
   return TRKS__planning ();
}

char
TRKS_replan             (void)
{
   TRKS_unplan ();
   return TRKS__planning ();
}

char
TRKS_now                (void)
{
   tTRKS      *x_cur       = NULL;
   tTRKS      *x_prv       = NULL;
   uchar      *p           = NULL;
   int         n           =    0;
   char        x_def       =    0;
   char        x_ch        =  '-';
   char        x_hr        =    0;
   char        x_usable    =  'y';
   char        x_type      =  '·';
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (my.hour >= 20)  x_hr = my.hour - 20;
   else                x_hr = my.hour + 4;
   x_cur = s_head;
   /*---(walk trackers)------------------*/
   while (x_cur != NULL) {
      DEBUG_INPT   yLOG_info    ("x_cur"     , x_cur->tracker);
      x_type = TRKS__typing (x_cur->tracker);
      /*---(switching files)-------------*/
      n = 6 + x_hr * 60 + my.minute;
      if (x_prv == NULL || strcmp (x_prv->file, x_cur->file) != 0) {
         x_usable = 'y';
      }
      /*---(ckeck validity)--------------*/
      if (x_type == KHRONOS_VALIDITY) {
         if (x_cur->actual [n] == (uchar) '­')  x_usable = '-';
      }
      /*---(remove from normal)----------*/
      if (x_type != KHRONOS_VALIDITY) {
         p = strchr (x_cur->actual, '');
         if (p == NULL)  p = strchr (x_cur->actual, '');
         while (p != NULL) {
            n = p - x_cur->actual;
            if      ((n - 6) % 60 == 0)  x_def = '¨';
            else if ((n - 6) % 30 == 0)  x_def = '|';
            else if ((n - 6) % 10 == 0)  x_def = '+';
            else                         x_def = '·';
            x_cur->actual [n] = x_def;
            p = strchr (x_cur->actual, '');
            if (p == NULL)  p = strchr (x_cur->actual, '');
         }
      }
      /*---(remove from validity)--------*/
      else {
         p = strchr (x_cur->actual, 'Š');
         if (p == NULL)  p = strchr (x_cur->actual, '');
         while (p != NULL) {
            n = p - x_cur->actual;
            if (x_cur->actual [n] == (uchar)'Š')   x_cur->actual [n] = '²';
            else                                   x_cur->actual [n] = '­';
            p = strchr (x_cur->actual, 'Š');
            if (p == NULL)  p = strchr (x_cur->actual, '');
         }
      }
      /*---(place new)-------------------*/
      n = 6 + x_hr * 60 + my.minute;
      x_ch = x_cur->actual [n];
      if (x_usable == 'y') {
         if (strchr ("¨|+·"  , x_ch) != NULL)   x_cur->actual [n] = '';
         if (strchr ("²"     , x_ch) != NULL)   x_cur->actual [n] = 'Š';
      } else {
         if (strchr ("¨|+·­²", x_ch) != NULL)   x_cur->actual [n] = '';
      }
      /*---(next)------------------------*/
      x_prv = x_cur;
      x_cur = x_cur->m_next;
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS__unplan_one        (tTRKS *a_trks)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         l1, l2;
   char        x_hr        =    0;
   char        x_min       =    0;
   char        x_def       =    0;
   int         n           =    0;
   char        x_ch        =  '-';
   char        x_type      =  '·';
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_trks"    , a_trks);
   --rce;  if (a_trks == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   l1 = strlen (a_trks->actual);
   DEBUG_INPT   yLOG_value   ("l1"        , l1);
   l2 = strlen (KHRONOS_DEFACTUAL);
   DEBUG_INPT   yLOG_value   ("l2"        , l2);
   --rce;  if (l1 != l2) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_type = TRKS__typing (a_trks->tracker);
   /*---(walk actual)--------------------*/
   for (x_hr = 0; x_hr < 25; ++x_hr) {
      for (x_min = 0; x_min < 60; ++x_min) {
         /*---(get current)--------------*/
         n = 6 + x_hr * 60 + x_min;
         x_ch = a_trks->actual [n];
         /*---(pick default)-------------*/
         if      (x_min ==  0)      x_def = '¨';
         else if (x_min == 30)      x_def = '|';
         else if (x_min % 10 == 0)  x_def = '+';
         else                       x_def = '·';
         /*---(unplan)-------------------*/
         if (x_type == KHRONOS_VALIDITY)               a_trks->actual [n] = x_def;
         else if (strchr ("¨|+·Ï´¥­²", x_ch) != NULL)  a_trks->actual [n] = x_def;
         /*---(done)---------------------*/
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TRKS_unplan             (void)
{
   tTRKS      *x_cur       = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   x_cur = s_head;
   while (x_cur != NULL) {
      DEBUG_INPT   yLOG_info    ("x_cur"     , x_cur->tracker);
      TRKS__unplan_one (x_cur);
      x_cur = x_cur->m_next;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char*
TRKS_actual_shift       (char *a_file, char *a_tracker, char a_shift)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tLINE      *x_line      = NULL;
   int         n           =    0;
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = yDLST_list_by_name (a_file, NULL, NULL);
   DEBUG_OUTP    yLOG_value   ("by_name"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return "";
   }
   rc = yDLST_line_by_name (YDLST_LOCAL, a_tracker, NULL, &x_line);
   DEBUG_OUTP    yLOG_value   ("by_name"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return "";
   }
   /*---(check existance)----------------*/
   DEBUG_OUTP    yLOG_info    ("->file"    , x_line->trks->file);
   DEBUG_OUTP    yLOG_info    ("->tracker" , x_line->trks->tracker);
   l = strlen (x_line->trks->actual);
   DEBUG_OUTP    yLOG_value   ("l"         , l);
   /*---(work offset)--------------------*/
   DEBUG_OUTP    yLOG_value   ("a_shift"   , a_shift);
   --rce;  if (a_shift < 0 || a_shift > 8) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return "";
   }
   n = 6 + a_shift * (60 * 3);
   DEBUG_OUTP    yLOG_value   ("n"         , n);
   /*---(save-back)----------------------*/
   ystrlcpy (g_print, x_line->trks->actual + n, 182);
   DEBUG_OUTP    yLOG_info    ("g_print"   , g_print);
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return g_print;
}

char*
TRKS_actual_shift_OLD   (char *a_tracker, char a_shift)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tLINE      *x_line      = NULL;
   int         n           =    0;
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = yDLST_line_by_name (YDLST_GLOBAL, a_tracker, NULL, &x_line);
   DEBUG_OUTP    yLOG_value   ("by_name"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return "";
   }
   /*---(check existance)----------------*/
   DEBUG_OUTP    yLOG_info    ("->file"    , x_line->trks->file);
   DEBUG_OUTP    yLOG_info    ("->tracker" , x_line->trks->tracker);
   l = strlen (x_line->trks->actual);
   DEBUG_OUTP    yLOG_value   ("l"         , l);
   /*---(work offset)--------------------*/
   DEBUG_OUTP    yLOG_value   ("a_shift"   , a_shift);
   --rce;  if (a_shift < 0 || a_shift > 8) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return "";
   }
   n = 6 + a_shift * (60 * 3);
   DEBUG_OUTP    yLOG_value   ("n"         , n);
   /*---(save-back)----------------------*/
   ystrlcpy (g_print, x_line->trks->actual + n, 182);
   DEBUG_OUTP    yLOG_info    ("g_print"   , g_print);
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return g_print;
}


char
TRKS__actual            (char *b_str, char a_hr, char a_mn, char a_code)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_next      =    0;
   char        x_hr        =    0;
   int         x_min       =    0;
   int         x_off       =    0;
   char        x_cur       =    0;
   char        x_prv       =    0;
   char        t           [LEN_LABEL] = "";
   int         l           =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_OUTP    yLOG_char    ("a_code"    , a_code);
   --rce;  if (a_code == 0 || strchr (KHRONOS_STAT "a", a_code) == NULL) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = TRKS__coded ('a', KHRONOS_DEFACTUAL, b_str);
   DEBUG_OUTP    yLOG_value   ("coded"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   ystrlcpy (t, b_str, 5);
   x_next  = atoi (t);
   DEBUG_OUTP    yLOG_complex ("x_next"    , "%s, %d", t, x_next);
   if (a_hr >= 20)  x_hr = a_hr - 20;
   else             x_hr = a_hr + 4;
   x_min   = x_hr * 60 + a_mn;
   DEBUG_OUTP    yLOG_complex ("x_min"     , "%dh, %dx, %dm, %d", a_hr, x_hr, a_mn, x_min);
   --rce;  if (x_min <= 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_off   = x_min + 6;
   x_cur   = b_str [x_off];
   x_prv   = b_str [x_off - 1];
   DEBUG_OUTP    yLOG_complex ("x_cur"     , "%d, %c", x_off, x_cur);
   /*---(update actual)------------------*/
   switch (a_code) {
   case KHRONOS_BEG  :
      b_str [x_off]     = '¼';
      break;
   case KHRONOS_BADD :
      b_str [x_off]     = 'B';
      break;
   case KHRONOS_SKIP :
      b_str [x_off]     = ' ';
      break;
   case KHRONOS_ACTV :
      if      (x_prv == '¼')   ;
      else if (x_prv == ' ')   ;
      else                     b_str [x_off - 1] = '-';
      break;
   case KHRONOS_PASS :
      if      (x_prv == '¼')   b_str [x_off - 1] = 'Ù';
      else if (x_prv == ' ')   ;
      else                     b_str [x_off - 1] = '½';
      break;
   default           :
      switch (a_code) {
      case KHRONOS_BOOM : b_str [x_off - 1] = 'O';    break;
      case KHRONOS_KILL : b_str [x_off - 1] = 'K';    break;
      case KHRONOS_TERM : b_str [x_off - 1] = 'T';    break;
      case KHRONOS_LGRA : b_str [x_off - 1] = 'g';    break;
      case KHRONOS_LVIO : b_str [x_off - 1] = 'v';    break;
      case KHRONOS_GGRA : b_str [x_off - 1] = 'G';    break;
      case KHRONOS_GVIO : b_str [x_off - 1] = 'V';    break;
      case KHRONOS_FAIL : b_str [x_off - 1] = '¯';    break;
      case KHRONOS_WARN : b_str [x_off - 1] = '›';    break;
      }
      break;
   }
   /*---(update history)-----------------*/
   sprintf (t, "%d", x_min);
   l = strlen (t);
   for (i = 0; i < 5; ++i)   b_str [i] = ' ';
   for (i = 0; i < l; ++i)   b_str [i] = t [i];
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      execution support                       ----===*/
/*====================------------------------------------====================*/
static void      o___EXEC____________________o (void) {;}

char
TRKS__exec              (char *a_func, tTRKS *a_cur, char a_hr, char a_mn, char a_code, int a_dur, int a_min, int a_est, int a_max)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_OUTP    yLOG_enter   (a_func);
   DEBUG_OUTP    yLOG_complex ("durs"      , "%7dd, %7dn, %7de, %7dx", a_dur, a_min, a_est, a_max);
   /*---(defense)------------------------*/
   DEBUG_OUTP    yLOG_point   ("a_cur"     , a_cur);
   --rce;  if (a_cur == NULL) {
      DEBUG_OUTP   yLOG_exitr   (a_func, rce);
      return rce;
   }
   /*---(update heartbeat)---------------*/
   ystrlcpy (a_cur->last, my.heartbeat, 47);
   /*---(update statistics)--------------*/
   DEBUG_OUTP    yLOG_info    ("->stats"   , a_cur->stats);
   rc = TRKS__stat  (a_cur->stats, a_code);
   DEBUG_OUTP    yLOG_value   ("stats"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (a_func, rce);
      return rce;
   }
   DEBUG_OUTP    yLOG_info    ("->stats"   , a_cur->stats);
   /*---(update duration)----------------*/
   if (strchr (KHRONOS_NODUR, a_code) == NULL) {
      rc = TRKS__durs   (a_cur->durs, a_dur, a_min, a_est, a_max);
      DEBUG_OUTP    yLOG_value   ("durs"      , rc);
      --rce;  if (rc < 0) {
         DEBUG_OUTP   yLOG_exitr   (a_func, rce);
         return rce;
      }
   }
   /*---(update actual)------------------*/
   rc = TRKS__actual (a_cur->actual, a_hr, a_mn, a_code);
   DEBUG_OUTP    yLOG_value   ("actual"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (a_func, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP    yLOG_exit    (a_func);
   return 0;
}

char
TRKS_launch             (tTRKS *a_cur, char a_hr, char a_mn)
{
   return TRKS__exec (__FUNCTION__, a_cur, a_hr, a_mn, KHRONOS_BEG , 0, 0, 0, 0);
}

char
TRKS_running            (tTRKS *a_cur, char a_hr, char a_mn)
{
   return TRKS__exec (__FUNCTION__, a_cur, a_hr, a_mn, KHRONOS_ACTV, 0, 0, 0, 0);
}

char
TRKS_complete           (tTRKS *a_cur, char a_hr, char a_mn, char a_code, int a_dur, int a_min, int a_est ,int a_max)
{
   return TRKS__exec (__FUNCTION__, a_cur, a_hr, a_mn, a_code, a_dur, a_min, a_est, a_max);
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
TRKS__unit              (char *a_question, int a_num)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_exist     =  '-';
   int         x_lines     =    0;
   char        x_heartbeat [LEN_HUND];
   tTRKS      *x_cur       = NULL;
   char        s           [LEN_HUND]  = "";
   char        t           [LEN_FULL]  = "";
   char        r           [LEN_LABEL] = "";
   int         x_fore      =    0;
   int         x_back      =    0;
   /*---(prepare)------------------------*/
   ystrlcpy  (unit_answer, "RPTG             : question not understood", LEN_HUND);
   /*---(simple requests)----------------*/
   if      (strcmp (a_question, "file"          )  == 0) {
      x_lines = yENV_lines (s_name);
      if (x_lines < 0)    x_lines = 0;
      else                x_exist = 'y';
      if (f_trks == NULL)  snprintf (unit_answer, LEN_HUND, "TRKS file        : %c  %3d  null            %2då%sæ", x_exist, x_lines, strlen (s_name), s_name);
      else                 snprintf (unit_answer, LEN_HUND, "TRKS file        : %c  %3d  %-12.12p  %2då%sæ", x_exist, x_lines, f_trks, strlen (s_name), s_name);
   }
   else if (strcmp (a_question, "count"         )  == 0) {
      x_cur = s_head; while (x_cur != NULL) { ++x_fore; x_cur = x_cur->m_next; }
      x_cur = s_tail; while (x_cur != NULL) { ++x_back; x_cur = x_cur->m_prev; }
      snprintf (unit_answer, LEN_HUND, "TRKS count       : %3da, %3df, %3db", s_count, x_fore, x_back);
   }
   else if (strcmp (a_question, "entry"         )  == 0) {
      rc = TRKS__by_index (a_num, &x_cur);
      if (x_cur  == NULL)  snprintf (unit_answer, LEN_HUND, "TRKS entry  (%2d) :  ·åæ                            ·åæ                            · · · ·   ·", a_num);
      else {
         sprintf  (s, "%2då%-.25sæ", strlen (x_cur->file)   , x_cur->file);
         sprintf  (t, "%2då%-.25sæ", strlen (x_cur->tracker), x_cur->tracker);
         strcpy (r, "· · · ·   ·");
         if (strcmp (x_cur->last  , KHRONOS_DEFLAST  ) != 0)       r [ 0] = 'Ï';
         if (strcmp (x_cur->stats + 1, KHRONOS_DEFSTATS + 1) != 0) r [ 2] = 'Ï';
         else if (x_cur->stats [0] != (uchar) '´')                 r [ 2] = 'r';
         if (strcmp (x_cur->durs  + 1, KHRONOS_DEFDURS  + 1) != 0) r [ 4] = 'Ï';
         else if (x_cur->durs  [0] != (uchar) '´')                 r [ 4] = 'r';
         if (strcmp (x_cur->actual, KHRONOS_DEFACTUAL) != 0)       r [ 6] = 'Ï';
         if (x_cur->parent != NULL)                                r [10] = 'Ï';
         snprintf (unit_answer, LEN_HUND, "TRKS entry  (%2d) : %-29.29s  %-29.29s   %s", a_num, s, t, r);
      }

   }
   else if (strcmp (a_question, "last"          )  == 0) {
      rc = TRKS__by_index (a_num, &x_cur);
      if (x_cur  == NULL)  snprintf (unit_answer, LEN_FULL, "TRKS last   (%2d) : åæ", a_num);
      else                 snprintf (unit_answer, LEN_FULL, "TRKS last   (%2d) : å%sæ", a_num, x_cur->last);
   }
   else if (strcmp (a_question, "stats"         )  == 0) {
      rc = TRKS__by_index (a_num, &x_cur);
      if (x_cur  == NULL)  snprintf (unit_answer, LEN_FULL, "TRKS stats  (%2d) : åæ", a_num);
      else                 snprintf (unit_answer, LEN_FULL, "TRKS stats  (%2d) : å%sæ", a_num, x_cur->stats);
   }
   else if (strcmp (a_question, "durs"          )  == 0) {
      rc = TRKS__by_index (a_num, &x_cur);
      if (x_cur  == NULL)  snprintf (unit_answer, LEN_FULL, "TRKS durs   (%2d) : åæ", a_num);
      else                 snprintf (unit_answer, LEN_FULL, "TRKS durs   (%2d) : å%sæ", a_num, x_cur->durs);
   }
   else if (strcmp (a_question, "actual"        )  == 0) {
      rc = TRKS__by_index (a_num, &x_cur);
      if (x_cur  == NULL)  snprintf (unit_answer, LEN_FULL, "TRKS actual (%2d) : åæ", a_num);
      else {
         ystrlcpy  (t, x_cur->actual + 6 + 60 * 4, LEN_HUND);
         snprintf (unit_answer, LEN_FULL, "TRKS actual (%2d) : å%sæ", a_num, t);
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


