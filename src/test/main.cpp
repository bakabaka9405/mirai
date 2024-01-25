#include<mirai/util/pipeline.hpp>
using namespace mirai;
int main(){
	for(auto [i,j]:enumerate(vector{1,2,3,4,5})){
		cout<<i<<" "<<j<<endl;
	}
	return 0;
}