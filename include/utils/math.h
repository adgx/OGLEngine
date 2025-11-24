#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/matrix4x4.h>

using Matrix4 = glm::mat4;
using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;


Matrix4 aiMat4_2_Mat4(const aiMatrix4x4& m)
{
    Matrix4 res;
    std::memcpy(&res, &m, sizeof(Matrix4));
    return res;
}

Matrix4 inverse(Matrix4 m)
{
    return glm::inverse(m);
}