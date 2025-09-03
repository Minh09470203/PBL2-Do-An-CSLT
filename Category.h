#ifndef CATEGORY_H_INCLUDED
#define CATEGORY_H_INCLUDED
#include <fstream>
#include <sstream>
class Category {
    private:
        string ID_Category;
        string Name_Category;
    public:
        Category(
                 string ID_Category = "",
                 string Name_Category = ""
        )
        : ID_Category(ID_Category), Name_Category(Name_Category) {}
        string getID_Category() {
            return ID_Category;
        }
        string getName_Category() {
            return Name_Category;
        }
        void setID_Category(string cateid) {
            this->ID_Category = cateid;
        }
        void setName_Category(string catename) {
            this->Name_Category = catename;
        }

};

MyMap<string, string> categoryMap;

bool loadCategoryMap(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Cannot open file " << filename << endl;
        return false;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, name;
        getline(ss, id, '|');
        getline(ss, name);
        categoryMap[id] = name;
    }
    file.close();
    return true;
}

void AddCategory(MyMap<string, string>& categoryMap) {
    string id, name;
    cout << "Enter new Category ID: "; getline(cin, id);
    cout << "Enter Category Name: "; getline(cin, name);

    // Check duplicate
    if (categoryMap.Find(id)) {
        cout << "Category ID already exists.\n";
        return;
    }

    categoryMap.Insert(id, name);

    ofstream file("Data/Category.txt", ios::app);
    if (file.is_open()) {
        file << id << "|" << name << "\n";
        file.close();
        cout << "Category added successfully." << endl;
    } else {
        cout << "Cannot open Category.txt." << endl;
    }
}

void ShowCategoryList(MyMap<string, string>& categoryMap) {
    cout << left << setw(10) << "CateID " << setw(20) << " Category Name" << endl;
    cout << string(25, '-') << endl;
    categoryMap.ShowAll();
}


#endif // CATEGORY_H_INCLUDED
