#include<iostream>
#include<math.h>
#include<fstream>
#include<algorithm>
#include<iomanip>
#include<vector>
#include<string>

using namespace std;

int row, column;
int supply[100], demand[100];
int cost[100][100];

int value[100], diferance[100][2];
int min1st, min2nd, min3rd, max1st, max2nd, max3rd;
int min1, row1, col1, min2, row2, col2, min3, row3, col3;
int minX, rX, cX, allocation[200][200], noOfAllocation;
int supply_check[100], demand_check[100];

//Funtion for data Read  
void dataRead()
{
    ifstream inFile;
    inFile.open("input.txt");
    if (!inFile) 
    {
        cout << "Unable to open file" <<endl;
        exit(1); 
    }
    inFile >> row;
    inFile >> column;
    //Read Cost data 
    for(int i=0; i<row; i++) 
    {
        for(int j=0; j<column; j++) 
        {
            inFile >> cost[i][j];
        }
    }
    //Read supply data 
    for(int i=0; i<row; i++) 
    {
        inFile >> supply[i];
    }
    //Read demand data
    for(int i=0; i<column; i++) 
    {
        inFile >> demand[i];
    }
}


void printTable()
{
    cout<<endl;
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<column; j++)
        {
            if(cost[i][j] != -999)
                cout<<setw(5)<<cost[i][j]<<"\t";
        }
        if(supply[i] != -999)
            cout<<supply[i]<<endl;
    }
    for(int i=0; i<column; i++)
        if(demand[i]!= -999)
            cout<<setw(5)<<demand[i]<<"\t";
    cout<<endl<<endl;
}



int calculateTotalCost()
{
    int i, j;
    long int total = 0;
    cout<<"Minimum Total cost : "<<endl;
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < column; j++)
        {
            if (cost[i][j] != -999)
            {
                cout<<cost[i][j]<<"x"<<supply[i]<<" + ";
                total = total + cost[i][j] * supply[i];
            }
        }
    }

    for (i = 0; i < noOfAllocation; i++)
    {
        cout<<allocation[i][0]<<"x"<<allocation[i][1];
        if(i < noOfAllocation-1)
            cout<<" + ";
        else
            cout<<" = ";
        total += allocation[i][0] * allocation[i][1];
    }
    cout<<total<<endl;
    return 0;
}

void operation()
{
    int rowColumn, i, j, min, r, c;
    cout<<"The Initail table : "<<endl; 
    printTable();

    int totalSupply = 0;
    int totalDemand = 0;

    //Calculate total Supply
    for(int i=0; i<row; i++)
    {
        totalSupply += supply[i];
    }

    //Calculate total Demand
    for(int i=0; i<column; i++)
    {
        totalDemand += demand[i];
    }
    
    //Check It is balanced or not 
    if(totalSupply != totalDemand)
    {
        cout<<"Table is unbalanced"<<endl;
        if(totalSupply < totalDemand)
        {
            int extra = totalDemand - totalSupply;
            supply[row] = extra;
            for(int i=0; i<column; i++)
            {
                cost[row][i] = 0;
            }
            row++;
        }
        else
        {
            int extra = totalSupply - totalDemand;
            demand[column] = extra;
            for(int i=0; i<row; i++)
            {
                cost[i][column] = 0;
            }
            column++;
        }
        cout<<"After make is balanced."<<endl;
        printTable();
    }
    else
    {
        cout<<"It is a balanced table."<<endl;
    }


    vector<int>v;
    for(int k=0; k<(row+column-2); k++)
    {
        max1st = max2nd = -1111;
        int sp = -1111, dm = -1111;
        
        //Row Pointer
        for(int i=0; i<row; i++)
        {
            v.clear();
            for(int j=0; j<column; j++)
            {
                if(cost[i][j] != -999)
                    v.push_back(cost[i][j]);
            }

            // cout<<"Row pointer : "<<endl;
            // for(auto it : v)
            //     cout<<it<<" ";
            // cout<<endl;

            sort(v.begin(), v.end());
            
            if(v.size()>0)
            {
                int mm;
                if(v.size()>1)
                    mm =  v[(int)(v.size())-1] - v[0];
                else
                    mm = v[0];
                
                //cout<<" = "<<mm<<endl;
                if(mm > max1st)
                {
                    max1st = mm;
                    sp = supply[i];
                    row1 = i;
                }
                else if(mm == max1st)
                {
                    if(sp < supply[i])
                    {
                        max1st = mm;
                        sp = supply[i];
                        row1 = i;
                    }
                }
            }
        }

        //Column Pointer
        for(int i=0; i<column; i++)
        {
            v.clear();
            for(int j=0; j<row; j++)
            {
                if(cost[j][i] != -999)
                    v.push_back(cost[j][i]);
            }

            // cout<<"Column pointer : "<<endl;
            // for(auto it : v)
            //     cout<<it<<" ";
            // cout<<endl;

            sort(v.begin(), v.end());

            if(v.size() >= 1)
            {
                int mm;
                if(v.size()>1)
                    mm =  v[(int)(v.size())-1] - v[0];
                else
                    mm = v[0];

                // cout<<" = "<<mm<<endl;
                if(mm > max2nd)
                {
                    max2nd = mm;
                    dm = demand[i];
                    col1 = i;
                }
                else if(mm == max2nd)
                {
                    if(dm < demand[i])
                    {
                        max2nd = mm;
                        dm = demand[i];
                        col1 = i;
                    }
                }
            }
        }

        // cout<<" row1 = "<<row1+1<<", col1 = "<<col1+1<<endl;

        
        if(supply[row1] < demand[col1]) 
        {
            // cout<<"Row = "<<row1+1<<endl;
            allocation[noOfAllocation][0] = supply[row1];
            allocation[noOfAllocation][1] = cost[row1][col1];
            // cout<<"add = "<< supply[row1] <<" X "<< cost[row1][col1] <<endl;
            demand[col1] = demand[col1] - supply[row1];
            supply[row1] = -999;
            for (i = 0; i < column; i++)
            {
                cost[row1][i] = -999;
            }
        }
        else
        {
            // cout<<" Column = "<<col1+1<<endl;
            allocation[noOfAllocation][0] = demand[col1];
            allocation[noOfAllocation][1] = cost[row1][col1];
            // cout<<"add = "<<  demand[col1] <<" X "<< cost[row1][col1] <<endl;
            supply[row1] = supply[row1] - demand[col1];
            demand[col1] = -999;
            for (i = 0; i < row; i++)
            {
                cost[i][col1] = -999;
            }
                
        }
        noOfAllocation++;

        string th;
        if(k==0)
            th = "1st";
        else if(k==1)
            th = "2nd";
        else if(k==2)
            th = "3rd";
        else
        {
            stringstream ss;
            ss << (k+1);
            th = ss.str();
            th += "th";
        }
        cout<<"After "<<th<<" allocation the table is : "<<endl;
        printTable();
    }
    calculateTotalCost();
}
int main()
{
    dataRead();
    operation();
}
