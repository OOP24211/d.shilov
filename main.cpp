#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <iomanip>
#include <algorithm>
#define length 1000

using namespace std;

static int COUNT;

class frequency 
{
public:
    
    string word;

    int count;

    float present;

};

class circlebuff
{
public:

    int start;

    string *text;

};

circlebuff buffer(circlebuff *a) {
    delete [] a->text;
    a->text = nullptr;
    a->text = new string[length];
    a->start = 0;
    return *a;
}

string clear_word(string word) {
    string new_word;
    for (char c : word) {
        if(!ispunct(static_cast<unsigned char>(c))) {
            new_word += c;
        }
    }
    return new_word;
}

string lower(string word) {
    string res;
    for (char c : word) {
        res += tolower((c));
    }
    return res;
}

int main() {
    setlocale(LC_ALL, "Rus");
    vector<frequency> words;
    circlebuff buff;
    buff.text = new string[length];
    buff.start = 0;
    ifstream file("text.txt");
    ofstream file2("file.csv");
    if (!file.is_open()) {
        return 0;
    }
    if (!file2.is_open()) {
        return 0;
    }
    file2 << "word" << "," << "count" << "," << "present" << endl; 
    string word;
    while (file >> word) {
        word = clear_word(word);
        word = lower(word);
        buff.text[buff.start] = word;
        buff.start++;
        if (buff.start == length) {
            for (int i = 0; i < buff.start; i++) {
            frequency object;
            int a = 0;
            COUNT++;
                for (int j = 0; j <= size(words); j++)
                if (size(words) != 0 && buff.text[i] == words[j].word) {
                    words[j].count++;
                    a++;
                }
            if (a == 0) {
                object.word = buff.text[i];
                object.count = 1;
                object.present = 0;
                words.push_back(object);
            }
            if (i + 1 == length) {
                buff = buffer(&buff);
            }
            }
        }
        else {
            continue;
        }
    }
    if (!buff.text->empty()) {
        for (int i = 0; i < buff.start; i++) {
            frequency object;
            int a = 0;
            COUNT++;
                for (int j = 0; j < size(words); j++)
                if (size(words) != 0 && buff.text[i] == words[j].word) {
                    words[j].count++;
                    a++;
                }
            if (a == 0) {
                object.word = buff.text[i];
                object.count = 1;
                object.present = 0;
                words.push_back(object);
            }
        }
    }
    for (int i = 0; i < size(words); i++) {
        words[i].present = ((float(words[i].count) / COUNT) * 100);
    }
    sort(words.begin(), words.end(), [](const frequency& a, const frequency& b) { 
        return a.count > b.count;
    });
    for (int i = 0; i < size(words); i++) {
        file2 << setprecision(3) << words[i].word << "," << words[i].count << "," << words[i].present << endl;
    }
    delete [] buff.text;
    buff.text = nullptr;
    file.close();
    file2.close();
    return 0;
}