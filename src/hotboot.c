/****************************************************************************
 *                   ^     +----- |  / ^     ^ |     | +-\                  *
 *                  / \    |      | /  |\   /| |     | |  \                 *
 *                 /   \   +---   |<   | \ / | |     | |  |                 *
 *                /-----\  |      | \  |  v  | |     | |  /                 *
 *               /       \ |      |  \ |     | +-----+ +-/                  *
 ****************************************************************************
 * AFKMud Copyright 1997-2003 by Roger Libiez (Samson),                     *
 * Levi Beckerson (Whir), Michael Ward (Tarl), Erik Wolfe (Dwip),           *
 * Cameron Carroll (Cam), Cyberfox, Karangi, Rathian, Raine, and Adjani.    *
 * All Rights Reserved.                                                     *
 *                                                                          *
 * Original SMAUG 1.4a written by Thoric (Derek Snider) with Altrag,        *
 * Blodkai, Haus, Narn, Scryn, Swordbearer, Tricops, Gorog, Rennard,        *
 * Grishnakh, Fireblade, and Nivek.                                         *
 *                                                                          *
 * Original MERC 2.1 code by Hatchet, Furey, and Kahn.                      *
 *                                                                          *
 * Original DikuMUD code by: Hans Staerfeldt, Katja Nyboe, Tom Madsen,      *
 * Michael Seifert, and Sebastian Hammer.                                   *
 ****************************************************************************
 *                             Hotboot module                               *
 ****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include "mud.h"

#ifdef I3
   bool I3_is_connected( void );
   void I3_savemudlist( void );
   void I3_savechanlist( void );
   void I3_loadhistory( void );
   void I3_savehistory( void );
   extern int I3_socket;
#endif
#ifdef IMC
   void imc_shutdown( bool reconnect );
#endif

#define MAX_NEST	100
static	OBJ_DATA *	rgObjNest	[MAX_NEST];

#ifdef MCCP
bool write_to_descriptor( DESCRIPTOR_DATA *d, char *txt, int length );
bool write_to_descriptor_old( int desc, char *txt, int length );
#else
bool write_to_descriptor( int desc, char *txt, int length );
#endif
extern ROOM_INDEX_DATA *room_index_hash[MAX_KEY_HASH];

/*
 * Save the world's objects and mobs in their current positions -- Scion
 */
void save_mobile( FILE *fp, CHAR_DATA *mob )
{
   AFFECT_DATA *paf;
   SKILLTYPE *skill = NULL;

   if ( !IS_NPC( mob ) || !fp )
	return;
   fprintf( fp, "%s", "#MOBILE\n" );
   fprintf( fp, "Vnum	%d\n", mob->pIndexData->vnum );
   fprintf( fp, "Level   %d\n", mob->level );
   fprintf( fp, "Gold	%d\n", mob->gold );
   if ( mob->in_room ) 
   {
      if( xIS_SET( mob->act, ACT_SENTINEL ) )
      {
		  /* Sentinel mobs get stamped with a "home room" when they are created
		  by create_mobile(), so we need to save them in their home room regardless
		  of where they are right now, so they will go to their home room when they
		  enter the game from a reboot or copyover -- Scion */
	   fprintf( fp, "Room	%d\n", mob->home_vnum );
	} 
      else
	   fprintf( fp, "Room	%d\n", mob->in_room->vnum );
   }
   else
	fprintf( fp, "Room	%d\n", ROOM_VNUM_LIMBO );
#ifdef OVERLANDCODE
   fprintf( fp, "Coordinates  %d %d %d\n", mob->x, mob->y, mob->map );
#endif
   //~ if ( QUICKMATCH( mob->name, mob->pIndexData->player_name) == 0 )
        fprintf( fp, "Name     %s~\n", mob->name );
   //~ if ( QUICKMATCH( mob->short_descr, mob->pIndexData->short_descr) == 0 )
  	fprintf( fp, "Short	%s~\n", mob->short_descr );
   //~ if ( QUICKMATCH( mob->long_descr, mob->pIndexData->long_descr) == 0 )
  	fprintf( fp, "Long	%s~\n", mob->long_descr );
   //~ if ( QUICKMATCH( mob->description, mob->pIndexData->description) == 0 )
  	fprintf( fp, "Description %s~\n", mob->description );
   fprintf( fp, "HpManaMove   %d %d %d %d %d %d\n",
	mob->hit, mob->max_hit, mob->mana, mob->max_mana, mob->move, mob->max_move );
   fprintf( fp, "Position %d\n", mob->position );
   fprintf( fp, "Flags %s\n",   print_bitvector(&mob->act) );
   if ( !xIS_EMPTY( mob->affected_by ) )
      fprintf( fp, "AffectedBy   %s\n",	print_bitvector(&mob->affected_by) );

   for ( paf = mob->first_affect; paf; paf = paf->next )
   {
     	if ( paf->type >= 0 && (skill=get_skilltype(paf->type)) == NULL )
	    continue;

	if ( paf->type >= 0 && paf->type < TYPE_PERSONAL )
	  fprintf( fp, "AffectData   '%s' %3d %3d %3d %s\n",
	    skill->name, paf->duration, paf->modifier, paf->location, print_bitvector(&paf->bitvector) );
	else
	  fprintf( fp, "Affect       %3d %3d %3d %3d %s\n",
	    paf->type, paf->duration, paf->modifier, paf->location, print_bitvector(&paf->bitvector) );
   }

   de_equip_char( mob );

   if ( mob->first_carrying )
	fwrite_obj( mob, mob->last_carrying, fp, 0, OS_CARRY, TRUE );

   re_equip_char( mob );

   fprintf( fp, "%s", "EndMobile\n\n" );
   return;
}

void save_world( CHAR_DATA *ch )
{
   FILE *mobfp;
   FILE *objfp;
   int mobfile=0;
   char filename[256];
   CHAR_DATA *rch;
   ROOM_INDEX_DATA *pRoomIndex;
   int iHash;

   log_string( "Preserving world state...." );

   snprintf( filename, 256, "%s%s", SYSTEM_DIR, MOB_FILE );
   if ( ( mobfp = fopen( filename, "w" ) ) == NULL )
   {
	bug( "%s", "save_world: fopen mob file" );
	perror( filename );
   }
   else
	mobfile++;
	
   for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
   {
	for( pRoomIndex = room_index_hash[iHash]; pRoomIndex; pRoomIndex = pRoomIndex->next )
	{
	   if ( pRoomIndex )
	   {
	      if( !pRoomIndex->first_content                      /* Skip room if nothing in it */
	        || IS_SET( pRoomIndex->room_flags, ROOM_CLANSTOREROOM ) /* These rooms save on their own */
	        )
	         continue;

	      snprintf( filename, 256, "%s%d", HOTBOOT_DIR, pRoomIndex->vnum );
	      if( ( objfp = fopen( filename, "w" ) ) == NULL )
	      {
		   bug( "save_world: fopen %d", pRoomIndex->vnum );
		   perror( filename );
		   continue;
	      }
            fwrite_obj( NULL, pRoomIndex->last_content, objfp, 0, OS_CARRY, TRUE );
	      fprintf( objfp, "%s", "#END\n" );
	      fclose( objfp );
            objfp = NULL;
	   }
	}
   }
	
   if( mobfile )
   {
	for( rch = first_char; rch; rch = rch->next )
      {
	   if( !IS_NPC(rch) || rch == supermob || xIS_SET( rch->act, ACT_PET ) )
	      continue;
	   else
		save_mobile( mobfp, rch );
	}
	fprintf( mobfp, "%s", "#END\n" );
	fclose( mobfp );
      mobfp = NULL;
   }
   return;
}

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !strcmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

CHAR_DATA *load_mobile( FILE *fp )
{
   CHAR_DATA *mob = NULL;
   char *word;
   bool fMatch;
   int inroom = 0;
   ROOM_INDEX_DATA *pRoomIndex = NULL;

  word   = feof( fp ) ? "EndMobile" : fread_word( fp );
  if ( !str_cmp( word, "Vnum" ) )
  {
    int vnum;
    
    vnum = fread_number( fp );
    if( get_mob_index( vnum ) == NULL )
    {
	bug( "load_mobile: No index data for vnum %d", vnum );
	return NULL;
    }
    mob = create_mobile( get_mob_index( vnum ) );
    if ( !mob )
    {
	for ( ; ; ) 
      {
	  word   = feof( fp ) ? "EndMobile" : fread_word( fp );
	  /* So we don't get so many bug messages when something messes up
	   * --Shaddai 
	   */
	  if ( !str_cmp( word, "EndMobile" ) )
		break;
	}
	bug( "load_mobile: Unable to create mobile for vnum %d", vnum );
	return NULL;
    }
  }
  else
  {
	for ( ; ; ) 
      {
	  word   = feof( fp ) ? "EndMobile" : fread_word( fp );
	  /* So we don't get so many bug messages when something messes up
	   * --Shaddai 
	   */
	  if ( !str_cmp( word, "EndMobile" ) )
		break;
      }
	extract_char( mob, TRUE );
	bug( "%s", "load_mobile: Vnum not found" );
	return NULL;
  }
  for( ; ; ) 
  {
      word   = feof( fp ) ? "EndMobile" : fread_word( fp );
      fMatch = FALSE; 
      switch ( UPPER(word[0]) ) 
	{
	   case '*':
           fMatch = TRUE;
           fread_to_eol( fp );
           break;  
	   case '#':
		if ( !str_cmp( word, "#OBJECT" ) )
		{
		   mob->tempnum = -9999; /* Hackish, yes. Works though doesn't it? */
		   fread_obj( mob, fp, OS_CARRY );
		}
         case 'A':
	     if ( !str_cmp( word, "Affect" ) || !str_cmp( word, "AffectData" ) )
	     {
		  AFFECT_DATA *paf;

		  CREATE( paf, AFFECT_DATA, 1 );
		  if ( !str_cmp( word, "Affect" ) )
		  {
		    paf->type	= fread_number( fp );
		  }
		  else
		  {
		    int sn;
		    char *sname = fread_word(fp);

		    if ( (sn=skill_lookup(sname)) < 0 )
		    {
			if ( (sn=herb_lookup(sname)) < 0 )
			    bug( "%s", "load_mobile: unknown skill." );
			else
			    sn += TYPE_HERB;
		    }
		    paf->type = sn;
		  }

		  paf->duration	= fread_number( fp );
		  paf->modifier	= fread_number( fp );
		  paf->location	= fread_number( fp );
		  if ( paf->location == APPLY_WEAPONSPELL
		  ||   paf->location == APPLY_WEARSPELL
		  ||   paf->location == APPLY_REMOVESPELL
		  ||   paf->location == APPLY_STRIPSN
		  ||   paf->location == APPLY_RECURRINGSPELL )
		    paf->modifier	= slot_lookup( paf->modifier );
		  paf->bitvector	= fread_bitvector( fp );
		  LINK( paf, mob->first_affect, mob->last_affect, next, prev );
		  fMatch = TRUE;
		  break;
	      }
	      KEY( "AffectedBy",	mob->affected_by,	fread_bitvector( fp ) );
	    	break;
#ifdef OVERLANDCODE
	   case 'C':
	      if ( !str_cmp( word, "Coordinates" ) )
	      {
		  mob->x = fread_number( fp );
		  mob->y = fread_number( fp );
		  mob->map	 = fread_number( fp );

		  fMatch = TRUE;
		  break;
	      }
		break;
#endif
	   case 'D':
		KEY( "Description", mob->description, fread_string(fp));
		break;
	   case 'E':
		if ( !str_cmp( word, "EndMobile" ) )
		{
		   if ( inroom == 0 )
			inroom = ROOM_VNUM_LIMBO;
		   pRoomIndex = get_room_index( inroom );
		   if ( !pRoomIndex )
			pRoomIndex = get_room_index( ROOM_VNUM_LIMBO );
		   char_to_room( mob, pRoomIndex );
               mob->tempnum = -9998; /* Yet another hackish fix! */
		   return mob;
		} 
		if( !str_cmp( word, "End" ) ) /* End of object, need to ignore this. sometimes they creep in there somehow -- Scion */
		   fMatch = TRUE; /* Trick the system into thinking it matched something */
		break;
 	   case 'F':
		KEY( "Flags", mob->act, fread_bitvector( fp ) );
         case 'G':
		KEY( "Gold",	mob->gold,	fread_number( fp ) );
	 	break;
	   case 'H':
	      if ( !str_cmp( word, "HpManaMove" ) )
	      {
		   mob->hit		= fread_number( fp );
		   mob->max_hit	= fread_number( fp );
		   mob->mana	= fread_number( fp );
		   mob->max_mana	= fread_number( fp );
		   mob->move	= fread_number( fp );
		   mob->max_move	= fread_number( fp );

		   if ( mob->max_move <= 0 )
		      mob->max_move = 150;

		   fMatch = TRUE;
		   break;
	      }
		break;
	   case 'L':
		KEY( "Long", mob->long_descr, fread_string( fp ) );
	        KEY( "Level", mob->level,	fread_number( fp ) );
		break;
	   case 'N':
		KEY( "Name", mob->name, fread_string( fp ) );
		break;
	   case 'P':
		KEY( "Position", mob->position, fread_number( fp ) );
		break;
         case 'R':
            KEY( "Room", inroom, fread_number( fp ) );
            break;
         case 'S':
		KEY( "Short", mob->short_descr, fread_string(fp));
		break;
	}
	if ( !fMatch && str_cmp( word, "End" ) )
	   bug( "load_mobile: no match: %s", word );
  }
  return NULL;
}

void read_obj_file( char *dirname, char *filename )
{
   ROOM_INDEX_DATA *room;
   FILE *fp;
   char fname[256];
   int vnum;

   vnum = atoi( filename );
   snprintf( fname, 256, "%s%s", dirname, filename );

   if ( ( room = get_room_index( vnum ) ) == NULL )
   {
	bug( "read_obj_file: ARGH! Missing room index for %d!", vnum );
      unlink( fname );
	return;
   }
   
   if ( ( fp = fopen( fname, "r" ) ) != NULL )
   {
	sh_int iNest;
	bool found;
	OBJ_DATA *tobj, *tobj_next;

	rset_supermob( room );
      for ( iNest = 0; iNest < MAX_NEST; iNest++ )
	  rgObjNest[iNest] = NULL;

	found = TRUE;
	for ( ; ; )
	{
	   char letter;
	   char *word;

	   letter = fread_letter( fp );
	   if ( letter == '*' )
	   {
		fread_to_eol( fp );
		continue;
	   }

	   if ( letter != '#' )
	   {
		bug( "%s", "read_obj_file: # not found." );
		break;
	   }

	   word = fread_word( fp );
	   if ( !str_cmp( word, "OBJECT" ) )	/* Objects	*/
		fread_obj( supermob, fp, OS_CARRY );
	   else
		if ( !str_cmp( word, "END"    ) )	/* Done		*/
		  break;
	      else
		{
		    bug( "read_obj_file: bad section: %s", word );
		    break;
		}
	}
	fclose( fp );
      fp = NULL;
      unlink( fname );
	for ( tobj = supermob->first_carrying; tobj; tobj = tobj_next )
	{
	   tobj_next = tobj->next_content;
#ifdef OVERLANDCODE
	   if( IS_OBJ_STAT( tobj, ITEM_ONMAP ) )
	   {
		SET_ACT_FLAG( supermob, ACT_ONMAP );
		supermob->map = tobj->map;
		supermob->x = tobj->x;
		supermob->y = tobj->y;
	   }
#endif
         obj_from_char( tobj );
#ifndef OVERLANDCODE
	   obj_to_room( tobj, room );
#else
	   obj_to_room( tobj, room, supermob );
	   REMOVE_ACT_FLAG( supermob, ACT_ONMAP );
	   supermob->map = -1;
	   supermob->x = -1;
	   supermob->y = -1;
#endif
	}
	release_supermob();
   }
   else
	log_string( "Cannot open obj file" );

   return;
}

void load_obj_files( void )
{
   DIR *dp;
   struct dirent *dentry;
   char directory_name[100];

   log_string( "World state: loading objs" );
   snprintf( directory_name, 100, "%s", HOTBOOT_DIR );
   dp = opendir( directory_name );
   dentry = readdir( dp );
   while ( dentry )
   {
      /* Added by Tarl 3 Dec 02 because we are now using CVS */
      if ( !str_cmp( dentry->d_name, "CVS" ) ) 
      {
         dentry = readdir( dp );
         continue;
      }
      if ( dentry->d_name[0] != '.' )
	   read_obj_file( directory_name, dentry->d_name );
      dentry = readdir( dp );
   }
   closedir( dp );
   return;
}

void load_world( CHAR_DATA *ch )
{
   FILE *mobfp;
   char file1[256];
   char *word;
   int done = 0;
   bool mobfile = FALSE;

   snprintf( file1, 256, "%s%s", SYSTEM_DIR, MOB_FILE );
   if( ( mobfp = fopen( file1, "r" ) ) == NULL )
   {
	bug( "%s", "load_world: fopen mob file" );
	perror( file1 );
   } 
   else
	mobfile = TRUE;

   if( mobfile )
   {
	log_string( "World state: loading mobs" );
	while( done == 0 )
      {
	   if( feof( mobfp ) )
	      done++;
	   else 
	   {
		word = fread_word( mobfp );
		if( str_cmp( word, "#END" ) )
		   load_mobile( mobfp );
		else
		   done++;
	   }	
	}
	fclose( mobfp );
      mobfp = NULL;
   }
	
   load_obj_files();

   /* Once loaded, the data needs to be purged in the event it causes a crash so that it won't try to reload */
   unlink( file1 );
   return;
}

/*  Warm reboot stuff, gotta make sure to thank Erwin for this :) */
void do_hotboot( CHAR_DATA *ch, char *argument )
{
   FILE *fp;
   CHAR_DATA *victim = NULL;
   DESCRIPTOR_DATA *d, *de_next;
   char buf [100], buf2[100], buf3[100], buf4[100], buf5[100], buf6[100];
   extern int control, control2, conclient, conjava;
   int count = 0;
   bool found = FALSE;

   for( d = first_descriptor; d; d = d->next )
   {
      if ( ( d->connected == CON_PLAYING || d->connected == CON_EDITING )
        && ( victim = d->character ) != NULL && !IS_NPC( victim ) && victim->in_room
        &&   victim->fighting && victim->level >= 1 && victim->level <= MAX_LEVEL )
      {
         found = TRUE;
	   count++;
      }
   }

   if( found )
   {
	ch_printf( ch, "Cannot hotboot at this time. There are %d combats in progress.\n\r", count );
	return;
   }

   found = FALSE;
   for( d = first_descriptor; d; d = d->next )
   {
	if( d->connected == CON_EDITING && d->character )
	{
	   found = TRUE;
	   break;
	}
   }

   if( found )
   {
	send_to_char( "Cannot hotboot at this time. Someone is using the line editor.\n\r", ch );
	return;
   }

    snprintf( log_buf, MAX_STRING_LENGTH, "Hotboot initiated by %s.", ch->name );
    log_string( log_buf );

    fp = fopen( HOTBOOT_FILE, "w" );
    
    if( !fp )
    {
      send_to_char( "Hotboot file not writeable, aborted.\n\r", ch );
	bug( "Could not write to hotboot file: %s. Hotboot aborted.", HOTBOOT_FILE );
      perror( "do_copyover:fopen" );
      return;
    }
    
    /* And this one here will save the status of all objects and mobs in the game.
     * This really should ONLY ever be used here. The less we do stuff like this the better.
     */
    save_world( ch );

#ifdef IMC
   imc_shutdown( FALSE );
#endif

    log_string( "Saving player files and connection states...." );
    if( ch && ch->desc )
#ifdef MCCP
	  write_to_descriptor( ch->desc, "\e[0m", 0 );
#else
        write_to_descriptor( ch->desc->descriptor, "\e[0m", 0 );
#endif
    snprintf( buf, 100, "\n\rThe flow of time is halted momentarily as the world is reshaped!\n\r" );
    /* For each playing descriptor, save its state */
    for( d = first_descriptor; d ; d = de_next )
    {
        CHAR_DATA *och = CH(d);
        de_next = d->next; /* We delete from the list , so need to save this */
        if( !d->character || d->connected < CON_PLAYING ) /* drop those logging on */
        {
#ifdef MCCP
            write_to_descriptor( d, "\n\rSorry, we are rebooting. Come back in a few minutes.\n\r", 0 );
#else
		write_to_descriptor( d->descriptor, "\n\rSorry, we are rebooting. Come back in a few minutes.\n\r", 0 );
#endif
            close_socket( d, FALSE ); /* throw'em out */
        }
        else
        {
            fprintf( fp, "%d %d %d %d %d %s %s\n",
                     d->descriptor,
#ifdef MCCP
                     d->can_compress,
#else
                     0,
#endif
                     och->in_room->vnum, d->port, d->idle, och->name, d->host );
            /* One of two places this gets changed */
            och->pcdata->hotboot = TRUE;
            save_char_obj( och );
#ifdef MCCP
            write_to_descriptor( d, buf, 0 );
            compressEnd( d );
#else
		write_to_descriptor( d->descriptor, buf, 0 );
#endif
        }
    }

    fprintf( fp, "0 0 0 0 %d maxp maxp\n", sysdata.maxplayers );
    fprintf( fp, "%s", "-1\n" );
    fclose( fp );
    fp = NULL;

#ifdef I3
   if( I3_is_connected() )
   {
	I3_savechanlist();
	I3_savemudlist();
	I3_savehistory();
   }
#endif

   /* added this in case there's a need to debug the contents of the various files */
   if( argument && !str_cmp( argument, "debug" ) )
   {
	log_string( "Hotboot debug - Aborting before execl" );
	return;
   }

    log_string( "Executing hotboot...." );
    /* Close reserve and other always-open files and release other resources */
    fclose( fpReserve );
    fpReserve = NULL;
    fclose( fpLOG );
    fpLOG = NULL;
    
    /* exec - descriptors are inherited */
    snprintf( buf,  100, "%d", port );
    snprintf( buf2, 100, "%d", control );
    snprintf( buf3, 100, "%d", control2 );
    snprintf( buf4, 100, "%d", conclient );
    snprintf( buf5, 100, "%d", conjava );
#ifdef I3
    snprintf( buf6, 100, "%d", I3_socket );
#else
    strncpy ( buf6, "-1", 100 );
#endif

    set_alarm( 0 );
    /* Uncomment this dlclose line if you've installed the dlsym snippet, you'll need it. */
    // dlclose( sysdata.dlHandle );
    execl( EXE_FILE, "smaug", buf, "hotboot",  buf2, buf3, buf4, buf5, buf6, (char *)NULL );
    
    /* Failed - sucessful exec will not return */
    perror( "do_hotboot: execl" );

    /* Here you might want to reopen fpReserve */
    /* Since I'm a neophyte type guy, I'll assume this is a good idea and cut and past from main()  */
    
    if( !( fpReserve = fopen( NULL_FILE, "r" ) ) )
    {
        perror( NULL_FILE );
        exit( 1 );
    }
    if( !( fpLOG = fopen( NULL_FILE, "r" ) ) )
    {
        perror( NULL_FILE );
        exit( 1 );
    }
    bug( "%s", "Hotboot execution failed!!" );
    send_to_char( "Hotboot FAILED!\n\r", ch );
}

/* Recover from a hotboot - load players */
void hotboot_recover( void )
{
    DESCRIPTOR_DATA *d = NULL;
    FILE *fp;
    char name[100];
    char host[MAX_STRING_LENGTH];
    int desc, dcompress, room, dport, idle, maxp = 0;
    bool fOld;
    
    fp = fopen( HOTBOOT_FILE, "r" );
    
    if (!fp) /* there are some descriptors open which will hang forever then ? */
    {
        perror( "hotboot_recover: fopen" );
        bug( "%s", "Hotboot file not found. Exitting." );
        exit( 1 );
    }
    
    unlink( HOTBOOT_FILE ); /* In case something crashes - doesn't prevent reading */
    for( ; ; )
    {
        d = NULL;
        
        fscanf( fp, "%d %d %d %d %d %s %s\n", &desc, &dcompress, &room, &dport, &idle, name, host );

        if (desc == -1 || feof(fp) )
            break;

	  if( !str_cmp( name, "maxp" ) || !str_cmp( host, "maxp" ) )
	  {
	   maxp = idle;
	   continue;
	  }

        /* Write something, and check if it goes error-free */
#ifdef MCCP
        if( !dcompress && !write_to_descriptor_old( desc, "\n\rThe ether swirls in chaos.\n\r", 0 ) )
#else
	  if( !write_to_descriptor( desc, "\n\rThe ether swirls in chaos.\n\r", 0 ) )
#endif
        {
            close( desc ); /* nope */
            continue;
        }
        
        CREATE( d, DESCRIPTOR_DATA, 1 );

        d->next		= NULL;
        d->descriptor	= desc;
        d->connected	= CON_GET_NAME;
        d->outsize	= 2000;
        d->idle		= 0;
        d->lines		= 0;
        d->scrlen		= 24;
        d->newstate	= 0;
        d->prevcolor	= 0x08;

        CREATE( d->outbuf, char, d->outsize );

        d->user = STRALLOC( "Unknown" );
        d->host = STRALLOC( host );
        d->port = dport;
        d->idle = idle;
#ifdef MCCP
        d->can_compress = dcompress;
        if( d->can_compress )
           compressStart( d );
#endif
        LINK( d, first_descriptor, last_descriptor, next, prev );
        d->connected = CON_COPYOVER_RECOVER; /* negative so close_socket will cut them off */

        /* Now, find the pfile */
        fOld = load_char_obj( d, name, FALSE, TRUE );
        
        if( !fOld ) /* Player file not found?! */
        {
#ifdef MCCP
            write_to_descriptor( d, "\n\rSomehow, your character was lost during hotboot. Contact the immortals ASAP.\n\r", 0 );
#else
		write_to_descriptor( d->descriptor, "\n\rSomehow, your character was lost during hotboot. Contact the immortals ASAP.\n\r", 0 );
#endif
            close_socket( d, FALSE );
        }
        else /* ok! */
        {
#ifdef MCCP
            write_to_descriptor( d, "\n\rTime resumes its normal flow.\n\r", 0 );
#else
		write_to_descriptor( d->descriptor, "\n\rTime resumes its normal flow.\n\r", 0 );
#endif
            d->character->in_room = get_room_index( room );
            if( !d->character->in_room )
                d->character->in_room = get_room_index( ROOM_VNUM_TEMPLE );
            
            /* Insert in the char_list */
            LINK( d->character, first_char, last_char, next, prev );
            
            char_to_room( d->character, d->character->in_room );
	    // act( AT_MAGIC, "A puff of ethereal smoke dissipates around you!", d->character, NULL, NULL, TO_CHAR );
            // act( AT_MAGIC, "$n appears in a puff of ethereal smoke!", d->character, NULL, NULL, TO_ROOM );
            d->connected = CON_PLAYING;

			open_lua (d->character);  /* fire up Lua state */
			call_lua (d->character, "reconnected", d->character->name);

            if ( ++num_descriptors > sysdata.maxplayers )
	         sysdata.maxplayers = num_descriptors;
#ifdef AUTO_AUTH
            check_auth_state( d->character ); /* new auth */
#endif
        }
    }
    fclose( fp );
    fp = NULL;
    if( maxp > sysdata.maxplayers )
	sysdata.maxplayers = maxp;
    log_string( "Hotboot recovery complete." );
    return;
}
