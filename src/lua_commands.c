/****************************************************************************
 * [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   *
 * -----------------------------------------------------------|   (0...0)   *
 * SMAUG 1.4 (C) 1994, 1995, 1996, 1998  by Derek Snider      |    ).:.(    *
 * -----------------------------------------------------------|    {o o}    *
 * SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,      |   / ' ' \   *
 * Scryn, Rennard, Swordbearer, Gorog, Grishnakh, Nivek,      |~'~.VxvxV.~'~*
 * Tricops and Fireblade                                      |             *
 * ------------------------------------------------------------------------ *
 * Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
 * Chastain, Michael Quan, and Mitchell Tse.                                *
 * Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
 * Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
 * ------------------------------------------------------------------------ *
 *			 Lua Commands Scripting Module     by Nick Gammon                   *
 ****************************************************************************/
 
#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mud.h"

#include "lualib.h"
#include "lauxlib.h"

#define COMMAND_LIBRARY "cmd"

/* Implemented in lua_scripting.c */

CHAR_DATA * L_getchar (lua_State *L);

#define LUA_IMPLEMENT_DO_FUN(arg)  \
static int L_do_##arg (lua_State *L)  \
  {  \
  char command [MAX_INPUT_LENGTH];  \
  strncpy (command, luaL_checkstring (L, 1), sizeof (command));  \
  command [sizeof (command) - 1] = 0;  \
  do_##arg (L_getchar (L), command); \
  return 0;  \
  }

LUA_IMPLEMENT_DO_FUN( aassign );
LUA_IMPLEMENT_DO_FUN( add_imm_host );
/*  LUA_IMPLEMENT_DO_FUN( adminlist );  */
LUA_IMPLEMENT_DO_FUN( advance );
LUA_IMPLEMENT_DO_FUN( affected );
LUA_IMPLEMENT_DO_FUN( afk );
LUA_IMPLEMENT_DO_FUN( aid );
LUA_IMPLEMENT_DO_FUN( allow );
LUA_IMPLEMENT_DO_FUN( ansi );
LUA_IMPLEMENT_DO_FUN( answer );
LUA_IMPLEMENT_DO_FUN( apply );
LUA_IMPLEMENT_DO_FUN( appraise );
LUA_IMPLEMENT_DO_FUN( areas );
LUA_IMPLEMENT_DO_FUN( aset );
LUA_IMPLEMENT_DO_FUN( ask );
LUA_IMPLEMENT_DO_FUN( astat );
LUA_IMPLEMENT_DO_FUN( at );
LUA_IMPLEMENT_DO_FUN( atobj );
LUA_IMPLEMENT_DO_FUN( auction );
LUA_IMPLEMENT_DO_FUN( authorize );
LUA_IMPLEMENT_DO_FUN( avtalk );
LUA_IMPLEMENT_DO_FUN( backstab );
LUA_IMPLEMENT_DO_FUN( balzhur );
LUA_IMPLEMENT_DO_FUN( bamfin );
LUA_IMPLEMENT_DO_FUN( bamfout );
LUA_IMPLEMENT_DO_FUN( watch );
LUA_IMPLEMENT_DO_FUN( ban );
LUA_IMPLEMENT_DO_FUN( bash );
LUA_IMPLEMENT_DO_FUN( bashdoor );
LUA_IMPLEMENT_DO_FUN( berserk );
LUA_IMPLEMENT_DO_FUN( bestow );
LUA_IMPLEMENT_DO_FUN( bestowarea );
LUA_IMPLEMENT_DO_FUN( bio );
LUA_IMPLEMENT_DO_FUN( bite );
LUA_IMPLEMENT_DO_FUN( bloodlet );
LUA_IMPLEMENT_DO_FUN( boards );
LUA_IMPLEMENT_DO_FUN( bodybag );
LUA_IMPLEMENT_DO_FUN( bolt );
LUA_IMPLEMENT_DO_FUN( brandish );
LUA_IMPLEMENT_DO_FUN( brew );
LUA_IMPLEMENT_DO_FUN( broach );
LUA_IMPLEMENT_DO_FUN( bset );
LUA_IMPLEMENT_DO_FUN( bstat );
LUA_IMPLEMENT_DO_FUN( bug );
LUA_IMPLEMENT_DO_FUN( bury );
LUA_IMPLEMENT_DO_FUN( buy );
LUA_IMPLEMENT_DO_FUN( cast );
LUA_IMPLEMENT_DO_FUN( cedit );
LUA_IMPLEMENT_DO_FUN( channels );
LUA_IMPLEMENT_DO_FUN( chat );
LUA_IMPLEMENT_DO_FUN( check_vnums );
LUA_IMPLEMENT_DO_FUN( circle );
LUA_IMPLEMENT_DO_FUN( clans );
LUA_IMPLEMENT_DO_FUN( clantalk );
LUA_IMPLEMENT_DO_FUN( claw );
LUA_IMPLEMENT_DO_FUN( climate ) /* FB */;
LUA_IMPLEMENT_DO_FUN( climb );
LUA_IMPLEMENT_DO_FUN( close );
LUA_IMPLEMENT_DO_FUN( cmdtable );
LUA_IMPLEMENT_DO_FUN( commands );
LUA_IMPLEMENT_DO_FUN( comment );
LUA_IMPLEMENT_DO_FUN( compare );
LUA_IMPLEMENT_DO_FUN( config );
LUA_IMPLEMENT_DO_FUN( consider );
LUA_IMPLEMENT_DO_FUN( cook );
LUA_IMPLEMENT_DO_FUN( council_induct );
LUA_IMPLEMENT_DO_FUN( council_outcast );
LUA_IMPLEMENT_DO_FUN( councils );
LUA_IMPLEMENT_DO_FUN( counciltalk );
LUA_IMPLEMENT_DO_FUN( credits );
LUA_IMPLEMENT_DO_FUN( cset );
LUA_IMPLEMENT_DO_FUN( deities );
LUA_IMPLEMENT_DO_FUN( delay );
LUA_IMPLEMENT_DO_FUN( deny );
LUA_IMPLEMENT_DO_FUN( description );
LUA_IMPLEMENT_DO_FUN( destro );
LUA_IMPLEMENT_DO_FUN( destroy );
LUA_IMPLEMENT_DO_FUN( detrap );
LUA_IMPLEMENT_DO_FUN( devote );
LUA_IMPLEMENT_DO_FUN( dig );
LUA_IMPLEMENT_DO_FUN( disarm );
LUA_IMPLEMENT_DO_FUN( disconnect );
LUA_IMPLEMENT_DO_FUN( dismiss );
LUA_IMPLEMENT_DO_FUN( dismount );
LUA_IMPLEMENT_DO_FUN( dmesg );
LUA_IMPLEMENT_DO_FUN( dnd );
LUA_IMPLEMENT_DO_FUN( down );
LUA_IMPLEMENT_DO_FUN( drag );
LUA_IMPLEMENT_DO_FUN( drink );
LUA_IMPLEMENT_DO_FUN( drop );
LUA_IMPLEMENT_DO_FUN( east );
LUA_IMPLEMENT_DO_FUN( eat );
LUA_IMPLEMENT_DO_FUN( ech );
LUA_IMPLEMENT_DO_FUN( echo );
LUA_IMPLEMENT_DO_FUN( elevate );
LUA_IMPLEMENT_DO_FUN( emote );
LUA_IMPLEMENT_DO_FUN( empty );
LUA_IMPLEMENT_DO_FUN( enter );
LUA_IMPLEMENT_DO_FUN( equipment );
LUA_IMPLEMENT_DO_FUN( examine );
LUA_IMPLEMENT_DO_FUN( exits );
LUA_IMPLEMENT_DO_FUN( feed );
LUA_IMPLEMENT_DO_FUN( fill );
LUA_IMPLEMENT_DO_FUN( findnote );
LUA_IMPLEMENT_DO_FUN( fire );
LUA_IMPLEMENT_DO_FUN( fixchar );
LUA_IMPLEMENT_DO_FUN( fixed );
LUA_IMPLEMENT_DO_FUN( flee );
LUA_IMPLEMENT_DO_FUN( foldarea );
LUA_IMPLEMENT_DO_FUN( follow );
LUA_IMPLEMENT_DO_FUN( for );
LUA_IMPLEMENT_DO_FUN( force );
LUA_IMPLEMENT_DO_FUN( forceclose );
LUA_IMPLEMENT_DO_FUN( fprompt );
LUA_IMPLEMENT_DO_FUN( fquit )   /* Gorog */;
LUA_IMPLEMENT_DO_FUN( form_password );
LUA_IMPLEMENT_DO_FUN( freeze );
LUA_IMPLEMENT_DO_FUN( fshow );
LUA_IMPLEMENT_DO_FUN( get );
LUA_IMPLEMENT_DO_FUN( gfighting );
LUA_IMPLEMENT_DO_FUN( give );
LUA_IMPLEMENT_DO_FUN( glance );
LUA_IMPLEMENT_DO_FUN( gold );
LUA_IMPLEMENT_DO_FUN( goto );
LUA_IMPLEMENT_DO_FUN( gouge );
LUA_IMPLEMENT_DO_FUN( group );
LUA_IMPLEMENT_DO_FUN( gtell );
LUA_IMPLEMENT_DO_FUN( guilds );
LUA_IMPLEMENT_DO_FUN( guildtalk );
LUA_IMPLEMENT_DO_FUN( gwhere );
LUA_IMPLEMENT_DO_FUN( hedit );
LUA_IMPLEMENT_DO_FUN( hell );
LUA_IMPLEMENT_DO_FUN( help );
LUA_IMPLEMENT_DO_FUN( hide );
LUA_IMPLEMENT_DO_FUN( hitall );
LUA_IMPLEMENT_DO_FUN( hl );
LUA_IMPLEMENT_DO_FUN( hlist );
LUA_IMPLEMENT_DO_FUN( holylight );
LUA_IMPLEMENT_DO_FUN( homepage );
LUA_IMPLEMENT_DO_FUN( hset );
LUA_IMPLEMENT_DO_FUN( ide );
LUA_IMPLEMENT_DO_FUN( idea );
LUA_IMPLEMENT_DO_FUN( ignore );
LUA_IMPLEMENT_DO_FUN( immortalize );
LUA_IMPLEMENT_DO_FUN( immtalk );
LUA_IMPLEMENT_DO_FUN( imm_morph );
LUA_IMPLEMENT_DO_FUN( imm_unmorph );
LUA_IMPLEMENT_DO_FUN( induct );
LUA_IMPLEMENT_DO_FUN( installarea );
LUA_IMPLEMENT_DO_FUN( instaroom );
LUA_IMPLEMENT_DO_FUN( instazone );
LUA_IMPLEMENT_DO_FUN( inventory );
LUA_IMPLEMENT_DO_FUN( invis );
LUA_IMPLEMENT_DO_FUN( ipcompare );
LUA_IMPLEMENT_DO_FUN( khistory );
LUA_IMPLEMENT_DO_FUN( kick );
LUA_IMPLEMENT_DO_FUN( kill );
LUA_IMPLEMENT_DO_FUN( languages );
LUA_IMPLEMENT_DO_FUN( last );
LUA_IMPLEMENT_DO_FUN( laws );
LUA_IMPLEMENT_DO_FUN( leave );
LUA_IMPLEMENT_DO_FUN( level );
LUA_IMPLEMENT_DO_FUN( light );
LUA_IMPLEMENT_DO_FUN( list );
LUA_IMPLEMENT_DO_FUN( litterbug );
LUA_IMPLEMENT_DO_FUN( loadarea );
LUA_IMPLEMENT_DO_FUN( loadup );
LUA_IMPLEMENT_DO_FUN( lock );
LUA_IMPLEMENT_DO_FUN( log );
LUA_IMPLEMENT_DO_FUN( look );
LUA_IMPLEMENT_DO_FUN( low_purge );
LUA_IMPLEMENT_DO_FUN( mailroom );
LUA_IMPLEMENT_DO_FUN( make );
/*  LUA_IMPLEMENT_DO_FUN( makeadminlist );  */
LUA_IMPLEMENT_DO_FUN( makeboard );
LUA_IMPLEMENT_DO_FUN( makeclan );
LUA_IMPLEMENT_DO_FUN( makecouncil );
LUA_IMPLEMENT_DO_FUN( makedeity );
/*  LUA_IMPLEMENT_DO_FUN( makeguild );  */
LUA_IMPLEMENT_DO_FUN( makerepair );
LUA_IMPLEMENT_DO_FUN( makeshop );
LUA_IMPLEMENT_DO_FUN( makewizlist );
LUA_IMPLEMENT_DO_FUN( memory );
LUA_IMPLEMENT_DO_FUN( mcreate );
LUA_IMPLEMENT_DO_FUN( mdelete );
LUA_IMPLEMENT_DO_FUN( mfind );
LUA_IMPLEMENT_DO_FUN( minvoke );
LUA_IMPLEMENT_DO_FUN( mistwalk );
LUA_IMPLEMENT_DO_FUN( mlist );
LUA_IMPLEMENT_DO_FUN( morphcreate );
LUA_IMPLEMENT_DO_FUN( morphdestroy );
LUA_IMPLEMENT_DO_FUN( morphlist );
LUA_IMPLEMENT_DO_FUN( morphset );
LUA_IMPLEMENT_DO_FUN( morphstat );
LUA_IMPLEMENT_DO_FUN( mortalize );
LUA_IMPLEMENT_DO_FUN( mount );
LUA_IMPLEMENT_DO_FUN( mset );
LUA_IMPLEMENT_DO_FUN( mstat );
LUA_IMPLEMENT_DO_FUN( murde );
LUA_IMPLEMENT_DO_FUN( murder );
LUA_IMPLEMENT_DO_FUN( muse );
LUA_IMPLEMENT_DO_FUN( music );
LUA_IMPLEMENT_DO_FUN( mwhere );
LUA_IMPLEMENT_DO_FUN( name );
LUA_IMPLEMENT_DO_FUN( newbiechat );
LUA_IMPLEMENT_DO_FUN( newbieset );
LUA_IMPLEMENT_DO_FUN( news );
LUA_IMPLEMENT_DO_FUN( newscore );
LUA_IMPLEMENT_DO_FUN( newzones );
LUA_IMPLEMENT_DO_FUN( noemote );
LUA_IMPLEMENT_DO_FUN( noresolve );
LUA_IMPLEMENT_DO_FUN( north );
LUA_IMPLEMENT_DO_FUN( northeast );
LUA_IMPLEMENT_DO_FUN( northwest );
LUA_IMPLEMENT_DO_FUN( notell );
LUA_IMPLEMENT_DO_FUN( notitle );
LUA_IMPLEMENT_DO_FUN( noteroom );
LUA_IMPLEMENT_DO_FUN( nuisance );
LUA_IMPLEMENT_DO_FUN( ocreate );
LUA_IMPLEMENT_DO_FUN( odelete );
LUA_IMPLEMENT_DO_FUN( ofind );
LUA_IMPLEMENT_DO_FUN( oinvoke );
LUA_IMPLEMENT_DO_FUN( oldscore );
LUA_IMPLEMENT_DO_FUN( olist );
LUA_IMPLEMENT_DO_FUN( opcopy );
LUA_IMPLEMENT_DO_FUN( open );
LUA_IMPLEMENT_DO_FUN( order );
LUA_IMPLEMENT_DO_FUN( orders );
LUA_IMPLEMENT_DO_FUN( ordertalk );
LUA_IMPLEMENT_DO_FUN( oset );
LUA_IMPLEMENT_DO_FUN( ostat );
LUA_IMPLEMENT_DO_FUN( ot );
LUA_IMPLEMENT_DO_FUN( outcast );
LUA_IMPLEMENT_DO_FUN( pager );
LUA_IMPLEMENT_DO_FUN( pardon );
LUA_IMPLEMENT_DO_FUN( password );
LUA_IMPLEMENT_DO_FUN( pcrename );
LUA_IMPLEMENT_DO_FUN( peace );
LUA_IMPLEMENT_DO_FUN( pick );
LUA_IMPLEMENT_DO_FUN( plist );
LUA_IMPLEMENT_DO_FUN( poison_weapon );
/*  LUA_IMPLEMENT_DO_FUN( pose );  */
LUA_IMPLEMENT_DO_FUN( practice );
LUA_IMPLEMENT_DO_FUN( project );
LUA_IMPLEMENT_DO_FUN( prompt );
LUA_IMPLEMENT_DO_FUN( pset );
LUA_IMPLEMENT_DO_FUN( pstat );
LUA_IMPLEMENT_DO_FUN( pull );
LUA_IMPLEMENT_DO_FUN( punch );
LUA_IMPLEMENT_DO_FUN( purge );
LUA_IMPLEMENT_DO_FUN( push );
LUA_IMPLEMENT_DO_FUN( put );
LUA_IMPLEMENT_DO_FUN( qpset );
LUA_IMPLEMENT_DO_FUN( qpstat );
LUA_IMPLEMENT_DO_FUN( quaff );
LUA_IMPLEMENT_DO_FUN( quest );
LUA_IMPLEMENT_DO_FUN( qui );
LUA_IMPLEMENT_DO_FUN( quit );
LUA_IMPLEMENT_DO_FUN( racetalk );
LUA_IMPLEMENT_DO_FUN( rank );
LUA_IMPLEMENT_DO_FUN( rap );
LUA_IMPLEMENT_DO_FUN( rat );
LUA_IMPLEMENT_DO_FUN( rdelete );
LUA_IMPLEMENT_DO_FUN( reboo );
LUA_IMPLEMENT_DO_FUN( reboot );
LUA_IMPLEMENT_DO_FUN( recall );
LUA_IMPLEMENT_DO_FUN( recho );
LUA_IMPLEMENT_DO_FUN( recite );
LUA_IMPLEMENT_DO_FUN( redit );
LUA_IMPLEMENT_DO_FUN( regoto );
LUA_IMPLEMENT_DO_FUN( remains );
LUA_IMPLEMENT_DO_FUN( remove );
LUA_IMPLEMENT_DO_FUN( rent );
LUA_IMPLEMENT_DO_FUN( repair );
LUA_IMPLEMENT_DO_FUN( repairset );
LUA_IMPLEMENT_DO_FUN( repairshops );
LUA_IMPLEMENT_DO_FUN( repairstat );
LUA_IMPLEMENT_DO_FUN( repeat );
LUA_IMPLEMENT_DO_FUN( reply );
LUA_IMPLEMENT_DO_FUN( report );
LUA_IMPLEMENT_DO_FUN( rescue );
LUA_IMPLEMENT_DO_FUN( reserve );
LUA_IMPLEMENT_DO_FUN( reset );
LUA_IMPLEMENT_DO_FUN( rest );
LUA_IMPLEMENT_DO_FUN( restore );
LUA_IMPLEMENT_DO_FUN( restoretime );
LUA_IMPLEMENT_DO_FUN( restrict );
LUA_IMPLEMENT_DO_FUN( retell );
LUA_IMPLEMENT_DO_FUN( retire );
LUA_IMPLEMENT_DO_FUN( retran );
LUA_IMPLEMENT_DO_FUN( return );
LUA_IMPLEMENT_DO_FUN( revert );
LUA_IMPLEMENT_DO_FUN( rip );
LUA_IMPLEMENT_DO_FUN( rlist );
LUA_IMPLEMENT_DO_FUN( rolldie );
LUA_IMPLEMENT_DO_FUN( rstat );
LUA_IMPLEMENT_DO_FUN( sacrifice );
LUA_IMPLEMENT_DO_FUN( save );
LUA_IMPLEMENT_DO_FUN( savearea );
LUA_IMPLEMENT_DO_FUN( say );
LUA_IMPLEMENT_DO_FUN( scan );
LUA_IMPLEMENT_DO_FUN( scatter );
LUA_IMPLEMENT_DO_FUN( score );
LUA_IMPLEMENT_DO_FUN( scribe );
LUA_IMPLEMENT_DO_FUN( search );
LUA_IMPLEMENT_DO_FUN( sedit );
LUA_IMPLEMENT_DO_FUN( sell );
LUA_IMPLEMENT_DO_FUN( set_boot_time );
LUA_IMPLEMENT_DO_FUN( setclan );
LUA_IMPLEMENT_DO_FUN( setclass );
LUA_IMPLEMENT_DO_FUN( setcouncil );
LUA_IMPLEMENT_DO_FUN( setdeity );
LUA_IMPLEMENT_DO_FUN( setrace );
LUA_IMPLEMENT_DO_FUN( setweather );
LUA_IMPLEMENT_DO_FUN( shops );
LUA_IMPLEMENT_DO_FUN( shopset );
LUA_IMPLEMENT_DO_FUN( shopstat );
LUA_IMPLEMENT_DO_FUN( shout );
LUA_IMPLEMENT_DO_FUN( shove );
LUA_IMPLEMENT_DO_FUN( showclass );
LUA_IMPLEMENT_DO_FUN( showclan );
LUA_IMPLEMENT_DO_FUN( showcouncil );
LUA_IMPLEMENT_DO_FUN( showdeity );
LUA_IMPLEMENT_DO_FUN( showrace );
LUA_IMPLEMENT_DO_FUN( showweather )   /* FB */;
LUA_IMPLEMENT_DO_FUN( shutdow );
LUA_IMPLEMENT_DO_FUN( shutdown );
LUA_IMPLEMENT_DO_FUN( silence );
LUA_IMPLEMENT_DO_FUN( sit );
LUA_IMPLEMENT_DO_FUN( skin );
LUA_IMPLEMENT_DO_FUN( sla );
LUA_IMPLEMENT_DO_FUN( slay );
LUA_IMPLEMENT_DO_FUN( sleep );
LUA_IMPLEMENT_DO_FUN( slice );
LUA_IMPLEMENT_DO_FUN( slist );
LUA_IMPLEMENT_DO_FUN( slookup );
LUA_IMPLEMENT_DO_FUN( smoke );
LUA_IMPLEMENT_DO_FUN( sneak );
LUA_IMPLEMENT_DO_FUN( snoop );
LUA_IMPLEMENT_DO_FUN( sober );
LUA_IMPLEMENT_DO_FUN( socials );
LUA_IMPLEMENT_DO_FUN( south );
LUA_IMPLEMENT_DO_FUN( southeast );
LUA_IMPLEMENT_DO_FUN( southwest );
LUA_IMPLEMENT_DO_FUN( speak );
LUA_IMPLEMENT_DO_FUN( split );
LUA_IMPLEMENT_DO_FUN( sset );
LUA_IMPLEMENT_DO_FUN( stand );
LUA_IMPLEMENT_DO_FUN( stat );
LUA_IMPLEMENT_DO_FUN( statreport );
LUA_IMPLEMENT_DO_FUN( statshield );
LUA_IMPLEMENT_DO_FUN( steal );
LUA_IMPLEMENT_DO_FUN( sting );
LUA_IMPLEMENT_DO_FUN( strew );
LUA_IMPLEMENT_DO_FUN( strip );
LUA_IMPLEMENT_DO_FUN( stun );
LUA_IMPLEMENT_DO_FUN( style );
LUA_IMPLEMENT_DO_FUN( supplicate );
LUA_IMPLEMENT_DO_FUN( switch );
LUA_IMPLEMENT_DO_FUN( tail );
LUA_IMPLEMENT_DO_FUN( tamp );
LUA_IMPLEMENT_DO_FUN( tell );
LUA_IMPLEMENT_DO_FUN( think );
LUA_IMPLEMENT_DO_FUN( time );
LUA_IMPLEMENT_DO_FUN( timecmd );
LUA_IMPLEMENT_DO_FUN( title );
LUA_IMPLEMENT_DO_FUN( track );
LUA_IMPLEMENT_DO_FUN( traffic );
LUA_IMPLEMENT_DO_FUN( transfer );
LUA_IMPLEMENT_DO_FUN( trust );
LUA_IMPLEMENT_DO_FUN( typo );
LUA_IMPLEMENT_DO_FUN( unbolt );
LUA_IMPLEMENT_DO_FUN( unfoldarea );
LUA_IMPLEMENT_DO_FUN( unhell );
LUA_IMPLEMENT_DO_FUN( unlock );
LUA_IMPLEMENT_DO_FUN( unnuisance );
LUA_IMPLEMENT_DO_FUN( unsilence );
LUA_IMPLEMENT_DO_FUN( up );
LUA_IMPLEMENT_DO_FUN( users );
LUA_IMPLEMENT_DO_FUN( value );
LUA_IMPLEMENT_DO_FUN( vassign );
LUA_IMPLEMENT_DO_FUN( version );
LUA_IMPLEMENT_DO_FUN( victories );
LUA_IMPLEMENT_DO_FUN( visible );
LUA_IMPLEMENT_DO_FUN( vnums );
LUA_IMPLEMENT_DO_FUN( vsearch );
LUA_IMPLEMENT_DO_FUN( wake );
LUA_IMPLEMENT_DO_FUN( warn );
LUA_IMPLEMENT_DO_FUN( wartalk );
LUA_IMPLEMENT_DO_FUN( wear );
LUA_IMPLEMENT_DO_FUN( weather );
LUA_IMPLEMENT_DO_FUN( west );
LUA_IMPLEMENT_DO_FUN( where );
LUA_IMPLEMENT_DO_FUN( whisper );
LUA_IMPLEMENT_DO_FUN( who );
LUA_IMPLEMENT_DO_FUN( whois );
LUA_IMPLEMENT_DO_FUN( wimpy );
LUA_IMPLEMENT_DO_FUN( wizhelp );
LUA_IMPLEMENT_DO_FUN( wizlist );
LUA_IMPLEMENT_DO_FUN( wizlock );
LUA_IMPLEMENT_DO_FUN( worth );
LUA_IMPLEMENT_DO_FUN( yell );
LUA_IMPLEMENT_DO_FUN( zap );
LUA_IMPLEMENT_DO_FUN( zones );
;
/* mob prog stuff */;
LUA_IMPLEMENT_DO_FUN( mp_close_passage );
LUA_IMPLEMENT_DO_FUN( mp_damage );
LUA_IMPLEMENT_DO_FUN( mp_log );
LUA_IMPLEMENT_DO_FUN( mp_restore );
LUA_IMPLEMENT_DO_FUN( mp_open_passage );
LUA_IMPLEMENT_DO_FUN( mp_practice );
LUA_IMPLEMENT_DO_FUN( mp_slay );
LUA_IMPLEMENT_DO_FUN( mpadvance );
LUA_IMPLEMENT_DO_FUN( mpasound );
LUA_IMPLEMENT_DO_FUN( mpasupress );
LUA_IMPLEMENT_DO_FUN( mpat );
LUA_IMPLEMENT_DO_FUN( mpcopy );
LUA_IMPLEMENT_DO_FUN( mpdream );
LUA_IMPLEMENT_DO_FUN( mp_deposit );
LUA_IMPLEMENT_DO_FUN( mp_fill_in );
LUA_IMPLEMENT_DO_FUN( mp_withdraw );
LUA_IMPLEMENT_DO_FUN( mpecho );
LUA_IMPLEMENT_DO_FUN( mpechoaround );
LUA_IMPLEMENT_DO_FUN( mpechoat );
LUA_IMPLEMENT_DO_FUN( mpechozone );
LUA_IMPLEMENT_DO_FUN( mpedit );
LUA_IMPLEMENT_DO_FUN( opedit );
LUA_IMPLEMENT_DO_FUN( rpedit );
LUA_IMPLEMENT_DO_FUN( mpforce );
LUA_IMPLEMENT_DO_FUN( mpinvis );
LUA_IMPLEMENT_DO_FUN( mpgoto );
LUA_IMPLEMENT_DO_FUN( mpjunk );
LUA_IMPLEMENT_DO_FUN( mpkill );
LUA_IMPLEMENT_DO_FUN( mpmload );
LUA_IMPLEMENT_DO_FUN( mpmset );
LUA_IMPLEMENT_DO_FUN( mpnothing );
LUA_IMPLEMENT_DO_FUN( mpoload );
LUA_IMPLEMENT_DO_FUN( mposet );
LUA_IMPLEMENT_DO_FUN( mppardon );
LUA_IMPLEMENT_DO_FUN( mppeace );
LUA_IMPLEMENT_DO_FUN( mppurge );
LUA_IMPLEMENT_DO_FUN( mpstat );
LUA_IMPLEMENT_DO_FUN( opstat );
LUA_IMPLEMENT_DO_FUN( rpstat );
LUA_IMPLEMENT_DO_FUN( mptransfer );
LUA_IMPLEMENT_DO_FUN( mpmorph );
LUA_IMPLEMENT_DO_FUN( mpunmorph );
LUA_IMPLEMENT_DO_FUN( mpnuisance );
LUA_IMPLEMENT_DO_FUN( mpunnuisance );
LUA_IMPLEMENT_DO_FUN( mpbodybag );
LUA_IMPLEMENT_DO_FUN( mpapply );
LUA_IMPLEMENT_DO_FUN( mpapplyb );
LUA_IMPLEMENT_DO_FUN( mppkset );
LUA_IMPLEMENT_DO_FUN( mpfavor );
LUA_IMPLEMENT_DO_FUN( mpscatter );
LUA_IMPLEMENT_DO_FUN( mpdelay );
LUA_IMPLEMENT_DO_FUN( mpsound );
LUA_IMPLEMENT_DO_FUN( mpsoundaround );
LUA_IMPLEMENT_DO_FUN( mpsoundat );
LUA_IMPLEMENT_DO_FUN( mpmusic );
LUA_IMPLEMENT_DO_FUN( mpmusicaround );
LUA_IMPLEMENT_DO_FUN( mpmusicat );



#define LUA_DECLARE_DO_FUN(arg) {#arg, L_do_##arg},

static const struct luaL_reg cmdlib [] = 
  {
  /* "do" functions */
  
  LUA_DECLARE_DO_FUN( aassign )
  LUA_DECLARE_DO_FUN( add_imm_host )
/*  LUA_DECLARE_DO_FUN( adminlist )  */
  LUA_DECLARE_DO_FUN( advance )
  LUA_DECLARE_DO_FUN( affected )
  LUA_DECLARE_DO_FUN( afk )
  LUA_DECLARE_DO_FUN( aid )
  LUA_DECLARE_DO_FUN( allow )
  LUA_DECLARE_DO_FUN( ansi )
  LUA_DECLARE_DO_FUN( answer )
  LUA_DECLARE_DO_FUN( apply )
  LUA_DECLARE_DO_FUN( appraise )
  LUA_DECLARE_DO_FUN( areas )
  LUA_DECLARE_DO_FUN( aset )
  LUA_DECLARE_DO_FUN( ask )
  LUA_DECLARE_DO_FUN( astat )
  LUA_DECLARE_DO_FUN( at )
  LUA_DECLARE_DO_FUN( atobj )
  LUA_DECLARE_DO_FUN( auction )
  LUA_DECLARE_DO_FUN( authorize )
  LUA_DECLARE_DO_FUN( avtalk )
  LUA_DECLARE_DO_FUN( backstab )
  LUA_DECLARE_DO_FUN( balzhur )
  LUA_DECLARE_DO_FUN( bamfin )
  LUA_DECLARE_DO_FUN( bamfout )
  LUA_DECLARE_DO_FUN( watch )
  LUA_DECLARE_DO_FUN( ban )
  LUA_DECLARE_DO_FUN( bash )
  LUA_DECLARE_DO_FUN( bashdoor )
  LUA_DECLARE_DO_FUN( berserk )
  LUA_DECLARE_DO_FUN( bestow )
  LUA_DECLARE_DO_FUN( bestowarea )
  LUA_DECLARE_DO_FUN( bio )
  LUA_DECLARE_DO_FUN( bite )
  LUA_DECLARE_DO_FUN( bloodlet )
  LUA_DECLARE_DO_FUN( boards )
  LUA_DECLARE_DO_FUN( bodybag )
  LUA_DECLARE_DO_FUN( bolt )
  LUA_DECLARE_DO_FUN( brandish )
  LUA_DECLARE_DO_FUN( brew )
  LUA_DECLARE_DO_FUN( broach )
  LUA_DECLARE_DO_FUN( bset )
  LUA_DECLARE_DO_FUN( bstat )
  LUA_DECLARE_DO_FUN( bug )
  LUA_DECLARE_DO_FUN( bury )
  LUA_DECLARE_DO_FUN( buy )
  LUA_DECLARE_DO_FUN( cast )
  LUA_DECLARE_DO_FUN( cedit )
  LUA_DECLARE_DO_FUN( channels )
  LUA_DECLARE_DO_FUN( chat )
  LUA_DECLARE_DO_FUN( check_vnums )
  LUA_DECLARE_DO_FUN( circle )
  LUA_DECLARE_DO_FUN( clans )
  LUA_DECLARE_DO_FUN( clantalk )
  LUA_DECLARE_DO_FUN( claw )
  LUA_DECLARE_DO_FUN( climate ) /* FB */
  LUA_DECLARE_DO_FUN( climb )
  LUA_DECLARE_DO_FUN( close )
  LUA_DECLARE_DO_FUN( cmdtable )
  LUA_DECLARE_DO_FUN( commands )
  LUA_DECLARE_DO_FUN( comment )
  LUA_DECLARE_DO_FUN( compare )
  LUA_DECLARE_DO_FUN( config )
  LUA_DECLARE_DO_FUN( consider )
  LUA_DECLARE_DO_FUN( cook )
  LUA_DECLARE_DO_FUN( council_induct )
  LUA_DECLARE_DO_FUN( council_outcast )
  LUA_DECLARE_DO_FUN( councils )
  LUA_DECLARE_DO_FUN( counciltalk )
  LUA_DECLARE_DO_FUN( credits )
  LUA_DECLARE_DO_FUN( cset )
  LUA_DECLARE_DO_FUN( deities )
  LUA_DECLARE_DO_FUN( delay )
  LUA_DECLARE_DO_FUN( deny )
  LUA_DECLARE_DO_FUN( description )
  LUA_DECLARE_DO_FUN( destro )
  LUA_DECLARE_DO_FUN( destroy )
  LUA_DECLARE_DO_FUN( detrap )
  LUA_DECLARE_DO_FUN( devote )
  LUA_DECLARE_DO_FUN( dig )
  LUA_DECLARE_DO_FUN( disarm )
  LUA_DECLARE_DO_FUN( disconnect )
  LUA_DECLARE_DO_FUN( dismiss )
  LUA_DECLARE_DO_FUN( dismount )
  LUA_DECLARE_DO_FUN( dmesg )
  LUA_DECLARE_DO_FUN( dnd )
  LUA_DECLARE_DO_FUN( down )
  LUA_DECLARE_DO_FUN( drag )
  LUA_DECLARE_DO_FUN( drink )
  LUA_DECLARE_DO_FUN( drop )
  LUA_DECLARE_DO_FUN( east )
  LUA_DECLARE_DO_FUN( eat )
  LUA_DECLARE_DO_FUN( ech )
  LUA_DECLARE_DO_FUN( echo )
  LUA_DECLARE_DO_FUN( elevate )
  LUA_DECLARE_DO_FUN( emote )
  LUA_DECLARE_DO_FUN( empty )
  LUA_DECLARE_DO_FUN( enter )
  LUA_DECLARE_DO_FUN( equipment )
  LUA_DECLARE_DO_FUN( examine )
  LUA_DECLARE_DO_FUN( exits )
  LUA_DECLARE_DO_FUN( feed )
  LUA_DECLARE_DO_FUN( fill )
  LUA_DECLARE_DO_FUN( findnote )
  LUA_DECLARE_DO_FUN( fire )
  LUA_DECLARE_DO_FUN( fixchar )
  LUA_DECLARE_DO_FUN( fixed )
  LUA_DECLARE_DO_FUN( flee )
  LUA_DECLARE_DO_FUN( foldarea )
  LUA_DECLARE_DO_FUN( follow )
  LUA_DECLARE_DO_FUN( for )
  LUA_DECLARE_DO_FUN( force )
  LUA_DECLARE_DO_FUN( forceclose )
  LUA_DECLARE_DO_FUN( fprompt )
  LUA_DECLARE_DO_FUN( fquit )   /* Gorog */
  LUA_DECLARE_DO_FUN( form_password )
  LUA_DECLARE_DO_FUN( freeze )
  LUA_DECLARE_DO_FUN( fshow )
  LUA_DECLARE_DO_FUN( get )
  LUA_DECLARE_DO_FUN( gfighting )
  LUA_DECLARE_DO_FUN( give )
  LUA_DECLARE_DO_FUN( glance )
  LUA_DECLARE_DO_FUN( gold )
  LUA_DECLARE_DO_FUN( goto )
  LUA_DECLARE_DO_FUN( gouge )
  LUA_DECLARE_DO_FUN( group )
  LUA_DECLARE_DO_FUN( gtell )
  LUA_DECLARE_DO_FUN( guilds )
  LUA_DECLARE_DO_FUN( guildtalk )
  LUA_DECLARE_DO_FUN( gwhere )
  LUA_DECLARE_DO_FUN( hedit )
  LUA_DECLARE_DO_FUN( hell )
  LUA_DECLARE_DO_FUN( help )
  LUA_DECLARE_DO_FUN( hide )
  LUA_DECLARE_DO_FUN( hitall )
  LUA_DECLARE_DO_FUN( hl )
  LUA_DECLARE_DO_FUN( hlist )
  LUA_DECLARE_DO_FUN( holylight )
  LUA_DECLARE_DO_FUN( homepage )
  LUA_DECLARE_DO_FUN( hset )
  LUA_DECLARE_DO_FUN( ide )
  LUA_DECLARE_DO_FUN( idea )
  LUA_DECLARE_DO_FUN( ignore )
  LUA_DECLARE_DO_FUN( immortalize )
  LUA_DECLARE_DO_FUN( immtalk )
  LUA_DECLARE_DO_FUN( imm_morph )
  LUA_DECLARE_DO_FUN( imm_unmorph )
  LUA_DECLARE_DO_FUN( induct )
  LUA_DECLARE_DO_FUN( installarea )
  LUA_DECLARE_DO_FUN( instaroom )
  LUA_DECLARE_DO_FUN( instazone )
  LUA_DECLARE_DO_FUN( inventory )
  LUA_DECLARE_DO_FUN( invis )
  LUA_DECLARE_DO_FUN( ipcompare )
  LUA_DECLARE_DO_FUN( khistory )
  LUA_DECLARE_DO_FUN( kick )
  LUA_DECLARE_DO_FUN( kill )
  LUA_DECLARE_DO_FUN( languages )
  LUA_DECLARE_DO_FUN( last )
  LUA_DECLARE_DO_FUN( laws )
  LUA_DECLARE_DO_FUN( leave )
  LUA_DECLARE_DO_FUN( level )
  LUA_DECLARE_DO_FUN( light )
  LUA_DECLARE_DO_FUN( list )
  LUA_DECLARE_DO_FUN( litterbug )
  LUA_DECLARE_DO_FUN( loadarea )
  LUA_DECLARE_DO_FUN( loadup )
  LUA_DECLARE_DO_FUN( lock )
  LUA_DECLARE_DO_FUN( log )
  LUA_DECLARE_DO_FUN( look )
  LUA_DECLARE_DO_FUN( low_purge )
  LUA_DECLARE_DO_FUN( mailroom )
  LUA_DECLARE_DO_FUN( make )
/*  LUA_DECLARE_DO_FUN( makeadminlist )  */
  LUA_DECLARE_DO_FUN( makeboard )
  LUA_DECLARE_DO_FUN( makeclan )
  LUA_DECLARE_DO_FUN( makecouncil )
  LUA_DECLARE_DO_FUN( makedeity )
/*  LUA_DECLARE_DO_FUN( makeguild )  */
  LUA_DECLARE_DO_FUN( makerepair )
  LUA_DECLARE_DO_FUN( makeshop )
  LUA_DECLARE_DO_FUN( makewizlist )
  LUA_DECLARE_DO_FUN( memory )
  LUA_DECLARE_DO_FUN( mcreate )
  LUA_DECLARE_DO_FUN( mdelete )
  LUA_DECLARE_DO_FUN( mfind )
  LUA_DECLARE_DO_FUN( minvoke )
  LUA_DECLARE_DO_FUN( mistwalk )
  LUA_DECLARE_DO_FUN( mlist )
  LUA_DECLARE_DO_FUN( morphcreate )
  LUA_DECLARE_DO_FUN( morphdestroy )
  LUA_DECLARE_DO_FUN( morphlist )
  LUA_DECLARE_DO_FUN( morphset )
  LUA_DECLARE_DO_FUN( morphstat )
  LUA_DECLARE_DO_FUN( mortalize )
  LUA_DECLARE_DO_FUN( mount )
  LUA_DECLARE_DO_FUN( mset )
  LUA_DECLARE_DO_FUN( mstat )
  LUA_DECLARE_DO_FUN( murde )
  LUA_DECLARE_DO_FUN( murder )
  LUA_DECLARE_DO_FUN( muse )
  LUA_DECLARE_DO_FUN( music )
  LUA_DECLARE_DO_FUN( mwhere )
  LUA_DECLARE_DO_FUN( name )
  LUA_DECLARE_DO_FUN( newbiechat )
  LUA_DECLARE_DO_FUN( newbieset )
  LUA_DECLARE_DO_FUN( news )
  LUA_DECLARE_DO_FUN( newscore )
  LUA_DECLARE_DO_FUN( newzones )
  LUA_DECLARE_DO_FUN( noemote )
  LUA_DECLARE_DO_FUN( noresolve )
  LUA_DECLARE_DO_FUN( north )
  LUA_DECLARE_DO_FUN( northeast )
  LUA_DECLARE_DO_FUN( northwest )
  LUA_DECLARE_DO_FUN( notell )
  LUA_DECLARE_DO_FUN( notitle )
  LUA_DECLARE_DO_FUN( noteroom )
  LUA_DECLARE_DO_FUN( nuisance )
  LUA_DECLARE_DO_FUN( ocreate )
  LUA_DECLARE_DO_FUN( odelete )
  LUA_DECLARE_DO_FUN( ofind )
  LUA_DECLARE_DO_FUN( oinvoke )
  LUA_DECLARE_DO_FUN( oldscore )
  LUA_DECLARE_DO_FUN( olist )
  LUA_DECLARE_DO_FUN( opcopy )
  LUA_DECLARE_DO_FUN( open )
  LUA_DECLARE_DO_FUN( order )
  LUA_DECLARE_DO_FUN( orders )
  LUA_DECLARE_DO_FUN( ordertalk )
  LUA_DECLARE_DO_FUN( oset )
  LUA_DECLARE_DO_FUN( ostat )
  LUA_DECLARE_DO_FUN( ot )
  LUA_DECLARE_DO_FUN( outcast )
  LUA_DECLARE_DO_FUN( pager )
  LUA_DECLARE_DO_FUN( pardon )
  LUA_DECLARE_DO_FUN( password )
  LUA_DECLARE_DO_FUN( pcrename )
  LUA_DECLARE_DO_FUN( peace )
  LUA_DECLARE_DO_FUN( pick )
  LUA_DECLARE_DO_FUN( plist )
  LUA_DECLARE_DO_FUN( poison_weapon )
/*  LUA_DECLARE_DO_FUN( pose )  */
  LUA_DECLARE_DO_FUN( practice )
  LUA_DECLARE_DO_FUN( project )
  LUA_DECLARE_DO_FUN( prompt )
  LUA_DECLARE_DO_FUN( pset )
  LUA_DECLARE_DO_FUN( pstat )
  LUA_DECLARE_DO_FUN( pull )
  LUA_DECLARE_DO_FUN( punch )
  LUA_DECLARE_DO_FUN( purge )
  LUA_DECLARE_DO_FUN( push )
  LUA_DECLARE_DO_FUN( put )
  LUA_DECLARE_DO_FUN( qpset )
  LUA_DECLARE_DO_FUN( qpstat )
  LUA_DECLARE_DO_FUN( quaff )
  LUA_DECLARE_DO_FUN( quest )
  LUA_DECLARE_DO_FUN( qui )
  LUA_DECLARE_DO_FUN( quit )
  LUA_DECLARE_DO_FUN( racetalk )
  LUA_DECLARE_DO_FUN( rank )
  LUA_DECLARE_DO_FUN( rap )
  LUA_DECLARE_DO_FUN( rat )
  LUA_DECLARE_DO_FUN( rdelete )
  LUA_DECLARE_DO_FUN( reboo )
  LUA_DECLARE_DO_FUN( reboot )
  LUA_DECLARE_DO_FUN( recall )
  LUA_DECLARE_DO_FUN( recho )
  LUA_DECLARE_DO_FUN( recite )
  LUA_DECLARE_DO_FUN( redit )
  LUA_DECLARE_DO_FUN( regoto )
  LUA_DECLARE_DO_FUN( remains )
  LUA_DECLARE_DO_FUN( remove )
  LUA_DECLARE_DO_FUN( rent )
  LUA_DECLARE_DO_FUN( repair )
  LUA_DECLARE_DO_FUN( repairset )
  LUA_DECLARE_DO_FUN( repairshops )
  LUA_DECLARE_DO_FUN( repairstat )
  LUA_DECLARE_DO_FUN( repeat )
  LUA_DECLARE_DO_FUN( reply )
  LUA_DECLARE_DO_FUN( report )
  LUA_DECLARE_DO_FUN( rescue )
  LUA_DECLARE_DO_FUN( reserve )
  LUA_DECLARE_DO_FUN( reset )
  LUA_DECLARE_DO_FUN( rest )
  LUA_DECLARE_DO_FUN( restore )
  LUA_DECLARE_DO_FUN( restoretime )
  LUA_DECLARE_DO_FUN( restrict )
  LUA_DECLARE_DO_FUN( retell )
  LUA_DECLARE_DO_FUN( retire )
  LUA_DECLARE_DO_FUN( retran )
  LUA_DECLARE_DO_FUN( return )
  LUA_DECLARE_DO_FUN( revert )
  LUA_DECLARE_DO_FUN( rip )
  LUA_DECLARE_DO_FUN( rlist )
  LUA_DECLARE_DO_FUN( rolldie )
  LUA_DECLARE_DO_FUN( rstat )
  LUA_DECLARE_DO_FUN( sacrifice )
  LUA_DECLARE_DO_FUN( save )
  LUA_DECLARE_DO_FUN( savearea )
  LUA_DECLARE_DO_FUN( say )
  LUA_DECLARE_DO_FUN( scan )
  LUA_DECLARE_DO_FUN( scatter )
  LUA_DECLARE_DO_FUN( score )
  LUA_DECLARE_DO_FUN( scribe )
  LUA_DECLARE_DO_FUN( search )
  LUA_DECLARE_DO_FUN( sedit )
  LUA_DECLARE_DO_FUN( sell )
  LUA_DECLARE_DO_FUN( set_boot_time )
  LUA_DECLARE_DO_FUN( setclan )
  LUA_DECLARE_DO_FUN( setclass )
  LUA_DECLARE_DO_FUN( setcouncil )
  LUA_DECLARE_DO_FUN( setdeity )
  LUA_DECLARE_DO_FUN( setrace )
  LUA_DECLARE_DO_FUN( setweather )
  LUA_DECLARE_DO_FUN( shops )
  LUA_DECLARE_DO_FUN( shopset )
  LUA_DECLARE_DO_FUN( shopstat )
  LUA_DECLARE_DO_FUN( shout )
  LUA_DECLARE_DO_FUN( shove )
  LUA_DECLARE_DO_FUN( showclass )
  LUA_DECLARE_DO_FUN( showclan )
  LUA_DECLARE_DO_FUN( showcouncil )
  LUA_DECLARE_DO_FUN( showdeity )
  LUA_DECLARE_DO_FUN( showrace )
  LUA_DECLARE_DO_FUN( showweather )   /* FB */
  LUA_DECLARE_DO_FUN( shutdow )
  LUA_DECLARE_DO_FUN( shutdown )
  LUA_DECLARE_DO_FUN( silence )
  LUA_DECLARE_DO_FUN( sit )
  LUA_DECLARE_DO_FUN( skin )
  LUA_DECLARE_DO_FUN( sla )
  LUA_DECLARE_DO_FUN( slay )
  LUA_DECLARE_DO_FUN( sleep )
  LUA_DECLARE_DO_FUN( slice )
  LUA_DECLARE_DO_FUN( slist )
  LUA_DECLARE_DO_FUN( slookup )
  LUA_DECLARE_DO_FUN( smoke )
  LUA_DECLARE_DO_FUN( sneak )
  LUA_DECLARE_DO_FUN( snoop )
  LUA_DECLARE_DO_FUN( sober )
  LUA_DECLARE_DO_FUN( socials )
  LUA_DECLARE_DO_FUN( south )
  LUA_DECLARE_DO_FUN( southeast )
  LUA_DECLARE_DO_FUN( southwest )
  LUA_DECLARE_DO_FUN( speak )
  LUA_DECLARE_DO_FUN( split )
  LUA_DECLARE_DO_FUN( sset )
  LUA_DECLARE_DO_FUN( stand )
  LUA_DECLARE_DO_FUN( stat )
  LUA_DECLARE_DO_FUN( statreport )
  LUA_DECLARE_DO_FUN( statshield )
  LUA_DECLARE_DO_FUN( steal )
  LUA_DECLARE_DO_FUN( sting )
  LUA_DECLARE_DO_FUN( strew )
  LUA_DECLARE_DO_FUN( strip )
  LUA_DECLARE_DO_FUN( stun )
  LUA_DECLARE_DO_FUN( style )
  LUA_DECLARE_DO_FUN( supplicate )
  LUA_DECLARE_DO_FUN( switch )
  LUA_DECLARE_DO_FUN( tail )
  LUA_DECLARE_DO_FUN( tamp )
  LUA_DECLARE_DO_FUN( tell )
  LUA_DECLARE_DO_FUN( think )
  LUA_DECLARE_DO_FUN( time )
  LUA_DECLARE_DO_FUN( timecmd )
  LUA_DECLARE_DO_FUN( title )
  LUA_DECLARE_DO_FUN( track )
  LUA_DECLARE_DO_FUN( traffic )
  LUA_DECLARE_DO_FUN( transfer )
  LUA_DECLARE_DO_FUN( trust )
  LUA_DECLARE_DO_FUN( typo )
  LUA_DECLARE_DO_FUN( unbolt )
  LUA_DECLARE_DO_FUN( unfoldarea )
  LUA_DECLARE_DO_FUN( unhell )
  LUA_DECLARE_DO_FUN( unlock )
  LUA_DECLARE_DO_FUN( unnuisance )
  LUA_DECLARE_DO_FUN( unsilence )
  LUA_DECLARE_DO_FUN( up )
  LUA_DECLARE_DO_FUN( users )
  LUA_DECLARE_DO_FUN( value )
  LUA_DECLARE_DO_FUN( vassign )
  LUA_DECLARE_DO_FUN( version )
  LUA_DECLARE_DO_FUN( victories )
  LUA_DECLARE_DO_FUN( visible )
  LUA_DECLARE_DO_FUN( vnums )
  LUA_DECLARE_DO_FUN( vsearch )
  LUA_DECLARE_DO_FUN( wake )
  LUA_DECLARE_DO_FUN( warn )
  LUA_DECLARE_DO_FUN( wartalk )
  LUA_DECLARE_DO_FUN( wear )
  LUA_DECLARE_DO_FUN( weather )
  LUA_DECLARE_DO_FUN( west )
  LUA_DECLARE_DO_FUN( where )
  LUA_DECLARE_DO_FUN( whisper )
  LUA_DECLARE_DO_FUN( who )
  LUA_DECLARE_DO_FUN( whois )
  LUA_DECLARE_DO_FUN( wimpy )
  LUA_DECLARE_DO_FUN( wizhelp )
  LUA_DECLARE_DO_FUN( wizlist )
  LUA_DECLARE_DO_FUN( wizlock )
  LUA_DECLARE_DO_FUN( worth )
  LUA_DECLARE_DO_FUN( yell )
  LUA_DECLARE_DO_FUN( zap )
  LUA_DECLARE_DO_FUN( zones )
  
  /* mob prog stuff */
  LUA_DECLARE_DO_FUN( mp_close_passage )
  LUA_DECLARE_DO_FUN( mp_damage )
  LUA_DECLARE_DO_FUN( mp_log )
  LUA_DECLARE_DO_FUN( mp_restore )
  LUA_DECLARE_DO_FUN( mp_open_passage )
  LUA_DECLARE_DO_FUN( mp_practice )
  LUA_DECLARE_DO_FUN( mp_slay )
  LUA_DECLARE_DO_FUN( mpadvance )
  LUA_DECLARE_DO_FUN( mpasound )
  LUA_DECLARE_DO_FUN( mpasupress )
  LUA_DECLARE_DO_FUN( mpat )
  LUA_DECLARE_DO_FUN( mpcopy )
  LUA_DECLARE_DO_FUN( mpdream )
  LUA_DECLARE_DO_FUN( mp_deposit )
  LUA_DECLARE_DO_FUN( mp_fill_in )
  LUA_DECLARE_DO_FUN( mp_withdraw )
  LUA_DECLARE_DO_FUN( mpecho )
  LUA_DECLARE_DO_FUN( mpechoaround )
  LUA_DECLARE_DO_FUN( mpechoat )
  LUA_DECLARE_DO_FUN( mpechozone )
  LUA_DECLARE_DO_FUN( mpedit )
  LUA_DECLARE_DO_FUN( opedit )
  LUA_DECLARE_DO_FUN( rpedit )
  LUA_DECLARE_DO_FUN( mpforce )
  LUA_DECLARE_DO_FUN( mpinvis )
  LUA_DECLARE_DO_FUN( mpgoto )
  LUA_DECLARE_DO_FUN( mpjunk )
  LUA_DECLARE_DO_FUN( mpkill )
  LUA_DECLARE_DO_FUN( mpmload )
  LUA_DECLARE_DO_FUN( mpmset )
  LUA_DECLARE_DO_FUN( mpnothing )
  LUA_DECLARE_DO_FUN( mpoload )
  LUA_DECLARE_DO_FUN( mposet )
  LUA_DECLARE_DO_FUN( mppardon )
  LUA_DECLARE_DO_FUN( mppeace )
  LUA_DECLARE_DO_FUN( mppurge )
  LUA_DECLARE_DO_FUN( mpstat )
  LUA_DECLARE_DO_FUN( opstat )
  LUA_DECLARE_DO_FUN( rpstat )
  LUA_DECLARE_DO_FUN( mptransfer )
  LUA_DECLARE_DO_FUN( mpmorph )
  LUA_DECLARE_DO_FUN( mpunmorph )
  LUA_DECLARE_DO_FUN( mpnuisance )
  LUA_DECLARE_DO_FUN( mpunnuisance )
  LUA_DECLARE_DO_FUN( mpbodybag )
  LUA_DECLARE_DO_FUN( mpapply )
  LUA_DECLARE_DO_FUN( mpapplyb )
  LUA_DECLARE_DO_FUN( mppkset )
  LUA_DECLARE_DO_FUN( mpfavor )
  LUA_DECLARE_DO_FUN( mpscatter )
  LUA_DECLARE_DO_FUN( mpdelay )
  LUA_DECLARE_DO_FUN( mpsound )
  LUA_DECLARE_DO_FUN( mpsoundaround )
  LUA_DECLARE_DO_FUN( mpsoundat )
  LUA_DECLARE_DO_FUN( mpmusic )
  LUA_DECLARE_DO_FUN( mpmusicaround )
  LUA_DECLARE_DO_FUN( mpmusicat )

  {NULL, NULL}
  };  /* end of cmdlib */

void RegisterLuaCommands (lua_State *L)
  {
  /* register all cmd.xxx routines (eg. do_look) */
  luaL_register (L, COMMAND_LIBRARY, cmdlib);
  }  /* end of RegisterLuaCommands */
  
  
