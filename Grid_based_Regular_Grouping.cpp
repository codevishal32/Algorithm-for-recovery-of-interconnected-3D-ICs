#include <bits/stdc++.h>
using namespace std;
const int min_n = 11; // Greater than 10
const int max_n = 20; // Adjust as needed
const int min_m = 4;  // Greater than 3
const int max_m = 10; // Adjust as needed

// Define the die area
const int die_width = 50;
const int die_height = 50;

// Function to generate random coordinates within the die area
pair<vector<pair<double, double>>, vector<pair<double, double>>> generateTestCase()
{
    // Generate values for n and m
    srand(static_cast<unsigned int>(time(0)));
    int n = 62; // n is greater than 10
    int m = 17;  // m is greater than 3

    // Ensure the ratio between functional TSVs and redundant TSVs is greater than 2
    while (static_cast<double>(n) / m <= 2)
    {
        n = rand() % 60 + 11;
        m = rand() % 25 + 4;
    }
   
    // Generate functional TSV coordinates
    vector<pair<double, double>> functional_tsvs;
    for (int i = 0; i < n; ++i)
    {
        int x = static_cast<double>(rand()) / RAND_MAX * 50;
        int y = static_cast<double>(rand()) / RAND_MAX * 50;
        double a = double(x);
        double b = double(y);
        functional_tsvs.push_back(make_pair(a, b));
    }

    // Generate redundant TSV coordinates
    vector<pair<double, double>> redundant_tsvs;
    for (int i = 0; i < m; ++i)
    {
        int x = static_cast<double>(rand()) / RAND_MAX * 50;
        int y = static_cast<double>(rand()) / RAND_MAX * 50;
        double a = double(x);
        double b = double(y);
        redundant_tsvs.push_back(make_pair(a, b));
    }

    return make_pair(functional_tsvs, redundant_tsvs);
}

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
    
        pair<vector<pair<double, double>>, vector<pair<double, double>>> coordinate = generateTestCase();
        int die_side=50, Nfunc, Nred, Nmux;
        Nfunc = coordinate.first.size();
        Nred = coordinate.second.size();
        cout << "Number of functional tsvs : " << Nfunc << '\n';
        cout << "Number of redundant tsvs : " << Nred << '\n';
        die_side = 50;
        vector<pair<double, double>> fncTsv, rdtTsv;
        cout << "the co-ordinates of functional TSVs on the die\n";
        for (auto itr : coordinate.first)
        {
            cout << itr.first << " " << itr.second << "\n";
            fncTsv.push_back({(double)itr.first, (double)itr.second});
        }
        cout << "the co-ordinates of redundant TSVs on the die\n";
        for (auto itr : coordinate.second)
        {
            cout << itr.first << " " << itr.second << "\n";
            rdtTsv.push_back({(double)itr.first, (double)itr.second});
        }
        
        double redious = (double)die_side / Nred;

        int max_grp_size = ceil((double)Nfunc / (double)Nred);
        vector<pair<double, double>> groups[Nred];

        vector<bool> done(Nfunc, false);
        int ind = 0, countfnc = 0;

        vector<vector<pair<double, double>>> parent(Nred, vector<pair<double, double>>(Nfunc));


        for (int i = 0; i < Nred; i++)
        {
            queue<pair<double, double>> qu;
            qu.push(rdtTsv[i]);
            
            map<pair<double, double>, bool> visited;
            visited[rdtTsv[i]] = true;
            double var_rad=redious;
            while (groups[i].size() < max_grp_size)
            {

                pair<double, double> current = qu.front();
                qu.pop();
                for (int j = 0; j < Nfunc; j++)
                {   

                    
                    if (!visited[fncTsv[j]] && distance(current, fncTsv[j]) <= var_rad)
                    {  
                        groups[i].push_back(fncTsv[j]);
                        qu.push(fncTsv[j]);
                        visited[fncTsv[j]] = true;
                        parent[i][j] = current;
                        if (!done[j])
                            countfnc++;

                        done[j] = true;
                    }
                    if (groups[i].size() == max_grp_size)
                    {
                        break;
                    }

                }
                if(groups[i].size() < max_grp_size && qu.empty()){
                    qu.push(rdtTsv[i]);
                    var_rad += 1;
                }
            }
        }
       
        cout<<"total functional tsv done "<< countfnc <<'\n';
      
        for (int j = 0; j < Nfunc; j++)
            {
                if (!done[j])
                {
                    double min_dist = (double)INT_MAX, min_dist1 = (double)INT_MAX;
                    int idx1, idx2;
                    for (int i = 0; i < Nred; i++)
                    {
                        if (min_dist > distance(fncTsv[j], rdtTsv[i]))
                        {
                            min_dist = distance(fncTsv[j], rdtTsv[i]);
                            idx1 = i;
                        }
                    }
                    for (int i = 0; i < Nfunc; i++)
                    {
                        if (i != j && done[i] && min_dist1 > distance(fncTsv[j], fncTsv[i]))
                        {
                            min_dist1 = distance(fncTsv[j], fncTsv[i]);
                            idx2 = i;
                        }
                    }
                    if (min_dist <= min_dist1)
                    {
                        groups[idx1].push_back(fncTsv[j]);
                        countfnc++;
                        done[j] = true;
                        parent[idx1][j] = rdtTsv[idx1];
                    }
                    else
                    {
                        for (int i = 0; i < Nred; i++)
                        {
                            auto it = find(groups[i].begin(), groups[i].end(), fncTsv[idx2]);
                            if (it != groups[i].end())
                            {
                                groups[i].push_back(fncTsv[j]);
                                done[j] = true;
                                parent[i][j] = fncTsv[idx2];
                                break;
                            }
                        }
                    }
                }
            }
        

        for (int i = 0; i < Nred; i++)
        {
            cout << "Group " << i + 1 << '\n';
            for (int j = 0; j < groups[i].size(); j++)
            {
                cout << '(' << groups[i][j].first << ',' << groups[i][j].second << ')' << ' ';
            }
            cout << '\n';
        }
        
        cout<<'\n';

        for (int i = 0; i < Nred; i++)
        {  
           for (int j = i+1; j < Nred; j++)
            { 
              vector<pair<double, double>> save;
               for(int k=0; k<groups[i].size(); k++){
                  for(int l=0; l<groups[j].size(); l++){
                       if(groups[i][k] == groups[j][l]){
                        save.push_back(groups[i][k]);
                       }
                  }
             }
            
           if(save.size()>0){
             cout << "Shared functional TSVs between groups " << i+1 << " and " << j+1 << '\n';
            for(auto it:save){
                cout << '(' << it.first << ',' << it.second << ')' << ' ';
            }
             cout<<'\n';
           }
         }
        
        }

       cout<<'\n';

        
        for (int faulty = 1; faulty <= Nred; faulty++)
        {
            int delay = 200;
            int cnt = 0;
            int num_of_mux=0, avg_num_of_mux=0;
            double total_wire_length=0, avg_wire_length=0;
            for (int round = 0; round < 100; round++)
            {
                set<int> faultyOnes = generateRandomNumbers(faulty, Nfunc, delay);
                int totFixed = 0;
                vector<bool> usedred(Nred, false);
                for (int idx : faultyOnes)
                {

                    bool fixed = false;
                    int cnt_of_rdt=0;
                    for (int i = 0; i < Nred; i++)
                    {
                        auto it = find(groups[i].begin(), groups[i].end(), fncTsv[idx]);

                        if(it != groups[i].end()){
                            cnt_of_rdt++; 
                        }

                        if (it != groups[i].end() && !usedred[i] && !fixed)
                        {   
                            usedred[i] = true;
                            fixed = true;

                            double wire_length=0;
                            int ind=idx;
                            while(true){
                                wire_length = max(wire_length, distance(fncTsv[ind], parent[i][ind]));
                                bool hmm=false;
                                for (int j = 0; j < Nfunc; j++){
                                    if(fncTsv[j]==parent[i][ind]){
                                         ind = j;
                                         hmm=true;
                                         break;
                                    }
                                }
                                if(!hmm){
                                    break;
                                }
                            }
                            total_wire_length += wire_length;
                           
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
            cout << "Averege minimum wire length required = " << avg_wire_length/100 << " unit" << '\n';
            cout << "Averege number of mux required = " << ceil(avg_num_of_mux/100) << '\n';
        }
        cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    
    return 0;
}
