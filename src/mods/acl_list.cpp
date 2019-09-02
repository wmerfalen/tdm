#include "acl_list.hpp"
#include <set>
#include <functional>
namespace mods::acl_list {
	namespace list_container{
		using player_acl_list_t = std::set<std::string>;
		static player_acl_list_t implementors;
		static player_acl_list_t gods;
		static player_acl_list_t builders;
	};

	void set_access_rights(player_ptr_t player,
			const std::string& group,bool on_or_off){
		struct _list_{
			list_container::player_acl_list_t* ptr;
			const char* name;
			std::function<void(player_ptr_t,bool)> setter;
		} mappings[] ={
			{&list_container::implementors,"implementors",[](player_ptr_t player,bool on){ player->set_imp_mode(on); } },
			{&list_container::gods,"gods", [](player_ptr_t player,bool on){ player->set_god_mode(on); } },
			{&list_container::builders,"builders",[](player_ptr_t player,bool on){ player->set_bui_mode(on); } },
			{nullptr,nullptr}
		};
		unsigned i = 0;
		for(auto ptr =mappings; ptr[i].ptr != nullptr; i++){
			if(ptr[i].ptr){
				if(group.compare(ptr[i].name) == 0){
					ptr[i].setter(player,on_or_off);
					if(on_or_off){
						ptr[i].ptr->insert(player->name().c_str());
						return;
					}else{
						auto it = std::find(ptr[i].ptr->begin(),
								ptr[i].ptr->end(),player->name().c_str());
						if(it != ptr[i].ptr->end()){
							ptr[i].ptr->erase(it);
							return;
						}
					}
				}
			}else{ return; }
		}
	}

};
