#ifndef __MENTOC_MODS_FLUXKRAFT_GENERATOR_HEADER__
#define __MENTOC_MODS_FLUXKRAFT_GENERATOR_HEADER__
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include <memory>
#include "../object-utils.hpp"
#include "../rand.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::fluxkraft::generator]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

namespace mods::fluxkraft {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	using random_number_type_t = uint64_t;
	static constexpr random_number_type_t MAX_FORGE_ENGINE_VALUE = 4294967295;

	struct generator {
			using exponential_distr_type_t = std::exponential_distribution<double>;
			using distr_type_t = std::uniform_int_distribution<uint32_t>;

			generator();
			~generator();

			float roll_float(float LO, float HI) {
				return LO + static_cast <float>(rand_xoroshiro()) / (static_cast <float>(UINT64_MAX/ (HI-LO)));
			}

			template <typename TUintWidth>
			TUintWidth roll_between(TUintWidth LO, TUintWidth HI) {
				return LO + static_cast <TUintWidth>(rand_xoroshiro()) / (static_cast <TUintWidth>(std::numeric_limits<TUintWidth>::max()/ (HI-LO)));
			}

			template <typename TRandomType>
			TRandomType roll() {
				TRandomType x = rand_xoroshiro();
				if(x == 0) {
					return 0;
				}
				return x % std::numeric_limits<TRandomType>::max();
			}

		private:
			void init();
	};
};

#undef m_debug
#endif
