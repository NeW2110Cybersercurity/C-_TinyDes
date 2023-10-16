#include<stdio.h>
#include<conio.h>
#include<iostream>
#include <cmath>
#include <string>
#include <stdlib.h>
#include <sstream>


using namespace std;
// He ma Feistel gom 3 vong
string round_keys[3];
// Ban ro (plain text)
string pt;

// HAm cong XOR
string Xor(string a, string b){ 
	string result = ""; 
	int size = b.size();
	for(int i = 0; i < size; i++){ 
		if(a[i] != b[i]){ 
			result += "1"; 
		}
		else{ 
			result += "0"; 
		} 
	} 
	return result; 
} 
// HAm chuyen thap phan sang nhi phan
string convertDecimalToBinary(int decimal)
{
	string binary;
    while(decimal != 0) {
		binary = (decimal % 2 == 0 ? "0" : "1") + binary; 
		decimal = decimal/2;
	}
	while(binary.length() < 4){
		binary = "0" + binary;
	}
    return binary;
}

// Ham chuyen ASCII
string convertASCII(int decimal)
{
	string binary;
    while(decimal != 0) {
		binary = (decimal % 2 == 0 ? "0" : "1") + binary; 
		decimal = decimal/2;
	}
	while(binary.length() < 8){
		binary = "0" + binary;
	}
    return binary;
}
// Ham chuyen nhi phan sang thap phan

int convertBinaryToDecimal(string binary)
{
    int decimal = 0;
	int counter = 0;
	int size = binary.length();
	for(int i = size-1; i >= 0; i--)
	{
    	if(binary[i] == '1'){
        	decimal += pow(2, counter);
    	}
    counter++;
	}
	return decimal;
}
// Ham quay trai 1 lan
string shift_left_once(string key_chunk){ 
    string shifted="";  
        for(int i = 1; i < 4; i++){ 
            shifted += key_chunk[i]; 
        } 
        shifted += key_chunk[0];   
    return shifted; 
} 
// Ham quay trai 2 lan
string shift_left_twice(string key_chunk){ 
    string shifted=""; 
    for(int i = 0; i < 2; i++){ 
        for(int j = 1; j < 4; j++){ 
            shifted += key_chunk[j]; 
        } 
        shifted += key_chunk[0]; 
        key_chunk= shifted; 
        shifted =""; 
    } 
    return key_chunk; 
}
void generate_keys(string key){
	// kt  
	//int pc1[8] = {
	//1,2,3,4,5,6,7,8
	//};
	// doi khóa 8 bit thanh 6 bit
	int pc2[8] = { 
	6,2,4,3,8,1
	}; 
	// 1. kt
	string perm_key =key; 
	//for(int i = 0; i < 8; i++){ 
	//	perm_key+= key[pc1[i]-1]; 
	//} 
	// 2. chia khóa thành 2 nua
	string left= perm_key.substr(0, 4); 
	string right= perm_key.substr(4, 4); 
	// 
	for(int i=0; i<3; i++){ 
		// 3.1. vòng 1,3 thì quay trái 1 lan
		if(i == 0 || i==2 ){
			left= shift_left_once(left); 
			right= shift_left_once(right);
		} 
		// 3.2. vòng 2 quay trái 2 lan
		else{
			left= shift_left_twice(left); 
			right= shift_left_twice(right);
		}
	// 4. cong 2 nua khoa lai
	string combined_key = left + right;
	string round_key = ""; 
	// 5. doi khoa 8 bit thanh 6 bit
	for(int i = 0; i < 8; i++){ 
		round_key += combined_key[pc2[i]-1]; 
	}   
	round_keys[i] = round_key;
		 
	} 

}

// Trien khai thuat toan
string TinyDES(){
	int initial_permutation[8] = { 
	1,2,3,4,5,6,7,8}; 
	// Expand
	int expansion_table[6] = { 
	3,4,2,3,2,1
	}; 
	// The substitution boxes. The should contain values
	// from 0 to 15 in any order.
	int substition_boxes[4][16]=  
	{
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7, 
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8, 
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0, 
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 
    }; 
    // P_Box
	int permutation_tab[4] = { 
	3,1,4,2
	};  
	string perm;
	for(int i = 0; i < 8; i++){ 
		perm += pt[initial_permutation[i]-1]; 
	}  
	// 1. Chia thanh hai nua L va R
	string left = perm.substr(0, 4); 
	string right = perm.substr(4, 4);
	// Thuc hien ma hoa 3 lan  
	for(int i=0; i<3; i++) { 
	//int k =i;
    	string right_expanded = ""; 
		// 2.1. thuc hien Expand
    	for(int i = 0; i < 6; i++) { 
      		right_expanded += right[expansion_table[i]-1]; 
    };  

    
	// 2.2. Xor voi key    
    string xored = Xor(round_keys[i], right_expanded);  
	string res = "";
	//Xet S_box
	
		string row1= xored.substr(0,1) + xored.substr(5,1);
      	int row = convertBinaryToDecimal(row1);
      	string col1 = xored.substr(1,1) + xored.substr(2,1) + xored.substr(3,1) + xored.substr(4,1);
		int col = convertBinaryToDecimal(col1);
		int val = substition_boxes[row][col];
		res += convertDecimalToBinary(val);  
		// 2.4. Tiep tuc hoan vi
		string perm2 =""; 
		for(int i = 0; i < 4; i++){ 
			perm2 += res[permutation_tab[i]-1]; 
		}
		
		xored = Xor(perm2, left);
		
		// 2.5. Hoan doi righ and right 
		left = xored; 
		cout<<endl<<"Vong "<<i+1<<endl;
		cout<<"Key "<<i+1<<": "<<round_keys[i]<<endl;
		cout<<"Left: "<<right<<endl;
		cout<<"Right: "<<xored<<endl;
		
		// 
			string temp = right;
			right = xored;
			left = temp;
		// 
	}  
	string combined_text = left + right;   
	return combined_text; 
}
	string Hex ="";
	string Mahoa ="";
	int ktdd;
	string pt2;
	//string cong ="";
	string Chuoi_ma_hoa = "";
	int ktdd2;
int main(){  
	
	string key;
	//string plaintext;
	cout<<"Nhap Key: ";
	cin>>key;
	generate_keys(key);
	string String;
	cout<<"Nhap Plain text: ";
	fflush(stdin);
	getline(cin,String);
	// Chuyen plain text sang bit
	char* c = new char[String.size()+1];
	copy(String.begin(),String.end(),c);
	c[String.size()] = '\0';

	for(int i =0;i< String.size();i++){
	int n = int (c[i]);
	Hex += convertASCII(n);
}
	cout<<"Ma ASCII cua plain text : "<<Hex<<endl;
	ktdd = Hex.length();
	for(int i = 0;i<ktdd/8;i++){
		pt = Hex.substr(i*8,8); 
		int dem = convertBinaryToDecimal(pt);
		char getASCIICharValue = (char)dem;
		//cong += getASCIICharValue;
		cout<<"-----------------------------------------"<<endl;
		cout<<"Ky tu ' "<<getASCIICharValue<<" ' "<<endl;
    	string ct = TinyDES();
		Mahoa += ct; 
		cout<<"Duoc ma hoa thanh: "<<ct<<endl;
		
}
	cout<<endl<<"-----------------------------------------"<<endl;
    cout<<"Plain text duoc ma hoa thanh dang bit: "<<Mahoa<<endl;
    ktdd2 = Mahoa.length();
    string getCharValue="";
    // Chuyen ve ma thap luc phan
    for(int i =0;i<ktdd2/4;i++){
    	pt2 = Mahoa.substr(i*4,4);
		int dem = convertBinaryToDecimal(pt2);
		do{
			if(dem==10){
				getCharValue = "A";
			}
			if(dem==11){
				getCharValue = "B";
			}
			if(dem==12){
				getCharValue ="C";
			}
			if(dem==13){
				getCharValue ="D";
			}
			if(dem==14){
				getCharValue ="E";
			}
			if(dem==15){
				getCharValue ="F";
			}
		}while(dem>15);
		if(getCharValue==""){
			ostringstream convert;   
    		convert << dem; 
			Chuoi_ma_hoa += convert.str();
		}
		else{
			Chuoi_ma_hoa += getCharValue;
		}
		getCharValue="";
		//char getASCIICharValue2 = (char)dem;		
    }
    cout<<endl<<"-----------------------------------------"<<endl;
    cout<<"Plain text duoc ma hoa thanh dang text: "<<Chuoi_ma_hoa<<endl;
} 
