#pragma once

enum class KeyState { UP, PRESSED, DOWN, RELEASED };

struct Input
{
	KeyState jump, crouch, left, right, yes, no;
};