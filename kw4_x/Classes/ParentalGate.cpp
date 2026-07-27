#include "stdafx.h"
#include "ParentalGate.h"
#include "SoundFactory.h"

// Korean UI text is given as explicit UTF-8 bytes. This project's sources are UTF-8
// and the Win32 build reads narrow literals as the system codepage, so the UTF8()
// macro double-converts and corrupts Korean (seen in the reset popup). Byte literals
// avoid that on every platform. Comments here are kept ASCII so the file needs no BOM.
#define PG_TITLE   "\xEB\xB6\x80\xEB\xAA\xA8\xEB\x8B\x98 \xED\x99\x95\xEC\x9D\xB8"   // "부모님 확인"
#define PG_CANCEL  "\xEC\xB7\xA8\xEC\x86\x8C"                                       // "취소"
#define PG_CLEAR   "\xEB\x8B\xA4\xEC\x8B\x9C"                                       // "다시"

static const int kParentalGateTag = 0x50617247; // 'ParG', prevents duplicate gates

void ParentalGate::present(Node* parent, const std::function<void()>& onSuccess)
{
	if (!parent) return;
	if (parent->getChildByTag(kParentalGateTag)) return; // already showing

	auto gate = new (std::nothrow) ParentalGate();
	if (gate && gate->initWithCallback(onSuccess))
	{
		gate->autorelease();
		gate->setTag(kParentalGateTag);
		parent->addChild(gate, 100000); // top-most
	}
	else
	{
		CC_SAFE_DELETE(gate);
	}
}

bool ParentalGate::initWithCallback(const std::function<void()>& onSuccess)
{
	if (!Node::init()) return false;
	m_onSuccess = onSuccess;

	auto glview = Director::getInstance()->getOpenGLView();
	const Size frame = glview->getDesignResolutionSize();
	const Vec2 center(frame.width * 0.5f, frame.height * 0.5f);

	// 1) Dim backdrop that swallows touches to the scene below
	auto dim = LayerColor::create(Color4B(0, 0, 0, 170));
	dim->setContentSize(frame);
	dim->setPosition(0, 0);
	this->addChild(dim, 0);

	auto swallow = EventListenerTouchOneByOne::create();
	swallow->setSwallowTouches(true);
	swallow->onTouchBegan = [](Touch*, Event*) { return true; };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(swallow, dim);

	// 2) Panel (border + inner) drawn so we control the size exactly
	const float W = FRAME_WIDTH * 0.80f;
	const float H = FRAME_WIDTH * 0.92f;
	auto panel = DrawNode::create();
	panel->setPosition(center);
	panel->drawSolidRect(Vec2(-W * 0.5f - 8, -H * 0.5f - 8), Vec2(W * 0.5f + 8, H * 0.5f + 8),
						  Color4F(0.76f, 0.48f, 0.09f, 1.0f)); // golden-brown border
	panel->drawSolidRect(Vec2(-W * 0.5f, -H * 0.5f), Vec2(W * 0.5f, H * 0.5f),
						  Color4F(1.0f, 0.99f, 0.94f, 1.0f));   // cream panel
	this->addChild(panel, 1);

	// 3) Title
	std::string title = PG_TITLE;
	auto titleLabel = Label::createWithTTF(title, KR_FONT_TTF, FRAME_WIDTH * 0.062f);
	titleLabel->setColor(Color3B(122, 66, 8));
	titleLabel->setPosition(center.x, center.y + H * 0.40f);
	this->addChild(titleLabel, 2);

	// 4) Multiplication problem
	m_problemLabel = Label::createWithTTF("", KR_FONT_TTF, FRAME_WIDTH * 0.095f);
	m_problemLabel->setColor(Color3B(30, 30, 30));
	m_problemLabel->setPosition(center.x, center.y + H * 0.26f);
	this->addChild(m_problemLabel, 2);

	// 5) Typed input
	m_inputLabel = Label::createWithTTF("", KR_FONT_TTF, FRAME_WIDTH * 0.095f);
	m_inputLabel->setColor(Color3B(30, 90, 165));
	m_inputLabel->setPosition(center.x, center.y + H * 0.13f);
	this->addChild(m_inputLabel, 2);

	// 6) Keypad (Menu + MenuItemLabel)
	auto menu = Menu::create();
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 3);

	const float keyFont = FRAME_WIDTH * 0.085f;
	const float gapX = W * 0.28f;
	const float gapY = H * 0.115f;
	const float baseY = center.y - H * 0.02f;

	auto makeKey = [&](const std::string& txt, const Color3B& col, const ccMenuCallback& cb) -> MenuItemLabel* {
		auto lbl = Label::createWithTTF(txt, KR_FONT_TTF, keyFont);
		lbl->setColor(col);
		auto item = MenuItemLabel::create(lbl, cb);
		menu->addChild(item);
		return item;
	};

	// 1..9 as a 3x3 grid
	for (int i = 1; i <= 9; ++i)
	{
		int col = (i - 1) % 3;
		int row = (i - 1) / 3;
		int d = i;
		auto item = makeKey(StringUtils::format("%d", d), Color3B(30, 30, 30), [this, d](Ref*) { onDigit(d); });
		item->setPosition(center.x + (col - 1) * gapX, baseY - row * gapY);
	}
	// Bottom row: [cancel] [0] [clear]
	auto cancelItem = makeKey(PG_CANCEL, Color3B(160, 60, 60), [this](Ref*) { onCancel(); });
	cancelItem->setPosition(center.x - gapX, baseY - 3 * gapY);
	auto zeroItem = makeKey("0", Color3B(30, 30, 30), [this](Ref*) { onDigit(0); });
	zeroItem->setPosition(center.x, baseY - 3 * gapY);
	auto clearItem = makeKey(PG_CLEAR, Color3B(90, 90, 90), [this](Ref*) { onClear(); });
	clearItem->setPosition(center.x + gapX, baseY - 3 * gapY);

	newProblem();
	return true;
}

void ParentalGate::newProblem()
{
	int a = 0, b = 0;
	do {
		a = rand() % 8 + 2; // 2..9
		b = rand() % 8 + 2; // 2..9
	} while (a * b < 11);   // keep the answer two digits (11..81)

	m_answer = a * b;
	m_input.clear();
	m_problemLabel->setString(StringUtils::format("%d \xC3\x97 %d = ?", a, b)); // \xC3\x97 = multiply sign
	m_inputLabel->setString("");
}

void ParentalGate::onDigit(int d)
{
	if (m_input.size() >= 2) return;
	m_input += (char)('0' + d);
	m_inputLabel->setString(m_input);
	if (m_input.size() == 2) checkAnswer();
}

void ParentalGate::onClear()
{
	m_input.clear();
	m_inputLabel->setString("");
}

void ParentalGate::onCancel()
{
	SoundFactory::Instance()->play(SOUND_FILE_click_effect);
	close();
}

void ParentalGate::checkAnswer()
{
	if (atoi(m_input.c_str()) == m_answer)
	{
		auto cb = m_onSuccess; // copy first: close() may release this
		SoundFactory::Instance()->play(SOUND_FILE_pass_effect);
		close();
		if (cb) cb();
	}
	else
	{
		// wrong: shake and pick a new problem
		SoundFactory::Instance()->play(SOUND_FILE_skip_effect);
		if (m_problemLabel)
		{
			m_problemLabel->runAction(Sequence::create(
				MoveBy::create(0.04f, Vec2(10, 0)),
				MoveBy::create(0.08f, Vec2(-20, 0)),
				MoveBy::create(0.04f, Vec2(10, 0)),
				nullptr));
		}
		newProblem();
	}
}

void ParentalGate::close()
{
	this->removeFromParent();
}
