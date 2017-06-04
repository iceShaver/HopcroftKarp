#include <cstdio>
#include <climits>
#include "Vector.h"
#include "Queue.h"

typedef unsigned short DataType; //change correspond format speciefiers in scanf, printf and INF const!!!
typedef Vector<DataType>::Iterator Iterator;
const DataType INF = USHRT_MAX; //UINT_MAX

bool bfs(Vector<DataType>&pairPeople, Vector<DataType>&pairBikes, Vector<DataType>&people, Vector<DataType>&bikes, Vector<DataType>&distances, Vector<Vector<DataType>>&choices) {
	Queue<DataType> queue;
	for (Iterator person = people.begin(); person != people.end(); ++person)
		if (pairPeople[*person] == NULL) {
			distances[*person] = 0;
			queue.push(*person);
		}
		else
			distances[*person] = INF;
	distances[NULL] = INF;
	while (!queue.empty()) {
		DataType person = queue.pop();
		if (distances[person] < distances[NULL])
			for (Iterator bike = choices[person].begin(); bike != choices[person].end(); ++bike)
				if (distances[pairBikes[*bike]] == INF) {
					distances[pairBikes[*bike]] = distances[person] + 1;
					queue.push(pairBikes[*bike]);
				}
	}
	return distances[NULL] != INF;
}

bool dfs(DataType person, Vector<DataType>&pairPeople, Vector<DataType>&pairBikes, Vector<DataType>&distances, Vector<Vector<DataType>>&choices) {
	if (person != NULL) {
		for (Iterator bike = choices[person].begin(); bike != choices[person].end(); ++bike)
			if (distances[pairBikes[*bike]] == distances[person] + 1)
				if (dfs(pairBikes[*bike], pairPeople, pairBikes, distances, choices)) {
					pairBikes[*bike] = person;
					pairPeople[person] = *bike;
					return true;
				}
		distances[person] = INF;
		return false;
	}
	return true;
}

int hopcroft_karp(Vector<DataType>&pairPeople, Vector<DataType>&pairBikes, Vector<DataType>&people, Vector<DataType>&bikes, Vector<DataType>&distances, Vector<Vector<DataType>>&choices) {

	for (Iterator u = people.begin(); u != people.end(); ++u)
		pairPeople[*u] = NULL;
	for (Iterator v = bikes.begin(); v != bikes.end(); ++v)
		pairBikes[*v] = NULL;
	DataType matching = 0;
	while (bfs(pairPeople, pairBikes, people, bikes, distances, choices))
		for (Iterator u = people.begin(); u != people.end(); ++u)
			if (pairPeople[*u] == NULL)
				if (dfs(*u, pairPeople, pairBikes, distances, choices))
					++matching;
	return matching;
}

int main(int argc, char* argv[]) {
	DataType peopleNumber, bikesNumber, assignmentsNumber, bike;
	scanf("%hu%hu", &peopleNumber, &bikesNumber);
	Vector<DataType> pairPeople;
	Vector<DataType> pairBikes;
	Vector<DataType> people(peopleNumber);
	Vector<DataType> bikes(bikesNumber);
	Vector<DataType> distances;
	Vector<Vector<DataType>> choices(peopleNumber + 1);
	for (int i = 1; i <= bikesNumber; ++i)
		bikes.pushLast(i);
	for (DataType person = 1; person <= peopleNumber; ++person) {
		people.pushLast(person);
		scanf("%hu", &assignmentsNumber);
		for (int assignment = 0; assignment < assignmentsNumber; ++assignment) {
			scanf("%hu", &bike);
			choices[person].pushLast(bike);
		}
	}
	printf("%hu", hopcroft_karp(pairPeople, pairBikes, people, bikes, distances, choices));
	return 0;
}
