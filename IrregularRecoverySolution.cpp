#include <bits/stdc++.h>
using namespace std;

std::set<int> generateRandomNumbers(int n, int N, int delay)
{
    std::srand(std::time(0));

    std::set<int> uniqueRandomNumbers;

    while (uniqueRandomNumbers.size() < n)
    {
        int randomNumber = std::rand() % N;
        if (uniqueRandomNumbers.find(randomNumber) == uniqueRandomNumbers.end())
        {
            uniqueRandomNumbers.insert(randomNumber);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    return uniqueRandomNumbers;
}

int main()
{
    int die_side, Nfunc, Nred, Nmux;
    std::cout << "Enter the values of die area and the number of functional and redundant TSVs\n";
    cin >> die_side >> Nfunc >> Nred;
    std::cout << "Enter the co-ordinates of functinal TSVs on the die\n";
    vector<pair<double, double>> fncTsv, rdtTsv;
    for (int i = 0; i < Nfunc; i++)
    {
        double x, y;
        cin >> x >> y;
        fncTsv.push_back({x, y});
    }
    std::cout << "Enter the co-ordinates of redundant TSVs on the die\n";
    for (int i = 0; i < Nred; i++)
    {
        double x, y;
        cin >> x >> y;
        rdtTsv.push_back({x, y});
    }

    double val = sqrt((double)Nred);
    int lines;
    if (val - (double)floor(val) >= 0.5)
    {
        lines = floor(val) + 1;
    }
    else
        lines = floor(val);

    map<pair<int, int>, vector<int>> grid1red;
    map<pair<int, int>, vector<int>> grid2fnc;
    map<pair<int, int>, bool> done;
    int groupId = 0;

    double minx = INT_MAX, maxx = 0, miny = INT_MAX, maxy = 0;

    for (int i = 0; i < Nred; i++)
    {
        minx = min(minx, rdtTsv[i].first);
        miny = min(miny, rdtTsv[i].second);
        maxx = max(maxx, rdtTsv[i].first);
        maxy = max(maxy, rdtTsv[i].second);
    }
    for (int i = 0; i < Nfunc; i++)
    {
        minx = min(minx, fncTsv[i].first);
        miny = min(miny, fncTsv[i].second);
        maxx = max(maxx, fncTsv[i].first);
        maxy = max(maxy, fncTsv[i].second);
    }

    double xgap = (maxx - minx) / lines;
    double ygap = (maxy - miny) / lines;

    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < lines; j++)
        {

            int ind = 0;

            do
            {

                grid1red[{i, j}].clear();
                grid2fnc[{i, j}].clear();

                for (int k = 0; k < Nred; k++)
                {
                    if ((rdtTsv[k].first >= (minx + i * xgap - ind) && rdtTsv[k].first <= (minx + (i + 1) * xgap + ind)) && (rdtTsv[k].second >= (miny + j * ygap - ind) && rdtTsv[k].second <= (miny + (j + 1) * ygap + ind)))
                    {
                        grid1red[{i, j}].push_back(k);
                    }
                }
                for (int k = 0; k < Nfunc; k++)
                {
                    if ((fncTsv[k].first >= (minx + i * xgap) && fncTsv[k].first <= (minx + (i + 1) * xgap)) && (fncTsv[k].second >= (miny + j * ygap) && fncTsv[k].second <= (miny + (j + 1) * ygap)))
                    {
                        grid2fnc[{i, j}].push_back(k);
                    }
                }
                ind++;
            } while (ind <= lines * lines && (grid2fnc[{i, j}].size() == 0 || (grid1red[{i, j}].size() > 0 && (Nfunc / Nred) > (grid2fnc[{i, j}].size() / grid1red[{i, j}].size()))));
        }
    }

    int groupid = 1;
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < lines; j++)
        {
            cout << "functional tsvs of group :" << groupid << '\n';

            for (int k = 0; k < grid2fnc[{i, j}].size(); k++)
            {
                int idx = grid2fnc[{i, j}][k];

                cout << "(" << fncTsv[idx].first << ',' << fncTsv[idx].second << ")"
                     << " ";
            }
            cout << '\n';
            cout << "redundant tsvs of group :" << groupid << '\n';
            for (int k = 0; k < grid1red[{i, j}].size(); k++)
            {
                int idx = grid1red[{i, j}][k];
                cout << "(" << rdtTsv[idx].first << ',' << rdtTsv[idx].second << ")"
                     << " ";
            }
            cout << '\n';
            groupid++;
        }
    }

    for (int faulty = 1; faulty <= Nred; faulty++)
    {
        int delay = 200;
        int cnt = 0;
        for (int round = 0; round < 100; round++)
        {
            set<int> faultyOnes = generateRandomNumbers(faulty, Nfunc, delay);
            map<pair<double, double>, bool> used;
            int totFixed = 0;
            vector<bool> usedred(Nred, false);
            for (int idx : faultyOnes)
            {
                bool fixed = false, found = false;

                
                for (int i = 0; i < lines; i++)
                {
                    for (int j = 0; j < lines; j++)
                    {
                        for (auto cord : grid2fnc[{i, j}])
                        {
                            if (cord == idx)
                            {
                                found = true;
                                for (auto reds : grid1red[{i, j}])
                                {
                                    if (!usedred[reds])
                                    {
                                        usedred[reds] = true;
                                        fixed = true;
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        if (found)
                            break;
                    }
                    if (found)
                        break;
                }

                if (fixed)
                    totFixed++;
            }

            if (totFixed == faulty)
                cnt++;
        }
        delay -= 33;
        cout << "recovery rate for N_functional = " << Nfunc << ", N_redundent = " << Nred << " and N_faulty = " << faulty << " is : " << cnt << " percent\n";
    }

return 0;
}