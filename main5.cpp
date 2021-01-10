//changed prime to ensure complete table is outside cache


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<iostream>
#include<vector>
#include<bits/stdc++.h>
using namespace std;

#define S 64
#define W 8
#define B 64
#define Te0 0x7ffff7b1e000
#define Te1 0x7ffff7b1dc00
#define Te2 0x7ffff7b1d800
#define Te3 0x7ffff7b1d400

typedef unsigned long long int ull;

const static unsigned char aes_key[]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
 
unsigned char hexs[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

void print_data(const char *tittle, const void* data, int len);

int tables[8][256];

void setup(){
	for(int i=0;i<8;i++){
		for(int j=0;j<256;j++){
			tables[i][j]=rand();
		}		
	}
}


class Sample{
	public:
	vector<unsigned char> p;
	int y;
	ull m;
	int l;
	Sample(vector<unsigned char> p_,int y_,int m_,int l_){
		for(int i=0;i<p_.size();i++)
			p.push_back(p_[i]);
		y=y_;
		m=m_;
		l=l_;
	}	
};

ull rdtsc(){
	unsigned int lo,hi;
	__asm__ __volatile ("rdtsc" : "=a" (lo), "=d" (hi));
	return ((ull)hi <<32) | lo;
}


vector<unsigned char> generatePlainText(){
	vector<unsigned char> p;
	for(int i=0;i<16;i++){
		p.push_back( (hexs[rand()%16]<<4) | hexs[rand()%16]);		
	}
	return p;
}

vector<unsigned char> AESencrypt(vector<unsigned char> p){
	vector<unsigned char> x,y;
	for(int i=0;i<16;i++){
		x.push_back(p[i]^aes_key[i]);		
	}
	int K[4];
	K[0]=tables[0][x[0]]^tables[1][x[5]]^tables[2][x[10]]^tables[3][x[15]];
	K[1]=tables[0][x[4]]^tables[1][x[9]]^tables[2][x[14]]^tables[3][x[3]];
	K[2]=tables[0][x[8]]^tables[1][x[13]]^tables[2][x[2]]^tables[3][x[7]];
	K[3]=tables[0][x[12]]^tables[1][x[1]]^tables[2][x[6]]^tables[3][x[11]];
	for(int i=0;i<9;i++){
		for(int j=0;j<4;j++){
			p[4*j+0]=K[j]^aes_key[4*j+0];
			p[4*j+1]=K[j]^aes_key[4*j+1];
			p[4*j+2]=K[j]^aes_key[4*j+2];
			p[4*j+3]=K[j]^aes_key[4*j+3];		
		}
	}
	

}
 

int main(){
	ios_base::sync_with_stdio(false);
	setup();
	vector<Sample> samples;
	printf("%lu\n",sizeof(unsigned char*));
	unsigned char* A,*bytes = (unsigned char*) malloc( ((S*B*W)+(S*B))*sizeof(unsigned char) );
	int i=0;
	cout<<"add"<<(long long)A<<" "<<(long long)bytes<<endl;
//	return 0;
	for(i=0;i<S*B;i++){
		if(((long long)(&bytes[i]))%(S*B)==(long long)(&tables[0][0])%(S*B)){
			A=&bytes[i];
			break;	
		}	
	}

	long long Mk[16][16];
	for(int u=0;u<16;u++){
		for(int v=0;v<16;v++){
			Mk[u][v]=0;
//			printf("%d ",Mk[u][v]);
		}	
//		printf("\n");
	}
	
	

	int cycles=1800;
	for(int c=0;c<cycles;c++){
	//prime
		vector<unsigned char> p1=generatePlainText();
		

		unsigned char p[16];
		for(i=0;i<16;i++){
			p[i]=p1[i];		
		}
		print_data("\n Plaintext:",p, 16);
		cout<<endl;
		
		int temp=1; 
		for(i=0;i<S*W*B;i+=B){
			temp=temp*A[i];	
		}
		for(i=0;i<S*W*B;i+=B){
			temp=temp+A[i];	
		}
		for(i=0;i<S*W*B;i+=B){
			temp=temp&A[i];	
		}
	////////////

	//encryption
/*	
//	 Init vector 
		unsigned char iv[AES_BLOCK_SIZE];
		memset(iv, 0x00, AES_BLOCK_SIZE);
		
// Buffers for Encryption and Decryption 
		unsigned char enc_out[((sizeof(p) + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE];
		unsigned char dec_out[sizeof(p)];
	
// AES-128 bit CBC Encryption 
		AES_KEY enc_key, dec_key;
		AES_set_encrypt_key(aes_key, sizeof(aes_key)*8, &enc_key);
		AES_cbc_encrypt(p, enc_out, sizeof(p), &enc_key, iv, AES_ENCRYPT);
		// AES-128 bit CBC Decryption //
		memset(iv, 0x00, AES_BLOCK_SIZE); // don't forget to set iv vector again, else you can't decrypt data properly
		AES_set_decrypt_key(aes_key, sizeof(aes_key)*8, &dec_key); // Size of key is in bits
		AES_cbc_encrypt(enc_out, dec_out, sizeof(p), &dec_key, iv, AES_DECRYPT);	

		// Printing and Verifying //
//		print_data("\n Original ",p, sizeof(p)); // you can not print data as a string, because after Encryption its not ASCII
	
//		print_data("\n Encrypted",enc_out, sizeof(enc_out));
		
//		print_data("\n Decrypted",dec_out, sizeof(dec_out));
	
	//////
*/

	printf("%d\n",temp);
	AESencrypt(p1);

	///probe

		clock_t start, end, cpu_time_used;
		time_t start1,end1,cpu_time_used1;
		ull start2,end2,cpu_time_used2;

		
		for(int l=0;l<4;l++){
			for(int y=0;y<256;y+=16){
				start = clock();	//this works
				//time(&start1);	//this doesn't
				start2=rdtsc();		//this too works

				for(int t=0;t<W;t++){
					temp*=(A[(1024*l+4*y+t*S*B)]);
				}
				end = clock();
				//time(&end1);
				end2=rdtsc();

				cpu_time_used = ((end - start));
				//cpu_time_used1 = ((end1 - start1));
				cpu_time_used2 = ((end2 - start2));
				printf("%llu, %llu \n", cpu_time_used,cpu_time_used2);
				//Mk(p,l,y)=cpu_time_used
			//	cout<<cpu_time_used<<" "<<cpu_time_used2<<endl;
				samples.push_back(Sample(p1,y,cpu_time_used2,l));
				//cout<<start1<<" "<<start<<endl;
				for(int i=0;i<16;i++){
					if(i%4==l)
						Mk[i][(p[i]^y)>>4] += cpu_time_used;			
				}
			}
		}
		for(int i=0;i<16;i++){
			p[i]=rand();	
		}
	}
	cout<<"samples:"<<samples.size()<<endl;
	ull total[16][256];
	int count[16][256];
	for(int i=0;i<16;i++){
		for(int k=0;k<256;k++){
			total[i][k]=0;
			count[i][k]=0;
		}
		//printf("\n");
	}
	for(int j=0;j<samples.size()-1;j++){
		if(samples[j+1].m>samples[j].m*10){
			for(int i=0;i<16;i++){
				cout<<((samples[j+1].p[i]^samples[j+1].y)>>4);
				cout<<" ";			
			}		
			cout<<endl;
		}
		for(int i=0;i<16;i++){
		//	if(i%4==(3-samples[j].l)){
				total[i][samples[j].p[i]^samples[j].y]+=samples[j].m;
				count[i][samples[j].p[i]^samples[j].y]++;			
		//	}		
		}	
	}
	/*for(int i=0;i<16;i++){
		for(int k=0;k<16;k++){
			printf("%lld ",Mk[i][k]);
		}
		printf("\n");
	}*/
	for(int i=0;i<16;i++){
		for(int k=0;k<16;k++){
			int ans=0;
			for(int u=0;u<16;u++){
				if(count[i][k*16+u]!=0)
					ans+=total[i][k*16+u]/count[i][k*16+u];
		//	else
		//		cout<<0<<" ";
			}
			cout<<ans<<" ";
		}
		cout<<"\n-----------------------------------------\n";
	}

	
}

void print_data(const char *tittle, const void* data, int len)
{
	printf("%s : ",tittle);
	const unsigned char * p = (const unsigned char*)data;
	int i = 0;
	
	for (; i<len; ++i)
		printf("%02X ", *p++);
	
	printf("\n");
}
















