#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;
int ticketsold = 0;
double totalrevenue = 0;
// define customer structure
struct customer{
    string ID;
    string Name;
    string Station;
    double Price;
};
struct node{
    customer data;
    node* pre;
    node* next;
};
struct Doublylist{
    node* head;
    node* tail;
};
struct StationStat{
    int count;
    string stationName;
};
// initialize a node
node* createnode(string ID, string Name, string Station, double Price){
    node* newnode = new node();
    newnode->data.ID = ID;
    newnode->data.Name = Name;
    newnode->data.Price = Price;
    newnode->data.Station = Station;
    newnode->next = nullptr;
    newnode->pre = nullptr;
    return newnode;
}
// check if the ID is duplicated or not
bool Isduplicated(Doublylist& list, string id){
    node* current = list.head; // use temporary node
    while(current != nullptr){ // traverse through the list
        if(current->data.ID == id){
            return true;
        }
        current = current->next;
    }
    return false;
}
void AddFunc(Doublylist& list, string ID, string Name, string Station, double Price){
    if(Isduplicated(list,ID)){
        cout << "Already involved !" << endl;
        return;
    }
    node* newnode = createnode(ID,Name,Station,Price);
    if(list.head == nullptr){
        list.head = list.tail = newnode;
        return;
    }
    else{
        list.tail->next = newnode;
        newnode->pre = list.tail;
        list.tail = newnode;
        return;
    }

}
void LoadfromFile(Doublylist& list, string filename){
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Khong the mo file: " << filename << ". Vui long kiem tra lai vi tri dat file!\n";
        return;
    }
    string line;
    int successfullyLoaded = 0;
    while (getline(file, line)) {
        if (line.empty()) continue; // Nếu dòng trống thì bỏ qua
        
        stringstream ss(line); // Chuyển chuỗi 'line' thành một luồng dữ liệu stream
        string id, name, station, priceStr;
        double price;
        
        // Tách dữ liệu dựa trên ký tự ngăn cách ';'
        if (getline(ss, id, ';') &&
            getline(ss, name, ';') &&
            getline(ss, station, ';') &&
            getline(ss, priceStr, ';')) {
            
            // Chuyển chuỗi chứa chữ số thành kiểu số thực double
            price = stod(priceStr); 
            
            // Đưa dữ liệu đã bóc tách vào danh sách liên kết đôi
            AddFunc(list, id, name, station, price);
            successfullyLoaded++;
        }
    }
    file.close(); // Đóng kết nối file sau khi xử lý xong để giải phóng bộ nhớ
    cout << "--- Da tai xong " << successfullyLoaded << " khach hang tu file vao he thong! ---\n";
}
void saveToFile(Doublylist list, string filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << " to save!\n";
        return;
    }

    node* current = list.head;
    int count = 0;

    while (current != nullptr) {
        file << current->data.ID << ";"
             << current->data.Name << ";"
             << current->data.Station << ";"
             << current->data.Price << "\n";

        current = current->next;
        count++;
    }

    file.close();

    cout << "Successfully saved "
         << count
         << " customers to "
         << filename
         << "!\n";
}
void sellticket(Doublylist& list){
    if(list.head == nullptr){
        cout << "Queue is empty !!! \n";
        return;
    }
    node* current = list.head;
    ticketsold++;
    totalrevenue += current->data.Price;
    list.head = current->next;
    delete current;
}
void leaving(Doublylist& list, string ID){
    if(list.head == nullptr){
        cout << "Queue is empty!!!\n";
        return;
    }
    node* current = list.head;
    while(current != nullptr){
        if(current->data.ID == ID){
            if(current->pre == nullptr){
                list.head = current->next;
                if(list.head != nullptr){
                    list.head->pre = nullptr; 
                }
                else {
                    list.tail = nullptr;      // in case there is only one customer in the queue
                }
            }
            else if(current->next == nullptr){ // in case the one want to leave is the tail
                list.tail = current->pre;
                list.tail->next = nullptr;
                
            }
            else{ // the guest is in the middle of the queue
                current->pre->next = current->next;
                current->next->pre = current->pre;
            }
            delete current;
            cout << "Customer with ID : " << ID << " has left \n";
            return;
        }
        current = current->next;
    }
    cout << "Couldn't find customer with ID : " << ID << endl;
}
void displayWaitingStations(Doublylist list) {
    if (list.head == nullptr) {
        cout << "Danh sach hien tai dang trong!\n";
        return;
    }
    // Dùng một mảng chứa tên các ga 
    string uniqueStations[10];
    int uniqueCount = 0;

    node* current = list.head;
    while (current != nullptr) {
        string currentStation = current->data.Station;
        bool isAlreadyInArray = false;
        // Kiểm tra xem ga của khách hàng này đã có trong mảng chưa
        for (int i = 0; i < uniqueCount; i++) {
            if (uniqueStations[i] == currentStation) {
                isAlreadyInArray = true;
                break; // Đã có rồi thì bỏ qua, không thêm nữa
            }
        }
        // Nếu là ga mới, thêm nó vào mảng
        if (!isAlreadyInArray) {
            uniqueStations[uniqueCount] = currentStation;
            uniqueCount++;
        }
        current = current->next;
    }

    // In kết quả ra màn hình
    cout << " Station list \n";
    for (int i = 0; i < uniqueCount; i++) {
        cout << uniqueStations[i];
        if (i < uniqueCount - 1) cout << ", "; // Thêm dấu phẩy trừ phần tử cuối
    }
}
void displayStationsAndCount(Doublylist list) {
    if (list.head == nullptr) {
        cout << "Queue is empty !!!\n";
        return;
    }
    // Dùng mảng StationStat đã khai báo ở trên để vừa lưu tên ga, vừa lưu số đếm
    StationStat stats[10]; 
    int uniqueCount = 0;

    node* current = list.head;
    while (current != nullptr) {
        string currentStation = current->data.Station;
        bool found = false;

        // Tìm xem ga này đã được thống kê trước đó chưa
        for (int i = 0; i < uniqueCount; i++) {
            if (stats[i].stationName == currentStation) {
                stats[i].count++;  // Nếu có rồi thì tăng số lượng khách lên 1
                found = true;
                break;
            }
        }

        // Nếu đây là ga hoàn toàn mới, thêm vào mảng và đặt đếm = 1
        if (!found) {
            stats[uniqueCount].stationName = currentStation;
            stats[uniqueCount].count = 1;
            uniqueCount++;
        }
        current = current->next;
    }

    // In kết quả thống kê chi tiết ra màn hình
    cout << " Numbers of guests of each station \n";
    for (int i = 0; i < uniqueCount; i++) {
        cout << "Station : " << stats[i].stationName << ": " << stats[i].count << " customers\n";
    }
}
void displayList(Doublylist list) {
    node* temp = list.head;
    if (temp == nullptr) {
        cout << "The current customers list is in.\n";
        return;
    }
    cout << "\n Number of guests are buyging tickets \n";
    while (temp != nullptr) {
        cout << "ID: " << temp->data.ID  << endl;
        cout << " | Name: " << temp->data.Name  << endl;
        cout << " | Station: " << temp->data.Station << endl;
        cout << " | Price: " << temp->data.Price << " VND\n";
        temp = temp->next;
    }
}
void displaystatistic(){
    cout << "Report : \n";
    if(ticketsold == 0) {
        cout << "No revenue made today !\n";
    }
    else{
        cout << "Tickets sold : " << ticketsold << endl;
        cout << "Total revenue : " << totalrevenue << endl;
    }
}
int menu() {
    int choice;

    cout << "\n========== TRAIN TICKET MANAGEMENT ==========\n";
    cout << "1. Add customer\n";
    cout << "2. Load customers from file\n";
    cout << "3. Save customers to file\n";
    cout << "4. Sell ticket\n";
    cout << "5. Remove customer from queue\n";
    cout << "6. Display customer list\n";
    cout << "7. Display waiting stations\n";
    cout << "8. Display stations and customer count\n";
    cout << "9. Display sales statistics\n";
    cout << "0. Exit\n";
    cout << "=============================================\n";
    cout << "Enter your choice: ";

    cin >> choice;
    return choice;
}
int main() {
    Doublylist list;
    list.head = nullptr;
    list.tail = nullptr;

    int choice;

    while (true) {
        choice = menu();

        switch (choice) {
        case 1: {
            string id, name, station;
            double price;

            cin.ignore();

            cout << "Enter customer ID: ";
            getline(cin, id);

            cout << "Enter customer name: ";
            getline(cin, name);

            cout << "Enter destination station: ";
            getline(cin, station);

            cout << "Enter ticket price: ";
            cin >> price;

            AddFunc(list, id, name, station, price);
            break;
        }
        case 2: {
            string filename;
            cin.ignore();
            cout << "Enter file name: ";
            getline(cin, filename);
            LoadfromFile(list, filename);
            break;
        }
        case 3: {
            string filename;

            cin.ignore();

            cout << "Enter file name to save: ";
            getline(cin, filename);

            saveToFile(list, filename);
            break;
        }
        case 4: {
            sellticket(list);
            break;
        }
        case 5: {
            string id;
            cin.ignore();
            cout << "Enter customer ID to remove: ";
            getline(cin, id);

            leaving(list, id);
            break;
        }
        case 6: {
            displayList(list);
            break;
        }
        case 7: {
            displayWaitingStations(list);
            cout << endl;
            break;
        }
        case 8: {
            displayStationsAndCount(list);
            break;
        }
        case 9: {
            displaystatistic();
            break;
        }
        case 0: {
            cout << "Exiting program...\n";
            return 0;
        }
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}