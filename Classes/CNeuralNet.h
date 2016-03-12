
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

//��ʱ�����ṹ
typedef vector<double> TempVector;


struct CrowdNeuron
{
	//������������Ŀ
	int				      m_iNumInputs;

	//����Ȩ��
	vector<double>	m_vecWeight;

	//��һ��Ȩ�ظ���ֵ
	vector<double>  m_vecPrevUpdate;

	//��Ԫ�ļ���ֵ
	double          m_dActivation;

	//����ֵ
	 double          m_dError;
	//������
	CrowdNeuron(int NumInputs);
};



//---------------------------------------------------------------------
//	������Ĳ�ṹ
//---------------------------------------------------------------------

struct CrowdNeuronLayer
{
	//�ò����Ԫ����
	int					      m_iNumNeurons;

	//����
	vector<CrowdNeuron>		m_vecNeurons;

	CrowdNeuronLayer(int NumNeurons, 
		int NumInputsPerNeuron);
};


//----------------------------------------------------------------------
//	��������
//----------------------------------------------------------------------

class CrowdNeuralNet
{
	
private:
	
	int					m_iNumInputs;

	int					m_iNumOutputs;

	int					m_iNumHiddenLayers;

	int					m_iNeuronsPerHiddenLyr;

	//ѧϰ��
	double      m_dLearningRate;

	//�����ֵ
	 double      m_dErrorSum;

	//�ж��Ƿ����ѵ��
	bool        m_bTrained;

	//ѵ������
	int         m_iNumEpochs;

	//�洢ÿ�����Ԫ�ṹ
	vector<CrowdNeuronLayer>	m_vecLayers;


	//�����˹�������
	void            Create_ANN();

	//��ʼ�����磬�����ֵ
	void            InitialzeANN();

	//sigmoid���ߺ���
	inline double	  Sigmoid(double activation,double response);

public:

	CrowdNeuralNet::CrowdNeuralNet(int    NumInputs,
		int    NumOutputs,
		int    HiddenNeurons,
		double LearningRate);

	//�������
	vector<double>	Update(vector<double> inputs);
	//ѵ������
	bool            ANN_BPTraining(vector<TempVector> &SetIn,
		vector<TempVector> &SetOut);
	//Ȩ�ص���ѵ������
	bool            Train(vector<vector<double>>SetIn,vector<vector<double>> SetOut);

	//�Ƿ�ѵ�����
	bool            Trained(){return m_bTrained;}
	//��ȡ���
	double          Error() {return m_dErrorSum;}



	

};
				

