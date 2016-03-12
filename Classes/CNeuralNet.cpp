#include "CNeuralNet.h"
#include <windows.h>


//*************************** �����緽��**********************
//
//---------------------------------------------------------------------
CrowdNeuron::CrowdNeuron(int NumInputs): m_iNumInputs(NumInputs+1),m_dActivation(0),m_dError(0)
											
{
	//Ȩ��Ҫ��һ��������+1
	for (int i=0; i<NumInputs+1; i++)
	{
		//�����Ȩ��
		m_vecWeight.push_back(RandomClamped());
		m_vecPrevUpdate.push_back(0);
	}
}




//************************ �񾭲㷽�� **********************

//-----------------------------------------------------------------------
//	���������
//-----------------------------------------------------------------------
CrowdNeuronLayer::CrowdNeuronLayer(int NumNeurons, 
                           int NumInputsPerNeuron):	m_iNumNeurons(NumNeurons)
{
	for (int i=0; i<NumNeurons; i++)

		m_vecNeurons.push_back(CrowdNeuron(NumInputsPerNeuron));
}




//************************ �������๹�췽�� ************************

CrowdNeuralNet::CrowdNeuralNet(int NumInputs,
					   int NumOutputs,
					   int HiddenNeurons,
					   double LearningRate) : m_iNumInputs(NumInputs),
					                         m_iNumOutputs(NumOutputs),
					                         m_iNumHiddenLayers(1),
					                         m_iNeuronsPerHiddenLyr(HiddenNeurons),
					                         m_dLearningRate(LearningRate),
					                         m_dErrorSum(9999),
					                         m_bTrained(false),
					                         m_iNumEpochs(0)
{


	Create_ANN();
	InitialzeANN();
}


//------------------------------����������ṹ------------------------------

void CrowdNeuralNet::Create_ANN()
{
	//���������
	if (m_iNumHiddenLayers > 0)
	{
		//�������ز�
	  m_vecLayers.push_back(CrowdNeuronLayer(m_iNeuronsPerHiddenLyr, m_iNumInputs));
       
	  //���ɶ�����ز�
      for (int i=0; i<m_iNumHiddenLayers-1; i++)
       {

			m_vecLayers.push_back(CrowdNeuronLayer(m_iNeuronsPerHiddenLyr, m_iNeuronsPerHiddenLyr));
       }

    //��������-�����
	  m_vecLayers.push_back(CrowdNeuronLayer(m_iNumOutputs, m_iNeuronsPerHiddenLyr));
	}

  else
  {
	  //���������
	  m_vecLayers.push_back(CrowdNeuronLayer(m_iNumOutputs, m_iNumInputs));
  }
}

//��ʼ��������
void CrowdNeuralNet::InitialzeANN()
{
	for(int i=0;i<m_iNumHiddenLayers+1;i++)
	{
		for (int n=0;n<m_vecLayers[i].m_iNumNeurons;n++)
		{
			for (int k=0;k<m_vecLayers[i].m_vecNeurons[n].m_iNumInputs;k++)
			{
				m_vecLayers[i].m_vecNeurons[n].m_vecWeight[k]=RandomClamped();
			}
		}
	}
	m_dErrorSum=9999;
	m_iNumEpochs=0;

}

//-------------------------------����������-----------------------------------

vector<double> CrowdNeuralNet::Update(vector<double> inputs)
{
	//�洢���
	vector<double> outputs;

	int cWeight = 0;

	//�������
	if (inputs.size() != m_iNumInputs)
	{
		//���ؿ��������
		return outputs;
	}

	//����ÿһ��
	for (int i=0; i<m_iNumHiddenLayers + 1; ++i)
	{

		if ( i > 0 )
		{
			inputs = outputs;
		}

		outputs.clear();

		cWeight = 0;

		//��Ԫ���Ȩֵ���ݹ���
		for (int n=0; n<m_vecLayers[i].m_iNumNeurons; ++n)
		{
			double netinput = 0.0f;

			int	NumInputs = m_vecLayers[i].m_vecNeurons[n].m_iNumInputs;

			for (int k=0; k<NumInputs - 1; ++k)
			{
				//Ȩ��������ĳ˻�
				netinput += m_vecLayers[i].m_vecNeurons[n].m_vecWeight[k] * 
					inputs[cWeight++];
			}

			netinput += m_vecLayers[i].m_vecNeurons[n].m_vecWeight[NumInputs-1] * 
				BIAS;
	     //����ֵ����
			m_vecLayers[i].m_vecNeurons[n].m_dActivation = 
				Sigmoid(netinput, ACTIVATION_RESPONSE);

			//�洢���
			outputs.push_back(m_vecLayers[i].m_vecNeurons[n].m_dActivation);

			cWeight = 0;
		}
	}

	return outputs;
}

//ѵ������
bool CrowdNeuralNet::ANN_BPTraining(vector<TempVector> &SetIn,
                                      vector<TempVector> &SetOut)
{
  //���ɵ�����
  vector<double>::iterator  curWeight;
  vector<CrowdNeuron>::iterator curNrnOut, curNrnHid;

  double WeightUpdate = 0;

  //��ʼ�����
  m_dErrorSum = 0;

  //ʹ��ÿ�����룬����Ȩ��
  for (int vec=0; vec<SetIn.size(); ++vec)
  {
    //��������һ��������
    vector<double> outputs = Update(SetIn[vec]);

    //������
    if (outputs.size() == 0)
    {
      return false;
    }

    //���ڹ���
    for (int op=0; op<m_iNumOutputs; ++op)
    {
      double err = (SetOut[vec][op] - outputs[op]) * outputs[op]
                   * (1 - outputs[op]);

      //���������
      m_dErrorSum += (SetOut[vec][op] - outputs[op]) *
                     (SetOut[vec][op] - outputs[op]);      

      //��¼���
      m_vecLayers[1].m_vecNeurons[op].m_dError = err;

      curWeight = m_vecLayers[1].m_vecNeurons[op].m_vecWeight.begin();
      curNrnHid = m_vecLayers[0].m_vecNeurons.begin();

      int w = 0;

  
      while(curWeight != m_vecLayers[1].m_vecNeurons[op].m_vecWeight.end()-1)
      {
        //�������Ȩ��
        WeightUpdate = err * m_dLearningRate * curNrnHid->m_dActivation;
        
        //ʹ����������������
        *curWeight += WeightUpdate + m_vecLayers[1].m_vecNeurons[op].m_vecPrevUpdate[w] * MOMENTUM;

        //��¼Ȩ�ظ���
        m_vecLayers[1].m_vecNeurons[op].m_vecPrevUpdate[w] = WeightUpdate;

        ++curWeight; ++curNrnHid; ++w;
      }

      //��Ԫ��ƫ��ֵ
      WeightUpdate = err * m_dLearningRate * BIAS;

      *curWeight += WeightUpdate + m_vecLayers[1].m_vecNeurons[op].m_vecPrevUpdate[w] * MOMENTUM;  

      //��¼Ȩ�ظ���
      m_vecLayers[1].m_vecNeurons[op].m_vecPrevUpdate[w] = WeightUpdate;
      
      
    }

   //���򴫲�
    curNrnHid = m_vecLayers[0].m_vecNeurons.begin();

    int n = 0;
    
    //�������ز㣬����
    while(curNrnHid != m_vecLayers[0].m_vecNeurons.end())
    {
      double err = 0;

      curNrnOut = m_vecLayers[1].m_vecNeurons.begin();

      //���ڹ���
      while(curNrnOut != m_vecLayers[1].m_vecNeurons.end())
      {
        err += curNrnOut->m_dError * curNrnOut->m_vecWeight[n];

        ++curNrnOut;
      }

      //�������
      err *= curNrnHid->m_dActivation * (1 - curNrnHid->m_dActivation);     
      
      //����ѧϰ�ʺ���������ȡ����Ȩ��
      for (int w=0; w<m_iNumInputs; ++w)
      {
        WeightUpdate = err * m_dLearningRate * SetIn[vec][w];

        //���㷴�򴫲�������Ȩ��
        curNrnHid->m_vecWeight[w] += WeightUpdate + curNrnHid->m_vecPrevUpdate[w] * MOMENTUM;
        
        //��¼Ȩ�ظ�����
        curNrnHid->m_vecPrevUpdate[w] = WeightUpdate;

      }

      WeightUpdate = err * m_dLearningRate * BIAS;

      curNrnHid->m_vecWeight[m_iNumInputs] += WeightUpdate + curNrnHid->m_vecPrevUpdate[n] * MOMENTUM;

       curNrnHid->m_vecPrevUpdate[n] = WeightUpdate;

      ++curNrnHid;
      ++n;
    }

  }//��һ����������
  return true;
}

// ѵ��
bool CrowdNeuralNet::Train(vector<vector<double>>SetIn,vector<vector<double>> SetOut)
{

	while(m_dErrorSum>ERROR_THRESHOLD)
	{
		if (!ANN_BPTraining(SetIn,SetOut))
		{
			return false;
		}
      
	}
	m_bTrained=true;
	return true;
}

//Sigmoid����
double CrowdNeuralNet::Sigmoid( double netinput, double response)
{
	return (1/(1+exp(-netinput/response)));
}


