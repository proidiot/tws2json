/* solution.c: Functions for reading and writing the solution files.
 *
 * Copyright (C) 2001-2006 by Brian Raiter, under the GNU General Public
 * License. No warranty. See COPYING for details.
 */

#ifndef	_solution_h_
#define	_solution_h_

#include	"fileio.h"

/* The standard Boolean values.
 */
#ifndef	TRUE
#define	TRUE	1
#endif
#ifndef	FALSE
#define	FALSE	0
#endif

/* The four directions plus one non-direction.
 */
#define	NIL	0
#define	NORTH	1
#define	WEST	2
#define	SOUTH	4
#define	EAST	8

/* Translating directions to and from a two-bit representation. (Note
 * that NIL will map to the same value as NORTH.)
 */
#define	diridx(dir)	((0x30210 >> ((dir) * 2)) & 3)
#define	idxdir(idx)	(1 << ((idx) & 3))

/* The frequency of the gameplay timer. Note that "seconds" refers to
 * seconds in the game, which are not necessarily the same length as
 * real-time seconds.
 */
#define	TICKS_PER_SECOND	20

/* The gameplay timer's value is forced to remain within 23 bits.
 * Thus, gameplay of a single level cannot exceed 4 days 20 hours 30
 * minutes and 30.4 seconds.
 */
#define	MAXIMUM_TICK_COUNT	0x7FFFFF

/* A magic number used to indicate an undefined time value.
 */
#define	TIME_NIL		0x7FFFFFFF


/*
 * Miscellaneous definitions.
 */

/* The various rulesets the program can emulate.
 */
enum {
    Ruleset_None = 0,
    Ruleset_Lynx = 1,
    Ruleset_MS = 2,
    Ruleset_Count
};

/* Pseudorandom number generators.
 */
typedef	struct prng {
    unsigned long	initial;	/* initial seed value */
    unsigned long	value;		/* latest random value */
    char		shared;		/* FALSE if independent sequence */
} prng;

/*
 * Definitions used in game play.
 */

/* Turning macros.
 */
#define	left(dir)	((((dir) << 1) | ((dir) >> 3)) & 15)
#define	back(dir)	((((dir) << 2) | ((dir) >> 2)) & 15)
#define	right(dir)	((((dir) << 3) | ((dir) >> 1)) & 15)

/* A move is specified by its direction and when it takes place.
 */
typedef	struct action { unsigned int when:23, dir:9; } action;

/* A structure for managing the memory holding the moves of a game.
 */
typedef struct actlist {
    int			allocated;	/* number of elements allocated */
    int			count;		/* size of the actual array */
    action	       *list;		/* the array */
} actlist;

/* A structure holding all the data needed to reconstruct a solution.
 */
typedef	struct solutioninfo {
    actlist		moves;		/* the actual moves of the solution */
    unsigned long	solutionsize;	/* the size of the read solution data */
    unsigned long	besttime;
    unsigned long	rndseed;	/* the PRNG's initial seed */
    unsigned long	flags;		/* other flags (currently unused) */
    unsigned short	number;		/* the number of the level */
    unsigned char	rndslidedir;	/* random slide's initial direction */
    signed char		stepping;	/* the timer offset */
    char		passwd[5];	/* the level's password */
} solutioninfo;

/* The range of relative mouse moves is a 19x19 square around Chip.
 * (Mouse moves are stored as a relative offset in order to fit all
 * possible moves in nine bits.)
 */
#define	MOUSERANGEMIN	-9
#define	MOUSERANGEMAX	+9
#define	MOUSERANGE	19


/*
 * Solution functions.
 */

/* Initialize or reinitialize list as empty.
 */
extern void initmovelist(actlist *list);

/* Append move to the end of list.
 */
extern void addtomovelist(actlist *list, action move);

/* Make to an independent copy of from.
 */
extern void copymovelist(actlist *to, actlist const *from);

/* Deallocate list.
 */
extern void destroymovelist(actlist *list);

/* Read the header bytes of the given solution file. flags receives
 * the option bytes (bytes 5-6). extra receives any bytes in the
 * header that this code doesn't recognize.
 */
int readsolutionheader(fileinfo *file, int *ruleset, int *flags,
		       int *extrasize, unsigned char *extra);

/* Read the data of a one complete solution from the given file into
 * a solutioninfo structure.
 */
int readsolution(fileinfo *file, solutioninfo *solution);

/* Take the given solution and compress it, storing the compressed
 * data as part of the level's setup. FALSE is returned if an error
 * occurs. (It is not an error to compress the null solution.)
 */
//extern int contractsolution(solutioninfo const *solution, gamesetup *game);


#endif
