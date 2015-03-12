#include <iostream>
#include "hello.h"
#include "console.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

struct Vertex {
    Vertex(string n, double s = 0.0) : name(n), suspicion(s), visitNum(0), lastVisited(false) {}
    string name;
    double suspicion;
    int visitNum;
    bool lastVisited;
    set<pair<Vertex *, double> > neighbors;
};

struct Graph {
    map<string, Vertex *> vertices;
    double suspicion_increase;
    Graph(double s_i = 5) : suspicion_increase(s_i){}
    void addVertex(string name, double suspicion = 50) {
        Vertex *v = new Vertex(name, suspicion);
        vertices[name] = v;
    }
    void connectVertices(Vertex * v1, Vertex *v2, double distance = 100) {
        v1->neighbors.insert(make_pair(v2, distance));
        v2->neighbors.insert(make_pair(v1, distance));
    }
    void connectVertices(string v1, string v2, double distance = 100){
        vertices[v1]->neighbors.insert(make_pair(vertices[v2], distance));
        vertices[v2]->neighbors.insert(make_pair(vertices[v1], distance));
    }

    Vertex * getVertex(string name) {
        return vertices[name];
    }

    void updateSuspicion() {
        for(auto const &it : vertices) {
//            if(it.second->lastVisited) it.second->lastVisited = false;
//            else it.second->suspicion += suspicion_increase;
            it.second->suspicion += suspicion_increase;
        }
    }

    double totalSuspicion() {
        double total = 0.0;
        for(auto const &it : vertices) {
            total += it.second->suspicion;
        }
        return total;
    }
};

struct Guard {
    Guard(string n, Vertex *startLoc) : name(n), currVertex(startLoc){}
    Vertex * currVertex;
    string name;
    void visitVertex(Vertex *target){
        currVertex = target;
        target->suspicion = std::max(0.0, target->suspicion-50);
        target->visitNum++;
        target->lastVisited = true;
    }
    friend bool operator< (const Guard& lhs, const Guard& rhs) {
        return lhs.name < rhs.name;
    }
};

void patrol(Graph &map, set<Guard *> &guards);
Vertex * selectNext(Graph &g, Vertex *currVertex);
double edgeHeuristic(Graph &g, set<Vertex *> &used, Vertex *currVertex);

int main() {
    Graph map1;
    map1.addVertex("room1", 0);
    map1.addVertex("room2", 2);
    map1.addVertex("room3", 2);
    map1.connectVertices("room1", "room2", 1);
    map1.connectVertices("room1", "room3", 1);
    Guard g("Dipshit", map1.getVertex("room1"));
    Guard g2("Shithead", map1.getVertex("room1"));
    set<Guard *> guards = {&g, &g2};
    //patrol(map1, guards);

    Graph map2;
    map2.addVertex("room1", 0);
    map2.addVertex("branch1room1", 2);
    map2.addVertex("branch1room2", 2);
    map2.addVertex("branch2room1", 2);
    map2.addVertex("branch2room2", 2);
    map2.addVertex("branch2room3", 2);
    map2.connectVertices("room1", "branch1room1");
    map2.connectVertices("room1", "branch2room1");
    map2.connectVertices("branch1room1", "branch1room2");
    map2.connectVertices("branch2room1", "branch2room2");
    map2.connectVertices("branch2room1", "branch2room3");
    g.currVertex = map2.getVertex("room1");
    g2.currVertex = map2.getVertex("room1");
    Guard g3("GermanScheiBePorn", map2.getVertex("room1"));
    guards.insert(&g3);
    //patrol(map2, guards);

    Graph map0;
    map0.addVertex("upperLeft", 0);
    map0.addVertex("bottomLeft");
    map0.addVertex("bottomRight");
    map0.addVertex("upperRight");
    map0.connectVertices("upperLeft", "bottomLeft");
    map0.connectVertices("upperLeft", "bottomRight");
    map0.connectVertices("upperLeft", "upperRight");
    map0.connectVertices("bottomLeft", "bottomRight");
    map0.connectVertices("upperRight", "bottomRight");
    Vertex * start = map0.getVertex("upperLeft");
    g.currVertex = g2.currVertex = g3.currVertex = start;
    guards.erase(&g3);
    //patrol(map0, guards);

    Graph map3;
    map3.addVertex("leftmostRoom",0);
    map3.addVertex("roomHallway");
    map3.addVertex("leftVerticalHallway");
    map3.addVertex("leftBottomRoom");
    map3.addVertex("bottomRoomLeftSide");
    map3.addVertex("bottomRoomRightSide");
    map3.addVertex("bottomHorizontalHallway");
    map3.addVertex("bottomRightRoom");
    map3.addVertex("middleRoomLeftSide");
    map3.addVertex("middleRoomRightSide");
    map3.addVertex("upHorizontalHallway");
    map3.addVertex("upRoom");
    map3.addVertex("rightVerticalHallway");
    map3.addVertex("rightRoom");
    map3.connectVertices("leftmostRoom","roomHallway", 50);
    map3.connectVertices("roomHallway", "leftVerticalHallway", 50);
    map3.connectVertices("leftVerticalHallway", "upHorizontalHallway", 100);
    map3.connectVertices("leftVerticalHallway","leftBottomRoom", 80);
    map3.connectVertices("leftVerticalHallway", "bottomRoomLeftSide", 100);
    map3.connectVertices("leftVerticalHallway", "middleRoomLeftSide", 80);
    map3.connectVertices("bottomRoomLeftSide", "bottomRoomRightSide", 50);
    map3.connectVertices("leftBottomRoom", "bottomHorizontalHallway", 100);
    map3.connectVertices("bottomHorizontalHallway", "bottomRoomLeftSide", 100);
    map3.connectVertices("bottomHorizontalHallway", "leftVerticalHallway", 100);
    map3.connectVertices("middleRoomRightSide", "bottomHorizontalHallway", 80);
    map3.connectVertices("bottomHorizontalHallway", "bottomRightRoom", 80);
    map3.connectVertices("middleRoomLeftSide", "middleRoomRightSide", 50);
    map3.connectVertices("bottomHorizontalHallway", "rightVerticalHallway", 100);
    map3.connectVertices("upHorizontalHallway", "upRoom", 50);
    map3.connectVertices("upHorizontalHallway", "rightVerticalHallway", 100);
    map3.connectVertices("rightVerticalHallway", "rightRoom", 80);
    guards.insert(&g3);
    g.currVertex = g2.currVertex = g3.currVertex = map3.getVertex("leftmostRoom");
    map3.suspicion_increase = 5;
    patrol(map3, guards);


    return 0;
}

void patrol(Graph &map, set<Guard *> &guards) {
    for (int i = 0; i < 20; i++) {
        while(map.totalSuspicion() > 0.0) {
            for(Guard *guard : guards) {
                Vertex *target = selectNext(map, guard->currVertex);
                guard->visitVertex(target);
                cout << guard->name << " visited " << target->name << endl;
            }
            cout << endl;
        }
        map.updateSuspicion();
    }
    for(auto const &it : map.vertices) {
        cout << it.first << " visit num: " << it.second->visitNum << endl;
    }
}

Vertex * selectNext(Graph &g, Vertex *currVertex) {
    double bestValue = -std::numeric_limits<float>::max();
    Vertex *bestVertex = NULL;
    for(pair<Vertex *, double> neighbor : currVertex->neighbors) {
        set<Vertex *> used = {currVertex};
        double thisValue = edgeHeuristic(g, used, neighbor.first)/(neighbor.second+ 1.0);
        if(thisValue >= bestValue) {
            bestValue = thisValue;
            bestVertex = neighbor.first;
        }
    }
    return bestVertex;
}

double edgeHeuristic(Graph &g, set<Vertex *> &used, Vertex *currVertex) {
    used.insert(currVertex);
    double heuristic = 0.0;
    for(pair<Vertex *, double> neighbor : currVertex->neighbors) {
        if(used.find(neighbor.first) != used.end()) continue;
        heuristic += edgeHeuristic(g, used, neighbor.first)/(neighbor.second + 1.0);
    }
    heuristic += currVertex->suspicion;
    used.erase(currVertex);
    return heuristic;
}
