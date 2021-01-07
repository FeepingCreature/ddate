/* $ DVCS ID: $jer|,523/lhos,KYTP!41023161\b"?" <<= DO NOT DELETE! */

/* ddate.c .. converts boring normal dates to fun Discordian Date -><-
   written  the 65th day of The Aftermath in the Year of Our Lady of 
   Discord 3157 by Druel the Chaotic aka Jeremy Johnson aka
   mpython@gnu.ai.mit.edu  
      28 Sever St Apt #3
      Worcester MA 01609

   and I'm not responsible if this program messes anything up (except your 
   mind, I'm responsible for that)

   (k) YOLD 3161 and all time before and after.
   Reprint, reuse, and recycle what you wish.
   This program is in the public domain.  Distribute freely.  Or not.

   Majorly hacked, extended and bogotified/debogotified on 
   Sweetmorn, Bureaucracy 42, 3161 YOLD, by Lee H:. O:. Smith, KYTP, 
   aka Andrew Bulhak, aka acb@dev.null.org

   Slightly hackled and crackled by a sweet firey stove on
   Boomtime, the 53rd day of Bureaucracy in the YOLD 3179,
   by Chaplain Nyan the Wiser, aka Dan Dart, aka ntw@dandart.co.uk

   and I'm not responsible if this program messes anything up (except your 
   mind, I'm responsible for that) (and that goes for me as well --lhos)

   Version history:
   Bureflux 3161:      First release of enhanced ddate with format strings
   59 Bcy, 3161:       PRAISE_BOB and KILL_BOB options split, other minor
                       changes.
   53 Bcy, 3179:       Fixed gregorian date conversions less than YOLD 1167

   1999-02-22 Arkadiusz Miskiewicz <misiek@pld.ORG.PL>
   - added Native Language Support

   2000-03-17 Burt Holzman <holzman+ddate@gmail.com>
   - added range checks for dates

   2014-06-07 William Woodruff <william@tuffbizz.com>
   - removed gettext dependent locale code

   15th of Confusion, 3180:
   - call out adherents of the wrong fruit

   FIVE TONS OF FLAX
*/

/* configuration options  VVVVV   READ THIS!!! */

/* If you wish ddate(1) to print the date in the same format as Druel's 
 * original ddate when called in immediate mode, define OLD_IMMEDIATE_FMT 
 */

#define OLD_IMMEDIATE_FMT

/* If you wish to use the US format for aneristic dates (m-d-y), as opposed to
 * the Commonwealth format, define US_FORMAT.
 */

/* #define US_FORMAT */

/* If you are ideologically, theologically or otherwise opposed to the 
 * Church of the SubGenius and do not wish your copy of ddate(1) to contain
 * code for counting down to X-Day, undefine KILL_BOB */

#define KILL_BOB 13013

/* If you wish ddate(1) to contain SubGenius slogans, define PRAISE_BOB */

/*#define PRAISE_BOB 13013*/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>


// work around includes and defines from formerly c.h
#ifndef ARRAY_SIZE
# define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
#endif

/* &a[0] degrades to a pointer: a different type from an array */
# define __must_be_array(a) \
	BUILD_BUG_ON_ZERO(__builtin_types_compatible_p(__typeof__(a), __typeof__(&a[0])))

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))

/* work around hacks for standalone package */
#define PACKAGE "ddate"
#define PACKAGE_STRING "Stand Alone"

#ifndef __GNUC__
#define inline /* foo */
#endif

#ifdef KILL_BOB
int xday_countdown(int yday, int year);
#endif


/* string constants */

char *day_long[5] = { 
    "Sweetmorn", "Boomtime", "Pungenday", "Prickle-Prickle", "Setting Orange"
};

/**
 * Produced by iteratedly feeding the list of five days into talktotransformer.com running the
 * 1.5 billion parameter GPT-2, which is effectively a supercomputer trained by feeding it a random
 * subset of the Internet.
 * This felt appropriate.
 * -- UPDATE: talktotransformer is still down. I went to a local GPT-2 instance.
 * This season, GPT-2 literally gave me all the day names in a list, on its own, breaking the
 * format I gave it, beginning with "The full list of Discordian Days of the Year is:",
 * and properly enumerating. I can take a hint, Goddess. Taken unmodified.
 */
char *day_extended[73] = {
  /* 0 */
  "Christmas Eve (celebrated in the US as \"Merry Christmas\"",
  "Halloween (celebrates the night before the holiday, which is celebrated in the same way as Christmas Eve)",
  "New Year (celebrate the day before the birthday, which also celebrates the day after the birthday)",
  "St. Patricks Day (celebration of the day that the saint is born)",
  "St Patrick's day (celebs the day the saint was born)",
  /* 5 */
  "Easter (celebes the day of the resurrection)",
  "Pilgrims Day (a celebration of the Pilgrimages of the early Church)",
  "St George's Day",
  "Easter Sunday (celebinates the day when Jesus was crucified)",
  "Pilgram's Day",
  /* 10 */ "St Andrew's Day", "St Nicholas Day", "St Matthew's Day", "St Paul's Day", "St James' Day",
  /* 15 */ "St John's Day", "St Jude's Day", "St Thomas' Day", "St Michael's Day", "St Margaret's Day",
  /* 20 */ "St Simon's Day", "St Philip's Day", "St Joseph's Day", "St Bartholomew's Day", "St Martin's Day",
  /* 25 */ "St Stephen's Day", "St Luke's Day", "St Simeon's Day", "St Ignatius's Day", "St Cyprian's Day",
  /* 30 */ "St Francis Day", "St Dominic's Day", "St Augustine's Day", "St Catherine's Day", "St Chrysostom's Day",
  /* 35 */ "St Jerome's Day", "St Peter's Day", "St Mary Magdalen's Day", "St Matthias' Day", "St Maximilian's Day",
  /* 40 */ "St Timothy's Day", "St Basil's Day" "St Ambrose's Day", "St Irenaeus' Day", "St Theophilus' Day",
  /* 45 */ "St Ephrem's Day", "St Athanasius' Day", "St Gregory the Great's Day", "St Julian of Norwich's Day", "St Benedict's Day",
  /* 50 */ "St Bernard's Day", "St Hilary of Poitiers' Day", "St Lawrence's Day", "St Cuthbert's Day", "St Bonaventure's Day",
  /* 55 */ "St Theodore's Day", "St Vincent de Paul's day", "St Anselm's Day", "St Leo's Day", "St Louis de Montfort's Day",
  /* 60 */ "St Charles Borromeo's Day", "St Edmund's Day", "St Alexander's Day", "St Edward's Day", "St Alphonsus' Day",
  // man, discordianism sure has a lot of saints
  /* 65 */ "St Denis' Day", "St Aloysius' day", "St Evagrius' Day", "St Agnes Day", "St Anthony of Padua's Day",
  /* 70 */ "St Barnabas Day", "St Bridget's Day", "St Cecilia Day"
  // and then the network continued:
  /**
   * 74. St Elizabeth of Hungary's Day 75. St Jovinian's Day 76. St Januarius' Day
   * 77. St Hugh of Lincoln's Day 78. St Judas of the Cross' Day 79. St Liguori's Day 80. St Mark's Day
   * 81. St Maarten's Day 82. St Mathias' day 83. St Martial's Day 84. St Mihiel's Day 85. St Nectarius' day
   * 86. St Nathaniel's Day 87. St Titus' Day 88. St Thérèse of Lisieux
   */
};

char *day_short[5] = {"SM","BT","PD","PP","SO"};

char *season_long[5] = { 
    "Chaos", "Discord", "Confusion", "Bureaucracy", "The Aftermath"
};

char *season_short[5] = {"Chs", "Dsc", "Cfn", "Bcy", "Afm"};

char *holyday[5][2] = { 
    { "Mungday", "Chaoflux" },
    { "Mojoday", "Discoflux" },
    { "Syaday",  "Confuflux" },
    { "Zaraday", "Bureflux" },
    { "Maladay", "Afflux" }
};

struct disc_time {
    int season; /* 0-4 */
    int day; /* 0-72 */
    int yday; /* 0-365 */
    int year; /* 3066- */
};

char *excl[] = {
    "Hail Eris!", "All Hail Discordia!", "Kallisti!", "Fnord.", "Or not.",
    "Wibble.", "Pzat!", "P'tang!", "Frink!",
#ifdef PRAISE_BOB
    "Slack!", "Praise \"Bob\"!", "Or kill me.",
#endif /* PRAISE_BOB */
    /* randomness, from the Net and other places. Feel free to add (after
       checking with the relevant authorities, of course). */
    "Grudnuk demand sustenance!", "Keep the Lasagna flying!",
    "You are what you see.",
    "Or is it?", "This statement is false.",
    "Lies and slander, sire!", "Hee hee hee!",
#if defined(linux) || defined (__linux__) || defined (__linux)
    "Hail Eris, Hack Linux!",
#elif defined(__APPLE__)
    "This Fruit is not the True Fruit of Discord.",
#endif
    ""
};

char default_fmt[] = "%{%A, %B %d%}, %Y YOLD";
char *default_immediate_fmt=
#ifdef OLD_IMMEDIATE_FMT
"Today is %{%A, the %e day of %B%} in the YOLD %Y%N%nCelebrate %H"
#else
default_fmt
#endif
;

#define DY(y) (y+1166)

static inline char *ending(int i) {
	return i/10==1?"th":(i%10==1?"st":(i%10==2?"nd":(i%10==3?"rd":"th")));
}

static inline int leapp(int i) {
	return (!(DY(i)%4))&&((DY(i)%100)||(!(DY(i)%400)));
}

/* select a random string */
static inline char *sel(char **strings, int num) {
	return(strings[random()%num]);
}

void print(struct disc_time,char **); /* old */
void format(char *buf, const char* fmt, struct disc_time dt);
/* read a fortune file */
int load_fortunes(char *fn, char *delim, char** result);

struct disc_time convert(int,int);
struct disc_time makeday(int,int,int);

int
main (int argc, char *argv[]) {
    time_t t;
    struct tm *eris;
    int bob,raw;
    struct disc_time hastur;
    char schwa[23*17], *fnord=0;
    int pi;
    char *progname, *p;

    progname = argv[0];
    if ((p = strrchr(progname, '/')) != NULL)
	progname = p+1;

    srandom(time(NULL));
    /* do args here */
    for(pi=1; pi<argc; pi++) {
	switch(argv[pi][0]) {
	case '+': fnord=argv[pi]+1; break;
	case '-': 
	    switch(argv[pi][1]) {
	    case 'V':
		printf(("%s (%s)\n"), progname, PACKAGE_STRING);
	    default: goto usage;
	    }
	default: goto thud;
	}
    }

  thud:
    if (argc-pi==3){ 
	int moe=atoi(argv[pi]), larry=atoi(argv[pi+1]), curly=atoi(argv[pi+2]);
	hastur=makeday(
#ifdef US_FORMAT
	    moe,larry,
#else
	    larry,moe,
#endif
	    curly);
	if (hastur.season == -1) {
		printf("Invalid date -- out of range\n");
		return -1;
	}
	fnord=fnord?fnord:default_fmt;
    } else if (argc!=pi) { 
      usage:
	fprintf(stderr,("usage: %s [+format] [day month year]\n"), argv[0]);
	exit(1);
    } else {
	t= time(NULL);
	eris=localtime(&t);
	bob=eris->tm_yday; /* days since Jan 1. */
	raw=eris->tm_year; /* years since 1980 */
	hastur=convert(bob,raw);
	fnord=fnord?fnord:default_immediate_fmt;
    }
    format(schwa, fnord, hastur);
    printf("%s\n", schwa);
   
    return 0;
}

void format(char *buf, const char* fmt, struct disc_time dt)
{
    int tib_start=-1, tib_end=0;
    int i, fmtlen=strlen(fmt);
    char *bufptr=buf;

/*    fprintf(stderr, "format(%p, \"%s\", dt)\n", buf, fmt);*/

    /* first, find extents of St. Tib's Day area, if defined */
    for(i=0; i<fmtlen; i++) {
	if(fmt[i]=='%') {
	    switch(fmt[i+1]) {
	    case 'A':
	    case 'a':
	    case 'd':
	    case 'e':
		if(tib_start>0)	    tib_end=i+1;
		else		    tib_start=i;
		break;
	    case '{': tib_start=i; break;
	    case '}': tib_end=i+1; break;
	    }
	}
    }

    /* now do the formatting */
    buf[0]=0;

    for(i=0; i<fmtlen; i++) {
	if((i==tib_start) && (dt.day==-1)) {
	    /* handle St. Tib's Day */
	    strcpy(bufptr, ("St. Tib's Day"));
	    bufptr += strlen(bufptr);
	    i=tib_end;
	} else {
	    if(fmt[i]=='%') {
		char *wibble=0, snarf[23];
		switch(fmt[++i]) {
		case 'A': wibble=day_long[dt.yday%5]; break;
		case 'a': wibble=day_short[dt.yday%5]; break;
		case 'B': wibble=season_long[dt.season]; break;
		case 'b': wibble=season_short[dt.season]; break;
		case 'd': sprintf(snarf, "%d", dt.day+1); wibble=snarf; break;
		case 'e': sprintf(snarf, "%d%s", dt.day+1, ending(dt.day+1)); 
		    wibble=snarf; break;
		case 'G': wibble=day_extended[dt.day]; break;
		case 'H': if(dt.day==4||dt.day==49)
		    wibble=holyday[dt.season][dt.day==49]; break;
		case 'N': if(dt.day!=4&&dt.day!=49) goto eschaton; break;
		case 'n': *(bufptr++)='\n'; break;
		case 't': *(bufptr++)='\t'; break;

		case 'Y': sprintf(snarf, "%d", dt.year); wibble=snarf; break;
		case '.': wibble=sel(excl, ARRAY_SIZE(excl));
		    break;
#ifdef KILL_BOB
		case 'X': sprintf(snarf, "%d", 
				  xday_countdown(dt.yday, dt.year));
				  wibble = snarf; break;
#endif /* KILL_BOB */
		}
		if(wibble) {
/*		    fprintf(stderr, "wibble = (%s)\n", wibble);*/
		    strcpy(bufptr, wibble); bufptr+=strlen(wibble);
		}
	    } else {
		*(bufptr++) = fmt[i];
	    }
	}
    }
  eschaton:
    *(bufptr)=0;
}

struct disc_time makeday(int imonth,int iday,int iyear) /*i for input */
{ 
    struct disc_time funkychickens;
    
    int cal[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    int dayspast=0;

    memset(&funkychickens,0,sizeof(funkychickens));
    /* basic range checks */
    if (imonth < 1 || imonth > 12 || iyear == 0) {
	    funkychickens.season = -1;
	    return funkychickens;
    }
    if (iday < 1 || iday > cal[imonth-1]) {
	    if (!(imonth == 2 && iday == 29 && iyear%4 == 0 &&
		  (iyear%100 != 0 || iyear%400 == 0))) {
		    funkychickens.season = -1;
		    return funkychickens;
	    }
    }
    
    imonth--;
    /*  note: gregorian year 0 doesn't exist so
     *  add one if user specifies a year less than 0 */
    funkychickens.year= iyear+1166 + ((0 > iyear)?1:0);
    while(imonth>0) { dayspast+=cal[--imonth]; }
    funkychickens.day=dayspast+iday-1;
    funkychickens.season=0;
    if((funkychickens.year%4)==2) {
	if (funkychickens.day==59 && iday==29)  funkychickens.day=-1;
    }
    funkychickens.yday=funkychickens.day;
/*               note: EQUAL SIGN...hopefully that fixes it */
    while(funkychickens.day>=73) {
	funkychickens.season++;
	funkychickens.day-=73;
    }
    return funkychickens;
}

struct disc_time convert(int nday, int nyear)
{  struct disc_time funkychickens;
   
   funkychickens.year = nyear+3066;
   funkychickens.day=nday;
   funkychickens.season=0;
   if ((funkychickens.year%4)==2)
     {if (funkychickens.day==59)
	funkychickens.day=-1;
     else if (funkychickens.day >59)
       funkychickens.day-=1;
    }
   funkychickens.yday=funkychickens.day;
   while (funkychickens.day>=73)
     { funkychickens.season++;
       funkychickens.day-=73;
     }
   return funkychickens;
  
 }

#ifdef KILL_BOB

/* Code for counting down to X-Day, X-Day being Cfn 40, 3164 
 *
 * After `X-Day' passed without incident, the CoSG declared that it had 
 * got the year upside down --- X-Day is actually in 8661 AD rather than 
 * 1998 AD.
 *
 * Thus, the True X-Day is Cfn 40, 9827.
 *
 */

int xday_countdown(int yday, int year) {
    int r=(185-yday)+(((yday<59)&&(leapp(year)))?1:0);
    while(year<9827) r+=(leapp(++year)?366:365);
    while(year>9827) r-=(leapp(year--)?366:365);
    return r;
}

#endif
