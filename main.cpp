#include <iostream>
#include "AVL.h"
#include <set>
#include <algorithm>
#include <assert.h>
using namespace std;
#define REQUIRE assert

int main() {
  /*  Set<int> my = {72, 106, 141, 350, 355, 382, 391, 422, 478, 547, 629, 648, 662, 667, 705, 718, 840, 902, 942, 944, 954, 966, 986};
    cout << *my.lower_bound(413);*/
    std::set<int> right;
    Set<int> my;
    int queries = 6e4;
    while (queries--) {
        for (auto i : my) {
            cout << i << " ";
        }
        cout << endl;
        for (auto i : right) {
            cout << i << " ";
        }
        cout << endl;
        int type = rand() % 4;
        if (type == 0) { //add
            int x = rand() % 1000;
            my.insert(x);
            right.insert(x);
        }
        if (type == 2) {
            int x = rand() % 1000;
            cout << "X = " << x << endl;
            if (right.lower_bound(x) != right.end()) {
                if (x == 413) {
                    cout << 1;
                }
                cout << *right.lower_bound(x) << " <- good and bad -> " << *my.lower_bound(x);
                REQUIRE(*right.lower_bound(x) == *my.lower_bound(x));
            } else {
                        REQUIRE(my.lower_bound(x) == my.end());
            }
        }

    }
}
