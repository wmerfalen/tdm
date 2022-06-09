

struct ___[[[TABLE]]]___ : public mods::orm::orm_base<___[[[TABLE]]]___,std::string> {
	MENTOC_ORM_CLASS(___[[[CAPS_TABLE]]]____ORM_MEMBERS,"___[[[TABLE]]]___");
	auto vnum() {
		return id;
	}

	___[[[TABLE]]]___() {
		this->init();
		loaded = 0;
		id = 0;
	}
	~___[[[TABLE]]]___() = default;

	MENTOC_ORM_PRIMARY_KEY_FUNCTIONS();
	MENTOC_ORM_FEED_MULTI(___[[[CAPS_TABLE]]]____ORM_MEMBERS,___[[[TABLE]]]___);

	std::deque<___[[[TABLE]]]___> rows;
	auto destroy() {
		return  remove(this);
	}
#ifdef ___[[[CAPS_TABLE]]]____INITIALIZE_ROW_MEMBERS
	MENTOC_ORM_INITIALIZE_ROW_USING(___[[[CAPS_TABLE]]]____INITIALIZE_ROW_MEMBERS);
#endif
};
struct ___[[[TABLE]]]____status_t {
	std::tuple<int16_t,std::string> orm_status;
	std::unique_ptr<___[[[TABLE]]]___> record;
	___[[[TABLE]]]____status_t() = default;
	___[[[TABLE]]]____status_t(___[[[TABLE]]]____status_t&& other) : orm_status(other.orm_status),
		record(std::move(other.record)) {}
};

using ___[[[TABLE]]]____list_t = std::deque<std::shared_ptr<___[[[TABLE]]]___>>;

___[[[TABLE]]]____list_t& ___[[[TABLE]]]____list();

___[[[TABLE]]]____list_t load_all____[[[TABLE]]]____list();
};

#endif
