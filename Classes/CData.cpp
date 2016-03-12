#include "CData.h"



//训练集
const double ANNTrainData[ANGLE_PATTERNS][NUM_VECIN*2]=
{
	0.939,0.342, 0.939,0.342,0.939,0.342, 0.939,0.342,  0.939,0.342,  0.939,0.342,  //20度

	0.866,0.5, 0.866,0.5, 0.866,0.5, 0.866,0.5, 0.866,0.5, 0.866,0.5,    //30度

	0.707,0.707, 0.707,0.707,0.707,0.707, 0.707,0.707, 0.707,0.707, 0.707,0.707,   //45度

	0.584,0.812,0.584,0.812,0.584,0.812,0.584,0.812, 0.584,0.812, 0.584,0.812,     //分水岭54.3度

	0.5,0.866, 0.5,0.866, 0.5,0.866, 0.5,0.866, 0.5,0.866, 0.5,0.866,  //60度

	0.259,0.966, 0.259,0.966,0.259,0.966, 0.259,0.966, 0.259,0.966, 0.259,0.966,     //75度

	0,1, 0,1, 0,1, 0,1,  0,1,  0,1,   //90度

	-0.259,0.996, -0.259,0.996,-0.259,0.996, -0.259,0.996, -0.259,0.996,-0.259,0.996,   //105度

	-0.5,0.866, -0.5,0.866,-0.5,0.866,-0.5,0.866, -0.5,0.866,-0.5,0.866,//120度

	-0.707,0.707, -0.707,0.707, -0.707,0.707, -0.707,0.707, -0.707,0.707, -0.707,0.707, //135度

	-0.866,0.5,-0.866,0.5,-0.866,0.5, -0.866,0.5,-0.866,0.5,-0.866,0.5,//150度

	-0.966,0.259, -0.966,0.259, -0.966,0.259, -0.966,0.259, -0.966,0.259, -0.966,0.259, //165度

	-1,0, -1,0,-1,0,-1,0, -1,0,-1,0,   //180度

}; 
//训练集输出
const double Classifier[ANGLE_PATTERNS][NUM_VECOUT]=
{
	8.6565,7.9827,1.0474,0.8723,//20

	8.5571,6.4580,1.1188,0.8032,//30

	9.1624,2.4132,1.1574,0.7445,//45

	10.2452,0.0396,1.1697,0.7304,//54.3

	9.9226,1.2410,1.1631,0.7273,//60

	10.9472,5.3067,1.1625,0.7268,//75

	12.3767,7.0980,1.1520,0.7370,//90

	12.9160,9.0594,1.1263,0.7663,//105

	14.9214,12.5060,1.1017,0.7752,//120

	16.8136,15.2285,1.0613,0.7957,//135

	18.3444,17.2689,1.0208,0.8293,//150

	19.1562,18.9033,0.9663,0.8820,//165

	20.2038,20.0835,0.9218,0.9174,//180

};


void Crowd_TData::InitData()
{
	//每一组数据
	for (int ptn=0; ptn<AnglePatterns; ptn++)
	{
		
		vector<double> AngleTemp;
		vector<double> AngleTemp2;
		for (int v=0; v<m_iVectorSize*2; ++v)
		{
			AngleTemp.push_back(ANNTrainData[ptn][v]);
		}

		m_vecPatterns.push_back(AngleTemp);

		for (int i=0;i<NUM_VECOUT;i++)
		{
			AngleTemp2.push_back(Classifier[ptn][i]);
		}
		//输出向量的具体选择
	    m_Vecout.push_back(AngleTemp2);
	}

}


void Crowd_TData::CreatANN_TrainSet()
{
	//清空向量
	ANN_SetIn.clear();
	ANN_SetOut.clear();

	
	for (int ptn=0; ptn<AnglePatterns; ++ptn)
	{    
		
		ANN_SetIn.push_back(m_vecPatterns[ptn]);
	
		vector<double> outputs(AnglePatterns, 0);
	
		//输出向量选择
		outputs[ptn] = 1;
		//加入输出向量中
		ANN_SetOut.push_back(outputs);
	}
}

vector<double> Crowd_TData::ClassifyPattern(int val)
{
	if (m_Vecout.size() > 0)
	{
		return m_Vecout[val];
	}

	else
	{

	}
}


bool Crowd_TData::AddTraingData(vector<double> &data, vector<double> New)
{
	m_Vecout.push_back(New);

	//add the data
	m_vecPatterns.push_back(data);
	++AnglePatterns;

	CreatANN_TrainSet();
	return true; 
}

