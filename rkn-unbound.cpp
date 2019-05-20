#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <set>

using namespace std;

int main(int argc, char *argv[], char *env[]){
    set<string> wdomains;
    set<string> domains;
    wdomains.clear();
    domains.clear();
    string fline;
    set<string>::iterator iter;
    set<string>::iterator iter2;
    string it2;
    string suf;

    if (argc < 4) {
        cout << "Usage: " << argv[0] << " <wdomain file> <domain file> <stubip>" << endl;
        return -1;
    }

    ifstream wdfile(argv[1]);
    ifstream dfile(argv[2]);
    string stubip = argv[3];

    if (wdfile.is_open()){
        while(getline(wdfile, fline)){
            if (fline != "") {
                wdomains.insert(fline);
            }
        }
        wdfile.close();
    } else {
        cout << "Error open file " << argv[1] << endl;
        return -1;
    }

    if (dfile.is_open()){
        while(getline(dfile, fline)){
            if (fline != "") {
                domains.insert(fline);
            }
        }
        dfile.close();
    } else {
        cout << "Error open file " << argv[2] << endl;
        return -1;
    }
    
    set<string> wdom(wdomains);
    for (iter = wdom.begin(); iter != wdom.end(); iter++){
        for (iter2 = wdom.begin(); iter2 != wdom.end(); iter2++){
            suf = '.' + *iter;
            it2 = *iter2;
            if (it2.length() >= suf.length()) {
                if (!(it2.compare(it2.length() - suf.length(), suf.length(), suf))){
//                    cout << it2 << " " << suf << endl;
                    wdomains.erase(*iter2);
                }
            }
        }
    }
        
    set<string> dom(domains);
    for (iter = wdomains.begin(); iter != wdomains.end(); iter++){
        for (iter2 = dom.begin(); iter2 != dom.end(); iter2++){
            suf = '.' + *iter;
            it2 = *iter2;
            if (*iter == *iter2) {
//                cout << it2 << " " << it << endl;
                domains.erase(*iter2);
            }
            if (it2.length() >= suf.length()) {
                if (!(it2.compare(it2.length() - suf.length(), suf.length(), suf))) {
//                    cout << it2 << " " << suf << endl;
                    domains.erase(*iter2);
                }
            }
        }
    }
        
    for (iter = domains.begin(); iter != domains.end(); iter++){
        cout << "local-zone: \"" << *iter << "\" transparent" << endl;
        cout << "local-data: \"" << *iter << ". IN A " << stubip << "\"" << endl << endl;
    }
    for (iter = wdomains.begin(); iter != wdomains.end(); iter++){
        cout << "local-zone: \"" << *iter << "\" redirect" << endl;
        cout << "local-data: \"" << *iter << ". IN A " << stubip << "\"" << endl << endl;
    }

    return 0;
}
