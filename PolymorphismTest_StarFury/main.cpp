#include <iostream>
#include <Windows.h>
#include <conio.h>

#define STAR_MAX 20
#define dfSCREEN_WIDTH 75
#define ENABLE_MEMORY_TRACKING

void KeyProcess();
void Update();
void Render();

char screen[STAR_MAX][dfSCREEN_WIDTH];
int starCnt = 0;

class BaseObject
{
protected:
	int _X;
	bool _release;
	BaseObject() : _X(0), _release(false) {}

public:
	virtual void Update() = 0;
	virtual void Render() = 0;
	
	bool getRelease() const { return _release; }
};

BaseObject* boPtr[STAR_MAX];

class OneStar : public BaseObject
{
	void Update() override
	{
		if (_X++ >= dfSCREEN_WIDTH)
		{
			_release = true;
		}
	}

	void Render() override
	{
		if (!_release)
		{
			screen[starCnt][_X] = '*';
		}
	}
};

class TwoStar : public BaseObject
{

	void Update() override
	{
		_X++;
		if (_X++ >= dfSCREEN_WIDTH-1)
		{
			_release = true;
		}
	}

	void Render() override
	{
		if (!_release)
		{
			screen[starCnt][_X] = '*';
			screen[starCnt][_X+1] = '*';
		}
	}
};

class ThreeStar : public BaseObject
{
	void Update() override
	{
		_X++;
		_X++;
		if (_X++ >= dfSCREEN_WIDTH-2)
		{
			_release = true;
		}
	}

	void Render() override
	{
		if (!_release)
		{
			screen[starCnt][_X] = '*';
			screen[starCnt][_X + 1] = '*';
			screen[starCnt][_X + 2] = '*';
		}
	}
};



int main()
{
	while (1)
	{
		KeyProcess();

		Update();

		system("cls");
		Render();

		Sleep(30);
	}
}

void KeyProcess()
{
	if (_kbhit())
	{
		char key = _getch();

		switch (key)
		{
		case '1':
			for (int i = 0; i < STAR_MAX; i++)
			{
				if (boPtr[i] == nullptr)
				{
					boPtr[i] = new OneStar();
					break;
				}
			}
			break;
		case '2':
			for (int i = 0; i < STAR_MAX; i++)
			{
				if (boPtr[i] == nullptr)
				{
					boPtr[i] = new TwoStar();
					break;
				}
			}
			break;
		case '3':
			for (int i = 0; i < STAR_MAX; i++)
			{
				if (boPtr[i] == nullptr)
				{
					boPtr[i] = new ThreeStar();
					break;
				}
			}
			break;
		default:
			break;
		}
	}
}
void Update()
{
	// BaseObject ��ü �迭 ��ü�� ���鼭
	for (starCnt = 0; starCnt < STAR_MAX; starCnt++)
	{
		if (boPtr[starCnt] != nullptr)
		{
			boPtr[starCnt]->Update();
		}
	}

	// release �� �ֵ� ����
	for (starCnt = 0; starCnt < STAR_MAX; starCnt++)
	{
		if (boPtr[starCnt] != nullptr)
		{
			if (boPtr[starCnt]->getRelease())
			{
				delete boPtr[starCnt];
				boPtr[starCnt] = nullptr;
			}
		}
	}
}
void Render()
{
	for (int i = 0; i < STAR_MAX; i++)
	{
		for (int j = 0; j < dfSCREEN_WIDTH; j++)
		{
			screen[i][j] = ' ';
		}
	}
	//BaseObject ��ü �迭 ��ü�� ���鼭
	for (starCnt = 0; starCnt < STAR_MAX; starCnt++)
	{
		if (boPtr[starCnt] != nullptr)
			boPtr[starCnt]->Render();
	}
	// �� �迭�� ��ü�� �ִ� ���� �� �ٲ��� ������.
	// ���ο� ��ü�� ������ �� �迭�� ã�Ƽ� �ְ� �ִµ�
	// ����ִ� �迭�� ���ؼ� �� �ٲ��� �����ָ� 
	// �ڲ� ���� �ٲ� ���Ⱑ ������ !
	for (int i = 0; i < STAR_MAX; i++)
	{
		for (int j = 0; j < dfSCREEN_WIDTH; j++)
		{
			std::cout << screen[i][j];
		}
		std::cout << "\n";
	}
}