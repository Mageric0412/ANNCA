
#include "Roadmap.h"
#ifdef _OPENMP
#include <omp.h>
#endif



#ifndef M_PI
const float M_PI = 3.14159265358979323846f;
#endif

class RoadmapVertex {
public:
	RVO::Vector2 position;
	std::vector<int> neighbors;
	std::vector<float> distToGoal;
};

/* Store the roadmap. */
std::vector<RoadmapVertex> roadmap;

/* Store the goals of the agents. */
std::vector<RVO::Vector2> goals;


void setupScenario(RVO::RVOSimulator *sim)
{
	/* Seed the random number generator. */
	std::srand(static_cast<unsigned int>(std::time(0)));

	/* Specify the global time step of the simulation. */
	sim->setTimeStep(0.4f);

	/* Specify the default parameters for agents that are subsequently added. */
	sim->setAgentDefaults(15.0f, 6, 10.0f, 5.0f, 2.0f, 3.0f);

	/*
	 * Add agents, specifying their start position, and store their goals on the
	 * opposite side of the environment.
	 */
	for (size_t i = 0; i < 5; ++i) {
		for (size_t j = 0; j < 5; ++j) {
			sim->addAgent(RVO::Vector2(55.0f + i * 10.0f,  55.0f + j * 10.0f));
			goals.push_back(RVO::Vector2(-55.0f -i * 10.0f, -55.0f -j * 10.0f));

			sim->addAgent(RVO::Vector2(-55.0f - i * 10.0f,  55.0f + j * 10.0f));
			goals.push_back(RVO::Vector2(55.0f + i * 10.0f, -55.0f - j * 10.0f));

			sim->addAgent(RVO::Vector2(55.0f + i * 10.0f, -55.0f - j * 10.0f));
			goals.push_back(RVO::Vector2(-55.0f - i * 10.0f,  55.0f + j * 10.0f));

			sim->addAgent(RVO::Vector2(-55.0f - i * 10.0f, -55.0f - j * 10.0f));
			goals.push_back(RVO::Vector2(55.0f + i * 10.0f,  55.0f + j * 10.0f));
		}
	}

	/*
	 * Add (polygonal) obstacles, specifying their vertices in counterclockwise
	 * order.
	 */
	std::vector<RVO::Vector2> obstacle1, obstacle2, obstacle3, obstacle4;

	obstacle1.push_back(RVO::Vector2(-10.0f, 40.0f));
	obstacle1.push_back(RVO::Vector2(-40.0f, 40.0f));
	obstacle1.push_back(RVO::Vector2(-40.0f, 10.0f));
	obstacle1.push_back(RVO::Vector2(-10.0f, 10.0f));

	obstacle2.push_back(RVO::Vector2(10.0f, 40.0f));
	obstacle2.push_back(RVO::Vector2(10.0f, 10.0f));
	obstacle2.push_back(RVO::Vector2(40.0f, 10.0f));
	obstacle2.push_back(RVO::Vector2(40.0f, 40.0f));

	obstacle3.push_back(RVO::Vector2(10.0f, -40.0f));
	obstacle3.push_back(RVO::Vector2(40.0f, -40.0f));
	obstacle3.push_back(RVO::Vector2(40.0f, -10.0f));
	obstacle3.push_back(RVO::Vector2(10.0f, -10.0f));

	obstacle4.push_back(RVO::Vector2(-10.0f, -40.0f));
	obstacle4.push_back(RVO::Vector2(-10.0f, -10.0f));
	obstacle4.push_back(RVO::Vector2(-40.0f, -10.0f));
	obstacle4.push_back(RVO::Vector2(-40.0f, -40.0f));

	sim->addObstacle(obstacle1);
	sim->addObstacle(obstacle2);
	sim->addObstacle(obstacle3);
	sim->addObstacle(obstacle4);

	/* Process the obstacles so that they are accounted for in the simulation. */
	sim->processObstacles();
}

void setupScenario2(RVO::RVOSimulator *sim)
{
     float  yoffense=12.0f;
    float    xoffense=12.0f;
	int num=5;
	std::srand(static_cast<unsigned int>(std::time(0)));

	/* Specify the global time step of the simulation. */
	sim->setTimeStep(0.25f);

	/* Specify the default parameters for agents that are subsequently added. */
	sim->setAgentDefaults(15.0f, 6, 10.0f, 5.0f, 2.5f, 3.0f);

	/*
	 * Add agents, specifying their start position, and store their goals on the
	 * opposite side of the environment.
	 */
	for (size_t i = 0; i < num; ++i) {
		for (size_t j = 0; j < num; ++j) {
			sim->addAgent(RVO::Vector2(-105.0f - i * xoffense,4 + j * yoffense));
			goals.push_back(RVO::Vector2(105.0f + i * xoffense, 4 +j * yoffense));

			

			sim->addAgent(RVO::Vector2(105.0f + i * xoffense, 4 + j * yoffense));
			goals.push_back(RVO::Vector2(-105.0f -i *xoffense, 4 + j * yoffense));
			sim->addAgent(RVO::Vector2(-105.0f -i * xoffense, -4 - j * yoffense));
			goals.push_back(RVO::Vector2(105.0f + i * xoffense, -4 - j * yoffense));
			sim->addAgent(RVO::Vector2(105.0f + i * xoffense, -4 - j * yoffense));
			goals.push_back(RVO::Vector2(-105.0f - i * xoffense, -4 - j * yoffense));
		}
	}

	/*
	 * Add (polygonal) obstacles, specifying their vertices in counterclockwise
	 * order.
	 */
	std::vector<RVO::Vector2> obstacle1, obstacle2;

	/*obstacle1.push_back(RVO::Vector2(100.0f, 33.3f));
	obstacle1.push_back(RVO::Vector2(-100.0f, 33.3f));
	obstacle1.push_back(RVO::Vector2(-100.0f, 20.0f));
	obstacle1.push_back(RVO::Vector2(100.0f, 20.0f));

	obstacle2.push_back(RVO::Vector2(100.0f, -20.0f));
	obstacle2.push_back(RVO::Vector2(-100.0f, -20.0f));
	obstacle2.push_back(RVO::Vector2(-100.0f, -33.3f));
	obstacle2.push_back(RVO::Vector2(100.0f, -33.3f));


	sim->addObstacle(obstacle1);
	sim->addObstacle(obstacle2);*/

	/* Process the obstacles so that they are accounted for in the simulation. */
	//sim->processObstacles();

}


void setupScenario3(RVO::RVOSimulator *sim,float angl)
{
    float   xoffense=40.0f,offense=18;
	double Pi=3.141592653;	
	int num=2;
	float xo=cosf(Pi/180*angl)*xoffense;
	float yo=sinf(Pi/180*angl)*xoffense;
	std::srand(static_cast<unsigned int>(std::time(0)));

	/* Specify the global time step of the simulation. */
	sim->setTimeStep(0.25f);

	/* Specify the default parameters for agents that are subsequently added. */
	sim->setAgentDefaults(15.0f, 6, 10.0f, 5.0f, 2.5f, 3.0f);
	
	//for (size_t i = 0; i < num; ++i) {
	//	for (size_t j = 0; j < num; ++j) {

	sim->addAgent(RVO::Vector2(xoffense ,0));
	sim->addAgent(RVO::Vector2(-xoffense ,0));
	sim->addAgent(RVO::Vector2(xoffense - 4,  offense+3));
	sim->addAgent(RVO::Vector2(-xoffense -4,offense+3));
	sim->addAgent(RVO::Vector2(xoffense +5,  -offense));
	sim->addAgent(RVO::Vector2(-xoffense -10,-offense));
		//}
//	}
	
			//sim->addAgent(RVO::Vector2(-xo, yo));
			//sim->addAgent(RVO::Vector2(-xoffense,  0));

			//goals.push_back(RVO::Vector2(xo, -yo));

			//sim->addAgent(RVO::Vector2(- xoffense,  0));
			//goals.push_back(RVO::Vector2( xoffense, 0));

			
			//goals.push_back(RVO::Vector2(40,  -0));

			//sim->addAgent(RVO::Vector2(xoffense, -  yoffense));
			//goals.push_back(RVO::Vector2( - xoffense,- yoffense));
	


}

void updateVisualization(RVO::RVOSimulator *sim)
{
	/* Output the current global time. */
	std::cout << sim->getGlobalTime();

	/* Output the current position of all the agents. */
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		std::cout << " " << sim->getAgentPosition(i);
	}

	std::cout << std::endl;
}

void setPreferredVelocities(RVO::RVOSimulator *sim,std::vector<RVO::Vector2> goals)
{
	/*
	 * Set the preferred velocity to be a vector of unit magnitude (speed) in the
	 * direction of the goal.
	 */

#ifdef _OPENMP
#pragma omp parallel for
#endif
	
	for (int i = 0; i < static_cast<int>(sim->getNumAgents()); ++i) {
		
		//此处进行添加速度改变方式
		
		RVO::Vector2 goalVector = goals[i] - sim->getAgentPosition(i);
		
		if (RVO::absSq(goalVector) > 1.0f) {
			goalVector = RVO::normalize(goalVector);
			//RVO::Vector2 cvec=RVO::Vector2(1,1);
			//goalVector=goalVector+cvec;
		}

		sim->setAgentPrefVelocity(i, goalVector);

		/*
		 * Perturb a little to avoid deadlocks due to perfect symmetry.
		 */
		float angle = std::rand() * 2.0f * M_PI / RAND_MAX;
		float dist = std::rand() * 0.01f / RAND_MAX;

		sim->setAgentPrefVelocity(i, sim->getAgentPrefVelocity(i) +
		                          dist * RVO::Vector2(std::cos(angle), std::sin(angle)));
	}

}

void setPreferredVelocities(RVO::RVOSimulator *sim,int i,std::vector<RVO::Vector2> goals)
{
	/*
	 * Set the preferred velocity to be a vector of unit magnitude (speed) in the
	 * direction of the goal.
	 */

#ifdef _OPENMP
#pragma omp parallel for
#endif
	
	
		RVO::Vector2 goalVector = goals[i] - sim->getAgentPosition(i);
		
		if (RVO::absSq(goalVector) > 1.0f) {
			goalVector = RVO::normalize(goalVector);
			//RVO::Vector2 cvec=RVO::Vector2(1,1);
			//goalVector=goalVector+cvec;
		}

		sim->setAgentPrefVelocity(i, goalVector);

		/*
		 * Perturb a little to avoid deadlocks due to perfect symmetry.
		 */
		float angle = std::rand() * 2.0f * M_PI / RAND_MAX;
		float dist = std::rand() * 0.01f / RAND_MAX;

		sim->setAgentPrefVelocity(i, sim->getAgentPrefVelocity(i) +
		                          dist * RVO::Vector2(std::cos(angle), std::sin(angle)));
	

}

void setMultiPreferredVelocities(RVO::RVOSimulator *sim,int i,std::vector<RVO::Vector2> vec)
{
#ifdef _OPENMP
#pragma omp parallel for
#endif
		if (RVO::absSq(vec[i]) > 1.0f) {
			vec[i] = RVO::normalize(vec[i]);
			//RVO::Vector2 cvec=RVO::Vector2(1,1);
			//goalVector=goalVector+cvec;
		}
		sim->setAgentPrefVelocity(i, vec[i]);
		float angle = std::rand() * 2.0f * M_PI / RAND_MAX;
		float dist = std::rand() * 0.01f / RAND_MAX;
		sim->setAgentPrefVelocity(i, sim->getAgentPrefVelocity(i) +dist * RVO::Vector2(std::cos(angle), std::sin(angle)));
}

bool reachedGoal(RVO::RVOSimulator *sim,std::vector<RVO::Vector2> goals)
{
	/* Check if all agents have reached their goals. */
	for (size_t i = 0; i < sim->getNumAgents(); ++i) {
		if (RVO::absSq(sim->getAgentPosition(i) - goals[i]) > 1.5f * 1.5f) {
			return false;
		}
	}

	return true;
}