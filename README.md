# Mirai

> easy, fast modern C++ library for programming competition

This library provides all kinds of useful tools for programming competitors, including data structures, algorithm functions, debugging tools and so on. And it provides a tool to embed all the headers from this library into the single source file, which competitors can directly submit to the online judge.

## Install

First, you need to install Xmake.

To build the embedder tool, run the following command at the root of the library:

```
xmake -w
```

After that, you can find the embedder tool named `embedder.exe` inside the `build` directory(also depending on the build mode you choose). Then you can add it to the path of environment variables.

## Example

Create a cpp file and add the Mirai library to the include search directories. For simplicity, you can directly write your code in `test/main.cpp`

Put these into the file:

```cpp
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
    vec operator-()const{ return {-val};}
    bool operator==(const vec& rt)const{ return val==rt.val;}
};
int main(){
    cin>>n>>q;
    alias::dsu<vec> D(n+1);
    for(ll i=1,a,b,c;i<=q;i++){
        cin>>a>>b>>c;
        if(b>n||c>n||(a==2&&b==c))ans++;
        else{
            if(a==1){ if(!D.try_merge(b,c,vec{0}))ans++;}
            else if(!D.try_merge(b,c,vec{1}))ans++;
        }
    }
    cout<<ans<<endl;
    return 0;
}
```

And then, run

```
embedder.exe ./test.cpp <PATH_TO_MIRAI_INCLUDE_DIR>
```

For example, run `embedder.exe ./test.cpp G:/Working/proj/mirai/include/`

The embedder will scan your source file, then embed all the header file needed into it. A new file `output.cpp` will be created, containing the parsed code. It will automatically opened by vscode (if installed). You can change the tool if you want.

You can use the code to pass problem P2024 on Luogu, but be careful not to get banned. HAHA.