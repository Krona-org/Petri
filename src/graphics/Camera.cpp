#include "Camera.h"

Camera::Camera(int wigth, int height, glm::vec3 position)
{
	Camera::width = wigth;
	Camera::height = height;
	Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position,Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}
void Camera::Inputs(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Position += speed * Orientation;  // Вперёд
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position -= speed * glm::normalize(glm::cross(Orientation, Up)); // Влево
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position -= speed * Orientation;  // Назад 
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += speed * glm::normalize(glm::cross(Orientation, Up)); // Вправо
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Position -= speed * Up;           // Вверх
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Position += speed * Up;           // Вниз
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = 0.4f;                     // Ускорение
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        speed = 0.1f;                     // Обычная скорость
    }

    static bool cursorCaptured = false;

// захват курсора при ЛКМ
if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !cursorCaptured)
{
    cursorCaptured = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    firstClick = true;
}

// освобождение курсора при ESC
if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
{
    cursorCaptured = false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    firstClick = true;
}

if (!cursorCaptured)
    return; // если курсор не захвачен, вращение не считаем

// вращение камеры
double mouseX, mouseY;
glfwGetCursorPos(window, &mouseX, &mouseY);

float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

if (!((glm::angle(newOrientation, Up) <= glm::radians(5.0f)) || (glm::angle(newOrientation, -Up) <= glm::radians(5.0f))))
{
    Orientation = newOrientation;
}

Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
glfwSetCursorPos(window, (width / 2), (height / 2));

}