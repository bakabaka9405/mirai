#include<mirai/ds/disjoint_set_union.hpp>
using namespace mirai;
ll T;
int main(){
	disjoint_set_union<ll,true,false> D(30001);
	cin>>T;
	while(T--){
		char c;
		ll x,y;
		cin>>c>>x>>y;
		if(c=='M'){
			x=D.find(x);
			y=D.find(y);
			D.merge(x,y,D.get_rank(y));
		}
		else{
			if(D.find(x)!=D.find(y))cout<<-1<<endl;
			else {
				if(x==y)cout<<0<<endl;
				else cout<<abs(D.query(x,y))-1<<endl;
			}
		}
	}
	return 0;
}