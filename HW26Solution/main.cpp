#include <iostream>
#include <windows.h>

using namespace std;

/*
==========================================================================================================================================
                        ДОМАШНЄ ЗАВДАННЯ 26. Дружні функції, << і >>, [], explicit operator bool, = delete. Клас Gradebook
==========================================================================================================================================
*/

class Gradebook
{
private:
    int* grades;
    int count;

public:
    explicit Gradebook(int count) : grades(nullptr), count(count)
    {
        if (this->count < 1)
        {
            this->count = 1;
        }
        grades = new int[this->count];
        for (int i = 0; i < this->count; i++)
        {
            grades[i] = 0;
        }
    }
    Gradebook(double count) = delete;
    //  5. Що сталося б при Gradebook copy = book;, якби копіювання не було заборонене?
    Gradebook(const Gradebook& other) = delete;
    Gradebook& operator=(const Gradebook& other) = delete;

    ~Gradebook()
    {
        delete[] grades;
    }

    //  2. Навіщо дві версії []? Яка з них викликається в bestGrade і чому?
    //  3. (Рівень 2) Чому через[] не можна перевірити, що оцінка в межах 1..12, а через setGrade — можна ?
    int& operator[](int lesson)
    {
        return grades[lesson];
    }

    //  2. Навіщо дві версії []? Яка з них викликається в bestGrade і чому?
    int operator[](int lesson) const
    {
        return grades[lesson];
    }

    int getCount() const
    {
        return count;
    }

    double average() const
    {
        int sum = 0;
        int graded = 0;
        for (int i = 0; i < count; i++)
        {
            if (grades[i] > 0)
            {
                sum += grades[i];
                graded++;
            }
        }
        if (graded == 0)
        {
            return 0;
        }
        return (double)sum / graded;
    }

    //  3. (Рівень 2) Чому через[] не можна перевірити, що оцінка в межах 1..12, а через setGrade — можна ?
    bool setGrade(int lesson, int grade)
    {
        if (lesson < 0 || lesson >= count || grade < 1 || grade > 12)
        {
            return false;
        }
        grades[lesson] = grade;
        return true;
    }

    //   4. Навіщо explicit в operator bool? Який рядок скомпілювався б без нього?
    explicit operator bool() const
    {
        for (int i = 0; i < count; i++)
        {
            if (grades[i] > 0)
            {
                return true;
            }
        }
        return false;
    }

    //  1. Чому operator<< для Gradebook — дружня функція, а не метод класу?
    friend ostream& operator<<(ostream& out, const Gradebook& book);
    friend istream& operator>>(istream& in, Gradebook& book);
};

    //  1. Чому operator<< для Gradebook — дружня функція, а не метод класу?
ostream& operator<<(ostream& out, const Gradebook& book)
{
    out << "Оцінки:";
    for (int i = 0; i < book.count; i++)
    {
        if (book.grades[i] > 0)
        {
            out << " " << book.grades[i];
        }
        else
        {
            out << " -";
        }
    }
    out << " | середній бал: " << book.average();
    return out;
}

istream& operator>>(istream& in, Gradebook& book)
{
    for (int i = 0; i < book.count; i++)
    {
        in >> book.grades[i];
        if (book.grades[i] < 0 || book.grades[i] > 12)
        {
            book.grades[i] = 0;
        }
    }
    return in;
}

//  2. Навіщо дві версії []? Яка з них викликається в bestGrade і чому?
int bestGrade(const Gradebook& book)
{
    int best = 0;
    for (int i = 0; i < book.getCount(); i++)
    {
        if (book[i] > best)
        {
            best = book[i];
        }
    }
    return best;
}

//  6. (Рівень 3) Яким ви зробили operator< — методом, дружньою чи глобальною функцією? Чому?
bool operator<(const Gradebook& first, const Gradebook& second)
{
    return first.average() < second.average();
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // ---------------- РІВЕНЬ 1 ----------------
    cout << "1. Порожній журнал" << endl;
    Gradebook olena(5);
    cout << "  " << olena << endl;
    if (!olena)
    {
        cout << "  Оцінок ще немає" << endl;
    }

    cout << "2. Ставимо оцінки через []" << endl;
    olena[0] = 10;
    olena[1] = 12;
    olena[3] = 9;
    cout << "  " << olena << endl;                      // Оцінки: 10 12 - 9 - | середній бал: 10.3333
    if (olena)
    {
        cout << "  У журналі є оцінки" << endl;
    }

    cout << "3. Найкраща оцінка (константна версія [])" << endl;
    cout << "  " << bestGrade(olena) << endl;           // 12

    // Gradebook copy = olena;      // ПОМИЛКА: конструктор копіювання видалено — журнал існує в одному екземплярі
    // Gradebook wrong(5.5);        // ПОМИЛКА: конструктор з double видалено
    // int n = olena;               // ПОМИЛКА: operator bool — explicit

    // ---------------- РІВЕНЬ 2 ----------------
    cout << "4. Рівень 2: setGrade з перевіркою" << endl;
    if (!olena.setGrade(4, 15))
    {
        cout << "  Оцінку 15 не поставлено: допустимо 1..12" << endl;
    }
    olena.setGrade(4, 11);
    cout << "  " << olena << endl;                      // Оцінки: 10 12 - 9 11 | середній бал: 10.5

    cout << "5. Рівень 2: введення оцінок через >>" << endl;
    Gradebook ihor(5);
    cout << "  Введіть 5 оцінок Ігоря (0 — оцінки немає): ";
    cin >> ihor;
    cout << "  " << ihor << endl;

    // ---------------- РІВЕНЬ 3 ----------------
    cout << "6. Рівень 3: у кого вищий середній бал" << endl;
    if (ihor < olena)
    {
        cout << "  У Олени" << endl;
    }
    else if (olena < ihor)
    {
        cout << "  В Ігоря" << endl;
    }
    else
    {
        cout << "  Однаковий" << endl;
    }

    return 0;
}

/*
  1. Чому operator<< для Gradebook — дружня функція, а не метод класу?
  2. Навіщо дві версії []? Яка з них викликається в bestGrade і чому?
  3. (Рівень 2) Чому через [] не можна перевірити, що оцінка в межах 1..12, а через setGrade — можна?
  4. Навіщо explicit в operator bool? Який рядок скомпілювався б без нього?
  5. Що сталося б при Gradebook copy = book;, якби копіювання не було заборонене?
  6. (Рівень 3) Яким ви зробили operator< — методом, дружньою чи глобальною функцією? Чому?

  ВІДПОВІДІ НА ЧАСТИНУ «ПОЯСНИ»

  1. 

  2. 

  3. 

  4. 

  5. 

  6. 
*/
