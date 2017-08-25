#include<iostream>
#include <functional>
class Lambda {
	public:
		Lambda() = default;
		Lambda(std::function<void (const char*)> func){
			m_func = func;
		};
		void operator()(const char* foo){ 
			std::cout << foo << "\n";
			m_func(foo);
		}
		private:
		std::function<void (const char*)> m_func;
};
int main(){
	Lambda foo([&](const char* f){
		std::cout << "Lambda: " << f << "\n";
	});
	foo("farts");
	return 0;
}
