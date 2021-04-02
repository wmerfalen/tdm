#ifndef __MODS_INDEX_TYPES_HEADER__
#define __MODS_INDEX_TYPES_HEADER__

#include <iostream>

#define CIRCLE_UNSIGNED_INDEX	1	/* 0 = signed, 1 = unsigned */

using sh_int = int32_t;
using ush_int = uint32_t;
#if CIRCLE_UNSIGNED_INDEX
#define IDXTYPE  ush_int
# define NOWHERE	((IDXTYPE)~0)
# define NOTHING	((IDXTYPE)~0)
# define NOBODY		((IDXTYPE)~0)
#else
#define IDXTYPE  sh_int
# define NOWHERE	(-1)	/* nil reference for rooms	*/
# define NOTHING	(-1)	/* nil reference for objects	*/
# define NOBODY		(-1)	/* nil reference for mobiles	*/
#endif

/* Various virtual (human-reference) number types. */
typedef IDXTYPE room_vnum;
typedef IDXTYPE obj_vnum;
typedef IDXTYPE mob_vnum;
typedef IDXTYPE zone_vnum;
typedef IDXTYPE shop_vnum;

/* Various real (array-reference) number types. */
typedef IDXTYPE room_rnum;
typedef IDXTYPE obj_rnum;
typedef IDXTYPE mob_rnum;
typedef IDXTYPE zone_rnum;
typedef IDXTYPE shop_rnum;



#endif
