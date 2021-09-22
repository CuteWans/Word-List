#include <bits/stdc++.h>
#include<unistd.h>

using namespace std;

int tim1, tim2;

template <class T>
class List {
    struct Node {
        Node *next;
        int num;
        T word;
    };
    Node *head, *tail;
    int len;
    vector<pair <Node *, int>> pii;
public:
    List() {
        head = nullptr;
        tail = nullptr;
        pii.clear();
        len = 0;
    };

    friend ofstream & operator << (ofstream &of, List &a) {
        for(auto it = a.head; it != nullptr; it = it -> next)
            of << "\"" << it -> word << "\":" << it -> num << endl;
        return of;
    }

    Node *create_node(Node *x) {
        auto nd = new Node;
        nd -> num = x -> num;
        nd -> word = x -> word;
        nd -> next = nullptr;
        return nd;
    }
    
    void insert_lex(const string &str) {
        //cout << str << endl;
        if(head == nullptr) {
            auto nd = new Node;
            nd -> num = 1;
            nd -> word = str;
            nd -> next = nullptr;
            head = nd;
            return ;
        } else if(str < head -> word) {
            auto nd = new Node;
            nd -> next = head;
            nd -> num = 1;
            nd -> word = str;
            head = nd;
            return ;
        }
        for(auto it = head; it != nullptr; it = it -> next) {
            if(str > it -> word && (it -> next == nullptr || str < it -> next -> word)) {
                auto nd = new Node;
                nd -> next = it -> next;
                nd -> num = 1;
                nd -> word = str;
                it -> next = nd;
            } else if(str == it -> word) {
                it -> num ++;
                break;
            }
        }
    }

    void insert_digit(Node *x) {
        // cout << x -> word << endl;
        if(head == nullptr) {
            head = create_node(x);
            tail = head;
            return ;
        } else if(head -> num < x -> num) {
            auto nd = create_node(x);
            nd -> next = head;
            head = nd;
            cout << nd -> word << endl;
            return ;
        }
        tim1 ++;
        for(auto it = head; it != nullptr; it = it -> next) {
            if(it -> num > x -> num && (it -> next == nullptr || it -> next -> num < x -> num)) {
                auto nd = create_node(x);
                nd -> next = it -> next;
                it -> next = nd;
                tim2 ++;
                // cout << nd -> word << endl;
            } else if(it -> num > x -> num && it -> next -> num == x -> num && x -> word < it -> next -> word) {
                auto nd = create_node(x);
                nd -> next = it -> next;
                it -> next = nd;
                tim2 ++;
                // cout << nd -> word << endl;
            } else if(it -> num == x -> num) {
                if(it -> word < x -> word && (it -> next == nullptr || (it -> next -> word > x -> word && it -> next -> num == x -> num) || it -> next -> num < x -> num)) {
                    auto nd = create_node(x);
                    nd -> next = it -> next;
                    it -> next = nd;
                    tim2 ++;
                    // cout << nd -> word << endl;
                }
            }
        }
    }

    void add_words(const List &l) {
        for(auto it = l.head; it != nullptr; it = it -> next)
            insert_digit(it);
    }

    ~List() {}
};

class Trie {
    struct Node {
        int cnt;
        char ch;
        Node *next, *son, *tail;
    };
    Node *root;
public:
    Trie() {
        root = newnode();
    }
    Node *newnode() {
        Node *nd = new Node;
        nd -> son = nullptr;
        nd -> next = nullptr;
        nd -> cnt = 0;
        nd -> tail = nullptr;
        return nd;
    }
    void insert_stopwords(const string &str) {
        auto nd = root;
        // cout << str << endl;
        for(int i = 0; i < str.length(); i ++) {
            if(nd -> son == nullptr) {
                nd -> son = newnode();
                nd -> son -> ch = str[i];
                // cout << nd -> son -> ch;
                nd -> tail = nd -> son;
                nd = nd -> son;
                continue;
            }
            Node *to = nullptr;
            for(auto it = nd -> son; it != nullptr; it = it -> next) {
                if(it -> ch == str[i]) {
                    to = it;
                    break;
                }
            }
            if(to == nullptr) {
                to = newnode();
                to -> ch = str[i];
                // cout << to -> ch;
                nd -> tail -> next = to;
                nd -> tail = to;
            }
            nd = to;
        }
        //cout << nd -> ch << endl;
        nd -> cnt = 1;
    }
    bool find_word(const string &str) {
        auto nd = root;
        //cout << str << endl;
        for(int i = 0; i < str.length(); i ++) {
            Node *to = nullptr;
            for(auto it = nd -> son; it != nullptr; it = it -> next) {
                if(it -> ch == str[i]) {
                    to = it;
                    //cout << it -> ch << "&" << str[i] << "-";
                    break;
                }
            }
            if(to == nullptr) {
                //cout << endl;
                return 0;
            }
            nd = to;
        }
        //cout << endl;
        return nd -> cnt;
    }
    ~Trie() {}
};

void read_stopwords(const string &stopwords_path, Trie &tr) {
    ifstream f1(stopwords_path);
    string words;
    while(f1 >> words) {
        tr.insert_stopwords(words);
    }
}

void format(vector<string> & words) {
    int i, j;
    string word = words[0];
    words.clear();
    for (i = 0; i < word.size(); ++i) {
        if (word[i] >= 'A' && word[i] <= 'Z') {
            word[i] += 32;
        }
    }
    for (i = 0; i < word.size(); ++i) {
        if (word[i] >= 'a' && word[i] <= 'z' || word[i] == '\'') {break;}
    }
    while (i < word.size()) {
        for (j = i + 1; j < word.size(); ++j) {
            if (!(word[j] >= 'a' && word[j] <= 'z')) {break;}
        }
        if (!(i + 1 == j && word[i] == '\'')) {
            words.push_back(word.substr(i, j - i));
        }
        for (i = j; i < word.size(); ++i) {
            if (word[i] >= 'a' && word[i] <= 'z' || word[i] == '\'') {break;}
        } 
    }
  }

void read_book(const string &path, Trie tr) {
    ifstream f(path);
    string str;
    List <string> l[27];
    int tim = 0;
    while(f >> str) {
        vector<string> words;
        words.push_back(str);
        format(words);
        for (auto word : words) {
            if(tr.find_word(word))   continue ;
            int opt = word[0] - 'a';
            //cout << str << ':' << opt << endl;
            l[opt].insert_lex(word);
        }
    }
    ofstream of("output.txt");
    List <string> li;
    for(int i = 0; i < 26; i ++) {
        li.add_words(l[i]);
    }
    of << li;
    cout << tim1 << ' ' << tim2 << endl;
}

int main(int argc, char* argv[]) {
    int ch;
    string book_path, stopwords_path;
    while(~(ch = getopt(argc, argv, "b:s:"))) {
        if(optarg)
            switch (ch) {
                case 'b':
                    book_path = optarg;
                    break;
                case 's':
                    stopwords_path = optarg;
                    break;
            }
    }
    Trie tr;
    read_stopwords(stopwords_path, tr);
    read_book(book_path, tr);
    return 0;
}