#include <iostream>
#include <string>
using namespace std;

bool IsKPeriodic(string s, int K) {
    if (s.length() % K != 0)
        return false;//длина строки не кратна числу - сразу нет... надеюсь... 
    int p = s.length() / K;//количество сегментов
    for (int i = 0; i < K; i++) {//смещение по буквам
        char c = s[i];
        for (int j = 1; j < p; j++) {//смещение по сегментам, первый(нулевой) сегмент - опорный
            if (s[j * K + i] != c) {
                return false;//при первом расхождении - завершаем поиск
            }
        }
    }

    return true;
}//в худшем случае сложность O(n) вроде... сверка всех букв (K-1)*p сверок, (K-1)*(n/K) = n-1/K   ~n

int main()
{    
    string s = "123123123123";
    for (int i = 1; i < 11; i++)
        cout << "K="<<i<<"  "<< ((IsKPeriodic(s, i))?"YES!":"no") << endl;
}
