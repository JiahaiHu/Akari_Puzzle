# include "akari.h"
# include <bits/stdc++.h>
using namespace std;
#pragma warning(disable:4996)

void checkAkari(vector<vector<int> > & g, vector<vector<int> > & ans);
void displayAkari(vector<vector<int> > & ans);

int main ()
{
    int n, m, tmp;
    cin >> n >> m;
    vector<vector<int> > g;
    for (int i = 1; i <= n; ++i) {
        g.push_back(vector<int>());
        for (int j = 1; j <= m; ++j) {
            cin >> tmp;
            g[i-1].push_back(tmp);
        }
    }
    vector<vector<int> > ans = aka::solveAkari(g);
    checkAkari(g, ans);
    return 0;
}

void checkAkari(vector<vector<int> > & g, vector<vector<int> > & ans)
{
    int n = (int)g.size(), m = (int)g[0].size();
    if (n != (int)ans.size() || m != (int)ans[0].size()) {
        cout << "The original size does not match!" << endl;
        displayAkari(ans);
        return ;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (g[i][j] >= -1 && g[i][j] <= 4) {
                if (ans[i][j] != g[i][j]) {
                    cout << "The original picture does not match the black grid!" << endl;
                    displayAkari(ans);
                    return ;
                }
            }
            else {
                if (ans[i][j] >= -1 && ans[i][j] <= 4) {
                    cout << "The original picture does not match the black grid!" << endl;
                    displayAkari(ans);
                    return ;
                }
            }
        }
    }
    int ps[4][2] = {-1, 0, 1, 0, 0, -1, 0, 1};
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (ans[i][j] >= 0 && ans[i][j] <= 4) {
                int cnt = 0;
                for (int k = 0; k < 4; ++k) {
                    int dx = i + ps[k][0], dy = j + ps[k][1];
                    if (dx >= 0 && dx < n && dy >= 0 && dy < m && ans[dx][dy] == 5) ++cnt;
                }
                if (cnt != ans[i][j]) {
                    cout << "The number of lights around the black grid does not match the number!" << endl;
                    displayAkari(ans);
                    return ;
                }
            }
            else if (ans[i][j] == 5) {
                for (int k = i + 1; k < n; ++k) {
                    if (ans[k][j] >= -1 && ans[k][j] <= 4) break;
                    if (ans[k][j] == 5) {
                        cout << "Two lights shine on each other!" << endl;
                        displayAkari(ans);
                        return ;
                    }
                }
                for (int k = j + 1; k < m; ++k) {
                    if (ans[i][k] >= -1 && ans[i][k] <= 4) break;
                    if (ans[i][k] == 5) {
                        cout << "Two lights shine on each other!" << endl;
                        displayAkari(ans);
                        return ;
                    }
                }
            }
            else if (ans[i][j] == -2) {
                bool flag = true;
                for (int k = i + 1; k < n; ++k) {
                    if (ans[k][j] >= -1 && ans[k][j] <= 4) break;
                    if (ans[k][j] == 5) flag = false;
                }
                for (int k = i - 1; k >= 0; --k) {
                    if (ans[k][j] >= -1 && ans[k][j] <= 4) break;
                    if (ans[k][j] == 5) flag = false;
                }
                for (int k = j + 1; k < m; ++k) {
                    if (ans[i][k] >= -1 && ans[i][k] <= 4) break;
                    if (ans[i][k] == 5) flag = false;
                }
                for (int k = j - 1; k >= 0; --k) {
                    if (ans[i][k] >= -1 && ans[i][k] <= 4) break;
                    if (ans[i][k] == 5) flag = false;
                }
                if (flag == true) {
                    cout << "There's a place where all the lights don't shine!" << endl;
                    displayAkari(ans);
                    return ;
                }
            }
        }
    }
    cout << "The answer is right!";
}

void displayAkari(vector<vector<int> > & ans)
{
    printf("your magic matrix is following: \n");
    int n = ans.size(), m = ans[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            printf("%4d", ans[i][j]);
        }
        printf("\n");
    }
}

