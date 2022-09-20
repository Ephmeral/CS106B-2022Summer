#include <sstream>
#include "pqclient.h"
#include "testing/SimpleTest.h"
#include "vector.h"
using namespace std;

/* 
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo. 
 */

void runDemos();

PROVIDED_TEST("Interactive Demos using PQ for cool data visualization") {

    Vector<DataPoint> vec = { { "A", 1 }, { "C", 3 }, { "B", 2 } };
    Vector<DataPoint> expected = { { "A", 1 }, { "B", 2 }, { "C", 3 } };
    Vector<DataPoint> topOne = { { "C", 3 } };
    stringstream stream;
    stream << vec[0] << vec[1] << vec[2];

    pqSort(vec);
    Vector<DataPoint> result = topK(stream, 1);
    bool pqclientFunctionsAppearToWork = (vec == expected && result == topOne);
    // if pqclient functions don't work, the demos are toast
    EXPECT(pqclientFunctionsAppearToWork);

    runDemos();
}
