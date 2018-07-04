#include "mods/behaviour_tree_node.hpp"
#include <array>

typedef struct{ \
	bool has_food;\
	bool has_fork;\
	bool hungry;\
	bool deer_available;\
	bool rabbit_available;\
	bool horse_available;\
} ch_data;
using node_type_t = mods::behaviour_tree_node<ch_data*>::node_type_t;
using callback_t = mods::behaviour_tree_node<ch_data*>::callback_t;
using node = mods::behaviour_tree_node<ch_data*>;
using status_t = mods::behaviour_tree_status;
status_t check_if_deer_available(ch_data* player){
	std::cout << "[lambda] checking if deer available\n";
	return (player->deer_available ? status_t::SUCCESS : 
			status_t::FAILURE);
}
status_t check_if_rabbit_available(ch_data* player){
	std::cout << "[lambda] checking if rabbit available\n";
	return (player->rabbit_available ? status_t::SUCCESS : 
			status_t::FAILURE);
}
status_t check_if_horse_available(ch_data* player){
	std::cout << "[lambda] checking if horse available\n";
	return (player->horse_available ? status_t::SUCCESS : 
			status_t::FAILURE);
}
status_t check_if_hungry(ch_data* player){
	std::cout << "[lambda] checking if hungry\n";
	return (player->hungry ? status_t::SUCCESS : 
			status_t::FAILURE);
}
status_t check_if_has_food(ch_data* player){
	std::cout << "[lambda] checking if has food\n";
	return (player->has_food ? status_t::SUCCESS : 
			status_t::FAILURE);
}
status_t check_if_has_fork(ch_data* player){
	std::cout << "[lambda] checking if has fork\n";
	return (player->has_fork ? status_t::SUCCESS : 
			status_t::FAILURE);
}
status_t eat_food(ch_data* player){
	std::cout << "[lambda] eating food\n";
	return status_t::SUCCESS;
}
int usage(){
	std::cerr << "usage: ./a.out <has_food> <has_fork> <hungry> <deer_available> <rabbit_available> <horse_available>\n" 
		<< "Use '1' or '0'.\n";
	return 0;
}
int main(int argc,char** argv){
	/* build a sequence for feeding */
	ch_data player;
	if(argc < 7){
		return usage();
	}
	player.has_food = !!!std::string(argv[1]).compare("1");
	player.has_fork = !!!std::string(argv[2]).compare("1");
	player.hungry = !!!std::string(argv[3]).compare("1");
	player.deer_available = !!!std::string(argv[4]).compare("1");
	player.rabbit_available = !!!std::string(argv[5]).compare("1");
	player.horse_available = !!!std::string(argv[6]).compare("1");

	if(player.has_food){
		std::cout << "Player has food\n";
	}else{
		std::cout << "Player doesn't have food\n";
	}
	if(player.has_fork){
		std::cout << "Player has fork\n";
	}else{
		std::cout << "Player doesn't have fork\n";
	}
	if(player.hungry){
		std::cout << "Player is hungry\n";
	}else{
		std::cout << "Player isn't hungry\n";
	}
	if(player.deer_available){
		std::cout << "Deer is available\n";
	}else{
		std::cout << "Deer isn't available\n";
	}
	if(player.rabbit_available){
		std::cout << "Rabbit is available\n";
	}else{
		std::cout << "Rabbit isn't available\n";
	}
	if(player.horse_available){
		std::cout << "Horse is available\n";
	}else{
		std::cout << "Horse isn't available\n";
	}
	std::array<callback_t,4> feeder_nodes;
	feeder_nodes[0] = check_if_hungry;
	feeder_nodes[1] = check_if_has_food;
	feeder_nodes[2] = check_if_has_fork;
	feeder_nodes[3] = eat_food;
	auto feeder = node::create_sequence({
		node::create_leaf(check_if_hungry,&player),
		node::create_leaf(check_if_has_food,&player),
		node::create_leaf(check_if_has_fork,&player),
		node::create_leaf(eat_food,&player)
	});
	std::cout << "[run]: feeder sequence\n";
	auto status = feeder->run();
	switch(static_cast<int>(status.status)){
		case status_t::SUCCESS:
			std::cout << "[success]\n";
			break;
		case status_t::FAILURE:
			std::cout << "[failure] -> id: " << status.id << "\n";
			break;
	}
	auto hunter_selector = node::create_selector({
			node::create_leaf(check_if_deer_available,&player),
			node::create_leaf(check_if_rabbit_available,&player),
			node::create_leaf(check_if_horse_available,&player)
	});
	std::cout << "[run]: hunter_selector\n";
	status = hunter_selector->run();
	switch(static_cast<int>(status.status)){
		case status_t::SUCCESS:
			std::cout << "[success]\n";
			break;
		case status_t::FAILURE:
			std::cout << "[failure] -> id: " << status.id << "\n";
			break;
	}
	/* create our hunter-feeder behaviour tree */
	auto hunter_feeder_sequence = node::create_sequence({
			hunter_selector,
			feeder
	});
	std::cout << "[run]: hunter_feeder_sequence\n";
	status = hunter_feeder_sequence->run();
	switch(static_cast<int>(status.status)){
		case status_t::SUCCESS:
			std::cout << "[success]\n";
			break;
		case status_t::FAILURE:
			std::cout << "[failure] -> id: " << status.id << "\n";
			break;
	}
	return 0;
}
