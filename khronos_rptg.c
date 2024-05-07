/*===========================[[ start-of-code ]]==============================*/
#include   "khronos.h"


/*-----------------------------------------------
 *
 *  21.11.30.02.15.00  12345  BEG
 *  21.11.30.02.15.00  12345  root.khronos_testing              1 always                         skip      ·     ·    ·  å21.11.30.02.15.01æ
 *  21.11.30.02.15.00  12345  root.khronos_testing              1 always                         pass  12345   120    0  å21.11.30.02.15.01æ
 *  21.11.30.02.15.00  12345  SIGTERM
 *  21.11.30.02.15.00  12345  root.khronos_testing              1 always                         fail  12345    60  -12  å21.11.30.02.15.01æ
 *  21.11.30.02.15.00  12345  root.khronos_testing              1 always                         boom  12345    60   11  å21.11.30.02.15.01æ
 *  21.11.30.02.15.00  12345  root.khronos_testing              1 always                         kill  12345   240    9  å21.11.30.02.15.01æ
 *  21.11.30.02.15.02  12345  END
 *
 *
 *
 */


/*>                                                                                                                                                                                          <* 
 *> daily for six months -- like just for validity dates                                                                                                                                     <* 
 *>                                                                                                                                                                                          <* 
 *> ¨-------------jan--------------¨------------feb-------------¨-------------mar--------------¨------------apr--------------¨-------------may--------------¨------------jun--------------   <* 
 *>                                                                                                                                                                                          <*/

/*>                                                                                                                                                                             <* 
 *> hourly for week                                                                                                                                                             <* 
 *>                                                                                                                                                                             <* 
 *> ¨-- jul 23, 2022 (we) --¨-- jul 24, 2022 (th) --¨-- jul 25, 2022 (fi) --¨-- jul 26, 2022 (sa) --¨-- jul 27, 2022 (su) --¨-- jul 28, 2022 (mo) --¨-- jul 29, 2022 (tu) --¨   <* 
 *> ¨·········1·········2···¨·········1·········2···¨·········1·········2···¨·········1·········2···¨·········1·········2···¨·········1·········2···¨·········1·········2···¨   <* 
 *> ¨123456789-123456789-123¨123456789-123456789-123¨123456789-123456789-123¨123456789-123456789-123¨123456789-123456789-123¨123456789-123456789-123¨123456789-123456789-123¨   <* 
 *> ´´´´´´¬¬¬¬¬¬¬¬¬¬¬¬¬´´´´´´´´´´´¬¬¬¬¬¬¬¬¬¬¬¬¬´´´´´´´´´´´¬¬¬¬¬¬¬¬¬¬¬¬¬´´´´´´´´´´´¬¬¬¬¬¬¬¬¬¬¬¬¬´´´´´´´´´´´¬¬¬¬¬¬¬¬¬¬¬¬¬´´´´´´´´´´´¬¬¬¬¬¬¬¬¬¬¬¬¬´´´´´´´´´´´¬¬¬¬¬¬¬¬¬¬¬¬¬´´´´´    <* 
 *> ·Ï······································································································································································    <* 
 *> ··Ï·····································································································································································    <* 
 *> ··Ï·····································································································································································    <* 
 *> ··Ï·····································································································································································    <* 
 *> ···Ï····································································································································································    <* 
 *> ···Ï····································································································································································    <* 
 *> ····Ï···································································································································································    <* 
 *>                                                                                                                                                                             <*/

/*>                                                                                                                                                     <* 
 *> 30 min for three days                                                                                                                               <* 
 *>                                                                                                                                                     <* 
 *> ¨------------- jul 23, 2022 (we) ---------------¨------------- jul 23, 2022 (th) ---------------¨------------- jul 23, 2022 (fi) ---------------¨   <* 
 *> ¨···················1···················2·······¨···················1···················2·······¨···················1···················2·······¨   <* 
 *> ¨·1·2·3·4·5·6·7·8·9·0·1·2·3·4·5·6·7·8·9·0·1·2·3·¨·1·2·3·4·5·6·7·8·9·0·1·2·3·4·5·6·7·8·9·0·1·2·3·¨·1·2·3·4·5·6·7·8·9·0·1·2·3·4·5·6·7·8·9·0·1·2·3·¨   <* 
 *>                                                                                                                                                     <*/


/*>                                                                                                                                                     <* 
 *> 10min for single day                                                                                                                                <* 
 *>                                                                                                                                                     <* 
 *> #batch------------------------------------¨¬¬¬¬¬¬¬¬¬¬¬¨---------------------normal-work---------------------¨¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¨batch>   <* 
 *> #----midnight-----¨----graveyard----¨------dusk-------¨----morning------¨-----midday------¨----afternoon----¨------dusk-------¨-----evening-----¨   <* 
 *> ¨-11p-¨--0a-¨--1a-¨--2a-¨--3a-¨--4a-¨--5a-¨--6a-¨--7a-¨--8a-¨--9a-¨-10a-¨-11a-¨-12a-¨--1p-¨--2p-¨--3p-¨--4p-¨--5p-¨--6p-¨--7p-¨--8p-¨--9p-¨-10p-¨   <* 
 *> machine.batch (7)                                                                                                                                   <* 
 *> ´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´´¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬´´´´´´    <* 
 *> ············Ï···································································································································    <* 
 *> ··················Ï´····························································································································    <* 
 *> ···················Ï´···························································································································    <* 
 *> ·····················Ï´´························································································································    <* 
 *> ························Ï´´·····················································································································    <* 
 *> ···························Ï´···················································································································    <* 
 *> ·································Ï´·············································································································    <* 
 *> member.testing (5)                                                                                                                                  <* 
 *> ················································································································································    <* 
 *> ················································································································································    <* 
 *> ················································································································································    <* 
 *> ················································································································································    <* 
 *> ················································································································································    <* 
 *> machine.background (5)                                                                                                                              <* 
 *> ÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏÏ    <* 
 *> ················································································································································    <* 
 *> ················································································································································    <* 
 *> ················································································································································    <* 
 *> ················································································································································    <* 
 *> monkey.daily (3)                                                                                                                                    <* 
 *> ················································································································································    <* 
 *> ················································································································································    <* 
 *> ················································································································································    <* 
 *>                                                                                                                                                     <*/

/*> #===[[ GRAVEYARD 2a-5a  ]]=========================================================================================================================================================#   <*/
/*> ¨---------|---------|-------- 2a -------|---------|---------¨---------|---------|-------- 3a -------|---------|---------¨---------|---------|-------- 4a -------|---------|-------->   <*/
/*> ¨123456789-123456789-123456789-123456789-123456789-123456789¨123456789-123456789-123456789-123456789-123456789-123456789¨123456789-123456789-123456789-123456789-123456789-123456789   <*/

/*> #===[[ midnight  23 to  1 ]]========================================================================================================================================================#   <*/
/*> #----´----Á----´----Â----´-> 23 <--´----Ä----´----Å----´----#----´----Á----´----Â----´--> 0 <--´----Ä----´----Å----´----#----´----Á----´----Â----´--> 1 <--´----Ä----´----Å----´----#   <*/

/*>                                                                                                                                                                                        <* 
 *> 1min detailed review                                                                                                                                                                   <* 
 *>                                                                                                                                                                                        <* 
 *> #===[[ GRAVEYARD 2a-5a  ]]=========================================================================================================================================================#   <* 
 *> ¨---------|---------|-------- 2a -------|---------|---------¨---------|---------|-------- 3a -------|---------|---------¨---------|---------|-------- 4a -------|---------|-------->   <* 
 *> ¨123456789-123456789-123456789-123456789-123456789-123456789¨123456789-123456789-123456789-123456789-123456789-123456789¨123456789-123456789-123456789-123456789-123456789-123456789   <* 
 *> machine.batch (7)                                                                                                                                                                      <* 
 *> ····················································································································································································   <* 
 *> Ï´´´´´´´´´´´´´´·····································································································································································   <* 
 *> ···············Ï´´´´´´´´´´··························································································································································   <* 
 *> ······························Ï´´´´´´´´´´´´´´·······································································································································   <* 
 *> ····························································Ï´´´´´´´´´´´´´´´´´´´´´´´´´´´´´··························································································   <* 
 *> ··························································································Ï´´´´´´´´´´´´´´···········································································   <* 
 *> ······················································································································································Ï´´´´´´´´´´´´´´···············   <* 
 *>                                                                                                                                                                                        <*/



/*====================------------------------------------====================*/
/*===----                         support functions                    ----===*/
/*====================------------------------------------====================*/
static void      o___SUPPORT_________________o (void) {;};

char     s_str          [LEN_LABEL] = "";

char*
RPTG__number            (int n, int v, char *s)
{
   switch (n) {
   case 1 :
      if (v >   9)  v =   9;
      break;
   case 2 :
      if (v >  99)  v =  99;
      break;
   case 3 :
      if (v > 999)  v = 999;
      break;
   }
   if (v == 0)   sprintf (s_str, "%*.*s·%s", n - 1, n - 1, "           ", s);
   else          sprintf (s_str, "%*d%s", n, v, s);
   return s_str;
}



/*====================------------------------------------====================*/
/*===----                          exec tracker                        ----===*/
/*====================------------------------------------====================*/
static void      o___TRACKER_________________o (void) {;};

static FILE    *f_track = NULL;
static int      s_line  = 0;

char
RPTG_track__guidelines  (FILE *f)
{
   int         c           =    0;
   c = yENV_lines (my.n_activity);
   if (c > 0)  return 0;
   fprintf (f, "## %s\n", P_ONELINE);
   fprintf (f, "## detailed launch tracker\n");
   fprintf (f, "#@ style     V = printable columnar values\n");
   fprintf (f, "#@ x-parse  19åÏ-·Ï-·Ï-·Ï-·Ï-·Ï-·Ï·Ï-·Ï--··Ï---------··Ï----··Ï-----------------------------·Ï---·Ï-----------------------------·Ï----·Ï----·Ï----·Ï---·Ï-----·Ï-------·æ\n");
   fprintf (f, "#@ titles     å##·mm·dd·hh·mm·ss·d·wy·doy··epoch·······ppid···file···························line·tracker························lead··pid···dur···rc···time···note·····æ\n");
   return 0;
}

char
RPTG_track__break       (FILE *f)
{
   fprintf (f               , "##·mm·dd·hh·mm·ss·d·wy·doy  epoch-----  -ppid  ---file----------------------- line ---tracker-------------------- lead- -pid- -dur- -rc- -time- --note-- ]\n");
   return 0;
}

char
rptg__track_open        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_spoint  (f_track);
   --rce;  if (f_track != NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   DEBUG_RPTG   yLOG_snote   (my.n_activity);
   f_track = fopen (my.n_activity, "at");
   DEBUG_RPTG   yLOG_spoint  (f_track);
   --rce;  if (f_track == NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(guides)-------------------------*/
   RPTG_track__guidelines (f_track);
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
rptg__track_close       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_spoint  (f_track);
   --rce;  if (f_track == NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = fclose (f_track);
   DEBUG_RPTG   yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(ground)-------------------------*/
   f_track = NULL;
   DEBUG_RPTG   yLOG_spoint  (f_track);
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
RPTG_track_exec         (tFILE *a_file, tLINE *a_line, char a_reason, char a_note)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_RPTG   yLOG_point   ("a_line"    , a_line);
   --rce;  if (a_line == NULL) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(non-track)----------------------*/
   DEBUG_RPTG   yLOG_char    ("tracking"  , a_line->track);
   if (a_line->track == '-') {
      DEBUG_RPTG   yLOG_note    ("tracking turned OFF, no output");
      DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(open)---------------------------*/
   rc = rptg__track_open ();
   DEBUG_RPTG   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(breaks)-------------------------*/
   if (s_line > 0 && s_line % 5 == 0) RPTG_track__break (f_track);
   ++s_line;
   /*---(prefix)-------------------------*/
   fprintf (f_track, "%-26.26s  %10d  %5d  %-30.30s %4d %-30.30s ",
         my.heartbeat, my.now, my.m_pid,
         a_file->title, a_line->recdno, a_line->tracker);
   /*---(message)------------------------*/
   switch (a_reason) {
   case KHRONOS_RUN  :
      fprintf (f_track, "····> %5d", a_line->rpid);
      break;
   case KHRONOS_SKIP :
      fprintf (f_track, "SKIP");
      break;
   case KHRONOS_BADD :
      fprintf (f_track, "BADD");
      break;
   case KHRONOS_BOOM :
      fprintf (f_track, "BOOM  ");
      break;
   case KHRONOS_KILL :
      fprintf (f_track, "KILL  ");
      break;
   case KHRONOS_TERM :
      fprintf (f_track, "TERM  ");
      break;
   case KHRONOS_LGRA :
      fprintf (f_track, "LGRA  ");
      break;
   case KHRONOS_LVIO :
      fprintf (f_track, "LVIO  ");
      break;
   case KHRONOS_FAIL :
      fprintf (f_track, "FAIL  ");
      break;
   case KHRONOS_PASS :
      fprintf (f_track, "- - - ");
      break;
   default     :
      break;
   }
   /*---(suffix)-------------------------*/
   if (strchr (KHRONOS_INFANT, a_reason) == NULL) {
      fprintf (f_track, "%5d %5d %4d ", a_line->l_rpid, a_line->l_dur, a_line->l_rc);
      switch (a_note) {
      case KHRONOS_EARL :
         fprintf (f_track, "EARLY  ");
         break;
      case KHRONOS_LATE :
         fprintf (f_track, "LATE   ");
         break;
      default     :
         fprintf (f_track, "OTD    ");   /* on-time delivery */
         break;
      }
   }
   /*---(suffix)-------------------------*/
   if (strchr (KHRONOS_SIGS, a_reason) != NULL) {
      switch (a_line->l_rc) {
      case SIGTERM :  fprintf (f_track, "sigterm");   break;
      case SIGKILL :  fprintf (f_track, "sigkill");   break;
      case SIGQUIT :  fprintf (f_track, "sigquit");   break;
      case SIGABRT :  fprintf (f_track, "sigabrt");   break;
      case SIGSEGV :  fprintf (f_track, "sigsegv");   break;
      case SIGFPE  :  fprintf (f_track, "sigfpe" );   break;
      case SIGILL  :  fprintf (f_track, "sigill" );   break;
      case SIGBUS  :  fprintf (f_track, "sigbus" );   break;
      case SIGPIPE :  fprintf (f_track, "sigpipe");   break;
      case SIGSYS  :  fprintf (f_track, "sigsys" );   break;
      default      :  fprintf (f_track, "sig????");   break;
      }
   }
   /*---(trailing)-----------------------*/
   fprintf (f_track, "\n");
   /*---(close)--------------------------*/
   rc = rptg__track_close ();
   DEBUG_RPTG   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
rptg__track_ends        (char a_sig, char *a_name, char *a_desc)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(globals)------------------------*/
   s_line = 0;
   /*---(open)---------------------------*/
   rc = rptg__track_open ();
   DEBUG_RPTG   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prefix)-------------------------*/
   if (strncmp (a_desc, "END", 3) == 0)  RPTG_track__break (f_track);
   if (a_sig < 0 && a_name == NULL) {
      fprintf (f_track, "%-26.26s  %10d  %5d  %s\n", my.heartbeat, my.now, my.m_pid, a_desc);
   } else {
      fprintf (f_track, "%-26.26s  %10d  %5d  %s (%d) %s\n", my.heartbeat, my.now, my.m_pid, a_name, a_sig, a_desc);
   }
   if (strncmp (a_desc, "BEG", 3) == 0)  RPTG_track__break (f_track);
   /*---(close)--------------------------*/
   rc = rptg__track_close ();
   DEBUG_RPTG   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char RPTG_track_beg     (void)        { return rptg__track_ends (-1, NULL, "BEG -------------------------- ---- ------------------------------ ----- ----- ----- ---- ------ -------- ]"); }
char RPTG_track_end     (void)        { return rptg__track_ends (-1, NULL, "END -------------------------- ---- ------------------------------ ----- ----- ----- ---- ------ -------- ]"); }
char RPTG_track_sig     (char a_sig, char *a_name, char *a_desc) { return rptg__track_ends (a_sig, a_name, a_desc); }



/*====================------------------------------------====================*/
/*===----                           status report                      ----===*/
/*====================------------------------------------====================*/
static void      o___STATUS__________________o (void) {;};

char
RPTG_status__open       (FILE **f)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_spoint  (f);
   --rce;  if (f == NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_RPTG   yLOG_spoint  (*f);
   --rce;  if (*f != NULL) {
      DEBUG_RPTG   yLOG_snote   ("already open");
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   DEBUG_RPTG   yLOG_snote   (my.n_status);
   *f = fopen (my.n_status, "wt");
   DEBUG_RPTG   yLOG_spoint  (*f);
   --rce;  if (*f == NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
RPTG_status__close      (FILE **f)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_spoint  (f);
   --rce;  if (f == NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_RPTG   yLOG_spoint  (*f);
   --rce;  if (*f == NULL) {
      DEBUG_RPTG   yLOG_snote   ("already closed");
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   DEBUG_RPTG   yLOG_snote   (my.n_status);
   rc = fclose (*f);
   DEBUG_RPTG   yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   *f = NULL;
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
RPTG_status__header     (FILE *f)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_spoint  (f);
   --rce;  if (f == NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   fprintf (f, "## %s\n", P_ONELINE);
   fprintf (f, "##    inventory with major tracking data, updated every minute on logging ram disk (fast)\n");
   fprintf (f, "##    heartbeat to verify update å%sæ\n", my.heartbeat);
   fprintf (f, "##\n");
   fprintf (f, "#@ x-parse  15åÏ-----------------------------·Ï---·Ï-----------------------------·Ï·Ï Ï---------·Ï----···Ï-----------------------------···Ï-----------------------------------------------···Ï----------------···Ï------------··Ï--------···Ï---·Ï---·Ï---·æ\n");
   fprintf (f, "#@ titles     åfile···························line·tracker························f·a·start······rpid····statistics·······················durations··········································rpid··dur··rc·······controls·······extras······est··min··max··æ\n");
   fprintf (f, "##\n");
   fprintf (f, "##                                                                 f a                                                                                                                          v t r s m m r  f t c d n                  ]\n");
   fprintf (f, "##                                                                 o c *---- curr ----*   *----------- counts ---------*   *-------------------- durations ---------------*   *---- last ---*   a k o t i a e  l h p s e   *-- timing --* ]\n");
   fprintf (f, "##          file               line           tracker              c t   start     rpid   r a  s b o k t  g v G V  f w p   e l  < 3 · 2 · 10·······1 0 1·······10 · 2 · 3 >    rpid  dur  rc    l r l r n x m  x r u k t    est  min  max ]\n");
   fprintf (f, "##---------------------------- ---- ------------------------------ - - ---------- -----   ---  ---------  -------  -----   ---  - ------- ---------- - ---------- ------- -   ----- ---- ----   - - - - - - -  - - - - -   ---- ---- ---- ]\n");
   fprintf (f, "##\n");
   fflush  (f);
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
RPTG_status__break      (FILE *f)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_spoint  (f);
   --rce;  if (f == NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   fprintf (f, "##----------file-------------- line ----------tracker------------- f a --start--- -rpid   r-a  s-b-o-k-t  g-v-G-V  f-w-p   e-l  <-3---2-- 1--------- 0 ---------1 --2---3->   -rpid -dur -rc-   v-t-r-s-n-x-r  f-t-c-d-n   -est -min -max ]\n");
   fflush  (f);
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
RPTG_status__footer     (FILE *f, int c)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_spoint  (f);
   --rce;  if (f == NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(write)--------------------------*/
   fprintf (f, "\n");
   fprintf (f, "## end-of-file.  %d lines.  done, finito, completare, whimper [Ï´···\n", c);
   fflush  (f);
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
RPTG__flag              (char a_flag)
{
   if (a_flag == '-')  return '·';
   return a_flag;
}

char
RPTG_status             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         c           =    0;
   int         i           =    0;
   FILE       *f           = NULL;
   tFILE      *x_file      = NULL;
   tFILE      *x_save      = NULL;
   tLINE      *x_line      = NULL;
   char        x_active    =  '-';
   char        x_focus     =  '-';
   char        t           [LEN_RECD]  = "";
   char        s           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   rc = RPTG_status__open (&f);
   DEBUG_RPTG   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(write header)-------------------*/
   rc = RPTG_status__header (f);
   DEBUG_RPTG   yLOG_value   ("header"    , rc);
   --rce;  if (rc < 0) {
      rc = RPTG_status__close (&f);
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk lines)---------------------*/
   rc = yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_HEAD, NULL, &x_line);
   while (rc >= 0 && x_line != NULL) {
      /*---(debug)-----------------------*/
      DEBUG_INPT   yLOG_point   ("x_line"    , x_line);
      DEBUG_INPT   yLOG_info    ("->title"   , x_line->tracker);
      /*---(prepare)---------------------*/
      rc = yDLST_list_by_cursor (YDLST_CURR, NULL, &x_file);
      if (x_file != x_save) {
         fprintf (f, "\n");
         i = 0;
      }
      if (i %  5 == 0)  RPTG_status__break (f);
      x_active = (yDLST_active_check (x_line->tracker) > 0) ? 'y' : '·';
      x_focus  = (yDLST_focus_check  (x_line->tracker) > 0) ? 'y' : '·';
      /*---(current)---------------------*/
      fprintf (f, "%-30.30s %4d %-30.30s ",
            x_file->title, x_line->recdno, x_line->tracker);
      fprintf (f, "%c %c ",
            x_focus, x_active);
      ystrlcpy (t, RPTG__number (10, x_line->start , " "), LEN_RECD);
      ystrlcat (t, RPTG__number ( 5, x_line->rpid  , " "), LEN_RECD);
      fprintf (f, "%s", t);
      /*---(statistics)------------------*/
      if (x_line->trks != NULL) {
         fprintf (f, "  %30.30s ", x_line->trks->stats + 3);
         fprintf (f, "  %49.49s ", x_line->trks->durs  + 3);
      } else {
         fprintf (f, "  %30.30s ", KHRONOS_DEFSTATS + 3);
         fprintf (f, "  %49.49s ", KHRONOS_DEFDURS  + 3);
      }
      /*---(last run)--------------------*/
      fprintf (f, " ");
      ystrlcpy (t, RPTG__number ( 5, x_line->l_rpid     , " "), LEN_RECD);
      if (x_line->l_rpid == 0) {
         ystrlcat (t, "   ·    · ", LEN_RECD);
      } else {
         sprintf (s, "%4d %4d ", x_line->l_dur / 60, x_line->l_rc);
         ystrlcat (t, s, LEN_RECD);
      }
      /*> ystrlcat (t, RPTG__number ( 4, x_line->l_dur / 60 , " "), LEN_RECD);         <*/
      /*> ystrlcat (t, RPTG__number ( 4, x_line->l_rc       , " "), LEN_RECD);         <*/
      fprintf (f, "%s  ", t);
      /*---(controls)--------------------*/
      if (x_line->tracker [0] != '.') {
         fprintf (f, "%c %c %c %c %c %c %c  ",
               RPTG__flag (x_line->value), RPTG__flag (x_line->track), RPTG__flag (x_line->rolling), RPTG__flag (x_line->strict), RPTG__flag (x_line->lower), RPTG__flag (x_line->upper), RPTG__flag (x_line->remedy));
         fprintf (f, "%c %c %c %c %c  ",
               RPTG__flag (x_line->flex), RPTG__flag (x_line->throttle), RPTG__flag (x_line->cpu), RPTG__flag (x_line->disk), RPTG__flag (x_line->net), RPTG__flag (x_line->upper), RPTG__flag (x_line->remedy));
         fprintf (f, " %4d %4d %4d\n",
               x_line->est / 60, x_line->est_min / 60000, x_line->est_max / 60000);
      } else {
         fprintf (f, "· · · · · · ·  · · · · ·      ·    ·    ·\n");
      }
      /*---(next)------------------------*/
      ++c; ++i;
      x_save = x_file;
      rc = yDLST_line_by_cursor (YDLST_GLOBAL, YDLST_NEXT, NULL, &x_line);
      /*---(done)------------------------*/
   }
   /*---(write footer)-------------------*/
   rc = RPTG_status__footer (f, c);
   DEBUG_RPTG   yLOG_value   ("footer"    , rc);
   --rce;  if (rc < 0) {
      rc = RPTG_status__close (&f);
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   rc = RPTG_status__close (&f);
   DEBUG_RPTG   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___DAILY___________________o (void) {;}

static int      s [LEN_HUND];
static char     s_title  [LEN_RECD] = "";

char
RPTG_by_min__prepare    (void)
{
   char        i           =    0;
   for (i = 0; i < LEN_HUND; ++i)   s [i] = 0;
   return 0;
}

char
RPTG_by_min__header     (FILE *f, char a_color, char *a_name, char a_beg, char a_end)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        i, j;
   char        t           [LEN_RECD]  = "";
   char        u           [LEN_RECD]  = "";
   char        v           [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_point   ("f"         , f);
   --rce;  if (f      == NULL) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(spacing)------------------------*/
   for (i = 0; i < 3; ++i) {
      if (a_color == 'y')  fprintf (f, "%s%s%s\n", BOLD_MAG, "##", BOLD_OFF);
      else                 fprintf (f, "##\n");
   }
   /*---(print main header)--------------*/
   if (a_color == 'y') fprintf (f, "%s", BOLD_MAG);
   if      (a_beg ==  0)  sprintf (t, "mid" , a_beg);
   else if (a_beg <  12)  sprintf (t, "%da" , a_beg);
   else if (a_beg == 12)  sprintf (t, "noon", a_beg);
   else if (a_beg >  12)  sprintf (t, "%dp" , a_beg - 12);
   if      (a_end ==  0)  sprintf (u, "mid" , a_end);
   else if (a_end <  12)  sprintf (u, "%da" , a_end);
   else if (a_end == 12)  sprintf (u, "noon", a_end);
   else if (a_end >  12)  sprintf (u, "%dp" , a_end - 12);
   snprintf (v, 181 + 31, "##===[[ %s %s to %s ]]%s", a_name, t, u, YSTR_EQUAL);
   fprintf  (f, "%s#", v);
   if (a_color == 'y') fprintf (f, "%s", BOLD_OFF);
   fprintf  (f, "\n");
   fflush  (f);
   /*---(save title line)----------------*/
   ystrlcpy (s_title, "", LEN_RECD);
   i = a_beg;
   for (j = 0; j < 3; j++) {
      if (i < 10)  sprintf (t, "[%d>-", i);
      else         sprintf (t, "[%2d>", i);
      if (j < 2)   sprintf (u, "%s-´----Á----´----Â----´----Ã----´----Ä----´----Å----´----", t);
      else         sprintf (u, "%s-´----Á----´----Â----´----Ã----´----Ä----´----Å----´--"  , t);
      ystrlcat (s_title, u, LEN_RECD);
      ++i;
      if (i >= 24)  i -= 24;
   }
   if (i < 10)  sprintf (t, "-[%d", i);
   else         sprintf (t, "[%2d", i);
   ystrlcat (s_title, t, LEN_RECD);
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG_by_min__hour       (char a_color, int n, tLINE *a_line, char a_yr, char a_mo, char a_dy, char a_hr, char *a_out)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        i           =    0;
   char        rc          =    0;
   char        x_type      =  '·';
   char        x_last      =  '/';
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_point   ("a_line"    , a_line);
   --rce;  if (a_line == NULL) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   ystrlcpy (a_out, "", LEN_RECD);
   if (a_color == 'y') ystrlcpy (a_out, FORE_OFF, LEN_RECD);
   ySCHED_date (a_yr, a_mo, a_dy);
   if (a_color == 'y' && s [n] >  0) ystrlcat (a_out, BOLD_CYN, LEN_RECD);
   for (i = 0; i < 60; ++i) {
      rc = ySCHED_test (a_line->sched, a_hr, i, NULL);
      if      (strncmp (a_line->tracker, ".graceful", 9) == 0) x_type = '·';
      else if (strncmp (a_line->tracker, ".violent" , 8) == 0) x_type = '·';
      else if (a_line->tracker [0] == '.')                     x_type = 'V';
      else                                                     x_type = '·';
      if (x_type == '·') {
         if      (rc <  0)   ystrlcat (a_out, "¢", LEN_RECD);
         else if (rc == 0) {
            if (s [n] > 0) {
               ystrlcat (a_out, "´", LEN_RECD);
               s [n] -= 60;
               if (a_color == 'y' && s [n] <= 0)     ystrlcat (a_out, BOLD_OFF, LEN_RECD);
            }
            else if (i ==  0)      ystrlcat (a_out, "¨", LEN_RECD);
            else if (i == 30)      ystrlcat (a_out, "|", LEN_RECD);
            else if (i % 10 == 0)  ystrlcat (a_out, "+", LEN_RECD);
            else                   ystrlcat (a_out, "·", LEN_RECD);
         }
         else if (rc >  0) {
            if (s [n] > 0)  {
               ystrlcat (a_out, "¥", LEN_RECD);
               s [n] -= 60;
            } else  {
               if (a_color == 'y') ystrlcat (a_out, BOLD_CYN, LEN_RECD);
               ystrlcat (a_out, "Ï", LEN_RECD);
               s [n] = a_line->est - 60;
               if (a_color == 'y' && s [n] <= 0)     ystrlcat (a_out, BOLD_OFF, LEN_RECD);
            }
         }
      } else {
         if (rc >  0)  {
            if (a_color == 'y' && x_last != '=') ystrlcat (a_out, BOLD_GRN, LEN_RECD);
            ystrlcat (a_out, "=", LEN_RECD);
         }
         else {
            if (a_color == 'y' && x_last != '­') ystrlcat (a_out, BOLD_RED, LEN_RECD);
            ystrlcat (a_out, "­", LEN_RECD);
         }
      }
   }
   if (a_color == 'y') ystrlcat (a_out, FORE_OFF, LEN_RECD);
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG_by_min__line       (FILE *f, char a_color, int n, tLINE *a_line, char a_yr, char a_mo, char a_dy, char a_hr, char a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_hour      =    0;
   char        i           =    0;
   char        t           [LEN_RECD]  = "";
   char        x_yr, x_mo, x_dy;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_point   ("f"         , f);
   --rce;  if (f      == NULL) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_RPTG   yLOG_point   ("a_line"    , a_line);
   --rce;  if (a_line == NULL) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_color == 'y' && f != NULL) fprintf (f, "%s", BOLD_OFF);
   if (f != NULL)  fprintf (f, "  %2d %-25.25s ", n, a_line->tracker);
   /*---(prepare)------------------------*/
   x_yr   = a_yr;
   x_mo   = a_mo;
   x_dy   = a_dy;
   x_hour = a_hr;
   /*---(loop hours)---------------*/
   for (i = 0; i < a_count; ++i) {
      if (x_hour >= 24) {
         x_hour -= 24;
         RPTG_by_min__offset (0, a_yr, a_mo, a_dy, 1, NULL, &x_yr, &x_mo, &x_dy);
      }
      RPTG_by_min__hour (a_color, n, a_line, x_yr, x_mo, x_dy, x_hour, t);
      if (f != NULL)  fprintf (f, "%s", t);
      ++x_hour;
      /*---(done)------------------*/
   }
   if (f != NULL)  fprintf (f, "¨");
   if (f != NULL && a_color == 'y')  fprintf (f, "%s", BOLD_OFF);
   if (f != NULL)  fprintf (f, "\n");
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG_by_min__block      (FILE *f, char a_color, char a_yr, char a_mo, char a_dy, char a_beg, char a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_hour      =    0;
   char        i           =    0;
   tFILE      *x_file      = NULL;
   tLINE      *x_line      = NULL;
   char        c           =    0;
   char        n           =    0;
   char        t           [LEN_HUND]  = "";
   char        l           =    0;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   x_hour = a_beg;
   /*---(loop files)---------------------*/
   rc = yDLST_list_by_cursor (YDLST_HEAD, NULL, &x_file);
   rc = yDLST_list_by_cursor (YDLST_NEXT, NULL, &x_file);
   while (rc >= 0 && x_file != NULL) {
      n = yDLST_line_count (YDLST_LOCAL);
      if (f != NULL) {
         fprintf (f, "\n");
         if (a_color == 'y') fprintf (f, "%s", BOLD_MAG);
         sprintf (t, "%s (%d)", x_file->title, n);
         fprintf (f, "%-30.30s %s", t, s_title);
         if (a_color == 'y') fprintf (f, "%s", BOLD_OFF);
         fprintf (f, "\n");
      }
      /*---(loop lines)------------------*/
      rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_HEAD, NULL, &x_line);
      l = 0;
      while (rc >= 0 && x_line != NULL) {
         if (l > 0 && l % 5 == 0 && f != NULL)  fprintf (f, "%-30.30s %s\n", YSTR_EMPTY, s_title);
         RPTG_by_min__line (f, a_color, c, x_line, a_yr, a_mo, a_dy, a_beg, a_count);
         rc = yDLST_line_by_cursor (YDLST_LOCAL, YDLST_NEXT, NULL, &x_line);
         ++c;
         ++l;
         /*---(done)---------------------*/
      }
      if ((l % 5 == 0 || l % 5 >  2) && f != NULL)  fprintf (f, "%-30.30s %s\n", YSTR_EMPTY, s_title);
      rc = yDLST_list_by_cursor (YDLST_NEXT, NULL, &x_file);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char       /*--> set the data for parsing ------------------------------------*/
RPTG_by_min__ymd2last   (char *a_yr, char *a_mo, char *a_dy)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   long        x_now       =    0;
   long        x_epoch     =    0;
   tTIME       x_broke;
   char        t           [200];
   /*---(header)-------------------------*/
   DEBUG_YSCHED  yLOG_enter   (__FUNCTION__);
   /*---(reset date fields)--------------*/
   ysched_date_reset ();
   /*---(check max ranges)---------------*/
   DEBUG_YSCHED  yLOG_complex ("a_yr"     , "%d (0 to 99)", *a_yr);
   --rce;  if (*a_yr < 0 || *a_yr > 99) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED  yLOG_complex ("a_mo"     , "%d (1 to 12)", *a_mo);
   --rce;  if (*a_mo < 1 || *a_mo > 12) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YSCHED  yLOG_complex ("a_dy"     , "%d (1 to 31)", *a_dy);
   --rce;  if (*a_dy < 1 || *a_dy > 31) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(get current date)---------------*/
   x_now             = time (NULL);
   DEBUG_YSCHED  yLOG_value   ("x_now"     , x_now);
   localtime_r (&x_now, &x_broke);
   /*---(clear small elements)-----------*/
   x_broke.tm_sec   = 0;
   x_broke.tm_min   = 0;
   x_broke.tm_hour  = 0;
   /*---(adjust to requsted)-------------*/
   x_broke.tm_year = *a_yr + 100;  /* from 1900 */
   DEBUG_YSCHED  yLOG_value   ("tm_year"   , x_broke.tm_year);
   x_broke.tm_mon  = *a_mo - 1;
   DEBUG_YSCHED  yLOG_value   ("tm_mon"    , x_broke.tm_mon);
   x_broke.tm_mday = *a_dy;
   DEBUG_YSCHED  yLOG_value   ("tm_mday"   , x_broke.tm_mday);
   DEBUG_YSCHED  yLOG_value   ("tm_hour"   , x_broke.tm_hour);
   /*---(get the format version)---------*/
   x_epoch  = mktime (&x_broke);
   DEBUG_YSCHED  yLOG_value   ("tm_hour"   , x_broke.tm_hour);
   while (x_broke.tm_mon != *a_mo - 1) {
      ++x_broke.tm_hour;
      DEBUG_YSCHED  yLOG_value   ("tm_hour"   , x_broke.tm_hour);
      x_epoch  = mktime (&x_broke);
   }
   DEBUG_YSCHED  yLOG_value   ("x_epoch"   , x_epoch);
   /*> printf ("\n");                                                                 <*/
   /*> printf ("%2d, %2d, %2d, %10d, %d\n", *a_yr, *a_mo, *a_dy, mySCHED.s_epoch, x_broke.tm_isdst);   <*/
   /*---(defenses : day)-----------------*/
   DEBUG_YSCHED  yLOG_value   ("tm_mon"    , x_broke.tm_mon);
   DEBUG_YSCHED  yLOG_value   ("a_mo"      , *a_mo - 1);
   --rce;  if (x_broke.tm_mon != *a_mo - 1) {
      DEBUG_YSCHED  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(four hours back)----------------*/
   x_epoch -= 60 * 4;
   localtime_r (&x_epoch, &x_broke);
   *a_yr = x_broke.tm_year - 100;  /* from 1900 */
   DEBUG_YSCHED  yLOG_value   ("tm_year"   , x_broke.tm_year);
   *a_mo = x_broke.tm_mon + 1;
   DEBUG_YSCHED  yLOG_value   ("tm_mon"    , x_broke.tm_mon);
   *a_dy = x_broke.tm_mday;
   DEBUG_YSCHED  yLOG_value   ("tm_mday"   , x_broke.tm_mday);
   /*---(complete)-----------------------*/
   DEBUG_YSCHED  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG_by_min__offset     (long a_epoch, cchar a_yr, cchar a_mo, cchar a_dy, int a_offset, long *r_epoch, char *r_yr, char *r_mo, char *r_dy)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   long        x_now       =    0;
   long        x_epoch     =    0;
   tTIME       x_broke;
   /*---(header)-------------------------*/
   DEBUG_OUTP  yLOG_enter   (__FUNCTION__);
   DEBUG_OUTP  yLOG_complex ("base date" , "%-10ld, %2dy, %2dm, %2dd", a_epoch, a_yr, a_mo, a_dy);
   /*---(default)------------------------*/
   if (r_epoch != NULL)  *r_epoch = 0;
   if (r_yr    != NULL)  *r_yr    = 0;
   if (r_mo    != NULL)  *r_mo    = 0;
   if (r_dy    != NULL)  *r_dy    = 0;
   /*---(check max ranges)---------------*/
   if (a_epoch > 0) {
      DEBUG_OUTP  yLOG_note    ("chose yr, mo, dy version");
      --rce;  if (a_yr < 0 || a_yr > 99) {
         DEBUG_OUTP  yLOG_note    ("a_yr not between 0 and 99");
         DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      --rce;  if (a_mo < 1 || a_mo > 12) {
         DEBUG_OUTP  yLOG_note    ("a_mo not between 1 and 12");
         DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      --rce;  if (a_dy < 1 || a_dy > 31) {
         DEBUG_OUTP  yLOG_note    ("a_dy not between 1 and 31");
         DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      x_now             = time (NULL);
   } else {
      DEBUG_OUTP  yLOG_note    ("chose epoch version");
      x_now             = a_epoch;
   }
   /*---(get solid template)-------------*/
   localtime_r (&x_now, &x_broke);
   x_broke.tm_sec   = x_broke.tm_min   = x_broke.tm_hour  =  0;
   /*---(adjust to requsted)-------------*/
   x_broke.tm_year = a_yr + 100;  /* from 1900 */
   x_broke.tm_mon  = a_mo - 1;
   x_broke.tm_mday = a_dy;
   /*---(get the format version)---------*/
   x_epoch  = mktime (&x_broke);
   while (x_broke.tm_mon != a_mo - 1) {
      ++x_broke.tm_hour;
      x_epoch  = mktime (&x_broke);
   }
   DEBUG_OUTP  yLOG_complex ("curr date" , "%-10ld, %2dy, %2dm, %2dd", x_epoch, x_broke.tm_year - 100, x_broke.tm_mon + 1, x_broke.tm_mday);
   /*---(defenses : day)-----------------*/
   --rce;  if (x_broke.tm_mon != a_mo - 1) {
      DEBUG_OUTP  yLOG_note    ("a_dy not legal for month");
      DEBUG_OUTP  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   x_epoch += 60 * 60 * 24 * a_offset;
   localtime_r (&x_epoch, &x_broke);
   DEBUG_OUTP  yLOG_complex ("new date"  , "%-10ld, %2dy, %2dm, %2dd", x_epoch, x_broke.tm_year - 100, x_broke.tm_mon + 1, x_broke.tm_mday);
   if (r_epoch != NULL)  *r_epoch = x_epoch;
   if (r_yr    != NULL)  *r_yr    = x_broke.tm_year - 100;  /* from 1900 */
   if (r_mo    != NULL)  *r_mo    = x_broke.tm_mon + 1;
   if (r_dy    != NULL)  *r_dy    = x_broke.tm_mday;
   /*---(complete)-----------------------*/
   DEBUG_OUTP  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG_by_min_direct      (FILE *f, char a_color, char a_yr, char a_mo, char a_dy)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_yr, x_mo, x_dy;
   /*---(header)-------------------------*/
   DEBUG_RPTG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_RPTG   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_RPTG   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(write heading)------------------*/
   if (a_color == 'y') fprintf (f, "%s", BOLD_MAG);
   fprintf (f, "## %s\n", P_ONELINE);
   if (a_color == 'y') fprintf (f, "%s%s", BOLD_OFF, BOLD_MAG);
   fprintf (f, "## projected run-times for all jobs for every minute\n");
   if (a_color == 'y') fprintf (f, "%s", BOLD_OFF);
   /*---(print report)-------------------*/
   RPTG_by_min__offset  (0, a_yr, a_mo, a_dy, -1, NULL, &x_yr, &x_mo, &x_dy);
   RPTG_by_min__block (NULL, '-', x_yr, x_mo, x_dy, 5, 3);
   RPTG_by_min__header (f, a_color, "evening"  , 20, 23);
   RPTG_by_min__block  (f, a_color, x_yr, x_mo, x_dy, 20, 3);
   RPTG_by_min__header (f, a_color, "midnight" , 23,  2);
   RPTG_by_min__block  (f, a_color, x_yr, x_mo, x_dy, 23, 3);
   RPTG_by_min__header (f, a_color, "graveyard",  2,  5);
   RPTG_by_min__block  (f, a_color, a_yr, a_mo, a_dy,  2, 3);
   RPTG_by_min__header (f, a_color, "dawn"     ,  5,  8);
   RPTG_by_min__block  (f, a_color, a_yr, a_mo, a_dy,  5, 3);
   RPTG_by_min__header (f, a_color, "morning"  ,  8, 11);
   RPTG_by_min__block  (f, a_color, a_yr, a_mo, a_dy,  8, 3);
   RPTG_by_min__header (f, a_color, "midday"   , 11, 14);
   RPTG_by_min__block  (f, a_color, a_yr, a_mo, a_dy, 11, 3);
   RPTG_by_min__header (f, a_color, "afternoon", 14, 17);
   RPTG_by_min__block  (f, a_color, a_yr, a_mo, a_dy, 14, 3);
   RPTG_by_min__header (f, a_color, "dusk"     , 17, 20);
   RPTG_by_min__block  (f, a_color, a_yr, a_mo, a_dy, 17, 3);
   fprintf (f, "## end-of-file.  done, finito, completare, whimper [Ï´···\n");
   fflush  (f);
   /*---(complete)-----------------------*/
   DEBUG_RPTG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
RPTG_by_min             (void)
{
   FILE       *f           = NULL;
   f = fopen ("/tmp/khronos_proposed.txt", "wt");
   RPTG_by_min_direct (f, 'y', 20, 4, 15);
   fclose (f);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit test accessor                      ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /*--> unit test accessor ------------------------------*/
rptg__unit              (char *a_question, int a_num)
{ 
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   char        x_heartbeat [LEN_HUND];
   /*---(prepare)------------------------*/
   ystrlcpy  (unit_answer, "RPTG             : question not understood", LEN_HUND);
   /*---(crontab name)-------------------*/
   /*---(complete)-----------------------*/
   return unit_answer;
}




