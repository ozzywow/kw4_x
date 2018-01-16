#pragma once

#include "cocos2d.h"
#include <string>
#include "Singleton.h"

typedef enum
{
	etNone = 0,
	etItem
} eElementType;


typedef enum
{
	PID_NONE = 0,
	PID_STEP2,
	PID_STEP3,
	PID_STEP4,
	PID_STEP5,
	PID_TOTAL,
	PID_MAX,
} PRODUCT_ID;


struct WordCard
{
	std::string			word;
	int					level;
	std::string			text;
};

typedef std::vector<WordCard>	vtWordCards;


class PointManager : public Singleton<PointManager>
{
public:

	PointManager();
	~PointManager();

	void init();
	
	int			m_currStage;

	int			_point;
	int			_level;
	bool		_hint;

	std::map<std::string, WordCard>		m_mCardsByWord;
	std::map<int, vtWordCards>			m_mCardsByLevel;

	std::map<std::string, bool>			m_mMastWords;

	std::vector<bool>					m_vCart;


	void	SetPoint(int point);
	int		AddPoint(int point);
	int		DelPoint(int point);
	int		GetPoint();


	void	SaveData();
	void	LoadData();
	void	CheckMast(std::string& word);
	bool	IsMasted(std::string& word);
	void	ResetMast();
	int		GetMastedCount();
	int		GetTotalCount();
	void	SetLevel(int level);
	int		GetLevel();
	bool	GetHintOption();
	void	SetHintOption(bool opt);

	bool	LoadXML();
	void	GetNextScene(bool isEnter, bool isNextStage = false);
	void	GetPrevSecene();

	void	SetCartWithPID(int pid,  bool bBought);
	bool	GetCartWithPID(int pid);
};
