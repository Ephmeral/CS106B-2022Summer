#include "labyrinth.h"
#include "demo/MazeGenerator.h"
#include "testing/SimpleTest.h"
using namespace std;

/* Change kYourName constant to your full name.
 *
 * WARNING: Once you've set set this constant and started exploring your labyrinth,
 * do NOT edit the value of kYourName. Changing kYourName will change which
 * maze you get back, which might invalidate all your hard work!
 */
const string kYourName = "ephmeral";

/* Change this constant to the path out of your labyrinth. */
const string kPathOutOfNormalMaze = "SNSNESNSNWSNSNEWEEWEENSNSWESNSNNSNEWEWEWEWENSNSWWWWESEWWWEWE";

/**
 * Given a location in a maze, returns whether the given sequence of
 * steps will let you escape the maze.
 *
 * To escape the maze, you need to collect all items in the `needs` set.
 * You can only take steps in the four cardinal directions,
 * and you can't move in directions that don't exist in the maze.
 */
bool isPathToFreedom(MazeCell* start, string moves, Set<string> needs) {
    /* TODO: Your code here */
    MazeCell *curr = start;
    if (curr == nullptr) {
        return false;
    }
    if (needs.contains(curr->contents)) {
        needs.remove(curr->contents);
    }

    for (auto move : moves) {
//        if (curr == nullptr) {
//            return false;
//        }
        if (move == 'N') {
            curr = curr->north;
        } else if (move == 'S') {
            curr = curr->south;
        } else if (move == 'W') {
            curr = curr->west;
        } else if (move == 'E') {
            curr = curr->east;
        } else {
            error("there is not correct character");
        }

        if (curr == nullptr) {
            return false;
        }
        if (needs.contains(curr->contents)) {
            needs.remove(curr->contents);
        }
        if (needs.isEmpty()) {
            return true;
        }
    }
    return needs.isEmpty();
}

//Map<MazeCell*, int> vis;

void saveMaze(MazeCell* start, string &moves, Set<string> &needs, Map<MazeCell*, int> &vis) {
    if (start == nullptr) {
        return;
    }
    if (needs.isEmpty()) {
        return;
    }
    if (vis.containsKey(start)) {
        if (vis[start] == -1) {
            return;
        } else if (vis[start] == 1) {
            vis[start] = -1;
        }
    } else {
        vis[start] = 1;
    }
    if (needs.contains(start->contents)) {
        needs.remove(start->contents);
    }

    if (start->north != nullptr) {
        moves.push_back('N');
        saveMaze(start->north, moves, needs, vis);
    }
    if (start->south != nullptr) {
        moves.push_back('S');
        saveMaze(start->south, moves, needs, vis);
    }
    if (start->east != nullptr) {
        moves.push_back('E');
        saveMaze(start->east, moves, needs, vis);
    }
    if (start->west != nullptr) {
        moves.push_back('W');
        saveMaze(start->west, moves, needs, vis);
    }


}

/* * * * * * Test Cases Below This Point * * * * * */

PROVIDED_TEST("Check paths in the sample from writeup") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES", allThree));

    /* This path works to pick up just potion */
    EXPECT(isPathToFreedom(maze[2][2], "ESNW", {"Potion"}));

    /* This path doesn't work, since doesn't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW", allThree));

    /* This path doesn't work, cannot walk through walls. */
    EXPECT(!isPathToFreedom(maze[2][2], "NNWWSSSEEE", allThree));
}

PROVIDED_TEST("Works when start on an item") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "E", {"Potion"}));
    EXPECT(isPathToFreedom(maze[0][1], "W", {"Potion", "Spellbook"}));
}

PROVIDED_TEST("Reports errors if given illegal characters") {
    auto maze = toMaze({"P-S-W"});

    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Q", {"Wand"}));
    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Ee", {"Wand"}));
}

PROVIDED_TEST("Escape from your personal labyrinth!") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    /* A maze for you to escape from. This maze will be personalized
     * based on the constant kYourName.
     */
    EXPECT(kYourName != "TODO");
    MazeCell* startLocation = mazeFor(kYourName);

    Set<string> needs = {"Spellbook", "Potion", "Wand"};
    string moves;
    Map<MazeCell*, int> vis;
    saveMaze(startLocation, moves, needs, vis);
    std::cout << "one solution of maze is " << moves << std::endl;
    std::cout << "check whether correct " << isPathToFreedom(startLocation, moves, allThree) << std::endl;
    /* Set a breakpoint here. As a reminder, the labyrinth you
     * get will be personalized to you, so don't start exploring the labyrinth
     * unless you've edited the constant kYourName to include your name(s)!
     * Otherwise, you'll be escaping the wrong labyrinth.
     */
    EXPECT(kPathOutOfNormalMaze != "TODO");
    EXPECT(isPathToFreedom(startLocation, kPathOutOfNormalMaze, allThree));
}

