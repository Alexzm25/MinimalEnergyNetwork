#pragma once
#include <vector>
#include <utility>
#include <queue>
#include <algorithm>
#include <limits>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include "Sensor.h"

using namespace std;

struct Edge {
    Sensor* from;
    Sensor* to;
    float weight;
    Edge(Sensor* f, Sensor* t, float w) : from(f), to(t), weight(w) {}
    
    bool operator>(const Edge& other) const {
        return weight > other.weight;
    }
};

class WeightedGraph {
private:
    vector<Sensor*> sensors;
    vector<Edge> edges;

    
    float calculateDistance(Sensor* s1, Sensor* s2) {
        float dx = s1->getX() - s2->getX();
        float dy = s1->getY() - s2->getY();
        return sqrt(dx * dx + dy * dy);
    }

    int findSensorIndex(Sensor* sensor) const {
        for (size_t i = 0; i < sensors.size(); i++) {
            if (sensors[i] == sensor) {
                return i;
            }
        }
        return -1;
    }

public:
    void addSensor(Sensor* sensor) {
        sensors.push_back(sensor);
    }
    
    void addConnection(Sensor* from, Sensor* to, float weight) {
        edges.emplace_back(from, to, weight);
        edges.emplace_back(to, from, weight);
    }

    void generateCompleteGraph() {
        edges.clear();
        for (size_t i = 0; i < sensors.size(); i++) {
            for (size_t j = i + 1; j < sensors.size(); j++) {
                float distance = calculateDistance(sensors[i], sensors[j]);
                addConnection(sensors[i], sensors[j], distance);
            }
        }
    }

    const vector<Sensor*>& getSensors() const {
        return sensors;
    }
    
    const vector<Edge>& getEdges() const {
        return edges;
    }
    pair<vector<Edge>, float> primMST() const {
        vector<Edge> mst;
        float totalCost = 0.0f;

        if (sensors.empty()) return make_pair(mst, totalCost);

        vector<bool> inMST(sensors.size(), false);
        priority_queue<Edge, vector<Edge>, greater<Edge>> pq;

        inMST[0] = true;
        
        for (const Edge& edge : edges) {
            if (edge.from == sensors[0]) {
                pq.push(edge);
            }
        }

        while (!pq.empty() && mst.size() < sensors.size() - 1) {
            Edge minEdge = pq.top();
            pq.pop();

            int toIdx = findSensorIndex(minEdge.to);
            if (toIdx == -1 || inMST[toIdx]) continue;

            mst.push_back(minEdge);
            totalCost += minEdge.weight;
            inMST[toIdx] = true;

            for (const Edge& edge : edges) {
                if (edge.from == minEdge.to) {
                    int nextIdx = findSensorIndex(edge.to);
                    if (nextIdx != -1 && !inMST[nextIdx]) {
                        pq.push(edge);
                    }
                }
            }
        }

        return make_pair(mst, totalCost);
    }

    bool saveResults(const string& filename, const vector<Edge>& mst, float totalCost) const {
        ofstream file(filename);
        if (!file.is_open()) return false;

        file << "=================================================\n";
        file << "  CONEXION OPTIMA ENTRE SENSORES INTELIGENTES\n";
        file << "=================================================\n\n";

        file << "Total de sensores: " << sensors.size() << "\n\n";

        file << "Sensores instalados:\n";
        file << "-------------------------------------------------\n";
        for (size_t i = 0; i < sensors.size(); i++) {
            file << (i + 1) << ". " << sensors[i]->getName()
                 << " (Posicion: " << sensors[i]->getX() 
                 << ", " << sensors[i]->getY() << ")\n";
        }

        file << "\n=================================================\n";
        file << "  ARBOL DE EXPANSION MINIMA (Algoritmo de Prim)\n";
        file << "=================================================\n\n";

        file << "Conexiones seleccionadas para la red optima:\n";
        file << "-------------------------------------------------\n";
        for (size_t i = 0; i < mst.size(); i++) {
            file << (i + 1) << ". " << mst[i].from->getName() 
                 << " <--> " << mst[i].to->getName()
                 << " (Costo energetico: " << mst[i].weight << " unidades)\n";
        }

        file << "\n=================================================\n";
        file << "COSTO TOTAL DE ENERGIA: " << totalCost << " unidades\n";
        file << "=================================================\n";

        file.close();
        return true;
    }

    
    bool loadSensorsFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) return false;

        sensors.clear();
        edges.clear();

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name;
            float x, y;
            char comma;

            if (getline(ss, name, ',') && ss >> x >> comma >> y) {
                Sensor* newSensor = new Sensor(x, y, name);
                addSensor(newSensor);
            }
        }

        file.close();
        return true;
    }

    bool saveSensorsToFile(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) return false;

        for (const Sensor* sensor : sensors) {
            file << sensor->getName() << "," 
                 << sensor->getX() << "," 
                 << sensor->getY() << "\n";
        }

        file.close();
        return true;
    }

    void clear() {
        for (Sensor* sensor : sensors) {
            delete sensor;
        }
        sensors.clear();
        edges.clear();
    }
};
