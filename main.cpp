;//
//  main.cpp
//  task3_prob4(dinica)
//
//  Created by Raisa Kim on 23.05.17.
//  Copyright © 2017 Raisa Kim. All rights reserved.
//
/*Расстояние Хэмминга между двумя строками равной длины — это количество индексов, в которых символы этих строк не равны.
 Определим расстояние от шаблона p до строки s как суммарное расстояние Хэмминга от p до всех подстрок s, имеющих длину |p|. В строке и в шаблоне некоторые символы стёрты. Нужно восстановить стёртые символы так, чтобы расстояние от p до s было минимально возможным.
 
 Формат входного файла.
 В первой строке дана строка s, во второй — шаблон p. Обе строки непусты, имеют длину не более 1000 и состоят из символов `0', `1' и `?'. `?' обозначает стёртые символы, которые нужно восстановить. Длина строки p не превосходит длины строки s.
 
 Формат выходного файла.
 В первой строке выведите минимальное расстояние от p до s после восстановления всех стёртых символов. Во второй и третьей строках соответственно выведите s и p, в которых символы `?' заменены на `0' и `1'.
*/

#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <fstream>
#include <string>

using namespace std;

using Graph = vector<vector<int>>;
//using Edge = vector<vector<int>>;

class Dinica{
    const int inf = INT_MAX;
    int n;//количество вершин
    int s;//исток
    int t;//сток
    Graph& capacity;//пропускная способность ребер
    Graph& flow;//поток через ребро
    int maxflow;
    vector<bool> mincut;
    vector<int> p;// вспомогательный массив
    vector<int> d;//длина кратчайшего пути из s в вершину, вспомогательный массив
    bool bfs();
    int dfs(int u, int minc);
    int findmaxflow();//алгоритм Диница
    public:
    Dinica(Graph& g, Graph& flow, int n, int s, int t): n(n), s(s), t(t), capacity(g), maxflow(0), flow(flow), mincut(n), d(n), p(n){
        maxflow = findmaxflow();
    }
    int Maxflow(){
        return maxflow;
    }
    vector<bool> Mincut(){
        return mincut;
    }
};

bool Dinica:: bfs(){
    queue<int> q;
    d.assign(n, inf);
    mincut.assign(n, false);
    d[s] = 0;
    q.push(s);
    int u;
    while(!q.empty()){
        u = q.front();
        q.pop();
        mincut[u] = true;
        for(int i = 0;i<n;i++){
            if(flow[u][i]<capacity[u][i] && d[i] == inf){
                d[i] = d[u] + 1;
                q.push(i);
            }
        }
    }
    if(d[t] != inf){
        return true;
    }
    else{
        return false;
    }
}

int Dinica::dfs(int u, int minc){
    //int v;
    int delta = 0;
    if(u==t || minc == 0){
        return minc;
    }
    for(int v = p[u]; v<n; v++){
        //v = edge[u][i];
        if(d[v] == d[u] + 1){
            delta = dfs(v, min(minc, capacity[u][v] - flow[u][v]));
            if(delta != 0){
                flow[u][v] = flow[u][v] + delta;
                flow[v][u] = flow[v][u] - delta;
                return delta;
            }
        }
        p[u]++;
    }
    return 0;
}

int Dinica::findmaxflow(){
    int maxflow1 = 0;
    int delta = 0;
    while (bfs()){
        p.assign(n, 0);
        delta = dfs(s, inf);
        while(delta != 0){
            maxflow1 = maxflow1 + delta;
            delta = dfs(s, inf);
        }
    }
    return maxflow1;
}

class Hamming_distance{
    /*копируем шаблон и строку, вместо вопросов ставим индекс. строим граф */
    int n;
    string s, p;
    vector<int> s1;
    vector<int> p1;
    vector<vector<int>> g;
    int hamming_distance = 0;
    public:
    Hamming_distance(string s, string p):n(2){
        for(auto j : s){
            if(j == '?'){
                s1.push_back(n);
                n++;
            }
            else{
                if(j == '0'){
                    s1.push_back(0);
                }
                if(j == '1'){
                    s1.push_back(1);
                }
            }
        }
        for(auto j : p){
            if(j == '?'){
                p1.push_back(n);
                n++;
            }
            else{
                if(j == '0'){
                    p1.push_back(0);
                }
                if(j == '1'){
                    p1.push_back(1);
                }
            }
        }
        size_t sl = s1.size();
        size_t pl = p1.size();
        int k1 = 0;
        int a;
        size_t m = sl - pl + 1;
        g.resize(n+1);
        for(auto & j : g){
            j.resize(n+1);
        }
        for(int i = 0; i<m; i++){
            k1 = i;
            for(int k : p1){
                a = s1[k1];
                if(k>1){
                    if(a>1){
                        g[k][a]++;
                        g[a][k]++;
                    }
                    else{
                        if(a == 1){
                            g[k][n]++;
                        }
                        if(a == 0){
                            g[1][k]++;
                        }
                    }
                }
                else{
                    if(a>1){
                        if(k == 0){
                            g[1][a]++;
                        }
                        if(k == 1){
                            g[a][n]++;
                        }
                    }
                    else{
                        if(a != k){
                            hamming_distance++;
                        }
                    }
                }
                k1++;
            }
        }
    }
    int Sizeofgraph(){
        return n;
    }
    Graph Graph_(){
        return g;
    }
    vector<int> Str(){
        return s1;
    }
    vector<int> Pattern(){
        return p1;
    }
};

int distance(vector<int> & s1, vector<int> & p1, vector<bool> & g1){
    size_t sl = s1.size();
    size_t pl = p1.size();
    int k1 = 0;
    int a;
    size_t m = sl - pl + 1;
    int h = 0;
    for(auto &j : s1){
        if(j>1){
            if(g1[j]){
                j = 0;
            }
            else{
                j = 1;
            }
        }
    }
    for(auto &j : p1){
        if(j>1){
            if(g1[j]){
                j  = 0;
            }
            else{
                j = 1;
            }
        }
    }
    for(int i = 0; i<m; i++){
        k1 = i;
        for(int k : p1){
            a = s1[k1];
            if(a!=k){
                h++;
            }
            k1++;
        }
    }
    return h;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    string s, p;
    Graph g;
    vector<bool> g1;
    vector<int> s1, p1;
    int n;
    int h;
    cin >> s;
    cin >> p;
    Hamming_distance dist(s, p);//строим граф, копируем строку и шаблон в массив(вопросы заменяем на индекс)
    n = dist.Sizeofgraph();//количество вершин в графе
    vector<vector<int>> flow(n+1, vector<int> (n+1));//вспомогательный массив
    g = dist.Graph_();//граф
    Dinica din(g, flow, n+1, 1, n);//пускаем максимальный поток
    g1 = din.Mincut();//достаем список достижимых вершин из истока
    s1 = dist.Str();//копия строки
    p1 = dist.Pattern();//копия шаблона
    h = distance(s1, p1, g1);//минимальное расстояние Хэмминга
    cout << h << endl;
    for(auto j : s1){
        cout << j;
    }
    cout << endl;
    for(auto j : p1){
        cout << j;
    }
    return 0;
}
