#pragma once

#include "cocos2d.h"
#include <functional>
#include <string>

using namespace cocos2d;

// Parental gate for the Kids category (Made for Kids).
// Call present() in front of actions a child must not trigger by mistake:
// in-app purchase, external links, sharing.
// The user must solve a simple multiplication on a number keypad; on success
// onSuccess() is invoked. Trivial for an adult, out of reach for a toddler.
class ParentalGate : public Node
{
public:
	// Show modally on top of parent. Ignored if one is already showing (no stacking).
	static void present(Node* parent, const std::function<void()>& onSuccess);

	bool initWithCallback(const std::function<void()>& onSuccess);

private:
	std::function<void()>	m_onSuccess;
	int						m_answer = 0;
	std::string				m_input;
	Label*					m_problemLabel = nullptr;
	Label*					m_inputLabel = nullptr;

	void newProblem();
	void onDigit(int d);
	void onClear();
	void onCancel();
	void checkAnswer();
	void close();
};
