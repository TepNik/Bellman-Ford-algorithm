#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <iomanip>

//#define DEBUG

struct vec
{
    int ind_from;
    int num;
};

void add_name(std::vector<std::string> &names,
              std::vector<std::vector<int>> &adj_matrix,
              const std::string &name);
void add_num(std::vector<std::string> &names,
             std::vector<std::vector<int>> &adj_matrix,
             const std::string &first_name,
             const std::string &second_name,
             int num);
void processing_line(std::vector<std::string> &names,
                     std::vector<std::vector<int>> &adj_matrix,
                     const std::string &line);

std::vector<vec> get_fastest_paths(const std::vector<std::vector<int>> &adj_matrix, int ind);

int INF = 100000000;

int main()
{
    std::ifstream fin("input.txt");
    std::vector<std::string> names;
    std::vector<std::vector<int>> adj_matrix;
    {
        std::string line;
        std::getline(fin, line);
        while(fin)
        {
            processing_line(names, adj_matrix, line);
            std::getline(fin, line);
        }
    }

    #ifdef DEBUG
    std::cout << "\nNames:\n";
    for(int i = 0; i < names.size(); ++i)
        std::cout << names[i] << '\n';

    std::cout << "\nMatrix:\n";
    for(int i = 0; i < adj_matrix.size(); ++i)
    {
        for(int j = 0; j < adj_matrix[i].size(); ++j)
            std::cout << std::setw(2) << (adj_matrix[i][j] < INF ? std::to_string(adj_matrix[i][j]) : "-") << ' ';
        std::cout  << '\n';
    }
    #endif

    std::cout << "From what city you want to fly: ";
    std::string first_name;
    std::getline(std::cin, first_name);
    std::cout << "To what city you want to fly: ";
    std::string second_name;
    std::getline(std::cin, second_name);
    int ind1, ind2;
    ind1 = std::find(names.begin(), names.end(), first_name) - names.begin();
    ind2 = std::find(names.begin(), names.end(), second_name) - names.begin();

    auto paths = get_fastest_paths(adj_matrix, ind1);
    if (paths[ind2].num == INF)
        std::cout << "There is no path from " << names[ind1] << " to " << names[ind2] << ".\n";
    else
    {
        #ifdef DEBUG
        std::cout << "Paths:\n";
        for (int i = 0; i < paths.size(); ++i)
            std::cout << "num = " << paths[i].num << "; last_ind = " << paths[i].ind_from << '\n';
        #endif
        std::vector<int> path;
        path.push_back(ind2);
        int ind_from = paths[ind2].ind_from;
        while(ind_from != -1)
        {
            path.push_back(ind_from);
            ind_from = paths[ind_from].ind_from;
        }
        std::reverse(path.begin(), path.end());
        std::cout << "Path:\n";
        for (int i = 0; i < path.size(); ++i)
            std::cout << names[path[i]] << (i < (path.size() - 1) ? " --- " : "");
        std::cout << "\nCost is: " << paths[ind2].num << '\n';
    }

    return 0;
}

void add_name(std::vector<std::string> &names,
              std::vector<std::vector<int>> &adj_matrix,
              const std::string &name)
{
    int find_ind = std::find(names.begin(), names.end(), name) - names.begin();
    if (find_ind == names.size())
    {
        names.push_back(name);
        adj_matrix.push_back(std::vector<int>(names.size(), INF));
        for(int i = 0; i < names.size() - 1; ++i)
            adj_matrix[i].push_back(INF);
    }
}

void add_num(std::vector<std::string> &names,
             std::vector<std::vector<int>> &adj_matrix,
             const std::string &first_name,
             const std::string &second_name,
             int num)
{
    int ind1 = std::find(names.begin(), names.end(), first_name) - names.begin();
    int ind2 = std::find(names.begin(), names.end(), second_name) - names.begin();

    adj_matrix[ind1][ind2] = num;
}

void processing_line(std::vector<std::string> &names,
                     std::vector<std::vector<int>> &adj_matrix,
                     const std::string &line)
{
    int ind1 = 0, ind2;
    ind2 = line.find_first_of(";", ind1);
    std::string first_name = line.substr(ind1, ind2 - ind1);
    add_name(names, adj_matrix, first_name);

    ind1 = ind2 + 1;
    ind2 = line.find_first_of(";", ind1);
    std::string second_name = line.substr(ind1, ind2 - ind1);
    add_name(names, adj_matrix, second_name);

    int num1, num2;
    ind1 = ind2 + 1;
    ind2 = line.find_first_of(";", ind1);
    std::string temp = line.substr(ind1, ind2 - ind1);
    if (temp == "N/A")
        num1 = INF;
    else
        num1 = std::stoi(temp);
    add_num(names, adj_matrix, first_name, second_name, num1);

    ind1 = ind2 + 1;
    temp = line.substr(ind1, line.size() - ind1);
    if (temp == "N/A")
        num2 = INF;
    else
        num2 = std::stoi(temp);
    add_num(names, adj_matrix, second_name, first_name, num2);
}

std::vector<vec> get_fastest_paths(const std::vector<std::vector<int>> &adj_matrix, int ind)
{
    std::vector<vec> paths(adj_matrix.size(), {-1, INF});
    paths[ind] = {-1, 0};

    for(int i = 1; i < adj_matrix.size() - 1; ++i)
        for (int i = 0; i < adj_matrix.size(); ++i)
            for(int j = 0; j < adj_matrix.size(); ++j)
                if (adj_matrix[i][j] != INF && j != ind && paths[i].num != INF)
                    if (paths[j].num > paths[i].num + adj_matrix[i][j])
                    {
                        paths[j].num = paths[i].num + adj_matrix[i][j];
                        paths[j].ind_from = i;
                    }

    return paths;
}