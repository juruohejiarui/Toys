#pragma once

extern float lstFrameTime;

#define updateTime() (lstFrameTime = glfwGetTime())

#define getCurTime() ((float)glfwGetTime())