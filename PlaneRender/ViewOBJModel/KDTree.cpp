#include "KDTree.h"

KDTree::KDTree() : root(nullptr) {}

KDTree::~KDTree() {
	_clear(root);
}

///-------------------------------------------

void KDTree::insert(const glm::vec3& point) {
	root = _insert(root, point, 0);
}

std::vector<glm::vec3> KDTree::findClosestPoints(glm::vec3 point, int k) {
	pqueue maxHeap;
	_search(root, point, 0, k, maxHeap);

	std::vector<glm::vec3> result;
	while (!maxHeap.empty()) {
		result.push_back(maxHeap.top().second);
		maxHeap.pop();
	}
	return result;
}

///---------------------------------------------

KDTree::KDNode* KDTree::_insert(KDNode* node, const glm::vec3& point, int depth) {
	if (!node) {
		return new KDNode(point);
	}

	int axis = depth % 3;
	bool axisX{ axis == 0 && point.x < node->point.x };
	bool axisY{ axis == 1 && point.y < node->point.y };
	bool axisZ{ axis == 2 && point.z < node->point.z };

	if (axisX || axisY || axisZ) {
		node->left = _insert(node->left, point, depth + 1);
	}
	else {
		node->right = _insert(node->right, point, depth + 1);
	}

	return node;
}

void KDTree::_search(KDNode* node, glm::vec3 queryPoint, int depth, int k, pqueue& maxHeap) {
	if (node == nullptr) {
		return;
	}

	double dx = queryPoint.x - node->point.x;
	double dy = queryPoint.y - node->point.y;
	double dz = queryPoint.z - node->point.z;
	double distanceSquared = dx * dx + dy * dy + dz * dz;

	if (maxHeap.size() < k) {
		maxHeap.emplace(distanceSquared, node->point);
	}
	else if (distanceSquared < maxHeap.top().first) {
		maxHeap.pop();
		maxHeap.emplace(distanceSquared, node->point);
	}

	int axis = depth % 3;
	double diff;
	switch (axis) {
	case 0: diff = queryPoint.x - node->point.x; break;
	case 1: diff = queryPoint.y - node->point.y; break;
	default: diff = queryPoint.z - node->point.z; break;
	}

	KDNode* nearChild = (diff < 0) ? node->left : node->right;
	KDNode* farChild = (diff < 0) ? node->right : node->left;

	_search(nearChild, queryPoint, depth + 1, k, maxHeap);
	double axisDistSquared = diff * diff;
	if (maxHeap.size() < k || axisDistSquared < maxHeap.top().first) {
		_search(farChild, queryPoint, depth + 1, k, maxHeap);
	}
}

void KDTree::_clear(KDNode* node) {
	if (!node) {
		return;
	}

	_clear(node->left);
	_clear(node->right);

	delete node;
}

///------------------------------INNER CLASSES------------------------------------

bool KDTree::Comparator::operator()(const std::pair<double, glm::vec3>& a, const std::pair<double, glm::vec3>& b) {
	return a.first < b.first;
}

KDTree::KDNode::KDNode(glm::vec3 p) : 
	point(p), left(nullptr), right(nullptr) 
{}