#include <iostream>
#include "BigInt.h"
#include <cmath>
#include <random>
#include <ctime>
#include <string>
using namespace std;


BigInt Modulo(BigInt A, BigInt B){
	if(B<BigInt(0))B=B*BigInt(-1);
	while(A<BigInt(0)){
		A=A+B;
	}
	return A%B;
}

BigInt Modulo_addition(BigInt A1,BigInt A2, BigInt B){
	return Modulo(Modulo(A1,B)+Modulo(A2,B),B);
}

BigInt Modulo_Multiplication(BigInt A1,BigInt A2, BigInt B){
	return Modulo(Modulo(A1,B)*Modulo(A2,B),B);
}


BigInt Modulo_Exponentiation(BigInt A1,BigInt A2, BigInt B){
	//cout<<endl;
	if(A2==BigInt(0)) return BigInt(1);
	else if(A2%BigInt(2)==BigInt(0)){
		BigInt AA = Modulo_Exponentiation(A1,A2/BigInt(2),B);

		AA=AA*AA;
		cout<<0<<" "<<AA<<endl; 
		return Modulo(AA, B);
	}
	else{

		BigInt AA = Modulo_Exponentiation(A1,A2/BigInt(2),B);
		
		AA=AA*AA;
		cout<<1<<" "<<AA<<endl; 
		return Modulo_Multiplication(AA,A1,B);
	}
}

BigInt GCD(BigInt X,BigInt Y){
	if(Y==BigInt(0))return X;
	else if(X==BigInt(0)) return Y;
	else return GCD(Y,Modulo(X,Y));
}

pair < BigInt , BigInt > Inverse_Modulo_helper(BigInt X,BigInt n){
	if(X==BigInt(0) || n==BigInt(0)) return pair< BigInt, BigInt>(BigInt(1),BigInt(1));
	else {
	//	cout<<"X"<<X<<" n"<<n<<endl;
		pair<BigInt,BigInt>A=Inverse_Modulo_helper(n,Modulo(X,n));
	//	cout<<jo++<<"First  : "<<A.second<<"   "<<A.first-A.second*(X/n)<<endl;
	//	cout<<"  "<<A.second<<"  "<<A.first<<" X : "<<X<<" n: "<<n<<" "<<(X/n)<<" Returns "<<A.second<<"  "<<A.first+(-(X/n)*A.second)<<" "<<" "<<endl;
	//	cout<<X*A.second + n*(A.first +(-(X/n)*A.second))<<endl; 
		return pair<BigInt,BigInt>(A.second,A.first +(-(X/n)*A.second));
	}
}

BigInt Inverse_Modulo(BigInt X,BigInt n){
	if(GCD(X,n)>BigInt(1))return BigInt(-1);
	else return Modulo( Inverse_Modulo_helper(X,n).first,n);
}


BigInt Discrete_logarithm(BigInt A,BigInt B, long long int n){ // A^x mod n = B mod n
	int m=sqrt(n);//////////////***************    sqrt
	BigInt array[m];
	//BigInt array(m)
	array[0]=BigInt(1);
	array[1]=Modulo(A,BigInt(n));
	for(int i=2;i<m;i++)
		array[i]=Modulo(array[0]*array[i-1],BigInt(n));

	BigInt A_m=Modulo_Exponentiation( Inverse_Modulo(A,BigInt(n)),BigInt(m),BigInt(n));
	BigInt y=B;
	BigInt j(0);
	while(true){
		for(int i=0;i<m;i++){
			if(y==array[i])return BigInt(i)+j;
		}
		j++;
		y=Modulo_Multiplication(y,B,BigInt(n));
	}
}


// bool isPrime(long long n)
// {
//     if (n <= (1))  return false;
//     if (n <= (3))  return true;

//     if (n%(2) == (0) || n%(3) == (0)) return false;

//     for ( int i=(5); i*i<=n; i=i+(6))
//         if (n%i == (0) || n%(i+(2)) == (0))
//            return false;

//     return true;
// }

bool prime_helper(int d, int n)
{
    int a = 2 + rand() % (n - 4);
 
    int x = power(a, d, n);
 
    if (x == 1  || x == n-1)
       return true;
    while (d != n-1)
    {
        x = (x * x) % n;
        d *= 2;
 
        if (x == 1)      return false;
        if (x == n-1)    return true;
    }
   return false;
}
 
bool isPrime(int n, int k)
{
    if (n <= 1 || n == 4)  return false;
    if (n <= 3) return true;
 
    int d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    for (int i = 0; i < k; i++)
         if (prime_helper(d, n) == false)
              return false;
 
    return true;
}



unsigned long long int prime_generator(long long int min ,long long int max){

	srand(time(0)); 

	long long int seed = rand();
	std::mt19937 rng(seed);
	std::uniform_int_distribution<unsigned long long > gen(min, max); 
	long long int r = gen(rng);
	while (!isPrime(r,8)){
	r = gen(rng);	
}

	return r;
}

int length(BigInt b){
	int d=1;
	while(b/BigInt(10)>BigInt( 0)){
		d++;
		b=b/BigInt(10);
	}
	return d;
}

BigInt RSA_encryption_key(BigInt d){
	string s="";
	srand(time(0));

	int seed = rand();
	std::mt19937 rng1(seed);

	std::uniform_int_distribution<int> gen1(1, int(length(d)-1));
	
	int r=gen1(rng1);
	int z=r;
	while(true){
	while(r-- >0){
		
		int seed1 = rand();
		std::mt19937 rng(seed1);
		std::uniform_int_distribution<int> gen(0, 9);
		
		int num=gen(rng);
		s+=to_string(num);
	}

	if(GCD(d,BigInt(s))==BigInt(1)) return BigInt(s);
	else {r=gen1(rng1); s="";continue;}
	
	}

	
}


BigInt RSA_decrytion_key(BigInt T,BigInt e){//totent Function//encription key
	return Inverse_Modulo(e,T);
}

BigInt RSA_encryption(BigInt m,BigInt e,BigInt n){//message//encrption key// Public Key
	return Modulo_Exponentiation(m,e,n);
}
BigInt RSA_decrytion(BigInt em,BigInt d,BigInt n){
	return Modulo_Exponentiation(em,d,n);
}

int main(){
	// unsigned long long x=pow(2,4);
	// unsigned long long y=pow(2,8);
	// unsigned long long int A= prime_generator(x,y);
	// unsigned long long int B= prime_generator(x,y);

	// BigInt n(A*B);
	
	BigInt A(197);
	BigInt B(197);

	BigInt n(197*197);

	BigInt a(14785);
	BigInt E(6541);
	BigInt c(120);
	cout<<Modulo_Exponentiation(a,E,n)<<endl;

	// cout<<A<<' '<<B<<endl;
	// BigInt AA=BigInt(A);
	// BigInt BB=BigInt(B);
	// BigInt d=(AA-BigInt(1))*(BB-BigInt(1));
	// cout<<"n " <<d<<endl;
	// BigInt e= RSA_encryption_key(d);
	// cout<<"encription_key "<<e<<endl;
	// BigInt D=RSA_decrytion_key(d,e);
	// cout<<"Decription key "<<D<<'\n';
	// BigInt message=BigInt(14785);
	// cout<<"Message "<<message<<endl;
	// BigInt encrip= RSA_encryption(message,e,n);
	// cout<<"Encripted Message "<<encrip<<endl;
	// BigInt decrip =RSA_decrytion(encrip,D,n);
	// cout<<"Dicripted Message  " << decrip<<endl;
}