#include "HelloWorldScene.h"
#include <math.h>
USING_NS_CC;
#define threshold 0.05 

#define RVO_ON
Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{   P=3.141592653;	
	 orderflag=15;
	 direc=40.0f;
	 m_dHighestOutput=0;
	 m_iBestMatch=-1;
	 m_iMatch=-1;
	 RealAngle=60;
	 SumVec=Vec2(0.0f,0.0f);
	 DeltaAverrageAllVec=Vec2(0.0f,0.0f);
	 Veccount=countAll=0;
	 tempfield=-8;
	 recorded=false;
	 recorded2=false;
	 ended=false;
	 RecordDeltavec=false;
	 blue=green=0;
	 //ANN
	 trained=false;
	 started=false;
	 finished=false;
	 avoid=false;
	 persisted=false;
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    s = Director::getInstance()->getWinSize();

    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_pData = new Crowd_TData(ANGLE_PATTERNS, NUM_VECIN);
	m_pNet = new CrowdNeuralNet(NUM_VECIN*2, ANGLE_PATTERNS,  NUM_HIDDEN_NEURONS, LEARNING_RATE);

	
	Crowd_Sim = new RVO::RVOSimulator();

#ifdef RVO_ON
	setupScenario2(Crowd_Sim);
#endif
#ifndef RVO_ON
	setupScenario3(Crowd_Sim,RealAngle);
#endif
	

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(s.width*0.9,origin.y +s.height*0.88+ closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
	auto listener=EventListenerTouchOneByOne::create();
	//auto listener=EventListenerTouchAllAtOnce::create();

	CCSprite *resultButton = CCSprite::create("reset.png");	
	CCSprite *resultButtoned = CCSprite::create("reseted.png");

	CCSprite *startButton = CCSprite::create("r1.png");	
	CCSprite *startButtoned = CCSprite::create("r2.png");

	CCSprite *trainButton = CCSprite::create("green_edit.png");	
	CCSprite *trainButtoned = CCSprite::create("yellow_edit.png");

	CCSprite *upButton = CCSprite::create("f1.png");
	CCSprite *upButtoned = CCSprite::create("f2.png");

	CCSprite *downButton = CCSprite::create("b1.png");
	CCSprite *downButtoned= CCSprite::create("b2.png");

	auto WBbg=CCSprite::create("bgWB.png");
	this->addChild(WBbg,-1);
	WBbg->setPosition(Vec2(s.width/2,s.height/2));

	CCMenuItemSprite *trainBtn =CCMenuItemSprite::create(trainButton, trainButtoned, trainButton, this, menu_selector(HelloWorld::menuCallback));
	CCMenuItemSprite *startBtn =CCMenuItemSprite::create(startButton, startButtoned, startButton, this, menu_selector(HelloWorld::menuCallback));
	CCMenuItemSprite *resetBtn=CCMenuItemSprite::create(resultButton,resultButtoned,resultButton,this,menu_selector(HelloWorld::menuCallback));
	CCMenuItemSprite *upBtn =CCMenuItemSprite::create(upButton, upButtoned, upButton, this, menu_selector(HelloWorld::menuCallback));
	CCMenuItemSprite *downBtn =CCMenuItemSprite::create(downButton, downButtoned, downButton, this, menu_selector(HelloWorld::menuCallback));

	trainBtn->setTag(301);
	//trainBtn->setPositionY(s.height*0.2);
	startBtn->setTag(302);
	
	resetBtn->setTag(303);

	upBtn->setTag(304);
	upBtn->setPositionX(s.width*0.2);
	downBtn->setTag(305);
	resetBtn->setPositionY(s.height*0.1);
	
	CCMenu *menu2 = CCMenu::create(resetBtn,startBtn,NULL);
	CCMenu *menu3=CCMenu::create(trainBtn,NULL);
	CCMenu *menu4=CCMenu::create(upBtn,downBtn,NULL);

	//menu2->setScale(0.5);
	menu2->setPosition(Vec2(s.width*0.75,s.height*0.8));
	//menu3->setScale(0.5);
	menu3->setPosition(Vec2(s.width*0.85,s.height*0.8));
	//menu4->setScale(0.5);
	menu4->setPosition(Vec2(s.width*0.4,s.height*0.8));
	

	this->addChild(menu2,1);
	this->addChild(menu3,1);
	this->addChild(menu4,1);
	
  

	listener->onTouchBegan=CC_CALLBACK_2(HelloWorld::onTouchBegan,this);
	listener->onTouchMoved=CC_CALLBACK_2(HelloWorld::onTouchMoved,this);
	listener->onTouchEnded=CC_CALLBACK_2(HelloWorld::onTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    //BatchNode=SpriteBatchNode::create("agent01.png");
	//addChild(BatchNode, 0, 1);
	
	//addObstacle2();
	//重置个体位置
	for (size_t i=0; i<Crowd_Sim->getNumAgents();++i)
	{
		recordAgentAvoid.clear();
		std::vector<bool> record;
		addNewAgent(Vec2(Crowd_Sim->getAgentPosition(i).x(),Crowd_Sim->getAgentPosition(i).y()),i);
        #ifndef RVO_ON
		goals.push_back(RVO::Vector2(-Crowd_Sim->getAgentPosition(i).x(),Crowd_Sim->getAgentPosition(i).y()));
        #endif
			for (size_t j=0; j<Crowd_Sim->getNumAgents();++j)
		{
		  recordAgentAvoid.push_back(0);
         // record.push_back(false);
		}
		recordAvoid.push_back(recordAgentAvoid);
		recordpersist.push_back(0);
	}
	
	label=CCLabelBMFont::create("DCAngle: 0","fonts/helvetica-32.fnt");
	label1=CCLabelBMFont::create("iCAngle: 0","fonts/helvetica-32.fnt");
	label2=CCLabelBMFont::create("jCAngle: 0","fonts/helvetica-32.fnt");
	label3=CCLabelBMFont::create("DeltaPosition","fonts/helvetica-32.fnt");
	label4=CCLabelBMFont::create("ANN ErrorSum:","fonts/helvetica-32.fnt");
	labelangle=CCLabelBMFont::create("相遇角度: 0","fonts/helvetica-32.fnt");

	label->setScale(0.2);
	label1->setScale(0.2);
	label2->setScale(0.2);
	label3->setScale(0.2);
	label4->setScale(0.5);
	labelangle->setScale(0.5);

	label->setPosition(Vec2(s.width*0.4,s.height*0.13));
	label1->setPosition(Vec2(s.width*0.6,s.height*0.13));
	label2->setPosition(Vec2(s.width*0.8,s.height*0.13));
	label3->setPosition(Vec2(s.width*0.2,s.height*0.13));
	label4->setPosition(Vec2(s.width*0.5,s.height*0.09));
	labelangle->setPosition(Vec2(s.width/2,s.height*0.8));

	this->addChild(label,3,120);
	this->addChild(label1,3,121);
	this->addChild(label2,3,122);
	this->addChild(label3,3,119);
	this->addChild(label4,3,123);
	this->addChild(labelangle,3,124);
    CCLOG("%lf,%lf",s.width,s.height);

    #ifdef RVO_ON
	menu4->setVisible(false);
	label->setVisible(false);
	label1->setVisible(false);
	label2->setVisible(false);
	label3->setVisible(false);
	label4->setVisible(false);
	labelangle->setVisible(false);
    #endif

	#ifndef RVO_ON
	menu4->setVisible(true);
	labelangle->setVisible(true);
	#endif

	scheduleUpdate();
    return true;
}


void HelloWorld::menuCallback(CCObject *pSender)
{
	CCMenuItemSprite *item = (CCMenuItemSprite*)pSender;
	if(item->getTag() == 303)
	{   
#ifdef RVO_ON

#endif

#ifndef RVO_ON
		Reset_Agent(RealAngle,direc);
		persisted=false;
#endif
	}
	else if(item->getTag()==302)
	{
		if (started==true)
		{		
			started=false;
		}
		else
			started=true;

	}
	else if(item->getTag()==301)
	{
		if(trained!=true)
		{  
			SetIn=m_pData->GetInputSet();
			SetOut=m_pData->GetOutputSet();
			trained=true;
			
		}
	}
	else if(item->getTag()==304)
	{
		RealAngle=RealAngle+1.0f;
	}
	else if(item->getTag()==305)
	{
		RealAngle=RealAngle-1.0f;
	}
}

void HelloWorld::Reset_Agent(float angle,float xoffense)
{
	goals.clear();
	float   offense=18;
	double Pi=3.141592653;	
	int num=2;
	float xo=cosf(P/180*angle)*xoffense;
	float yo=sinf(P/180*angle)*xoffense;
	

	Crowd_Sim->setAgentPosition(0,RVO::Vector2(xoffense ,0));
	Crowd_Sim->setAgentPosition(1,RVO::Vector2(-xoffense,0));
	Crowd_Sim->setAgentPosition(2,RVO::Vector2(xoffense - 4,  offense+3));
	Crowd_Sim->setAgentPosition(3,RVO::Vector2(-xoffense -4,offense+3));
	Crowd_Sim->setAgentPosition(4,RVO::Vector2(xoffense +5,  -offense));
	Crowd_Sim->setAgentPosition(5,RVO::Vector2(-xoffense -10,  -offense));

		for (size_t i=0; i<Crowd_Sim->getNumAgents();++i)
		{
			auto sp=(CCSprite*)this->getChildByTag(i);
			sp->setPosition(Vec2( Crowd_Sim->getAgentPosition(i).x()*3+s.width/2, Crowd_Sim->getAgentPosition(i).y()*3+s.height/2) );
		   goals.push_back(RVO::Vector2(-Crowd_Sim->getAgentPosition(i).x(),Crowd_Sim->getAgentPosition(i).y()));
		   recordAgentAvoid[i]=0;
		   recordpersist[i]=0;
		}
		started=false;
		SumVec=Vec2(0.0f,0.0f);
		DeltaAverrageAllVec=Vec2(0.0f,0.0f);
		Veccount=countAll=0;
		tempfield=-8;
		recorded=false;
		recorded2=false;
		RecordDeltavec=false;
		blue=green=0;
		avoid=false;
		persisted=false;
		scheduleUpdate();
}

//使用ANN
bool HelloWorld::TestForMatch(std::vector<double> vecinput)
{
	vector<double> outputs = m_pNet->Update(vecinput);
	m_dHighestOutput = 0;
	m_iBestMatch = 0;
	m_iMatch = -1;
	if (outputs.size() == 0)
	{
		CCLOG("Error in with ANN output");
		return false;
	}

	for (int i=0; i<outputs.size(); ++i)
	{
		if (outputs[i] > m_dHighestOutput)
		{
			//make a note of the most likely candidate
			m_dHighestOutput = outputs[i];
			m_iBestMatch = i;
			//if the candidates output exceeds the threshold we 
			//have a match! ...so make a note of it.
			if (m_dHighestOutput > MATCH_TOLERANCE)
			{
				m_iMatch = m_iBestMatch;
			}
		}
	}
	return true;
}

bool HelloWorld::TrainNetwork()
{
	if (!m_pNet->ANN_BPTraining(SetIn,SetOut))
	{
		return false;
	}
	return true;
}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}


void HelloWorld::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{
	

	Director *director = Director::getInstance();
	
    #ifdef RVO_ON
    #endif
	if (started)
	{
		for (int i=0;i<Crowd_Sim->getNumAgents();i++)
		{
			curPos[i]=this->getChildByTag(i)->getPosition();
			if (lastPos[i]!=curPos[i])
			{
				agentPos[i].push_back(curPos[i]);
			}
		}
		
	}
	
	//CCASSERT(nullptr != director, "Director is null when setting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
	//绘制实时相对位置，相对速度、各agent速度
	CHECK_GL_ERROR_DEBUG();
	glLineWidth(1);
	
	//每次存储一次绘制坐标，再次绘制
	
	for (int i=0;i<Crowd_Sim->getNumAgents();i++)
	{
		DrawPrimitives::setDrawColor4B(GLubyte(colorVec[i*3]), GLubyte(colorVec[i*3+1]), GLubyte(colorVec[i*3+2]), GLubyte(250));
		for (int j=0;j<agentPos[i].size()-1;j++)
		{
			DrawPrimitives::drawLine(agentPos[i][j],agentPos[i][j+1]);
		}

	}
		
	
	CHECK_GL_ERROR_DEBUG();
	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	
    for (int i=0;i<Crowd_Sim->getNumAgents();i++)
    {
		lastPos[i]=curPos[i];

    }
     
	
}	

//判断方向
bool HelloWorld::thinkdirection(int i,int j)
{
	Vec2 vec1=Vec2(Crowd_Sim->getAgentPrefVelocity(i).x_,Crowd_Sim->getAgentPrefVelocity(i).y_);
	Vec2 vec2=Vec2(Crowd_Sim->getAgentPrefVelocity(j).x_,Crowd_Sim->getAgentPrefVelocity(j).y_);
	
  if (vec1.dot(vec2)>0)
  { //同向
	  return false;
  }

  //异向
  return true;
}

//获取double输入
std::vector<double> HelloWorld::getNormalInput(int i,int j)
{

	Vec2 veci=Vec2(Crowd_Sim->getAgentPrefVelocity(i).x(),Crowd_Sim->getAgentPrefVelocity(i).y());
	Vec2 vecj=Vec2(Crowd_Sim->getAgentPrefVelocity(j).x(),Crowd_Sim->getAgentPrefVelocity(j).y());
	float cosflo=veci.dot(vecj)/(veci.length()*vecj.length());
	std::vector<double> vecinput;
	double sinflo=sqrt(1-cosflo*cosflo);
	for (int i=0;i<6;i++)
	{
		vecinput.push_back(cosflo);
	    vecinput.push_back(sinflo);
	}

   return vecinput;
}


//判断j在i的范围分域中的哪一块
int HelloWorld::Range_Field(int i,int j)
{
	Vec2 posvec=Vec2((Crowd_Sim->getAgentPosition(j)-Crowd_Sim->getAgentPosition(i)).x_,(Crowd_Sim->getAgentPosition(j)-Crowd_Sim->getAgentPosition(i)).y_);
    Vec2 vec=Vec2(Crowd_Sim->getAgentPrefVelocity(i).x_,Crowd_Sim->getAgentPrefVelocity(i).y_);
	float agentcos=posvec.dot(vec)/(posvec.length()*vec.length());
	float directnum=posvec.x*vec.y-vec.x*posvec.y;
		
        //在60度到81度之间	
	if (agentcos>0.1564&&agentcos<=0.5)
		{
			return (directnum>0?3:-3);
		}
		//在30度到60度之间
		else if(agentcos>0.5&&agentcos<=0.866)
		{
			return(directnum>0?2:-2);
		}
		//在0到30度之间
		else if(agentcos>0.866&&agentcos<1)
		{
			return(directnum>0?1:-1);
		}

		//在81度到90度之间
		else if (abs(agentcos)<0.1564)
		{
			return(directnum>0?4:-4);
		}

		//约为0度
		else if(abs(agentcos-1)<0.00001)
		{
			 return 0;
		}
		//其他情况
		else
		 return(directnum>0?7:-7);

}

//计算j是否会与i碰撞
int HelloWorld::thinkcollison(int i,int j,float r)
{ 
	 //float r=2.5;
	Vec2 vec=Vec2((Crowd_Sim->getAgentPrefVelocity(i)-Crowd_Sim->getAgentPrefVelocity(j)).x_,(Crowd_Sim->getAgentPrefVelocity(i)-Crowd_Sim->getAgentPrefVelocity(j)).y_);
	Vec2 cpos=Vec2((Crowd_Sim->getAgentPosition(i)-Crowd_Sim->getAgentPosition(j)).x_,(Crowd_Sim->getAgentPosition(i)-Crowd_Sim->getAgentPosition(j)).y_);
 
	if (cpos.length()<15&&((vec.dot(-cpos))/(vec.length()*cpos.length())>0))
  {
	  if(cpos.length()>2*r)
		{ 
			if ((cpos.x*cpos.x*vec.y*vec.y+vec.x*vec.x*cpos.y*cpos.y-2*cpos.x*cpos.y*vec.x*vec.y-4*r*r*(vec.x*vec.x+vec.y*vec.y)<=0))
		   {  
			   //大约偏转在6度以内标记为0
			 if (abs((vec.dot(-cpos))/(vec.length()*cpos.length())-1)<=0.05&&vec.length()>threshold)
		     {
				 return 0;
		     }
			 else return 1;
		   }		 
		  else return 2;
	   }
	  else return 0;	 
  }
 else return 2;
}

int HelloWorld::thinkcollison(RVO::Vector2 veci,RVO::Vector2 vecj,RVO::Vector2 vecposi,RVO::Vector2 vecposj,float r)
{ 
	//float r=2.5;
	Vec2 vec=Vec2((veci-vecj).x_,(veci-vecj).y_);
	Vec2 cpos=Vec2((vecposi-vecposj).x_,(vecposi-vecposj).y_);

	if (cpos.length()<15&&((vec.dot(-cpos))/(vec.length()*cpos.length())>0))
	{
		if(cpos.length()>2*r)
		{ 
			if ((cpos.x*cpos.x*vec.y*vec.y+vec.x*vec.x*cpos.y*cpos.y-2*cpos.x*cpos.y*vec.x*vec.y-4*r*r*(vec.x*vec.x+vec.y*vec.y)<=0))
			{  
				//大约偏转在6度以内标记为0
				if (abs((vec.dot(-cpos))/(vec.length()*cpos.length())-1)<=0.05&&vec.length()>threshold)
				{
					return 0;
				}
				else return 1;
			}		 
			else return 2;
		}
		else return 0;	 
	}
	else return 2;
}

//非质心碰撞的偏移向量改变比率
double HelloWorld::GetColliShift(int i,int j,float radius)
{  //获取第一次取小值时候的偏移碰撞向量
	Vec2 deltavec=Vec2((Crowd_Sim->getAgentPrefVelocity(i)-Crowd_Sim->getAgentPrefVelocity(j)).x_,(Crowd_Sim->getAgentPrefVelocity(i)-Crowd_Sim->getAgentPrefVelocity(j)).y_);
	Vec2 deltapos=Vec2((Crowd_Sim->getAgentPosition(i)-Crowd_Sim->getAgentPosition(j)).x_,(Crowd_Sim->getAgentPosition(i)-Crowd_Sim->getAgentPosition(j)).y_);
	float alpha=(-deltapos.length()*deltapos.dot(deltavec)/(deltapos.length()*deltavec.length())-2*radius)/deltavec.length();
	Vec2 shiftVec=deltapos+alpha*deltavec;

	//计算相对位置到非质心碰撞的质心相对向量的位置关系，在相对速度左为1，右为0
	int shiftpos=deltapos.x*shiftVec.y-deltapos.y*shiftVec.x>0? 1:0;
	//计算个体j的速度向量到相对位置间的位置关系，在个体j左为1，右为0
	int vecpos=Crowd_Sim->getAgentPrefVelocity(j).x()*deltapos.y-Crowd_Sim->getAgentPrefVelocity(j).y()*deltapos.x>0?1:0;
	//计算质心相对向量与相对位置的cos值
	double shiftCos=shiftVec.dot(deltapos)/(shiftVec.length()*deltapos.length());
	//计算临界碰撞情况时的cos值
	double normCos=2*radius/deltapos.length();
	//反三角函数计算角度比例
	double collishift=(acosf(normCos)-acosf(shiftCos))/acosf(normCos);
	//返回非质心的偏移比率，正数表示个体j先通过，i后通过，负数表示个体i先通过，个体j后通过
	return  ((shiftpos==vecpos)?collishift:-collishift);
}




void HelloWorld::addObstacle()
{
	auto ob1=CCSprite::create("ob1.png");
	ob1->setScale(9/8);
	ob1->setPosition(Vec2(-75+s.width/2,75+s.height/2));
	auto ob2=CCSprite::create("ob1.png");
	ob2->setScale(9/8);
	ob2->setPosition(Vec2(-75+s.width/2,-75+s.height/2));
	auto ob3=CCSprite::create("ob1.png");
	ob3->setScale(9/8);
	ob3->setPosition(Vec2(75+s.width/2,-75+s.height/2));
	auto ob4=CCSprite::create("ob1.png");
	ob4->setScale(9/8);
	ob4->setPosition(Vec2(75+s.width/2,75+s.height/2));
	addChild(ob1,0,2);
	addChild(ob2,0,3);
	addChild(ob3,0,4);
	addChild(ob4,0,5);

}


void HelloWorld::addObstacle2()
{
	auto ob1=CCSprite::create("ob2.png");
	auto ob2=CCSprite::create("ob2.png");
	ob1->setPosition(Vec2(s.width/2,s.height/2+80));
	ob2->setPosition(Vec2(s.width/2,s.height/2-80));
	addChild(ob1,0,300);
	addChild(ob2,0,301);

}





void HelloWorld::addNewAgent(Vec2 p,int i)
{  
	 std::vector<Vec2> tmpVec;
	 tmpVec.push_back(Vec2(0,0));
	  char str[100] = {0};
	   int choice=i%2+1;  
      sprintf(str, "agent%02d.png",choice);
      auto sprite=Sprite::create(str);
	  
	  int r=rand()%255;
	  int g=rand()%255;
	  int b=rand()%255;
	  colorVec.push_back(r);
	  colorVec.push_back(g);
	  colorVec.push_back(b);

	  
	  sprite->setScale(0.8);
	   sprite->setPosition( Vec2( p.x*3+s.width/2, p.y*3+s.height/2) );
	   Color3B colr=Color3B(GLubyte(r),GLubyte(g),GLubyte(b));
	   sprite->setColor(colr);
	   curPos.push_back(sprite->getPosition());
	   tmpVec[0]=sprite->getPosition();
	   agentPos.push_back(tmpVec);
	   lastPos.push_back(sprite->getPosition());
	   addChild(sprite,1,i);
	
}


bool HelloWorld::onTouchBegan(Touch *touch,Event *unused_event)
{
	return true;
}


void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
  //addNewAgent(Vec2(touch->getLocation().x,touch->getLocation().y));
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void HelloWorld::addTrackflag(int i,int j,int field)
{
	CCSprite* ob5=CCSprite::create("flag.png");
	CCSprite* ob6=CCSprite::create("flag.png");
	ob6->setPosition(Vec2(Crowd_Sim->getAgentPosition(j).x()*3+s.width/2,Crowd_Sim->getAgentPosition(j).y()*3+s.height/2));
	ob5->setPosition(Vec2(Crowd_Sim->getAgentPosition(i).x()*3+s.width/2,Crowd_Sim->getAgentPosition(i).y()*3+s.height/2));
	float veclen=Vec2(Crowd_Sim->getAgentPrefVelocity(i).x(),Crowd_Sim->getAgentPrefVelocity(i).y()).length();
	float veclen1=Vec2(Crowd_Sim->getAgentPrefVelocity(j).x(),Crowd_Sim->getAgentPrefVelocity(j).y()).length();
	
	if (field!=Range_Field(i,j))
	{
		blue+=50;
		green+=50;
	}

	ob5->setColor(Color3B(int(200*veclen),green,0));
	ob6->setColor(Color3B(int(200*veclen1),green,0));
	//ob5->setOpacity(200*veclen);
	addChild(ob5,3,orderflag);
	addChild(ob6,3,orderflag+1);
	orderflag++;
	
    
}


void HelloWorld::update(float dt)
{
	//auto BatchNode=static_cast<SpriteBatchNode*>( getChildByTag(1));
	labelangle->setString(CCString::createWithFormat("Angle:%.1f",RealAngle)->getCString());

#ifdef RVO_ON
    if(started)
	{
	  if(!reachedGoal(Crowd_Sim))
	  {
	    setPreferredVelocities(Crowd_Sim);
	    Crowd_Sim->doStep();

		for (size_t i=0; i<Crowd_Sim->getNumAgents();i++)
		{
			//CCSprite* sp=(CCSprite*)BatchNode->getChildByTag(i);
			CCSprite* sp=(CCSprite*)this->getChildByTag(i);
			sp->setPosition(Vec2(Crowd_Sim->getAgentPosition(i).x()*3+s.width/2,Crowd_Sim->getAgentPosition(i).y()*3+s.height/2));
		
		}
	  }
	}
	
#endif
	 
#ifndef RVO_ON

	//ANN训练的实时显示
  if(!finished)
   {	
	 if (trained)
	  {
		TrainNetwork();
	    label4->setString(CCString::createWithFormat("ErrorSum:%.4f",float(m_pNet->Error()))->getCString());	

	   if (m_pNet->Error()<ERROR_THRESHOLD)
	   {
		   finished=true;
		   label4->setString(CCString::createWithFormat("ErrorSum:%.4f",float(m_pNet->Error()))->getCString());
	   }
      }	
   }

 else if(started&&finished)
  {
	

	if(!reachedGoal(Crowd_Sim,goals))
	{	
		//if(!persisted)
		{
	   for (size_t i=0;i<Crowd_Sim->getNumAgents();i++)
		{
			if(recordpersist[i]==0)
			setPreferredVelocities(Crowd_Sim,i,goals);
		}
	  // persisted=true;
		}


	for (size_t i=0; i<Crowd_Sim->getNumAgents();i++)
	{
		//CCSprite* sp=(CCSprite*)BatchNode->getChildByTag(i);
		CCSprite* sp=(CCSprite*)this->getChildByTag(i);
		  sp->setPosition(Vec2(Crowd_Sim->getAgentPosition(i).x()*3+s.width/2,Crowd_Sim->getAgentPosition(i).y()*3+s.height/2));
		 
		 
	   for(int j=0;j<Crowd_Sim->getNumAgents();j++)
		{ 
			if(i!=j&&i<j)
	      {
			  //mageric
			  Vec2 JudgePos=Vec2((Crowd_Sim->getAgentPosition(i)-Crowd_Sim->getAgentPosition(j)).x_,(Crowd_Sim->getAgentPosition(i)-Crowd_Sim->getAgentPosition(j)).y_);

			 // if(JudgePos.length()<15)
			 // {		  
			  if (thinkcollison(i,j,2.5)!=2)
		     {
			 
			 //ANN
	//			if (!avoid)
					std::vector<double> input=getNormalInput(i,j);
					if(TestForMatch(input))
					{
						float shift=GetColliShift(i,j,2.5);
						if (abs(shift)<0.9)
						{
							shift>0?setNewAvoidVelocity(Crowd_Sim,j,i,m_iBestMatch,abs(shift)):setNewAvoidVelocity(Crowd_Sim,i,j,m_iBestMatch,abs(shift));
						}
						else
						{
							int ra[2];
							ra[0]=i;
							ra[1]=j;
							int rad=rand()%2;						
							setNewAvoidVelocity(Crowd_Sim,ra[rad],ra[1-rad],m_iBestMatch,1);
						}
					}				
				//	avoid=true;
					if (input[0]>0.5&&input[0]<0.965||input[0]>-1&&input[0]<0.5)
					{
										
						recordpersist[i]=1;
						recordpersist[j]=1;
						recordAgentAvoid[i]=1;
						recordAgentAvoid[j]=1;
					}
			
		    }//collision deal	
			else  if(recordAgentAvoid[i]==2&&recordAgentAvoid[j]==2&&thinkcollison(i,j,2.5)==2/* thinkcollison(RVO::normalize(goals[i] - Crowd_Sim->getAgentPosition(i)),RVO::normalize(goals[j] - Crowd_Sim->getAgentPosition(j)),Crowd_Sim->getAgentPosition(i),Crowd_Sim->getAgentPosition(j),2.5)==2*/)
		    {
			 // return formal deltavec  by mageric 1.13.2015
			  setPreferredVelocities(Crowd_Sim,i,goals);
			  setPreferredVelocities(Crowd_Sim,j,goals);		
			  recordAgentAvoid[i]=0;
			  recordAgentAvoid[j]=0;
		   }
			
	//	 }
	  
	   }//i<j
      }//j

	 
    }// i

	for (size_t i=0;i<Crowd_Sim->getNumAgents();i++)
	{
		int countavoid=0;
		for (size_t k=0;k<Crowd_Sim->getNumAgents();k++)
		{
			int count=abs(Range_Field(i,k))>2?1:0;
			countavoid+=count;
		}
		if (recordpersist[i]==1&&countavoid>1)
		{
			recordAgentAvoid[i]=2;
		}
		
	}
	//setPreferredVelocities(Crowd_Sim);
	Crowd_Sim->doStep();


	}
	else
	{ 
		this->unscheduleUpdate();
	}

  }
  #endif
}


void HelloWorld::setNewAvoidVelocity(RVO::RVOSimulator *Crowd_Sim,int agenti,int agentj,int bestMatch,double collishift)
{
      float judge=Range_Field(agenti,agentj)>0?1.0f:-1.0f;
     std::vector<double> vecout=m_pData->ClassifyPattern(bestMatch);
  if (bestMatch>=1)
  {
	double x1=Crowd_Sim->getAgentPrefVelocity(agenti).x()*cosf(judge*(vecout[0]*P/180)*collishift)-Crowd_Sim->getAgentPrefVelocity(agenti).y()*sinf(judge*(vecout[0]*P/180)*collishift);
	double y1=Crowd_Sim->getAgentPrefVelocity(agenti).y()*cosf(judge*(vecout[0]*P/180)*collishift)+Crowd_Sim->getAgentPrefVelocity(agenti).x()*sinf(judge*(vecout[0]*P/180)*collishift);

	double x2=Crowd_Sim->getAgentPrefVelocity(agentj).x()*cosf(judge*(vecout[1]*P/180)*collishift)-Crowd_Sim->getAgentPrefVelocity(agentj).y()*sinf(judge*(vecout[1]*P/180)*collishift);
	double y2=Crowd_Sim->getAgentPrefVelocity(agentj).y()*cosf(judge*(vecout[1]*P/180)*collishift)+Crowd_Sim->getAgentPrefVelocity(agentj).x()*sinf(judge*(vecout[1]*P/180)*collishift);

	RVO::Vector2 Nveci=RVO::Vector2(x1,y1)*vecout[2];
	RVO::Vector2 Nvecj=RVO::Vector2(x2,y2)*vecout[3];

	Crowd_Sim->setAgentPrefVelocity(agenti,Nveci);
	Crowd_Sim->setAgentPrefVelocity(agentj,Nvecj);

  }
  else
  {
	double x1=Crowd_Sim->getAgentPrefVelocity(agenti).x()*cosf((-judge)*(vecout[0]*P/180)*collishift)-Crowd_Sim->getAgentPrefVelocity(agenti).y()*sinf((-judge)*(vecout[0]*P/180)*collishift);
	double y1=Crowd_Sim->getAgentPrefVelocity(agenti).y()*cosf((-judge)*(vecout[0]*P/180)*collishift)+Crowd_Sim->getAgentPrefVelocity(agenti).x()*sinf((-judge)*(vecout[0]*P/180)*collishift);

	double x2=Crowd_Sim->getAgentPrefVelocity(agentj).x()*cosf(judge*(vecout[1]*P/180)*collishift)-Crowd_Sim->getAgentPrefVelocity(agentj).y()*sinf(judge*(vecout[1]*P/180)*collishift);
	double y2=Crowd_Sim->getAgentPrefVelocity(agentj).y()*cosf(judge*(vecout[1]*P/180)*collishift)+Crowd_Sim->getAgentPrefVelocity(agentj).x()*sinf(judge*(vecout[1]*P/180)*collishift);

	RVO::Vector2 Nveci=RVO::Vector2(x1,y1)*vecout[2];
	RVO::Vector2 Nvecj=RVO::Vector2(x2,y2)*vecout[3];

	Crowd_Sim->setAgentPrefVelocity(agenti,Nveci);
	Crowd_Sim->setAgentPrefVelocity(agentj,Nvecj);

  }
	
}