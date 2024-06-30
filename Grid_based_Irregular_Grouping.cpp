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


double distance(const std::pair<double, double>& a, const std::pair<double, double>& b)
{
    return std::sqrt(std::pow(a.first - b.first, 2) + std::pow(a.second - b.second, 2));
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
    map<int, bool> done;
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
            } while ((ind <= lines + lines/2) && (((grid2fnc[{i, j}].size() == 0) && (grid1red[{i, j}].size() != 0)) || ((grid2fnc[{i, j}].size() != 0) && (grid1red[{i, j}].size() == 0)) || (grid1red[{i, j}].size() > 0 && (double)(Nfunc / Nred) > (double)(grid2fnc[{i, j}].size() / grid1red[{i, j}].size()))));
            for(int l=0; l<grid2fnc[{i, j}].size(); l++){
                done[grid2fnc[{i, j}][l]]=true;
            }
        }
    }
      for (int k = 0; k < Nfunc; k++){
        if(!done[k]){
            double max_dist=(double)INT_MAX;
            int ind;
             for (int i = 0; i < Nred; i++){
                if(distance(fncTsv[k], rdtTsv[i]) < max_dist){
                   max_dist=distance(fncTsv[k], rdtTsv[i]);
                   ind = i;
                }
             }
    
             bool  found=false;
             for (int i = 0; i < lines; i++)
                {
                    for (int j = 0; j < lines; j++)
                    {
                        for (auto cord : grid1red[{i, j}])
                        {
                            if (cord == ind)
                            {
                                found = true;
                                grid2fnc[{i, j}].push_back(k);
                                break;
                            }
                        }
                        if (found)
                            break;
                    }
                    if (found)
                        break;
                }
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
            cout << '\n' << '\n';
            groupid++;
        }
    }

    for (int faulty = 1; faulty <= Nred; faulty++)
    {
        int delay = 200;
        int cnt = 0;
        int num_of_mux=0, avg_num_of_mux=0;
        double total_wire_length=0, avg_wire_length=0;
        for (int round = 0; round < 100; round++)
        {
            set<int> faultyOnes = generateRandomNumbers(faulty, Nfunc, delay);
            map<pair<double, double>, bool> used;
            int totFixed = 0;
            vector<bool> usedred(Nred, false);
            for (int idx : faultyOnes)
            {
                bool fixed = false, found = false;

                int cnt_of_rdt=0;
                for (int i = 0; i < lines; i++)
                {
                    for (int j = 0; j < lines; j++)
                    {
                        for (auto cord : grid2fnc[{i, j}])
                        {
                            if (cord == idx)
                            {
                                found = true;
                                cnt_of_rdt += grid1red[{i, j}].size();
                                if(!fixed){
                                for (auto reds : grid1red[{i, j}])
                                {
                                    if (!usedred[reds])
                                    {
                                        usedred[reds] = true;
                                        fixed = true;
                                        total_wire_length += distance(fncTsv[idx], rdtTsv[reds]);
                                        break;
                                    }
                                }
                                }
                                break;
                                
                            }
                        }
                       
                    }
                  
                }

                if (fixed)
                    totFixed++;
                num_of_mux += ceil(log2(cnt_of_rdt));
            }

            if (totFixed == faulty){
                cnt++;
            }
              avg_wire_length += total_wire_length;
              total_wire_length=0;
             avg_num_of_mux += num_of_mux;
             num_of_mux = 0;
        }
        delay -= 33;
        cout << "recovery rate for N_functional = " << Nfunc << ", N_redundent = " << Nred << " and N_faulty = " << faulty << " is : " << cnt << " percent\n";
        cout << "Averege minimum wire length required = " << avg_wire_length/100 << "unit " << '\n';
        cout << "Averege number of mux required = " << ceil(avg_num_of_mux/100) << '\n' << '\n';
    }

return 0;
}
