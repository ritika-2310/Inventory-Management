#include<iostream>
#include<cctype>
#include<vector>
#include<cstdlib>
#include<cstring>
#include"cJSON.h"
// ANSI color codes
#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"

using namespace std;
//structure
struct item{
    int id;
    string name;
    int quantity;
    float price;
};
//function declaration and defination
void menu(){
    cout<<MAGENTA<<"\n======================="<<RESET;
    cout<<YELLOW<<" Welcome!!!! "<<RESET;
    cout<<MAGENTA<<"===========================\n"<<RESET;
    cout<<MAGENTA<<"================="<<RESET;
    cout<<YELLOW<<"Inventory Management System"<<RESET;
    cout<<MAGENTA<<"=================\n"<<RESET;
    cout<<YELLOW;
    cout<<"1. Add New Item\n";
    cout<<"2. View All Item\n";
    cout<<"3. Search Item\n";
    cout<<"4. Update Item\n";
    cout<<"5. Display Out Of Stocks Item\n";
    cout<<"6. Exit\n";
    cout<<RESET;
}
cJSON* read(){
    FILE *fptr=fopen("product.json","r");
    if(fptr==NULL){
        cout<<RED<<"No items to display. File not found\n"<<RESET;
        return NULL;
    }
    fseek(fptr,0,SEEK_END);
    long len=ftell(fptr);
    rewind(fptr);

    if (len == 0) {
        fclose(fptr);
        cout<<RED<<"File is empty\n"<<RESET;
        return NULL; // Empty file
    }

    char *data = (char*)malloc(len+1);
    fread(data,1,len,fptr);
    data[len]='\0';
    fclose(fptr);
    //take the data which is raw data into root to parse it in json format object
    cJSON *root = cJSON_Parse(data);
    free(data);
    if (!root) {
        cout<<RED<<"Error parsing JSON.\n"<<RESET;
        return NULL;
    }
    return root;
}
int generateId() {
    cJSON *root = read();
    int maxId = 0;

    if (root && cJSON_IsArray(root)) {
        cJSON *item = NULL;
        cJSON_ArrayForEach(item, root) {
            cJSON *id = cJSON_GetObjectItem(item, "id");
            if (id && cJSON_IsNumber(id)) {
                if (id->valueint > maxId) {
                    maxId = id->valueint;
                }
            }
        }
    }

    cJSON_Delete(root);
    return maxId + 1;
}
void saveToFile(item newitem){
    cJSON *root = read();
    //check if file was empty or corrupted so if the data wasnt added before to root like root is NULL so add create new array so that atleast if the file is empty so atleast the array should be created so to add things into that array
    if(root==NULL){
        root = cJSON_CreateArray();
    }
    //create new product in the json file like appending to the already left data in the file and if theres no data in the file already like root is NULL then create array in that file so to append data after previous datas
    cJSON *product = cJSON_CreateObject();
    cJSON_AddNumberToObject(product,"id",newitem.id);
    cJSON_AddStringToObject(product,"name",newitem.name.c_str()); //since i didnt used constant char so i had to put .c_str() so to access C++ style string
    cJSON_AddNumberToObject(product,"price",newitem.price);
    cJSON_AddNumberToObject(product,"quantity",newitem.quantity);
    //add product just now made into the array 
    cJSON_AddItemToArray(root,product); //root ke andar bani hui woh array
    //write this modified data into the file again cause we cant jsut appedn things like this into file when its json we need to take whole data then append in it
    char *string = cJSON_Print(root); //in form of string this data is written into the json file to make it readable
    FILE *fptr=fopen("product.json","w");
    if(fptr!=NULL){
        fputs(string,fptr); //if there is some value in pointer then outting value in fptr so that file has it using fputs
        fclose(fptr);
    }
    //cleanup 
    cJSON_Delete(root);
    free(string);
}
//add item function
void additem(vector<item> &items){ //take value by reference & cause we need to change the actual array
    item newitem; //datatype item is structure we made with new object newitem just for the sake of appending it into the vector dynamic array
    newitem.id = generateId();
    cout<<"enter name of the product : ";
    cin.ignore(); // Clears newline left in buffer
    getline(cin, newitem.name); // Allows space in name
    cout<<"enter quantity of the product : ";
    cin>>newitem.quantity;
    cout<<"enter price of the product : ";
    cin>>newitem.price;
    items.push_back(newitem);
    saveToFile(newitem);
    cout<<GREEN<<"items added successfully\n"<<RESET;
    cout<<MAGENTA<<"-----------------------------\n"<<RESET;
}
//display function
void display(){
    cJSON *root = read();
    if(root==NULL || !cJSON_IsArray(root)){
        cout<<RED<<"Error reading product list or file is empty.\n"<<RESET;
        cJSON_Delete(root);
        return;
    }
    cout<<BLUE<<"\n========= All Inventory Items =========\n"<<RESET;
    int index=0;
    cJSON* item = NULL;
    bool found=false;
    cJSON_ArrayForEach(item,root){
        cJSON *id = cJSON_GetObjectItem(item,"id");
        cJSON *name = cJSON_GetObjectItem(item,"name");
        cJSON *quantity = cJSON_GetObjectItem(item,"quantity");
        cJSON *price = cJSON_GetObjectItem(item,"price");
        found=true;
        //listing all items 
        cout<<BLUE<<"Item#"<<++index<<RESET<<endl;
        cout<<MAGENTA<<"===========================\n"<<RESET;
        /*
        cout<<"ID : "<<id->valueint<<endl;
        cout<<"NAME : "<<name->valuestring<<endl;
        cout<<"QUANTITY : "<<quantity->valueint<<endl;
        cout<<"PRICE : "<<price->valueint<<endl;
        */
       cout<<CYAN;
       cout << "ID : " << (id ? id->valueint : -1) << endl;
       cout << "NAME : " << (name ? name->valuestring : "Unknown") << endl;
       cout << "QUANTITY : " << (quantity ? quantity->valueint : 0) << endl;
       cout << "PRICE : " << (price ? price->valueint : 0) << endl;
       cout<<RESET;
       //safer checks
    }
    if(!found){
        cout<<RED<<"Items not found!\n"<<RESET;
        return;
    }
    cout<<GREEN<<"All Items displayed\n"<<RESET;
    cout<<MAGENTA<<"-----------------------------\n"<<RESET;
    cJSON_Delete(root);
}
//search function
void menu_search(){
    cout<<YELLOW;
    cout<<"1. search by Id\n";
    cout<<"2. search by Name\n";
    cout<<RESET;
    cout<<MAGENTA<<"-----------------------------\n"<<RESET;
}
bool searchById(cJSON *root){
    int searchId;
    cout<<"enter search Id : ";
    cin>>searchId;
    cJSON *item = NULL;
    cJSON_ArrayForEach(item,root){
        cJSON* id = cJSON_GetObjectItem(item,"id");
        if(id && id->valueint == searchId){
            cJSON* name = cJSON_GetObjectItem(item,"name");
            cJSON* quantity = cJSON_GetObjectItem(item,"quantity");
            cJSON* price = cJSON_GetObjectItem(item,"price");
            cout<<GREEN<<" Item found : \n"<<RESET;
            cout<<CYAN;
            cout<<"ID : "<<id->valueint<<endl;
            cout<<"NAME : "<<name->valuestring<<endl;
            cout<<"QUANTITY : "<<quantity->valueint<<endl;
            cout<<"PRICE : "<<price->valuedouble<<endl;
            cout<<RESET;
            cout<<MAGENTA<<"-----------------------------\n"<<RESET;
            cJSON_Delete(root);
            return true;
        }
    }
    cout<<MAGENTA<<"-----------------------------\n"<<RESET;
    return false;
}
bool searchByName(cJSON *root){
    string searchName;
    cout<<"enter search Name : ";
    cin>>searchName;
    cJSON *item = NULL;
    cJSON_ArrayForEach(item,root){
        cJSON* name = cJSON_GetObjectItem(item,"name");
        if(name && searchName == name->valuestring){
            cJSON* id = cJSON_GetObjectItem(item,"id");
            cJSON* quantity = cJSON_GetObjectItem(item,"quantity");
            cJSON* price = cJSON_GetObjectItem(item,"price");
            cout<<GREEN<<" Item found : \n"<<RESET;
            cout<<CYAN;
            cout<<"ID : "<<id->valueint<<endl;
            cout<<"NAME : "<<name->valuestring<<endl;
            cout<<"QUANTITY : "<<quantity->valueint<<endl;
            cout<<"PRICE : "<<price->valuedouble<<endl;
            cout<<RESET;
            cout<<MAGENTA<<"-----------------------------\n"<<RESET;
            cJSON_Delete(root);
            return true;
        }
    }
    cout<<MAGENTA<<"-----------------------------\n"<<RESET;
    return false;
}
void search(){
    bool found=false;
    cJSON *root = read();
    if(root==NULL || !cJSON_IsArray(root)){
        cout<<RED<<"Error reading product list or file is empty.\n"<<RESET;
        cJSON_Delete(root);
        return;
    }
    menu_search();
    int ch;
    cout<<"enter choice : ";
    cin>>ch;
    if(ch==1){
        found = searchById(root);
    }
    else if(ch==2){
        found = searchByName(root);
    }
    if(!found){
        cout<<RED<<"Item not found\n"<<RESET;
        cout<<MAGENTA<<"-----------------------------\n"<<RESET;
    }
    cJSON_Delete(root);
}
//update function
void menu_update(){
    cout<<YELLOW;
    cout<<"1. update by Id\n";
    cout<<"2. update by Name\n";
    cout<<RESET;
    cout<<MAGENTA<<"-----------------------------\n"<<RESET;
}
bool updateById(cJSON *root){
    int updateId;
    cout<<"enter update Id : ";
    cin>>updateId;
    cJSON *item = NULL;
    string newName;
    int newQuantity;
    float newPrice;
    cin.ignore(); // Flush input buffer
    cout << "Enter new name: ";
    getline(cin, newName);
    cout << "Enter new quantity: ";
    cin >> newQuantity;
    cout << "Enter new price: ";
    cin >> newPrice;
    cJSON_ArrayForEach(item,root){
        cJSON* id = cJSON_GetObjectItem(item,"id");
        if(id && id->valueint == updateId){
            cJSON_ReplaceItemInObjectCaseSensitive(item,"name",cJSON_CreateString(newName.c_str()));
            /* The .c_str() function in C++ is used to convert a std::string
into a C-style string (const char*), which is required by many legacy C functions (like strcmp, printf, cJSON functions, etc.).*/
            cJSON_ReplaceItemInObjectCaseSensitive(item,"quantity",cJSON_CreateNumber(newQuantity));
            cJSON_ReplaceItemInObjectCaseSensitive(item,"price",cJSON_CreateNumber(newPrice));
            break;
        }
    }
    char *updatedData = cJSON_Print(root);
    FILE *fptr;
    fptr = fopen("product.json","w");
    if(fptr){
        fputs(updatedData,fptr);
        fclose(fptr);
        cout<<GREEN<<"Item updated successfully\n"<<RESET<<endl;
        cout<<MAGENTA<<"-----------------------------\n"<<RESET;
        cJSON_Delete(root);
        free(updatedData);
        return true;
    }
    else{
        cout<<RED<<"Item not updated successfully\n"<<RESET<<endl;
        cout<<MAGENTA<<"-----------------------------\n"<<RESET;
        cJSON_Delete(root);
        free(updatedData);
        return false;
    }
}
bool updateByName(cJSON *root){
    string updateName;
    cout<<"enter update Name : ";
    cin>>updateName;
    cJSON *item = NULL;
    int newQuantity;
    float newPrice;
    cout << "Enter new quantity: ";
    cin >> newQuantity;
    cout << "Enter new price: ";
    cin >> newPrice;
    cJSON_ArrayForEach(item,root){
        cJSON* name = cJSON_GetObjectItem(item,"name");
        if(name && updateName == name->valuestring){
            cJSON_ReplaceItemInObjectCaseSensitive(item,"quantity",cJSON_CreateNumber(newQuantity));
            cJSON_ReplaceItemInObjectCaseSensitive(item,"price",cJSON_CreateNumber(newPrice));
            break;
        }
    }
    char *updatedData = cJSON_Print(root);
    FILE *fptr;
    fptr = fopen("product.json","w");
    if(fptr){
        fputs(updatedData,fptr);
        fclose(fptr);
        cout<<GREEN<<"Item updated successfully\n"<<RESET<<endl;
        cout<<MAGENTA<<"-----------------------------\n"<<RESET;
        cJSON_Delete(root);
        free(updatedData);
        return true;
    }
    else{
        cout<<RED<<"Item not updated successfully\n"<<RESET<<endl;
        cout<<MAGENTA<<"-----------------------------\n"<<RESET;
        cJSON_Delete(root);
        free(updatedData);
        return false;
    }
}
void update(){
    cJSON *root = read();
    if(root==NULL || !cJSON_IsArray(root)){
        cout<<RED<<"Error reading product list or file is empty.\n"<<RESET;
        cJSON_Delete(root);
        return;
    }
    bool found=false;
    menu_update();
    int ch;
    cout<<"enter choice : ";
    cin>>ch;
    if(ch==1){
        found=updateById(root);
    }
    else if(ch==2){
        found=updateByName(root);
    }
    if(!found){
        cout<<RED<<"Item not found\n"<<RESET;
        cout<<MAGENTA<<"-----------------------------\n"<<RESET;
    }
    cJSON_Delete(root);
}
//display of items out of stock
void display_outofstock(){
    cJSON *root=read();
    if(root==NULL || !cJSON_IsArray(root)){
        cout<<RED<<"Error reading product list or file is empty.\n"<<RESET;
        cJSON_Delete(root);
        return;
    }
    cJSON *item=NULL;
    bool found=false;
    cout<<BLUE<<"=== Out of Stock Items ===\n"<<RESET;
    cJSON_ArrayForEach(item,root){
        cJSON *qty=cJSON_GetObjectItemCaseSensitive(item,"quantity");
        if(qty && qty->valueint == 0){
            found=true;
            cJSON *id=cJSON_GetObjectItemCaseSensitive(item,"id");
            cJSON *name = cJSON_GetObjectItem(item,"name");
            cJSON *price = cJSON_GetObjectItem(item,"price");
            cout<<CYAN;
            cout << "ID : " << (id ? id->valueint : -1) << endl;
            cout << "NAME : " << (name ? name->valuestring : "Unknown") << endl;
            cout << "PRICE : " << (price ? price->valueint : 0) << endl;
            cout<<RESET;
            cout<<MAGENTA<<"-----------------------------\n"<<RESET;
       //safer checks
        }
    }
    if(!found){
        cout<<RED<<"Item not found!\n"<<RESET;
        return;
    }
    cout<<GREEN<<"All Items out of stock displayed\n"<<RESET;
    cout<<MAGENTA<<"-----------------------------\n"<<RESET;
    cJSON_Delete(root);
}

int main(){
    int choice;
    char proceed;
    vector<item> items; //vector used when we dont know the size//items is a dynamic array having data type of structure so that each element is a structure data type element
    do{
        menu();
        cout<<"enter your choice : ";
        cin>>choice;
        if(choice==1){
            additem(items);
        }
        else if(choice==2){
            display();
        }
        else if(choice==3){
            search();
        }
        else if(choice==4){
            update();
        }
        else if(choice==5){
            display_outofstock();
        }
        else if(choice==6){
            break;
        }
        else{
            cout<<RED<<"invalid choice! try again!"<<RESET;
        }
        cout<<"enter y or n if want to continue : ";
        cin>>proceed;
    }while(tolower(proceed)=='y');
    return 0;
}
/*
how to run 
g++ main.cpp cJSON.c -o main
then :
.\main
*/