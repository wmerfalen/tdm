#ifndef __MENTOC_MODS_POSITIONS_HEADER__
#define __MENTOC_MODS_POSITIONS_HEADER__

namespace mods::pos {
	/* Positions */
	constexpr static uint8_t DEAD       = 0;	/* dead			*/
	constexpr static uint8_t MORTALLYW  = 1;	/* mortally wounded	*/
	constexpr static uint8_t INCAP      = 2;	/* incapacitated	*/
	constexpr static uint8_t STUNNED    = 3;	/* stunned		*/
	constexpr static uint8_t SLEEPING   = 4;	/* sleeping		*/
	constexpr static uint8_t RESTING    = 5;	/* resting		*/
	constexpr static uint8_t SITTING    = 6;	/* sitting		*/
	constexpr static uint8_t FIGHTING   = 7;	/* fighting		*/
	constexpr static uint8_t STANDING   = 8;	/* standing		*/
};

enum position_t : int8_t {
	INVALID = -1,
	DEAD = mods::pos::DEAD,
	MORTALLYW = mods::pos::MORTALLYW,
	INCAP = mods::pos::INCAP,
	STUNNED = mods::pos::STUNNED,
	SLEEPING = mods::pos::SLEEPING,
	RESTING = mods::pos::RESTING,
	SITTING = mods::pos::SITTING,
	FIGHTING = mods::pos::FIGHTING,
	STANDING = mods::pos::STANDING,
};
#endif
