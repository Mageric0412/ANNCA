#include "CNeuralNet.h"
#include <windows.h>


//*************************** 神经网络方法**********************
//
//---------------------------------------------------------------------
CrowdNeuron::CrowdNeuron(int NumInputs): m_iNumInputs(NumInputs+1),m_dActivation(0),m_dError(0)
											
{
	//权重要多一个，所以+1
	for (int i=0; i<NumInputs+1; i++)
	{
		//随机赋权重
		m_vecWeight.push_back(RandomClamped());
		m_vecPrevUpdate.push_back(0);
	}
}




//************************ 神经层方法 **********************

//-----------------------------------------------------------------------
//	生成网络层
//-----------------------------------------------------------------------
CrowdNeuronLayer::CrowdNeuronLayer(int NumNeurons, 
                           int NumInputsPerNeuron):	m_iNumNeurons(NumNeurons)
{
	for (int i=0; i<NumNeurons; i++)

		m_vecNeurons.push_back(CrowdNeuron(NumInputsPerNeuron));
}




//************************ 神经网络类构造方法 ************************

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


//------------------------------生成神经网络结构------------------------------

void CrowdNeuralNet::Create_ANN()
{
	//生成网络层
	if (m_iNumHiddenLayers > 0)
	{
		//生成隐藏层
	  m_vecLayers.push_back(CrowdNeuronLayer(m_iNeuronsPerHiddenLyr, m_iNumInputs));
       
	  //生成多层隐藏层
      for (int i=0; i<m_iNumHiddenLayers-1; i++)
       {

			m_vecLayers.push_back(CrowdNeuronLayer(m_iNeuronsPerHiddenLyr, m_iNeuronsPerHiddenLyr));
       }

    //生成隐藏-输出层
	  m_vecLayers.push_back(CrowdNeuronLayer(m_iNumOutputs, m_iNeuronsPerHiddenLyr));
	}

  else
  {
	  //生成输出层
	  m_vecLayers.push_back(CrowdNeuronLayer(m_iNumOutputs, m_iNumInputs));
  }
}

//初始化神经网络
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

//-------------------------------运行神经网络-----------------------------------

vector<double> CrowdNeuralNet::Update(vector<double> inputs)
{
	//存储输出
	vector<double> outputs;

	int cWeight = 0;

	//检查输入
	if (inputs.size() != m_iNumInputs)
	{
		//返回空输出向量
		return outputs;
	}

	//对于每一层
	for (int i=0; i<m_iNumHiddenLayers + 1; ++i)
	{

		if ( i > 0 )
		{
			inputs = outputs;
		}

		outputs.clear();

		cWeight = 0;

		//神经元间的权值传递过程
		for (int n=0; n<m_vecLayers[i].m_iNumNeurons; ++n)
		{
			double netinput = 0.0f;

			int	NumInputs = m_vecLayers[i].m_vecNeurons[n].m_iNumInputs;

			for (int k=0; k<NumInputs - 1; ++k)
			{
				//权重与输入的乘积
				netinput += m_vecLayers[i].m_vecNeurons[n].m_vecWeight[k] * 
					inputs[cWeight++];
			}

			netinput += m_vecLayers[i].m_vecNeurons[n].m_vecWeight[NumInputs-1] * 
				BIAS;
	     //激励值计算
			m_vecLayers[i].m_vecNeurons[n].m_dActivation = 
				Sigmoid(netinput, ACTIVATION_RESPONSE);

			//存储输出
			outputs.push_back(m_vecLayers[i].m_vecNeurons[n].m_dActivation);

			cWeight = 0;
		}
	}

	return outputs;
}

//训练过程
bool CrowdNeuralNet::ANN_BPTraining(vector<TempVector> &SetIn,
                                      vector<TempVector> &SetOut)
{
  //生成迭代器
  vector<double>::iterator  curWeight;
  vector<CrowdNeuron>::iterator curNrnOut, curNrnHid;

  double WeightUpdate = 0;

  //初始化误差
  m_dErrorSum = 0;

  //使用每组输入，调节权重
  for (int vec=0; vec<SetIn.size(); ++vec)
  {
    //首先运行一遍神经网络
    vector<double> outputs = Update(SetIn[vec]);

    //检查输出
    if (outputs.size() == 0)
    {
      return false;
    }

    //调节过程
    for (int op=0; op<m_iNumOutputs; ++op)
    {
      double err = (SetOut[vec][op] - outputs[op]) * outputs[op]
                   * (1 - outputs[op]);

      //更新误差量
      m_dErrorSum += (SetOut[vec][op] - outputs[op]) *
                     (SetOut[vec][op] - outputs[op]);      

      //记录误差
      m_vecLayers[1].m_vecNeurons[op].m_dError = err;

      curWeight = m_vecLayers[1].m_vecNeurons[op].m_vecWeight.begin();
      curNrnHid = m_vecLayers[0].m_vecNeurons.begin();

      int w = 0;

  
      while(curWeight != m_vecLayers[1].m_vecNeurons[op].m_vecWeight.end()-1)
      {
        //计算更新权重
        WeightUpdate = err * m_dLearningRate * curNrnHid->m_dActivation;
        
        //使用随机向量参与更新
        *curWeight += WeightUpdate + m_vecLayers[1].m_vecNeurons[op].m_vecPrevUpdate[w] * MOMENTUM;

        //记录权重更新
        m_vecLayers[1].m_vecNeurons[op].m_vecPrevUpdate[w] = WeightUpdate;

        ++curWeight; ++curNrnHid; ++w;
      }

      //神经元的偏移值
      WeightUpdate = err * m_dLearningRate * BIAS;

      *curWeight += WeightUpdate + m_vecLayers[1].m_vecNeurons[op].m_vecPrevUpdate[w] * MOMENTUM;  

      //记录权重更新
      m_vecLayers[1].m_vecNeurons[op].m_vecPrevUpdate[w] = WeightUpdate;
      
      
    }

   //反向传播
    curNrnHid = m_vecLayers[0].m_vecNeurons.begin();

    int n = 0;
    
    //对于隐藏层，更新
    while(curNrnHid != m_vecLayers[0].m_vecNeurons.end())
    {
      double err = 0;

      curNrnOut = m_vecLayers[1].m_vecNeurons.begin();

      //调节过程
      while(curNrnOut != m_vecLayers[1].m_vecNeurons.end())
      {
        err += curNrnOut->m_dError * curNrnOut->m_vecWeight[n];

        ++curNrnOut;
      }

      //计算误差
      err *= curNrnHid->m_dActivation * (1 - curNrnHid->m_dActivation);     
      
      //基于学习率和误差，继续获取更新权重
      for (int w=0; w<m_iNumInputs; ++w)
      {
        WeightUpdate = err * m_dLearningRate * SetIn[vec][w];

        //计算反向传播所更新权重
        curNrnHid->m_vecWeight[w] += WeightUpdate + curNrnHid->m_vecPrevUpdate[w] * MOMENTUM;
        
        //记录权重更新量
        curNrnHid->m_vecPrevUpdate[w] = WeightUpdate;

      }

      WeightUpdate = err * m_dLearningRate * BIAS;

      curNrnHid->m_vecWeight[m_iNumInputs] += WeightUpdate + curNrnHid->m_vecPrevUpdate[n] * MOMENTUM;

       curNrnHid->m_vecPrevUpdate[n] = WeightUpdate;

      ++curNrnHid;
      ++n;
    }

  }//下一组输入向量
  return true;
}

// 训练
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

//Sigmoid函数
double CrowdNeuralNet::Sigmoid( double netinput, double response)
{
	return (1/(1+exp(-netinput/response)));
}


