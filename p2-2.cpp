#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include<bits/stdc++.h>

using namespace std;
using std::max;
default_random_engine generator(10);
// generator.seed(10);


#define WMAT 2
#define WMIS -1
#define WGAP -1
#define P_MAX 100
#define RUN 100


struct item {
    int item_score;
    vector<string> seq;
    vector<string> seq_b;
};


int m,n;

int w(string a, string b);
item busca_local(vector<string> a, vector<string> b, int n, int m);



int main() {
    int n=0;
    vector<string> a;
    vector<string> b;
    

 // Lendo tamanho das sequências   
    cin >> n >> m;


// Lendo sequências iniciais
    string base;
    cin >> base;
    for(int i = 0; i < n; i++){
        a.push_back({base[i]});
    }
    cin >> base;
    for(int i = 0; i < m; i++){
        b.push_back({base[i]});
    }

// Imprimido sequências iniciais
    cout << endl << "----- Sequências iniciais -----" << endl;
    cout << "a - tamanho: " << n << endl;
    for(auto& el: a){
        cout << el;
    }
    cout << endl;
    cout << "b - tamanho: " << m << endl;
    for(auto& el: b){
        cout << el;
    }
    cout << endl;


    vector<int> scores;
    vector<string> melhor_seq, melhor_seq_b;
    int max_score_global=0;
    item melhor_item;
// Obtenção do score e das sequências alinhadas
    for (int i=0; i<RUN; i++){
        melhor_item = busca_local(a,b,n,m);
        scores.push_back(melhor_item.item_score);
        if (melhor_item.item_score > max_score_global){ 
            max_score_global = melhor_item.item_score;
            melhor_seq = melhor_item.seq;
            melhor_seq_b = melhor_item.seq_b;

        }
    }

// Score obtido
    cout << endl << endl << "----- Score -----" << endl;
    cout << "max_score: "<< max_score_global << endl << endl;

    cout << " ----- Melhor subsequência de A -----" << endl;
    for (auto& el : melhor_seq){
        cout << el << " ";
    }
    cout << endl;

    cout << " ----- Subsequência B correspondente -----" << endl;
    for (auto& el : melhor_seq_b){
        cout << el << " ";
    }
    cout << endl;

     return 0;
}


int w(string a, string b){
    if (a == b)
        return WMAT;  //match
    else if (a != b)
        return WMIS;
   else
        return WGAP;  // gap

}


item busca_local(vector<string> a, vector<string> b, int n, int m){
    
    vector<string> sa,sb;
    vector<vector<string>> subs_sa;

    uniform_int_distribution<int> distribution_k(1,m);
    uniform_int_distribution<int> distribution_p(1,P_MAX);
    
    int k,p;
    int j = 0;
    int i=0;
    
    k = distribution_k(generator);
    p = distribution_p(generator);

    uniform_int_distribution<int> distribution_j(0,m-k);
    uniform_int_distribution<int> distribution_i(0,n-k);

    
    j = distribution_j(generator);


    sa.reserve(k);
    sb.reserve(k);
    
    for (int ki=0; ki<k; ki++){
        sb.push_back(b[j+ki]);
    }
    
    for (int pi=0; pi<p; pi++){  
        vector<string> sa;
        i = distribution_i(generator);
        for (int ki=0; ki<k; ki++){
            sa.push_back(a[i+ki]);
        }
        subs_sa.push_back(sa);
    }
    

    cout  << "_______________________" << endl;
    cout << "k: " << k << " | " << "p: " << p << endl;
    // cout <<  "----- sb -----" << endl;
    // for(auto& el: sb) cout << el << " ";
    // cout << endl << endl;
    // cout <<  "----- subsequências sa -----" << endl;
    // for(auto& linha: subs_sa){
    //     for(auto& el: linha){
    //         cout << el << " ";
    //     } 
    //     cout << endl;
    // } 

    int max_score = 0; 
    vector<string> melhor_subseq;
    
    for(auto& el: subs_sa){
        int score=0;
        for (int i=0; i<k;i++){
            score += w(el[i],sb[i]); 
        } 
        if (score > max_score){
            max_score = score;  
            melhor_subseq = el; 
        }  
    } 


    item melhor;
    melhor.item_score = max_score;
    melhor.seq = melhor_subseq;
    melhor.seq_b = sb;

    return melhor;
}





// Para compilar: 

// g++ -Wall -O3 p2-2.cpp -o p2-2
// ./p2-p2 < dna.seq