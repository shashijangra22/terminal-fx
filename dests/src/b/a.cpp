#include<bits/stdc++.h>
using namespace std;
#define ll long long
int main(){
    ll s,temp,a=0,b=0,q;
    cin>>s;
    map<ll,ll> m;
    for(ll i=1; i<=s; i++){
        cin>>temp;
        m[temp]=i;
    }
    cin>>q;
    while(q--){
        cin>>temp;
        a+=m[temp];
        b+=(s-m[temp]+1);
    }
    cout<<a<<" "<<b;
}