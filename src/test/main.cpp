#include<mirai/ds/disjoint_set_union.hpp>
using namespace mirai;
ll n,q,ans;
struct vec{
	ll val=0;
	vec operator+(const vec& rt)const{
		ll v=val+rt.val;
		if(v==1) return {1};
		else if(v==-1)return {-1};
		else if(v==2)return {-1};
		else if(v==-2)return {1};
		else return {0};
	}
	vec operator-()const{
		return {-val};
	}
	bool operator==(const vec& rt)const{
		return val==rt.val;
	}
};
int main(){
	cin>>n>>q;
	alias::dsu<vec> D(n+1);
	for(ll i=1,a,b,c;i<=q;i++){
		cin>>a>>b>>c;
		if(b>n||c>n||(a==2&&b==c))ans++;
		else{
			if(a==1){
				if(!D.try_merge(b,c,vec{0}))ans++;
			}
			else{
				if(!D.try_merge(b,c,vec{1}))ans++;
			}
		}
	}
	cout<<ans<<endl;
	return 0;
}