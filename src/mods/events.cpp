#include "events.hpp"
#include "../globals.hpp"

/** When events are called **
 * ------------------------
 *  Events are called after everything that's needed
 *  to be done for the event type. For example, the
 *  OBJECT_FROM_ROOM event is published when the object
 *  is fully removed from the room. It is not called before
 *  or during the object being extracted from the room. 
 *  These events are to be emitted when the action the event
 *  represents is complete.
 */

namespace mods::events {
	static listener_collection_t listeners;
	namespace publish {
		namespace objects {
			void create(uuid_t uuid){
				publish_event(event_t::CREATE_OBJECT,uuid,optr_by_uuid(uuid).get(),1);
			}
			void destroy(uuid_t uuid){
				publish_event(event_t::DESTROY_OBJECT,uuid,nullptr,0);
			}
		};
		namespace players {
			void create(uuid_t uuid){
				publish_event(event_t::CREATE_PLAYER,uuid,ptr_by_uuid(uuid).get(),1);
			}
			void destroy(uuid_t uuid){
				publish_event(event_t::DESTROY_PLAYER,uuid,nullptr,0);
			}
		};
		namespace rooms {
			void object_from_room(uuid_t uuid,room_rnum room){
				publish_event(event_t::OBJECT_FROM_ROOM,uuid,optr_by_uuid(uuid).get(),room);
			}
			void object_to_room(uuid_t uuid,room_rnum room){
				publish_event(event_t::OBJECT_TO_ROOM,uuid,optr_by_uuid(uuid).get(),room);
			}
		};
	};// end publish
	void publish_event(event_t event,const uuid_t& entity_uuid,void* data,int data2){
		auto it = mods::events::listeners.find(event);
		if(it != mods::events::listeners.end()){
			for(auto cbit = it->second.begin();
					cbit != it->second.end(); ++cbit){
				if((*cbit)(entity_uuid,it->first,data,data2) < 0){
					it->second.erase(cbit);
				}
			}
		}
	}
	void register_callback(event_t event,callback_t cb){
		mods::events::listeners[event].emplace_back(cb);
	}
};
