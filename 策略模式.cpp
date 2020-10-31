#include <iostream>
#include <memory>

/*----------------------------------------------------------------------------------------*/
class FlyBehavior {
public:
	virtual void fly() = 0; // 接口继承，分离变化
};

class FlyWithWings : public FlyBehavior {
public:
    virtual void fly() override;
};

class FlyNoWay : public FlyBehavior {
public:
    virtual void fly() override;
};

class FlyWithRocket : public FlyBehavior {
public:
    virtual void fly() override;
};

void FlyWithWings::fly() 
{	
	std::cout << "I am flying !" << std::endl;
}

void FlyNoWay::fly()
{	
	std::cout << "I cannot fly !" << std::endl;
}

void FlyWithRocket::fly()
{	
	std::cout << "I am flying with a rocket !" << std::endl;
}
/*----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------*/
class Duck {
public:
    Duck(FlyBehavior *fb);
	void PerformFly();
    void SetFlyBehavior(FlyBehavior *fb);

private:
    std::unique_ptr<FlyBehavior> uPtr;
};

Duck::Duck(FlyBehavior *fb) : uPtr(fb) {}

void Duck::SetFlyBehavior(FlyBehavior *fb)
{
    uPtr = std::unique_ptr<FlyBehavior>(fb);
}

void Duck::PerformFly()
{
    uPtr->fly();
}

class MallardDuck : public Duck {
public:
    MallardDuck(FlyBehavior *fb) : Duck(fb) {}
};

class RedheadDuck : public Duck {
public:
    RedheadDuck(FlyBehavior *fb) : Duck(fb) {}
};

class RubberDuck : public Duck {
public:
    RubberDuck(FlyBehavior *fb) : Duck(fb) {}
};
/*----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------*/
void main()
{
    FlyBehavior *pfWings = new FlyWithWings;
    FlyBehavior *pfNo = new FlyNoWay;
    FlyBehavior *pfRocket = new FlyWithRocket;

    Duck *pDuck = new MallardDuck(pfWings);
    pDuck->PerformFly();

    pDuck->SetFlyBehavior(pfNo);
    pDuck->PerformFly();

    pDuck->SetFlyBehavior(pfRocket);
    pDuck->PerformFly();
}
/*----------------------------------------------------------------------------------------*/
