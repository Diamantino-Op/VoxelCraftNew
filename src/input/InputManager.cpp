#include "InputManager.h"
#include "../graphics/WindowManager.h"

InputManager::InputManager() : window_(nullptr), scroll_(0.0f), mousePos_(glm::vec2(0.0f, 0.0f)), prevMousePos_(glm::vec2(0.0f, 0.0f))
{
	// Get window
	const WindowManager &windowManager = WindowManager::Instance();
	window_ = windowManager.GetWindow();

	// Get mouse variables
	double xpos, ypos;
	glfwGetCursorPos(window_, &xpos, &ypos);
	mousePos_ = { xpos, ypos };
	prevMousePos_ = mousePos_;

	// Set glfw callbacks
	glfwSetKeyCallback(window_, &InputManager::KeyCallback);
	glfwSetCursorPosCallback(window_, &InputManager::MouseCallback);
	glfwSetMouseButtonCallback(window_, &InputManager::MouseButtonCallback);
	glfwSetScrollCallback(window_, &InputManager::ScrollCallback);
}

bool InputManager::GetKeyPressed(int key)
{
	return keyStates_[key] == KEY_PRESSED;
}

bool InputManager::GetKey(int key)
{
	const KeyState &state = keyStates_[key];
	return state == KEY_PRESSED || state == KEY_HELD;
}

bool InputManager::GetKeyReleased(int key)
{
	return keyStates_[key] == KEY_RELEASED;
}

glm::vec2 InputManager::GetDeltaMouse() const
{
	return prevMousePos_ - mousePos_;
}

float InputManager::GetScroll() const
{
	return scroll_;
}

void InputManager::Update()
{
	scroll_ = 0.0f;
	prevMousePos_ = mousePos_;

	UpdateKeyStates();
	glfwPollEvents();
}

void InputManager::UpdateKeyStates()
{
	for (auto& keyState : keyStates_)
	{
		if (keyState.second == KEY_PRESSED)
			keyState.second = KEY_HELD;

		if (keyState.second == KEY_RELEASED)
			keyState.second = KEY_UP;
	}
}

void InputManager::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	InputManager &input = InputManager::Instance();

	// Add or set key data in keyStates_
	switch (action)
	{
	case GLFW_PRESS:
		input.keyStates_[key] = KEY_PRESSED;
		break;
	case GLFW_RELEASE:
		input.keyStates_[key] = KEY_RELEASED;
		break;
	default:
		break;
	}
}

void InputManager::MouseCallback(GLFWwindow *window, double xpos, double ypos)
{
	InputManager::Instance().mousePos_ = { xpos, ypos };
}

void InputManager::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	InputManager::Instance().KeyCallback(window, button, 0, action, mods);
}

void InputManager::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	InputManager::Instance().scroll_ = static_cast<float>(yoffset);
}
