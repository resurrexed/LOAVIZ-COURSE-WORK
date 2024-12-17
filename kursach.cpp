#include <iostream>
#include <cstdlib>
#include <new>
#include <ctime>
#include <stack>
#include <vector>
#include <unordered_set>
#include <clocale>
#include <limits>
using namespace std;

class Matrix {
public:
    int size;
    vector<vector<int>> matrix;
    vector<vector<int>> transposedMatrix;
    vector<bool> visited;
    vector<int> finishTime;
    int time;

    Matrix(int size) : size(size), matrix(size, vector<int>(size)), transposedMatrix(size, vector<int>(size)), 
                       visited(size, false), finishTime(size, 0), time(0) {}

    void GenerateMatrix(int random) {
        for(int i=0; i<size; i++) {
            matrix[i][i] = 0;
            for(int j=0; j<size; j++) {
                if(i==j) continue;
                if(random < rand() % 101) {
                    matrix[i][j] = 0;
                    transposedMatrix[j][i] = 0;
                } else {
                    matrix[i][j] = 1;
                    transposedMatrix[j][i] = 1;
                }
            }
        }
    }
    void InputMatrix() {
        int value;
        cout << "Введите матрицу смежности (строки через пробел, 0 или 1):" << endl;
        for (int i = 0; i < size; i++) 
        {
            cout << "Строка " << i + 1 << ": ";
            for (int j = 0; j < size; j++) 
            {
                do {
                    cin >> value;
                    if (value != 0 && value != 1) 
                    {
                        cout << "Ошибка: можно вводить только 0 или 1. Повторите ввод для элемента [" << i << "][" << j << "]: ";
                    }
                } while (value != 0 && value != 1); // Проверка, что введено 0 или 1

                matrix[i][j] = value;
                transposedMatrix[j][i] = value;
            }
        }
    }    
    void PrintMatrix(const vector<vector<int>>& mat) {
        for(int i=0; i<size; i++) {
            for(int j=0; j<size; j++) {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }
    }

    void DFSFirstPass(int v) {
        visited[v] = true;
        for (int i = 0; i < size; ++i) {
            if (matrix[v][i] && !visited[i]) {
                DFSFirstPass(i);
            }
        }
        finishTime[time++] = v;
    }

    void DFSSecondPass(int v) {
        visited[v] = true;
        cout << v + 1 << " ";
        for (int i = 0; i < size; ++i) {
            if (transposedMatrix[v][i] && !visited[i]) {
                DFSSecondPass(i);
            }
        }
    }

    void FindStrongComponents() {
        for(int i = 0; i < size; ++i) {
            if(!visited[i]) {
                DFSFirstPass(i);
            }
        }
        fill(visited.begin(), visited.end(), false);
        cout << "Strongly Connected Components:" << endl;
        for (int i = size - 1; i >= 0; --i) {
            int v = finishTime[i];
            if (!visited[v]) {
                cout << "Component: ";
                DFSSecondPass(v);
                cout << endl;
            }
        }
    }

    void FindWeakComponents() {
        fill(visited.begin(), visited.end(), false);
        cout << "\nWeakly Connected Components:" << endl;
        for(int i = 0; i < size; ++i) {
            if(!visited[i]) {
                cout << "Component: ";
                stack<int> st;
                st.push(i);
                while (!st.empty()) {
                    int current = st.top();
                     st.pop();
                    if (!visited[current]) {
                        visited[current] = true;
                        cout << current + 1 << " ";
                        for (int j = 0; j < size; ++j) {
                            if ((matrix[current][j] || matrix[j][current]) && !visited[j]) {
                                st.push(j);
                            }
                        }
                    }
                }
                cout << endl;
            }
        }
    }
};

int main() {
    system("clear");
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");
    cout <<"Курсовая работа\nВыполнена сутдентом группы 23ВВВ3:\nСаранцевым Е.А.\nНажмите ENTER чтобы начать"<<endl;
    char input;
    do{
        cin.get(input);
        if(input!='\n')
        {
            cout<<"Ошибка, нажмите ENTER";
        }
    }while(input!='\n');
    system("clear");
    cout<<"Введите тип ввода графа:\n1-Ручной ввод\n2-Сгенерированный граф\n";
    do{
        cin>>input;
        if(input!='1' && input !='2') 
        {
            cout<<"Ошибка, введите 1 или 2: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }while(input!='1' && input!='2');
    if(input=='1')
    {
        int size;
        cout << "Введите количество вершин графа: ";
        do {
            cin >> size;
            if (size <= 0) {
                cout << "Ошибка, невозможное количество вершин в графе, введите еще раз: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (size <= 0);
        Matrix graph(size);
        graph.InputMatrix();
        cout << "Original Matrix:" << endl;
        graph.PrintMatrix(graph.matrix);
        cout << "\nTransposed Matrix:" << endl;
        graph.PrintMatrix(graph.transposedMatrix);

        graph.FindStrongComponents();
        graph.FindWeakComponents();
    } else {
        int size, random;
        cout <<"Введите количество вершин: ";
        do{
            cin >> size;
            if(size<=0)
            {
                cout<<"Ошибка, невозможное количество вершин в графе, введите еще раз: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }while(size<=0);
        cout<<"Введите шанс генерации ребер в %: ";
        do{
            cin>>random;
            if(random<0 || random>100)
            {
                cout<<"Ошибка, невозможный процент генерации ребер в графе, введите еще раз: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }while(random<0 || random>100);
        Matrix graph(size);
        graph.GenerateMatrix(random);
        cout << "Original Matrix:" << endl;
        graph.PrintMatrix(graph.matrix);
        cout << "\nTransposed Matrix:" << endl;
        graph.PrintMatrix(graph.transposedMatrix);

        graph.FindStrongComponents();
        graph.FindWeakComponents();
    }
    return 0;
}
