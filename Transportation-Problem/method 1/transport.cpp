#include<iostream>
#include<math.h>
#include<fstream>
#include<algorithm>
#include<iomanip>
#include<vector>

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
        cout << "Unable to open file";
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

void Exchange()
{
    //Interchange odd position row
    for(int i=0; i+2<row; i+=4)
    {
        for(int j=0; j<column; j++)
        {
            swap(cost[i][j], cost[i+2][j]);         
        }
    }
    //also Interchange supply
    for(int i=0; i+2<row; i+=4)
    {
        swap(supply[i], supply[i+2]);
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

int findRowColumn(int num)
{
    int i, rowcolumn;
    for (i = 0; i < row + column; i++)
    {
        if (num == diferance[i][0])
        {
            diferance[i][0] = 0;
            rowcolumn = diferance[i][1];
            break;
        }
    }
    return rowcolumn;
}


int calculatemin(int rowColumn, int min, int r, int c)
{
    int i;
    if (rowColumn < 100)
    {
        min = abs(cost[rowColumn][0]);
        r = rowColumn;
        c = 0;
        for (i = 1; i < column; i++)
        {
            if (abs(min) > abs(cost[rowColumn][i]) && cost[rowColumn][i] != -999)
            {
                min = cost[rowColumn][i];
                c = i;
            }
        }
    }
    else
    {
        min = abs(cost[0][rowColumn - 100]);
        r = 0;
        c = rowColumn - 100;
        for (i = 1; i < row; i++)
        {
            if (abs(min) > abs(cost[i][rowColumn - 100]) && cost[i][rowColumn - 100] != -999)
            {
                min = cost[i][rowColumn - 100];
                r = i;
            }
        }
    }

    minX = min;
    rX = r;
    cX = c;
    return 0;
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

    Exchange();
    cout<<"After exchange the odd row."<<endl;
    printTable();

    vector<int>v;
    for(int k=0; k<(row+column-2); k++)
    {
        //Row Penalty
        for(int i=0; i<row; i++)
        {
            v.clear();
            for(int j=0; j<column; j++)
            {
                if(cost[i][j] != -999)
                    v.push_back(cost[i][j]);
                //value[j] = cost[i][j];
            }

            // cout<<"Row Penalty : "<<endl;
            // for(auto it : v)
            //     cout<<it<<" ";
            // cout<<endl;

            sort(v.begin(), v.end());

            if(v.size() > 1)
                diferance[i][0] = v[1] - v[0];
            else
                diferance[i][0] = -999;
            // cout<<" = "<<diferance[i][0]<<endl;
            
            diferance[i][1] = i;
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

            // cout<<"Column Pointer"<<endl;
            // for(auto it : v)
            //     cout<<it<<" ";       
            // cout<<endl;

            sort(v.begin(), v.end());
            
            if(v.size() > 1)
                diferance[row+i][0] = v[(int)v.size()-1] - v[(int)v.size()-2];
            else
                diferance[row+i][0] = -999;
            
            // cout<<" = "<<diferance[row+i][0]<<endl;

            diferance[row+i][1] = i+100;
        }
        v.clear();
        for(int i = 0; i < row + column; i++) 
            if(diferance[i][0] != -999)
                v.push_back(diferance[i][0]);

        sort(v.begin(), v.end());
        reverse(v.begin(), v.end());

        // cout<<"Choise max value : "<<endl;
        // for(auto it : v)
        //     cout<<it<<" ";
        // cout<<endl;
        

        // max1st = v[0];
        // max2nd = v[1];
        // max3rd = v[2];
        // cout<<max1st<<" "<<max2nd<<" "<<max3rd<<endl;
        min1 = min2 = min3 = 1000;

        rowColumn = findRowColumn(v[0]);
        calculatemin(rowColumn, min, r, c);
        min1 = minX;
        row1 = rX;
        col1 = cX;
        // cout<<" For max1 "<<endl;
        // cout<<minX<<" "<<rX<<" "<<cX<<endl;

        if(v.size()>1)
        {
            //cout<<"max2nd = "<<max2nd<<endl;
            rowColumn = findRowColumn(v[1]);
            calculatemin(rowColumn, min, r, c);
            min2 = minX;
            row2 = rX;
            col2 = cX;
            // cout<<" For max2 "<<endl;
            // cout<<minX<<" "<<rX<<" "<<cX<<endl;
        }

        if(v.size() > 2)
        {
            //cout<<"max3rd = "<<max3rd<<endl;
            rowColumn = findRowColumn(v[2]);
            calculatemin(rowColumn, min, r, c);
            min3 = minX;
            row3 = rX;
            col3 = cX;
            // cout<<" For max3 "<<endl;
            // cout<<minX<<" "<<rX<<" "<<cX<<endl;
        }

        //Allocation
        if (min1 <= min2 && min1 <= min3)
        {
            if (supply[row1] < demand[col1]) 
            {
                allocation[noOfAllocation][0] = supply[row1];
                allocation[noOfAllocation][1] = min1;
                demand[col1] = demand[col1] - supply[row1];
                supply[row1] = 0;
            }
            else
            {
                allocation[noOfAllocation][0] = demand[col1];
                allocation[noOfAllocation][1] = min1;
                supply[row1] = supply[row1] - demand[col1];
                demand[col1] = 0;
            }
            noOfAllocation++;
        }
        else if (min2 <= min1 && min2 <= min3)
        {
            if (supply[row2] < demand[col2])
            {
                allocation[noOfAllocation][0] = supply[row2];
                allocation[noOfAllocation][1] = min2;
                demand[col2] = demand[col2] - supply[row2];
                supply[row2] = 0;
            }
            else
            {
                allocation[noOfAllocation][0] = demand[col2];
                allocation[noOfAllocation][1] = min2;
                supply[row2] = supply[row2] - demand[col2];
                demand[col2] = 0;
            }
            noOfAllocation++;
        }
        else if (min3 <= min1 && min3 <= min1)
        {
            if (supply[row3] < demand[col3]) 
            {
                allocation[noOfAllocation][0] = supply[row3];
                allocation[noOfAllocation][1] = min3;
                demand[col3] = demand[col3] - supply[row3];
                supply[row3] = 0;
            }
            else
            {
                allocation[noOfAllocation][0] = demand[col3];
                allocation[noOfAllocation][1] = min3;
                supply[row3] = supply[row3] - demand[col3];
                demand[col3] = 0;
            }
            noOfAllocation++;
        }

        
        //forming new matrix;  
        int tcost_supply, tcost_demand, row_supply, col_demand;
        tcost_supply = 0; 
        row_supply = -999;
        col_demand = -999;
        for (i = 0; i < row; i++)
        {
            if (supply[i] == 0)
            {
                row_supply = i;
                for (j = 0; j < column; j++)
                {
                    if (cost[i][j] != -999)
                        tcost_supply += cost[i][j];
                }
                break;
            }
        }
        tcost_demand = 0; 
        for (i = 0; i < column; i++)
        {
            if (demand[i] == 0)
            {
                col_demand = i;
                for (j = 0; j < row; j++)
                {
                    if (cost[j][i] != -999)
                        tcost_demand += cost[j][i];
                }
                break;
            }
        }
        if (row_supply != -999 && col_demand != -999)
        {
            demand_check[col_demand] = 0;
            supply_check[row_supply] = 0;
            if (tcost_demand > tcost_supply)

            {
                for (i = 0; i < row; i++)
                {
                    cost[i][col_demand] = -999;
                }
                demand[col_demand] = -999;
            }
            else
            {
                for (i = 0; i < column; i++)
                {
                    cost[row_supply][i] = -999;
                }
                supply[row_supply] = -999;
            }
        }
        else if (row_supply != -999)
        {
            for (i = 0; i < column; i++)
            {
                cost[row_supply][i] = -999;
            }
            supply[row_supply] = -999;
            supply_check[row_supply] = 0;
        }
        else if (col_demand != -999)
        {
            for (i = 0; i < row; i++)
            {
                cost[i][col_demand] = -999;
            }
            demand[col_demand] = -999;
            demand_check[col_demand] = 0;
        }
        string th;
        if(k==0)
            th = "1st";
        else if(k==1)
            th = "2nd";
        else if(k==2)
            th = "3rd";
        else
            th = to_string(k+1)+"th";
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
