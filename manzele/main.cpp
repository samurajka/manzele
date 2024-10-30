#include <iostream>
#include <vector>
#include <queue>
#include <set>

using namespace std;

class State {
public:

	vector<int> wives;
	vector<int> husbands;

	int pairCount;

	int boat;

	State(int numOfPairs) {
		wives.resize(numOfPairs);
		fill(wives.begin(), wives.end(), 0);
		husbands.resize(numOfPairs);
		fill(husbands.begin(), husbands.end(), 0);
		pairCount = numOfPairs;
		boat = 0;
	}

	State(const vector<int>& inWives, const vector<int>& inHusbands, const int inBoat) {
		wives = inWives;
		husbands = inHusbands;
		boat = inBoat;
		pairCount = wives.size();
	}
	

	bool isValid(State s) const {
		bool wifeAlone = false;
		bool husbandAlone = false;
		for (int i = 0; i < s.pairCount;i++) {
			if (s.wives[i]==0 and s.husbands[i] == 1) {
				wifeAlone = true;
			}
			if (s.husbands[i] == 0 and s.wives[i] == 1) {
				husbandAlone = true;
			}
		}

		return !(wifeAlone and husbandAlone);
	}

	bool isFinal(State s) const {
		for (int i = 0;i < s.pairCount;i++) {
			if (s.wives[i] == 0) { return false; }
			if (s.husbands[i] == 0) { return false; }
		}
		return true;
	}

	vector<State> generateChildren() const {
		vector<State> result;

		State startingState = State(wives, husbands, boat);
		
		State potentialState = startingState;


		//husband jede sam
		for (int i = 0; i < pairCount; i++) {
			potentialState = startingState;
			if (potentialState.husbands[i] == potentialState.boat) {
				potentialState.husbands[i] = 1 - potentialState.husbands[i];
				potentialState.boat = 1 - potentialState.boat;
				if (isValid(potentialState)) {
					State newState = potentialState;
					result.push_back(newState);
				}
			}
		}

		//wife jede sama
		for (int i = 0; i < pairCount; i++) {
			potentialState = startingState;
			if (potentialState.wives[i] == potentialState.boat) {
				potentialState.wives[i] = 1 - potentialState.wives[i];
				potentialState.boat = 1 - potentialState.boat;
				if (isValid(potentialState)) {
					State newState = potentialState;
					result.push_back(newState);
				}
			}
		}

		//husband jede se svou wife
		for (int i = 0; i < pairCount; i++)
		{
			potentialState = startingState;
			if (potentialState.wives[i] == potentialState.husbands[i] and potentialState.wives[i] == potentialState.boat) {
				potentialState.husbands[i] = 1 - potentialState.husbands[i];
				potentialState.wives[i] = 1 - potentialState.wives[i];
				potentialState.boat = 1 - potentialState.boat;
				if (isValid(potentialState)) {
					State newState = potentialState;
					result.push_back(newState);
				}
			}
		}



		return result;
	}

	bool operator<(const State& other) const {
		vector<int> original;
		original = wives;
		original.insert(original.end(), husbands.begin(), husbands.end());
		original.push_back(boat);

		vector<int> second;
		second = other.wives;
		second.insert(second.end(), other.husbands.begin(), other.husbands.end());
		second.push_back(other.boat);

	 	return original < second;
	}
	
};


int transportCountForNPairs(int n) {
	queue<State> BFS_q;
	set<State> visited;

	State initialState(n);

	BFS_q.push(initialState);
	visited.insert(initialState);

	int counter = 0;
	bool resultFound = false;


	while (!BFS_q.empty() and !resultFound) {
		counter++;
		size_t length = BFS_q.size();
		for (size_t i = 0; i < length and !resultFound; i++) {
			State currentState = BFS_q.front();
			BFS_q.pop();
			auto children = currentState.generateChildren();
			for (const auto& child : children) {
				if (child.isFinal(child)) {
					resultFound = true;
					break;
				}
				if (visited.find(child) == visited.end()) {
					BFS_q.push(child);
					visited.insert(child);
				}
			}

		}
	}

	return counter;
}


int main() {

	//test
	for (int i = 2; i < 5; i++) {
		cout << i << " pairs - " << transportCountForNPairs(i) << " transports" << endl;
	}

	return 0;
}