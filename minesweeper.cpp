/********************************************
CPEG 476/676
Project3_Command-Line Minesweeper

Group5: Ye Wang, Hancheng Zhao, Yan Zheng
*********************************************/

#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <string>

using namespace std;

const string GREEN("\033[0;32m");
const string RESET("\033[0m");

void updateMap(vector<int>& nums, vector<char>& cover, vector<int>& waiter, int& wait, int pos);
void printVectorInt(vector<int>& v, int length, int size);
void printVectorChar(vector<char>& v, int length, int size);
void printAllMaps(vector<int>& waiter,vector<int>& nums,vector<int>& mines,vector<char>& chars,vector<char>& cover, int length, int size);
void generateMines(vector<int>& nums,vector<int>& mines,vector<char>& chars,vector<char>& cover,vector<int>& waiter, int total_mines,int length,int size);

int main()
{
    int wait=0, theone=0, extra1=0, extra2=0;
    char char1=0, inputCol=0;
    int inputRow=-1, num=1, x=0, y=0;
    int total_mines, length, width;
    
    cout<<"\nSelect length (1-26) of the map: ";
    cin>>length;
    while(cin.fail() || length <1 || length>26){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"\nInvalid input!! \nSelect length (1-26) of the map: ";
        cin>>length;
    }
    cin.clear();
    
    cout<<"\nSelect width of the map: ";
    cin>>width;
    while(cin.fail() || width<1){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"\nInvalid input!! \nSelect width of the map: ";
        cin>>width;
    }
    cin.clear();
    
    cout<<"\nSelect total number of the mines (at least 1): ";
    cin>>total_mines;
    while(cin.fail() || total_mines<1){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        cout<<"\nInvalid input!! \nSelect total number of the mines (at least 1): ";
        cin>>total_mines;
    }
    cin.clear();
    
    
    int size = length * width;
    
    if(total_mines>=size){
        total_mines=size-1;// spare at least one blank so the first click is not mine
    }

    srand (time(NULL));

    vector<char>cover;//cover for the mines, set as "+" here
    vector<int>nums;
    vector<int>waiter;
    vector<int>mines;//indicate the place of mines
    vector<char>chars;//display the character on that place

    cout<<"\n";
    
    generateMines(nums,mines,chars,cover,waiter,total_mines,length,size);

    bool firstcheck=true;
    chrono::time_point<chrono::system_clock> start, end;
    start = chrono::system_clock::now();

    while(1)
    {   
        
        system("clear");//clear console screen
        // printAllMaps(waiter,nums,mines,chars,cover,length,size);
        cout<<"\n\nMines left: "<<total_mines<<endl<<endl;
        string header = "    ";
        for(int i=0;i<length;i++){
            char tmp = (char) ('a'+i);
            header = header + " " + tmp;
        }//set length indicator as "a, b, c, d..."
        cout<<header<<"\n";
        string line = "     ";
        for(int i=0;i<2*length-1;i++){
            line = line + "-";
        }//set up border lines
        cout<<line<<"\n";
        cout.width(5); cout<<right<<"0| ";
        num=1;
        for(int i=0;i<size;i++)
        {
            if( theone == i+1){
                cout<<GREEN<<chars[i+1]<<RESET<<" ";//set the last step as green
            }else{
                if(cover[i+1]=='+')
                    cout<<cover[i+1]<<" ";
                else
                    cout<<chars[i+1]<<" ";
            }
            if(i%length==length-1 && num!=width){
                    cout<<"|"<<num-1<<"\n";
                    cout.width(3);cout<<num<<"| ";
                    num++;
                }
        }
        cout<<"|"<<width-1<<"\n"<<line<<endl<<header;
        
        
        cin.ignore();
        cout<<"\nChoose a column: ";
        string tmp_col;
        getline(cin,tmp_col);
        while(tmp_col.length() !=1 || tmp_col[0]<'a' || tmp_col[0]>(char) 'a'+length){
            // cin.clear();
            cout<<"\n Invalid input!!\nChoose a column: ";
            getline(cin, tmp_col);
        }
        inputCol = tmp_col[0];
        
        cout<<"\nChoose a row:";
        cin>>inputRow;
        while(cin.fail() || inputRow>=width || inputRow<0){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout<<"\nInvalid input!! \nChoose a row: ";
            cin>>inputRow;
        }
        
        
        wait=1;
        extra1 = (int) (inputCol - 'a'+1);// column index
        extra2 = inputRow;// row index

 
        theone=extra1+length*extra2; // the chosen positon
        
        
        if(firstcheck){
            firstcheck=false;
            while(chars[theone]=='M'){
                generateMines(nums,mines,chars,cover,waiter,total_mines,length,size);
            }
        }
        else if(chars[theone]=='M'){
            cout<<"Game over!"<<endl;
            break;
        }
        cover[theone]='-';
        
        waiter[wait]=theone;
        
        while(wait!=0){
            if(chars[theone]=='0'){
                //top left
                if(theone==1){
                    updateMap(nums,cover,waiter,wait,2);
                    updateMap(nums,cover,waiter,wait,length+1);
                    updateMap(nums,cover,waiter,wait,length+2);
                }
                //top right
                if(theone==length && cover[length]=='+') {
                    updateMap(nums,cover,waiter,wait,length-1);
                    updateMap(nums,cover,waiter,wait,2*length-1);
                    updateMap(nums,cover,waiter,wait,2*length);
                }
                //bottom left
                if(theone==size-length+1 && cover[size-length+1]=='+') {
                    updateMap(nums,cover,waiter,wait,size-2*length+1);
                    updateMap(nums,cover,waiter,wait,size-2*length+2);
                    updateMap(nums,cover,waiter,wait,size-length+2);
                }
                //bottom right
                if(theone==size && cover[size]=='+') {
                    updateMap(nums,cover,waiter,wait,size-length-1);
                    updateMap(nums,cover,waiter,wait,size-length);
                    updateMap(nums,cover,waiter,wait,size-1);
                }
                //top
                for(int i=2;i<length;i++){
                    if(theone==i){
                        updateMap(nums,cover,waiter,wait,i-1);
                        updateMap(nums,cover,waiter,wait,i+1);
                        updateMap(nums,cover,waiter,wait,i+length-1); 
                        updateMap(nums,cover,waiter,wait,i+length);
                        updateMap(nums,cover,waiter,wait,i+length+1);
                    }
                }
                //bottom
                for(int i=size-length+2;i<size;i++){
                    if(theone==i){
                        updateMap(nums,cover,waiter,wait,i-1);
                        updateMap(nums,cover,waiter,wait,i+1);
                        updateMap(nums,cover,waiter,wait,i+length-1); 
                        updateMap(nums,cover,waiter,wait,i+length);
                        updateMap(nums,cover,waiter,wait,i+length+1);
                    }
                }
                //left
                for(int i=length;i<size-2*length+2;i++){
                    if(i%length==1 && theone==i){
                        updateMap(nums,cover,waiter,wait,i-length);
                        updateMap(nums,cover,waiter,wait,i-length+1);
                        updateMap(nums,cover,waiter,wait,i+1); 
                        updateMap(nums,cover,waiter,wait,i+length);
                        updateMap(nums,cover,waiter,wait,i+length+1);
                    }
                }
                //right
                for(int i=2*(length-1);i<size-length+1;i++){
                    if(i%length==0 && theone==i){
                        updateMap(nums,cover,waiter,wait,i-length-1);
                        updateMap(nums,cover,waiter,wait,i-length);
                        updateMap(nums,cover,waiter,wait,i-1); 
                        updateMap(nums,cover,waiter,wait,i+length+1);
                        updateMap(nums,cover,waiter,wait,i+length);
                    }
                }
                //all else
                for(int i=length+2;i<size-length;i++){
                    if(i%length!=0 && i%length!=1 && theone==i){
                        updateMap(nums,cover,waiter,wait,i-length-1);
                        updateMap(nums,cover,waiter,wait,i-length);
                        updateMap(nums,cover,waiter,wait,i-length+1);
                        updateMap(nums,cover,waiter,wait,i-1);
                        updateMap(nums,cover,waiter,wait,i+1);
                        updateMap(nums,cover,waiter,wait,i+length-1);
                        updateMap(nums,cover,waiter,wait,i+length);
                        updateMap(nums,cover,waiter,wait,i+length+1);
                    }
                }
            }
            theone=waiter[wait];
            wait--;
        }
        // check win
        int count=0;
        for(int i=1;i<=size;i++){
            if((cover[i]=='+'&&chars[i]=='M') || (cover[i]=='-'&&chars[i]!='M')){
                count++;
            }else{
                break;
            }
        }

        if(count==size){
            cout<<"You won!"<<endl;
            break;
        }
        
    }
    cout<<endl;
    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = chrono::system_clock::to_time_t(end);
 
    cout << "You used: " << elapsed_seconds.count() << "s\n\n";
    
    
    printVectorChar(chars,length,size);
    
}

void updateMap(vector<int>& nums, vector<char>& cover, vector<int>& waiter, int& wait, int pos){
    if(nums[pos]==0 && cover[pos]=='+'){
        wait++;
        waiter[wait]=pos;
    }
    cover[pos]='-';
}

void printAllMaps(vector<int>& waiter,vector<int>& nums,vector<int>& mines,vector<char>& chars,vector<char>& cover, int length, int size){
    cout<<"waiter:"<<endl;
    printVectorInt(waiter,length,size);
    cout<<"nums:"<<endl;
    printVectorInt(nums,length,size);
    cout<<"mines:"<<endl;
    printVectorInt(mines,length,size);
    cout<<"chars:"<<endl;
    printVectorChar(chars,length,size);
    cout<<"cover:"<<endl;
    printVectorChar(cover,length,size);
}


void printVectorInt(vector<int>& nums, int length, int size){
    for (int i=1; i<size+1; i++){
        cout<<nums[i]<<" ";
        if(i%length == 0){
            cout<<endl;
        }
    }
    cout<<endl<<endl;
}

void printVectorChar(vector<char>& cs, int length, int size){
    for (int i=1; i<size+1; i++){
        cout<<cs[i]<<" ";
        if(i%length == 0){
            cout<<endl;
        }
    }
    cout<<endl<<endl;
}

void generateMines(vector<int>& nums, vector<int>& mines, vector<char>& chars, vector<char>& cover, vector<int>& waiter,
                    int total_mines, int length, int size){
    
    waiter.clear();
    nums.clear();
    mines.clear();
    chars.clear();
    cover.clear();
    for(int i=0;i<size+1;i++)
    {
        waiter.push_back(0);
        nums.push_back(0);
        mines.push_back(0);
        chars.push_back('0');
        cover.push_back('+');
    }
    
    
    for(int i=0;i<total_mines;i++){
        int x=rand();
        int y=x%size+1;
        
        if(mines[y]==0){
            mines[y]=1;
            // chars[y]='M';//set mines
        }else{//if the mine is set, find another place
            i--;
        }
    }
    
    for(int i=0;i<size+1;i++){
        if(mines[i]!=0)
            chars[i]='M';
    }
   
    
    //top left
    if(chars[1]!='M')   
    {
        if(chars[2]=='M')
            nums[1]++;
        if(chars[length+1]=='M') //27
            nums[1]++;
        if(chars[length+2]=='M') //28
            nums[1]++;
    }
    
    //top right
    if(chars[length]!='M'){
        if(chars[length-1]=='M')
            nums[length]++;
        if(chars[2*length-1]=='M')
            nums[length]++;
        if(chars[2*length]=='M')
            nums[length]++;
    }
    //bottom left
    if(chars[size-length+1]!='M'){ 
        if(chars[size-2*length+1]=='M')
            nums[size-length+1]++;
        if(chars[size-2*length+2]=='M')
            nums[size-length+1]++;
        if(chars[size-length+2]=='M')
            nums[size-length+1]++;
    }
    //bottom right
    if(chars[size]!='M') {
        if(chars[size-length-1]=='M')
            nums[size]++;
        if(chars[size-length]=='M')
            nums[size]++;
        if(chars[size-1]=='M')
            nums[size]++;
    }
    
    //top
    for(int i=2;i<length;i++){
        if(chars[i]!='M'){
            if(chars[i-1]=='M')
                nums[i]++;
            if(chars[i+1]=='M')
                nums[i]++;
            if(chars[i+length-1]=='M')
                nums[i]++;
            if(chars[i+length]=='M')
                nums[i]++;
            if(chars[i+length+1]=='M')
                nums[i]++;
        }
    }
    //bottom
    for(int i=size-length+2;i<size;i++)   {
        if(chars[i]!='M'){
            if(chars[i-1]=='M')
                nums[i]++;
            if(chars[i+1]=='M')
                nums[i]++;
            if(chars[i-length+1]=='M')
                nums[i]++;
            if(chars[i-length]=='M')
                nums[i]++;
            if(chars[i-length-1]=='M')
                nums[i]++;
        }
    }
    //left
    for(int i=length;i< size-2*length+2;i++){
        if(i%length==1&&chars[i]!='M'){
            if(chars[i-length]=='M')
                nums[i]++;
            if(chars[i-length+1]=='M')
                nums[i]++;
            if(chars[i+1]=='M')
                nums[i]++;
            if(chars[i+length]=='M')
                nums[i]++;
            if(chars[i+length+1]=='M')
                nums[i]++;
        }
    }
    //right
    for(int i=2*(length-1);i<size-length+1;i++)   
    {
        if(i%length==0 && chars[i]!='M')
        {
            if(chars[i-length-1]=='M')
                nums[i]++;
            if(chars[i-length]=='M')
                nums[i]++;
            if(chars[i-1]=='M')
                nums[i]++;
            if(chars[i+length-1]=='M')
                nums[i]++;
            if(chars[i+length]=='M')
                nums[i]++;
        }
    }
    //all else
    for(int i=length+2;i<size-length;i++)
    {
        if(i%length!=0 && i%length!=1 && chars[i]!='M')
        {
            if(chars[i-length-1]=='M')
                nums[i]++;
            if(chars[i-length]=='M')
                nums[i]++;
            if(chars[i-length+1]=='M')
                nums[i]++;
            if(chars[i-1]=='M')
                nums[i]++;
            if(chars[i+1]=='M')
                nums[i]++;
            if(chars[i+length-1]=='M')
                nums[i]++;
            if(chars[i+length]=='M')
                nums[i]++;
            if(chars[i+length+1]=='M')
                nums[i]++;
        }
    }
    
    for(int i=0;i<size+1;i++){
        if(chars[i] != 'M')
            chars[i] = nums[i]+'0';
    }
}