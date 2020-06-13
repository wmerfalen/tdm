#ifndef __MENTOC_MODS_OBJECT_UTILS_HEADER__
#define __MENTOC_MODS_OBJECT_UTILS_HEADER__
#include "../globals.hpp"
#include <variant>
extern obj_ptr_t optr_by_uuid(uuid_t);
extern obj_ptr_t create_object(int type,std::string yaml_file);
namespace mods::object_utils {

	/** I understand that this may seem like it should be in the yaml.hpp file
	 * instead of here in object_utils, but the only time we're catching
	 * exceptions from the yaml-cpp library is when we're importing
	 * objects. While this may be extracted to a more generic library for
	 * other types of entities in-game, this will suffice.
	 */
	static std::string yaml_initiator = "";
	static uint32_t yaml_transaction_id = 0;
	struct YamlFeedExceptionInfo {
		std::string to_string() {
			std::string m = "[when:";
			m += std::ctime(&this->time);
			m += "]";
			m += "msg:'";
			m += msg;
			m += "'\r\n";
			if(fields_fed_so_far.size()){
				m += "last_field_fed:'";
				m += fields_fed_so_far.back();
				m += "'\r\n";
			}
			m += "initiator:'";
			m += this->initiator;
			m += "'\r\n";
			return m;
		}
		YamlFeedExceptionInfo() = default;
		YamlFeedExceptionInfo(
				YAML::Mark& arg_mark,
				std::string& arg_msg,
				std::vector<std::string>& arg_fed
				) :
			mark(arg_mark), msg(arg_msg), fields_fed_so_far(arg_fed) {
			this->time = std::time(nullptr);
			initiator = yaml_initiator;
			transaction_id = yaml_transaction_id;
		}
		YAML::Mark mark;
		std::string msg;
		std::vector<std::string> fields_fed_so_far;
		time_t time;
		std::string initiator;
		uint32_t transaction_id;
	};

	using yaml_exception_list_t = std::vector<std::variant<YamlFeedExceptionInfo,std::string>>;
	static yaml_exception_list_t yaml_feed_exceptions;

	static inline auto& get_yaml_exception_list() {
		return yaml_feed_exceptions;
	}
	static inline void set_yaml_initiator(
			std::string arg_initiator,
			std::string arg_yaml_type,
			std::string arg_yaml_file){
		yaml_initiator = arg_initiator;
		if(arg_yaml_type.length()){
			yaml_initiator += "|" + arg_yaml_type;
		}
		if(arg_yaml_file.length()){
			yaml_initiator += "|" + arg_yaml_file;
		}
	}
	static inline void yaml_file_doesnt_exist(std::string from_where){
		std::variant<YamlFeedExceptionInfo,std::string> item;
		item.emplace<1>("[yaml-feed::initiator::" + yaml_initiator +
			"::transaction_id::" + std::to_string(yaml_transaction_id) + "]" +
			"[error]: File does not exist, when feeding '" + from_where + "'"
		);
		yaml_feed_exceptions.emplace_back(item);
	}
	static inline void increment_yaml_transaction_id(){
		++yaml_transaction_id;
	}
	static inline void set_yaml_transaction_id(uint32_t i){
		yaml_transaction_id = i;
	}
	static inline uint32_t get_yaml_transaction_id(){
		return yaml_transaction_id;
	}

	template <typename TException>
	static inline void report_yaml_exception(TException& e,std::vector<std::string>& items_fed){
		std::variant<YamlFeedExceptionInfo,std::string> item;
		item.emplace<0>(e.mark,e.msg,items_fed);
		yaml_feed_exceptions.emplace_back(item);
	}
	template <typename TPlayerPointer>
	static inline void send_yaml_exceptions_to(TPlayerPointer& player){
		for(auto & e : yaml_feed_exceptions){
			if(std::holds_alternative<std::string>(e)){
				player->sendln(std::get<std::string>(e));
				continue;
			}
			player->sendln(std::get<YamlFeedExceptionInfo>(e).to_string());
		}
	}

	static inline void clear_yaml_exceptions(){
		yaml_feed_exceptions.clear();
	}
	static inline std::tuple<int32_t,std::string> save_yaml_exceptions_to_disk(){
		std::ofstream outfile("yaml-exceptions.log",std::ios::app);
		if(!outfile.good()){
			return {-1,"Couldn't open yaml-exceptions.log"};
		}
		int32_t ctr = 0;
		for(auto & e : yaml_feed_exceptions){
			if(std::holds_alternative<std::string>(e)){
				outfile << std::get<std::string>(e) << "\n";
				++ctr;
				continue;
			}
			++ctr;
			outfile << std::get<YamlFeedExceptionInfo>(e).to_string() << "\n";
		}
		outfile.close();
		return {ctr,"Success"};
	}


	constexpr static bitvector_t STATUS_INSTALLING = (1 << 0);
	constexpr static bitvector_t STATUS_BREACHING = (1 << 1);
	constexpr static obj_data::location_data_t INSTALL_MASK = 16;
	constexpr static obj_data::location_data_t BREACH_MASK = 32;
	constexpr static uint8_t INSTALL_TICKS_DURATION = 30;
	constexpr static uint8_t BREACH_TICKS_DURATION = 15;
	constexpr static uint8_t THERMITE_BREACH_TICKS_DURATION = 35;
	template <typename T>
	static inline bool is_claymore(T& obj){
		if(obj->has_explosive()){
			switch(obj->explosive()->attributes->type){
				case mw_explosive::CLAYMORE_MINE:
					return true;
				default: break;
			}
		}
		return false;
	}
	template <typename T>
	static inline bool is_camera(T& obj){
		if(obj->has_gadget()){
			switch(obj->gadget()->attributes->type){
				case mw_gadget::CAMERA:
				case mw_gadget::THERMAL_CAMERA:
				case mw_gadget::NIGHT_VISION_CAMERA:
					return true;
				default: break;
			}
		}
		return false;
	}
	template <typename T>
	static inline bool is_night_vision_camera(T& obj){
		return is_camera(obj) && obj->gadget()->attributes->type == mw_gadget::NIGHT_VISION_CAMERA;
	}
	template <typename T>
	static inline bool is_thermal_camera(T& obj){
		return is_camera(obj) && obj->gadget()->attributes->type == mw_gadget::THERMAL_CAMERA;
	}
	template <typename T>
	static inline bool is_night_vision_camera(T obj){
		return is_camera(obj) && obj->gadget()->attributes->type == mw_gadget::NIGHT_VISION_CAMERA;
	}
	template <typename T>
	static inline bool is_thermal_camera(T obj){
		return is_camera(obj) && obj->gadget()->attributes->type == mw_gadget::THERMAL_CAMERA;
	}
	template <>
	inline bool is_night_vision_camera(uuid_t& obj_uuid){
		auto obj = optr_by_uuid(obj_uuid);
		return is_camera(obj) && obj->gadget()->attributes->type == mw_gadget::NIGHT_VISION_CAMERA;
	}
	template <>
	inline bool is_thermal_camera(uuid_t& obj_uuid){
		auto obj = optr_by_uuid(obj_uuid);
		return is_camera(obj) && obj->gadget()->attributes->type == mw_gadget::THERMAL_CAMERA;
	}
	template <>
	inline bool is_night_vision_camera(uuid_t obj_uuid){
		auto obj = optr_by_uuid(obj_uuid);
		return is_camera(obj) && obj->gadget()->attributes->type == mw_gadget::NIGHT_VISION_CAMERA;
	}
	template <>
	inline bool is_thermal_camera(uuid_t obj_uuid){
		auto obj = optr_by_uuid(obj_uuid);
		return is_camera(obj) && obj->gadget()->attributes->type == mw_gadget::THERMAL_CAMERA;
	}

	template <typename T>
	static inline mw_explosive get_explosive_type(T& obj){
		return static_cast<mw_explosive>(obj->explosive()->attributes->type);
	}
	template <>
	inline mw_explosive get_explosive_type<uuid_t>(uuid_t& obj_uuid){
		return static_cast<mw_explosive>(optr_by_uuid(obj_uuid)->explosive()->attributes->type);
	}
	template <typename T>
	static inline bool is_breach_charge(T& obj){
		if(obj->has_explosive()){
			switch(obj->explosive()->attributes->type){
				case mw_explosive::BREACH_CHARGE:
				case mw_explosive::THERMITE_CHARGE:
					return true;
				default: break;
			}
		}
		return false;
	}
	template <typename T>
		bool is_installed(T& obj){
			if(is_claymore(obj) || is_camera(obj)){
				return obj->location_data() & INSTALL_MASK;
			}
			return false;
		}
	template <typename T>
		bool is_installing(T& obj){
			if(is_claymore(obj) || is_camera(obj)){
				return obj->obj_flags.bitvector & STATUS_INSTALLING;
			}
			return false;
		}
	template <typename T,typename P>
		void set_is_breaching(T& obj, P& player,int direction){
			obj->obj_flags.bitvector |= STATUS_BREACHING;
			obj->in_room = player->room();
			obj->set_owner(player->uuid());
			obj->set_location_data(direction + BREACH_MASK);
		}
	template <typename T,typename P>
		void set_is_installing(T& obj, P& player,int direction){
			obj->obj_flags.bitvector |= STATUS_INSTALLING;
			obj->in_room = player->room();
			obj->set_owner(player->uuid());
			obj->set_location_data(direction + INSTALL_MASK);
		}
	template <typename T>
		void set_done_installing(T& obj){
			obj->obj_flags.bitvector ^= STATUS_INSTALLING;
		}
	template <typename T>
		void set_done_breaching(T& obj){
			obj->obj_flags.bitvector ^= STATUS_INSTALLING;
		}
	template <typename T,typename PlayerT>
		bool is_owner(T& obj, PlayerT& player){
			return player->uuid() == obj->get_owner();
		}
	template <typename T>
		bool can_attack_same_room(T& obj){
			return obj->rifle()->attributes->base_stat_list->at(0).allow;
		}

	static inline obj_ptr_t yaml_import(std::string object_type,std::string yaml_file){
#define MENTOC_F_IMPORT(CLASS_TYPE,IT_TYPE)\
		if(object_type.compare(#CLASS_TYPE) == 0){\
			return std::move(create_object(BOOST_PP_CAT(ITEM_,IT_TYPE),yaml_file));\
		}
		MENTOC_F_IMPORT(rifle,RIFLE);
		MENTOC_F_IMPORT(explosive,EXPLOSIVE);
		MENTOC_F_IMPORT(drone,DRONE);
		MENTOC_F_IMPORT(attachment,ATTACHMENT);
		MENTOC_F_IMPORT(gadget,GADGET);
		MENTOC_F_IMPORT(armor,ARMOR);
		MENTOC_F_IMPORT(trap,TRAP);
		MENTOC_F_IMPORT(consumable,CONSUMABLE);
#undef MENTOC_F_IMPORT

		return nullptr;
	}

};//End namespace

#endif