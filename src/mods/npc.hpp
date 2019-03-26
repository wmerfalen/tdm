#ifndef __MENTOC_MODS_NPC_HEADER__
#define  __MENTOC_MODS_NPC_HEADER__

#include <iostream>
#include "../structs.h"

namespace mods {
	struct npc{
		public:
			/* constructors and destructors */
			npc();
			npc(char_data*);
			npc(mods::npc*);
			~npc();
			void init();
			/* setters */
			void set_cd(char_data* ch);
			void set_shared_ptr(std::shared_ptr<mods::npc>&);

		private: 
			char_data*   m_char_data;
			std::shared_ptr<char_data> m_shared_ptr;
	};
};

#endif
