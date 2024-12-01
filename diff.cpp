#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <stack>
#include <map>
#include <climits>

using namespace std;

typedef unsigned char uc;

void error()
{
    cerr << "Error!" << endl;
    exit(-1);
}

// бинарный поиск
int BS(const vector<int>& v, const int& fKey)
{
    int s = 1;
    int r = v.size() - 1;
    int m = (s + r) >> 1; // (s + r) / 2
    while (r > s)
    {
        if (fKey < v[m])
            r = m;
        else if (fKey > v[m])
            s = m + 1;
        else
            return m;
        m = (s + r) >> 1;
    }

    return m;
}

/// преобразование строки в нижний регистр
string lower(const string& s)
{
    string ans;
    for (int i = 0; i < s.length(); ++i)
        ans += tolower(s[i]);
    return ans;
}

// вывод на экран различных строк
void PrintStrings(const int& side, const vector<string>& v, const int& s, const int&e)
{
    for (int i = s; i < e; ++i)
        cout << (side == 1 ? "< " : "> ") << v[i] << endl;
}

int main(int argc, char* argv[])
{
    if (argc > 4 || argc < 3)
        error();

    int cnt = 0;

    bool ignoreRegister = false;
    char *fName, *testName;

    if (argc == 3)
    {
        fName = argv[1];
        testName = argv[2];
    }
    else
    {
        if (strcmp("-i", argv[1]))
            error();
        else
            ignoreRegister = true;
        fName = argv[2];
        testName = argv[3];
    }

    vector<string> OrigVecFILE;
    vector<string> CompVecFILE;

    ifstream origFile(fName, ios::in);
    ifstream compFile(testName, ios::in);

    map< string, vector<int> > Positions;

    string oStr, compStr;

    while (getline(origFile, oStr))
        OrigVecFILE.push_back(ignoreRegister ? lower(oStr) : oStr); // проверка на то, был ли прописан ключ '-i'

    while (getline(compFile, compStr))
        CompVecFILE.push_back(ignoreRegister ? lower(compStr) : compStr); // аналогично

    origFile.close();
    compFile.close();

    for (int i = CompVecFILE.size() - 1; i >= 0; --i)
        Positions[CompVecFILE[i]].push_back(i);

    vector<int> PosEachString;

    for (int i = 0; i < OrigVecFILE.size(); ++i)
    {
        map< string, vector<int> >::const_iterator it = Positions.find(OrigVecFILE[i]);
        if (it == Positions.end())
            continue;
        for (int j = 0; j < it->second.size(); ++j)
            PosEachString.push_back(it->second[j]);
    }

    vector<int> d(PosEachString.size() + 1);
    vector<int> pos(PosEachString.size() + 1);
    vector<int> prev(PosEachString.size());

    int length = 0;
    pos[0] = -1;
    d[0] = -INT_MAX;
    for (int i = 1; i <= PosEachString.size(); ++i)
        d[i] = INT_MAX;

    for (int i = 0; i < PosEachString.size(); ++i)
    {
        int col = BS(d, PosEachString[i]);
        if (d[col - 1] < PosEachString[i] && PosEachString[i] < d[col])
        {
            d[col] = PosEachString[i];
            pos[col] = i;
            prev[i] = pos[col - 1];
            length = max(length, col);
        }
    }
    vector<int> answer;
    int p = pos[length];
    while (p != -1)
    {
        answer.push_back(PosEachString[p]);
        p = prev[p];
    }

    int start1, start2;
    start1 = start2 = 1;

    //так как ответ содержит позиции общих строк в порядке убывания => необходимо его реверсировать => for i = n to 1
    for (int i = answer.size() - 1; i >= 0; --i)
    {
        string cmp = CompVecFILE[answer[i]]; // общая строка

        int pos1 = start1;
        int pos2 = start2;

        vector<string> f1;
        vector<string> f2;

        // набиваем вектор общими строками для первого файла
        for (int i = start1 - 1; i < OrigVecFILE.size(); ++i)
        {
            if (OrigVecFILE[i] != cmp)
                f1.push_back(OrigVecFILE[i]);
            else
                break;
        }

        // набиваем вектор общими строками для второго файла
        for (int i = start2 - 1; i < CompVecFILE.size(); ++i)
        {
            if (CompVecFILE[i] != cmp)
                f2.push_back(CompVecFILE[i]);
            else
                break;
        }
        // если оба вектора не пустые => нужно заменить строки первого файла на строки второго файла
        if (!!f1.size() && !!f2.size())
        {
            if (f1.size() == 1)
                cout << start1;
            else
                cout << start1 << "," << start1 + f1.size() - 1;
            cout << "c";
            if (f2.size() == 1)
                cout << start2;
            else
                cout << start2 << "," << start2 + f2.size() - 1;
            cout << endl;

            PrintStrings(1, OrigVecFILE, start1 - 1, start1 + f1.size() - 1);

            cout << "---" << endl;
            PrintStrings(0, CompVecFILE, start2 - 1, start2 + f2.size() - 1);
            start1 += f1.size() + 1;
            start2 += f2.size() + 1;
        }
        // если оба вектора пустые => икрементируем все счётчики i
        else if (!f1.size() && !f2.size())
        {
            start1++;
            start2++;
        }
        // если первый вектор не пуст, а второй пуст => из первого файла необходимо удалить строки,
        //для того, чтобы сравнять со вторым файлом
        else if (!!f1.size() && !f2.size())
        {
            if (f1.size() == 1)
                cout << start1;
            else
                cout << start1 << "," << start1 + f1.size() - 1;
            cout << "d";
            cout << start2 - 1;
            cout << endl;

            PrintStrings(1, OrigVecFILE, start1 - 1, start1 + f1.size() - 1);
            start1 += f1.size() + 1;
            start2++;
        }
        // обратно пропорционально
        else
        {
            cout << start1 - 1;
            cout << "a";
            if (f2.size() == 1)
                cout << start2;
            else
                cout << start2 << "," << start2 + f2.size() - 1;

            cout << endl;

            PrintStrings(0, CompVecFILE, start2 - 1, start2 + f2.size() - 1);

            start1++;
            start2 += f2.size() + 1;
        }
    }

    if (!!answer.size())
    {
        start1--;
        start2--;
    }

    // если мы прошли не до конца как первый, так и второй файл =>
    //оставшаяся часть не входит в общую подпоследовательность => меняем строки первого файла на строки второго
    if (start1 < OrigVecFILE.size() && start2 < CompVecFILE.size())
    {
        start1++;
        start2++;

        if (OrigVecFILE.size() - start1 + 1 == 1)
            cout << start1;
        else
            cout << start1 << "," << OrigVecFILE.size();
        cout << "c";
        if (CompVecFILE.size() - start2 + 1 == 1)
            cout << start2;
        else
            cout << start2 << "," << CompVecFILE.size();
        cout << endl;

        PrintStrings(1, OrigVecFILE, start1 - 1, OrigVecFILE.size());
        cout << "---" << endl;
        PrintStrings(0, CompVecFILE, start2 - 1, CompVecFILE.size());
    }
    // если мы прошли не до конца второго вектора-файла, а первый файл прошли полностью =>
    //необходимо добавить к первому файлу строки из второго
    else if (start1 >= OrigVecFILE.size() && start2 < CompVecFILE.size())
    {
        start2++;

        cout << OrigVecFILE.size();
        cout << "a";
        if (CompVecFILE.size() - start2 + 1 == 1)
            cout << start2;
        else
            cout << start2 << "," << CompVecFILE.size();
        cout << endl;
        PrintStrings(0, CompVecFILE, start2 - 1, CompVecFILE.size());
    }
    // если мы прошли не до конца первого вектора-файла, а второй
    //файл прошли полностью => необходимо удалить из первого файла строки
    else if (start2 >= CompVecFILE.size() && start1 < OrigVecFILE.size())
    {
        start1++;

        if (OrigVecFILE.size() - start1 + 1 == 1)
            cout << start1;
        else
            cout << start1 << "," << OrigVecFILE.size();
        cout << "d";
        cout << CompVecFILE.size();
        cout << endl;
        PrintStrings(1, OrigVecFILE, start1 - 1, OrigVecFILE.size());
    }

    return 0;
}
