
#include <vector>
#include <math.h>
#include <fstream>
#include <windows.h>
#include "defines.h"
#include "CData.h"
#include "utils.h"
#include "CParams.h"
#pragma once

using namespace std;

//临时向量结构
typedef vector<double> TempVector;


struct CrowdNeuron
{
	//神经网络输如数目
	int				      m_iNumInputs;

	//输入权重
	vector<double>	m_vecWeight;

	//上一次权重更新值
	vector<double>  m_vecPrevUpdate;

	//神经元的激活值
	double          m_dActivation;

	//错误值
	 double          m_dError;
	//构造体
	CrowdNeuron(int NumInputs);
};



//---------------------------------------------------------------------
//	神经网络的层结构
//---------------------------------------------------------------------

struct CrowdNeuronLayer
{
	//该层的神经元个数
	int					      m_iNumNeurons;

	//层数
	vector<CrowdNeuron>		m_vecNeurons;

	CrowdNeuronLayer(int NumNeurons, 
		int NumInputsPerNeuron);
};


//----------------------------------------------------------------------
//	神经网络类
//----------------------------------------------------------------------

class CrowdNeuralNet
{
	
private:
	
	int					m_iNumInputs;

	int					m_iNumOutputs;

	int					m_iNumHiddenLayers;

	int					m_iNeuronsPerHiddenLyr;

	//学习率
	double      m_dLearningRate;

	//误差总值
	 double      m_dErrorSum;

	//判断是否完成训练
	bool        m_bTrained;

	//训练代数
	int         m_iNumEpochs;

	//存储每层的神经元结构
	vector<CrowdNeuronLayer>	m_vecLayers;


	//生成人工神经网络
	void            Create_ANN();

	//初始化网络，随机赋值
	void            InitialzeANN();

	//sigmoid曲线函数
	inline double	  Sigmoid(double activation,double response);

public:

	CrowdNeuralNet::CrowdNeuralNet(int    NumInputs,
		int    NumOutputs,
		int    HiddenNeurons,
		double LearningRate);

	//网络更新
	vector<double>	Update(vector<double> inputs);
	//训练过程
	bool            ANN_BPTraining(vector<TempVector> &SetIn,
		vector<TempVector> &SetOut);
	//权重调节训练过程
	bool            Train(vector<vector<double>>SetIn,vector<vector<double>> SetOut);

	//是否训练完毕
	bool            Trained(){return m_bTrained;}
	//获取误差
	double          Error() {return m_dErrorSum;}



	

};
				

