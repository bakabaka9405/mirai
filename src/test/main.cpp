#include<mirai/util/random.hpp>
using namespace mirai;
int main(){
	vector v{1,2,3,4,5};
	shuffle(v);
	for(auto i:v)cout<<i<<" ";
	return 0;
}