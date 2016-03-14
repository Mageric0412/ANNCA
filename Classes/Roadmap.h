#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <vector>
#include "RVO.h"


void setupScenario(RVO::RVOSimulator *sim);


void setupScenario2(RVO::RVOSimulator *sim);

void setupScenario3(RVO::RVOSimulator *sim,float angl);

void buildRoadmap(RVO::RVOSimulator *sim);

void updateVisualization(RVO::RVOSimulator *sim);

void setPreferredVelocities(RVO::RVOSimulator *sim);

bool reachedGoal(RVO::RVOSimulator *sim,std::vector<RVO::Vector2> goals);

bool reachedGoal(RVO::RVOSimulator *sim);

void setPreferredVelocities(RVO::RVOSimulator *sim,int i,std::vector<RVO::Vector2> goals);

void setMultiPreferredVelocities(RVO::RVOSimulator *sim,int i,std::vector<RVO::Vector2> vec);