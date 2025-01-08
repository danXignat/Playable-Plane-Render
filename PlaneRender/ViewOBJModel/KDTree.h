#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <random>
#include <glm/glm.hpp>

class KDTree {
private:
    struct Comparator {
        bool operator()(const std::pair<double, glm::vec3>& a, const std::pair<double, glm::vec3>& b);
    };
    using pqueue = std::priority_queue < std::pair<double, glm::vec3>, std::vector<std::pair<double, glm::vec3>>, Comparator>;

    struct KDNode {
        glm::vec3 point;
        KDNode* left;
        KDNode* right;

        KDNode(glm::vec3 p);
    };

public:
    KDTree();

    ~KDTree();

    void insert(const glm::vec3& point);

    std::vector<glm::vec3> findClosestPoints(glm::vec3 point, int k = 3);

private:
    KDNode* root;

    KDNode* _insert(KDNode* node, const glm::vec3& point, int depth);

    void _search(KDNode* node, glm::vec3 point, int depth, int k, pqueue& maxHeap);

    void _clear(KDNode* node);
};

inline void testTree() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> dis(1.0, 10.0);

    std::vector<glm::vec3> numbers;
    KDTree tree;

    for (int i = 0; i < 30; i++) {
        glm::vec3 point{ dis(gen), dis(gen), dis(gen) };
        numbers.push_back(point);
        tree.insert(point);
    }
    glm::vec3 point{ 0, 0 ,0 };

    auto print = [point](const glm::vec3& kpoint) {
        std::cout << kpoint.x << " " << kpoint.y << " " << kpoint.z << " " << glm::distance(point, kpoint) << std::endl;
        };

    std::sort(numbers.begin(), numbers.end(),
        [point](const glm::vec3& first, const glm::vec3& second) -> bool {
            return glm::distance(point, first) < glm::distance(point, second);
        });
    print(numbers[0]);
    print(numbers[1]);
    print(numbers[2]);
    std::cout << std::endl;

    auto res = tree.findClosestPoints(point, 3);
    for (const auto& el : res) {
        print(el);
    }
}