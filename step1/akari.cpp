# include <bits/stdc++.h>
# include <algorithm>
# include "akari.h"
using namespace std;

namespace aka{
//请在命名空间内编写代码，否则后果自负

#define WHITE -2
#define BLACK -1

struct Position {
    int row;
    int col;
    int number;
};

struct Light {
    // bool lighted;
    int setable;
    int hasLight;
};

vector<vector<int> > puzzle;
vector<vector<int> > ans;
vector<vector<Light> > light;
vector<Position> numberGrids;
vector<Position> unlightedGrids;
vector<Position> unsetableGrids;
vector<Position> mustSetGrids;

int row, column;

void displayLight()
{
    int n = light.size(), m = light[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            printf("%4d", light[i][j].hasLight);
        }
        printf("\n");
    }
}

bool isLighted(int row, int col)
{
    bool lighted = false;

    if (puzzle[row][col] != WHITE) return false;
    if (light[row][col].hasLight > 0) return true;

    for (int i = row - 1; i >= 0; i--)
    {
        if (puzzle[i][col] == WHITE)
        {
            if (light[i][col].hasLight > 0)
                lighted = true;
        }
        else break; // blocked
    }

    for (int i = row + 1; i < column; i++)
    {
        if (puzzle[i][col] == WHITE)
        {
            if (light[i][col].hasLight > 0)
                lighted = true;
        }
        else break;
    }

    for (int j = col - 1; j >= 0; j--)
    {
        if (puzzle[row][j] == WHITE)
        {
            if (light[row][j].hasLight > 0)
                lighted = true;
        }
        else break;
    }

    for (int j = col + 1; j < column; j++)
    {
        if (puzzle[row][j] == WHITE)
        {
            if (light[row][j].hasLight > 0)
                lighted = true;
        }
        else break;
    }

    return lighted;
}

bool isLightable(int row, int col)
{
    for (int i = row - 1; i >= 0; i--)
    {
        if (puzzle[i][col] == WHITE)
        {
            if (light[i][col].setable > 0 && !isLighted(i, col))
                return true;
        }
        else break; // blocked
    }

    for (int i = row + 1; i < column; i++)
    {
        if (puzzle[i][col] == WHITE)
        {
            if (light[i][col].setable > 0 && !isLighted(i, col))
                return true;
        }
        else break;
    }

    for (int j = col - 1; j >= 0; j--)
    {
        if (puzzle[row][j] == WHITE)
        {
            if (light[row][j].setable > 0 && !isLighted(row, j))
                return true;
        }
        else break;
    }

    for (int j = col + 1; j < column; j++)
    {
        if (puzzle[row][j] == WHITE)
        {
            if (light[row][j].setable > 0 && !isLighted(row, j))
                return true;
        }
        else break;
    }

    return false;
}

bool setLight(int row, int col)
{
    if (row < 0 || row >= aka::row) return false;
    if (col < 0 || col >= aka::column) return false;

    if (light[row][col].setable <= 0) return false;
    if (light[row][col].hasLight > 0)
    {
        light[row][col].hasLight++;
        return true;
    }
    if (isLighted(row, col)) return false;  // duplicate

    light[row][col].hasLight++;    // set a light

    return true;
}

// for recovery
void removeLight(int row, int col)
{
    light[row][col].hasLight--;
}

void displayLighted()
{
    int n = puzzle.size(), m = puzzle[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (light[i][j].hasLight > 0) printf("%4d", 5);
            else if (isLighted(i, j)) printf("%4d", 8);
            else printf("%4d", puzzle[i][j]);
        }
        printf("\n");
    }
}

bool isSolution()
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (puzzle[i][j] == WHITE)
            {
                if (isLighted(i, j)) continue;
                return false;
            }
        }
    }
    return true;
}

bool disable(int row, int col)
{
    if (row < 0 || row >= aka::row) return true;
    if (col < 0 || col >= aka::column) return true;

    if (light[row][col].hasLight > 0) return false;
    
    light[row][col].setable--;
    
    return true;
}

// for recovery
void enable(int row, int col)
{
    if (row < 0 || row >= aka::row) return;
    if (col < 0 || col >= aka::column) return;

    light[row][col].setable++;
}

bool mySort (const Position &p1, const Position &p2)
{
    return p1.number > p2.number;
}

void scanNumberGrids()
{
    Position pos;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (puzzle[i][j] == WHITE || puzzle[i][j] == BLACK) continue;
            pos.row = i;
            pos.col = j;
            pos.number = puzzle[i][j];
            numberGrids.push_back(pos);
        }
    }
    sort(numberGrids.begin(), numberGrids.end(), mySort);
}

void scanUnsetableGrids()
{
    unsetableGrids.clear();
    Position pos;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (puzzle[i][j] == WHITE && light[i][j].setable <= 0 && !isLighted(i, j))
            {
                pos.row = i;
                pos.col = j;
                pos.number = 0;
                unsetableGrids.push_back(pos);
            }
        }
    }
}

void scanUnlightedGrids()
{
    unlightedGrids.clear();
    Position pos;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (light[i][j].setable > 0 && !isLighted(i, j))
            {
                pos.row = i;
                pos.col = j;
                pos.number = 0;
                unlightedGrids.push_back(pos);
            }
        }
    }
}

void scanMustSetGrids()
{
    mustSetGrids.clear();
    Position pos;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (light[i][j].setable > 0 && !isLighted(i, j) && !isLightable(i, j))
            {
                pos.row = i;
                pos.col = j;
                pos.number = 0;
                mustSetGrids.push_back(pos);
            }
        }
    }
}

int count = 0;

bool complement()
{
    if (isSolution()) return true;
    if (count == unlightedGrids.size()) return false;

    for (int i = 0; i < unlightedGrids.size(); i++)
    {
        int row = unlightedGrids[i].row;
        int col = unlightedGrids[i].col;
        int number = unlightedGrids[i].number;
        
        if (number > 0) continue; // enumed
        
        unlightedGrids[i].number = 1;
        count++;
        bool ret = setLight(row, col);

        if (complement()) return true;
        
        // recover
        unlightedGrids[i].number = 0;
        count--;
        if (ret) removeLight(row, col);
    }

    return false;
}

bool step(int index)
{
    // found a posible solution
    if (index == numberGrids.size()) 
    {
        // priority processing
        // check those unsetable grids first
        scanUnsetableGrids();
        for (int i = 0; i < unsetableGrids.size(); i++)
        {
            if (!isLightable(unsetableGrids[i].row, unsetableGrids[i].col)) return false;
        }

        scanMustSetGrids();
        for (int i = 0; i < mustSetGrids.size(); i++)
        {
            light[mustSetGrids[i].row][mustSetGrids[i].col].hasLight++;
        }
        
        // complement
        scanUnlightedGrids();

        // for (int i = 0; i < unlightedGrids.size(); i++)
        //     printf("%d %d\n", unlightedGrids[i].row, unlightedGrids[i].col);
        
        return complement();
    }
    if (isSolution()) return true;

    int row = numberGrids[index].row;
    int col = numberGrids[index].col;
    int number = numberGrids[index].number;
    //printf("%d %d %d\n", row, col, number);

    // enum all posible solutions
    switch (number)
    {
        case 0:
            if (disable(row - 1, col))
            {
                if (disable(row + 1, col))
                {
                    if (disable(row, col - 1))
                    {
                        if (disable(row, col + 1))
                        {
                            if (step(index + 1)) return true;
                            enable(row, col + 1);
                        }
                        enable(row, col - 1);
                    }
                    enable(row + 1, col);
                }
                enable(row - 1, col);
            }
            break;
        case 1:
            if (disable(row + 1, col))
            {
                if (disable(row, col - 1))
                {
                    if (disable(row, col + 1))
                    {
                        if (setLight(row - 1, col))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row - 1, col);
                        }
                        enable(row, col + 1);
                    }
                    enable(row, col - 1);
                }
                enable(row + 1, col);
            }
            if (disable(row - 1, col))
            {
                if (disable(row, col - 1))
                {
                    if (disable(row, col + 1))
                    {
                        if (setLight(row + 1, col))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row + 1, col);
                        }
                        enable(row, col + 1);
                    }
                    enable(row, col - 1);
                }
                enable(row - 1, col);
            }
            if (disable(row - 1, col))
            {
                if (disable(row + 1, col))
                {
                    if (disable(row, col + 1))
                    {
                        if (setLight(row, col - 1))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row, col - 1);
                        }
                        enable(row, col + 1);
                    }
                    enable(row + 1, col);
                }
                enable(row - 1, col);
            }
            if (disable(row - 1, col))
            {
                if (disable(row + 1, col))
                {
                    if (disable(row, col - 1))
                    {
                        if (setLight(row, col + 1))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row, col + 1);
                        }
                        enable(row, col - 1);
                    }
                    enable(row + 1, col);
                }
                enable(row - 1, col);
            }
            break;
        case 2:
            if (disable(row, col - 1))
            {
                if (disable(row, col + 1))
                {
                    if (setLight(row - 1, col))
                    {
                        if (setLight(row + 1, col))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row + 1, col);
                        }
                        removeLight(row - 1, col);
                    }
                    enable(row, col + 1);
                }
                enable(row, col - 1);
            }
            if (disable(row + 1, col))
            {
                if (disable(row, col + 1))
                {
                    if (setLight(row - 1, col))
                    {
                        if (setLight(row, col - 1))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row, col - 1);
                        }
                        removeLight(row - 1, col);
                    }                        
                    enable(row, col + 1);
                }
                enable(row + 1, col);
            }
            if (disable(row + 1, col))
            {
                if (disable(row, col - 1))
                {
                    if (setLight(row - 1, col))
                    {
                        if (setLight(row, col + 1))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row, col + 1);
                        }
                        removeLight(row - 1, col);
                    }
                    enable(row, col - 1);
                }
                enable(row + 1, col);
            }
            if (disable(row - 1, col))
            {
                if (disable(row, col + 1))
                {
                    if (setLight(row + 1, col))
                    {
                        if (setLight(row, col - 1))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row, col - 1);
                        }
                        removeLight(row + 1, col);
                    }
                    enable(row, col + 1);
                }
                enable(row - 1, col);
            }
            if (disable(row - 1, col))
            {
                if (disable(row, col - 1))
                {
                    if (setLight(row + 1, col))
                    {
                        if (setLight(row, col + 1))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row, col + 1);
                        }
                        removeLight(row + 1, col);
                    }
                    enable(row, col - 1);
                }
                enable(row - 1, col);
            }
            if (disable(row - 1, col))
            {
                if (disable(row + 1, col))
                {
                    if (setLight(row, col - 1))
                    {
                        if (setLight(row, col + 1))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row, col + 1);
                        }
                        removeLight(row, col - 1);
                    }
                    enable(row + 1, col);
                }
                enable(row - 1, col);
            }
            break;
        case 3:
            if (disable(row, col + 1))
            {
                if (setLight(row - 1, col))
                {
                    if (setLight(row + 1, col))
                    {
                        if (setLight(row, col - 1))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row, col - 1);
                        }
                        removeLight(row + 1, col);
                    }
                    removeLight(row - 1, col);
                }
                enable(row, col + 1);
            }
            if (disable(row, col - 1))
            {
                if (setLight(row - 1, col))
                {
                    if (setLight(row + 1, col))
                    {
                        if (setLight(row, col + 1))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row, col + 1);
                        }
                        removeLight(row + 1, col);
                    }
                    removeLight(row - 1, col);
                }
                enable(row, col - 1);
            }
            if (disable(row + 1, col))
            {
                if (setLight(row - 1, col))
                {
                    if (setLight(row, col - 1))
                    {
                        if (setLight(row, col + 1))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row, col + 1);
                        }
                        removeLight(row, col - 1);
                    }
                    removeLight(row - 1, col);
                }
                enable(row + 1, col);
            }
            if (disable(row - 1, col))
            {
                if (setLight(row + 1, col))
                {
                    if (setLight(row, col - 1))
                    {
                        if (setLight(row, col + 1))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row, col + 1);
                        }
                        removeLight(row, col - 1);
                    }
                    removeLight(row + 1, col);
                }
                enable(row - 1, col);
            }
            break;
        case 4:
            if (setLight(row - 1, col))
            {
                if (setLight(row + 1, col))
                {
                    if (setLight(row, col - 1))
                    {
                        if (setLight(row, col + 1))
                        {
                            if (step(index + 1)) return true;
                            removeLight(row, col + 1);
                        }
                        removeLight(row, col - 1);
                    }
                    removeLight(row + 1, col);
                }
                removeLight(row - 1, col);
            }
            break;
    }
    //printf("dead\n");
    return false;
}

vector<vector<int> > solveAkari(vector<vector<int> > & g)
{
    // 请在此函数内返回最后求得的结果
    row = g.size();
    column = g[0].size();
    puzzle = g;
    ans = g;
    Light l;

    for (int i = 0; i < row; ++i)
    {
        light.push_back(vector<Light>());
        for (int j = 0; j < column; ++j)
        {
            if (puzzle[i][j] == WHITE) l.setable = 1;
            else l.setable = 0;
            // l.lighted = false;
            l.hasLight = 0;
            light[i].push_back(l);
        }
    }

    scanNumberGrids();

    if (step(0))
    {
        
    }

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < column; ++j)
        {
            if (light[i][j].hasLight > 0)
                ans[i][j] = 5;
        }
    }

    return ans;
}

}

