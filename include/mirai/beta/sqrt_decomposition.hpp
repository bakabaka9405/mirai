#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	template <auto& config>
		requires requires {
			config.L[0];
			config.R[0];
			config.F[0];
			config.len;
			config.K;
		}
	inline void sqrt_decomposition_build(ll n, double ratio = 0.4) {
		config.len=pow(n,ratio);
		config.K=(n+config.len-1)/config.len;
		for(ull i=1;i<=config.K;i++){
			config.L[i]=config.R[i-1]+1;
			config.R[i]=config.L[i]+config.len-1;
		}
		config.R[config.K]=n;
		for(ull i=1;i<=config.K;i++){
			for(ull j=config.L[i];j<=config.R[i];j++){
				config.F[j]=i;
			}
		}
	}
} // namespace mirai