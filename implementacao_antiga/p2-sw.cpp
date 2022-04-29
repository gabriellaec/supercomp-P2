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
    vector<string> rec_a;
    vector<string> rec_b;
};

struct item_sw {
    int valor;
    int salto_i;
    int salto_j;
};


int m,n;
int w(string a, string b);
item busca_local(vector<string> a, vector<string> b, int n, int m);
int smith_waterman(vector<string> a, vector<string> b, int n, int m, vector<string> &match_seq_a, vector<string> &match_seq_b);
item_sw find_max(int diagonal, int delecao, int insercao);



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
    // cout << endl << "----- Sequências iniciais -----" << endl;
    // cout << "a - tamanho: " << n << endl;
    // for(auto& el: a){
    //     cout << el;
    // }
    // cout << endl;
    // cout << "b - tamanho: " << m << endl;
    // for(auto& el: b){
    //     cout << el;
    // }
    // cout << endl;

    vector<int> scores;
    vector<string> melhor_seq, melhor_seq_b, melhor_alinhamento_a, melhor_alinhamento_b;
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
            melhor_alinhamento_a=melhor_item.rec_a;
            melhor_alinhamento_b=melhor_item.rec_b;

        }
    }

// Score obtido
    // cout << endl << endl << "----- Score -----" << endl;
    // cout << "max_score: "<< max_score_global << endl << endl;

    // cout << " ----- Melhor subsequência de A -----" << endl;
    // for (auto& el : melhor_seq){
    //     cout << el << " ";
    // }
    // cout << endl;

    // cout << " ----- Subsequência B correspondente -----" << endl;
    // for (auto& el : melhor_seq_b){
    //     cout << el << " ";
    // }
    // cout << endl;


    // cout << " ----- Melhor alinhamento das sequências por Smith Waterman -----" << endl;
    // for (auto& el : melhor_alinhamento_a){
    //     cout << el << " ";
    // }
    // cout << endl;
    // for (auto& el : melhor_alinhamento_b){
    //     cout << el << " ";
    // }
    // cout << endl;

    cout << max_score_global;

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

    int k_max=m;
    if (k_max>n)k_max=n;

    uniform_int_distribution<int> distribution_k(1,k_max);
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
    

    // cout  << "_______________________" << endl;
    // cout << "k: " << k << " | " << "p: " << p << endl;
    

    int max_score = 0; 
    vector<string> melhor_subseq;
    vector<string> match_seq_a, match_seq_b, melhor_match_seq_a, melhor_match_seq_b;
 
    for(auto& el: subs_sa){
        int score=smith_waterman(el, sb, k, k, match_seq_a,match_seq_b);
       
        if (score > max_score){
            max_score = score;  
            melhor_subseq = el; 
            melhor_match_seq_a=match_seq_a;
            melhor_match_seq_b=match_seq_b;
        }  
    } 


    item melhor;
    melhor.item_score = max_score;
    melhor.seq = melhor_subseq;
    melhor.seq_b = sb;
    melhor.rec_a = melhor_match_seq_a;
    melhor.rec_b = melhor_match_seq_b;

    return melhor;
}


item_sw find_max(int diagonal, int delecao, int insercao){
    item_sw maior;
    int max_score_local=0;
    max_score_local = max({0, diagonal, delecao, insercao});

    maior.valor = 0;
    maior.salto_i = 0;
    maior.salto_j = 0;
    if (diagonal == max_score_local){
        maior.valor = diagonal;
        maior.salto_i = 1;  // salto na diagonal
        maior.salto_j = 1;
    } 

    else if (delecao == max_score_local){
        maior.valor = delecao;
        maior.salto_i = 1;  // salto para cima
        maior.salto_j = 0;
    } 

    else if (insercao == max_score_local){
        maior.valor = insercao;
        maior.salto_i = 0;
        maior.salto_j = 1; // salto da esquerda para a direita
    } 

    return maior;
}


int smith_waterman(vector<string> a, vector<string> b, int n, int m, vector<string> &match_seq_a, vector<string> &match_seq_b){
    item_sw H[n+1][m+1];
    

    // Zerando colunas especificadas
    for (int i = 0; i <= n; i++) {
        H[i][0].valor=0;
    }
    for (int j = 0; j <= m; j++) {
        H[0][j].valor = 0;
    }


    // Obtendo matriz e achando o valor máximo
    int diagonal, delecao, insercao;
    int maximo_H = 0; int max_val_i = 0; int max_val_j = 0;

    for (int i=1; i<=n; i++){
        for (int j=1; j<=m; j++){
            diagonal = H[i-1][j-1].valor + w(a[i-1],b[j-1]);
            delecao = H[i-1][j].valor - 1;
            insercao = H[i][j-1].valor - 1;

            H[i][j]=find_max(diagonal, delecao, insercao);

            if (H[i][j].valor > maximo_H) {
                    maximo_H=H[i][j].valor;
                    max_val_i=i;
                    max_val_j=j;
            }
        }
    }


    // Imprimindo a matriz obtida
    // cout << endl << endl << "----- Matriz -----" << endl;
    // for (int i=0; i<=n; i++){
    //     for (int j=0; j<=m; j++){
    //         cout << H[i][j].valor << " ";
    //     }
    //     cout << endl;
    // }


// ____________________________________________ //
//  Reconstrução do alinhamento das sequências //
// __________________________________________ //

    

    int i=max_val_i; int j=max_val_j;


    // Reconstruindo o caminho a partir dos saltos do struct
    while ( (i>0 && j>0)  && (!(H[i][j].salto_j==0 && H[i][j].salto_i==0)) ) {
        int pos_i=i;
        int pos_j=j;
        if (H[i][j].valor == 0) break; // célula da matriz com valor zero

        if (H[pos_i][pos_j].salto_i==0 && H[pos_i][pos_j].salto_j ==1){
            match_seq_a.push_back("_");
            match_seq_b.push_back(b[j-1]);
        }
        else if (H[pos_i][pos_j].salto_i==1 && H[pos_i][pos_j].salto_j ==0){
            match_seq_a.push_back(a[i-1]);
            match_seq_b.push_back("_");
        }
        else{
            match_seq_a.push_back(a[i-1]);
            match_seq_b.push_back(b[j-1]);
        }
        
        
        i= i- H[pos_i][pos_j].salto_i;
        j=j- H[pos_i][pos_j].salto_j;

                
    }

    

    // Invertendo sequências
    reverse(match_seq_a.begin(),match_seq_a.end());
    reverse(match_seq_b.begin(),match_seq_b.end());

    // Imprimindo resultado
    // cout << endl << "----- Reconstrução do alinhamento -----" << endl;
    // cout << "a: ";
    // for(auto& el: match_seq_a){
    //     cout << el;
    // }
    // cout << endl;
    // cout << "b: ";
    // for(auto& el: match_seq_b){
    //     cout << el;
    // }


    return maximo_H;
}




// Para compilar: 

// g++ -Wall -O3 p2-sw.cpp -o busca-local-melhorada
// ./p2-sw < dna.seq