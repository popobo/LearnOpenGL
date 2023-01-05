#include "glad/glad.h"

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

#define GLCall(x)                                                              \
    GLClearError();                                                            \
    x;                                                                         \
    GLLogCall(#x, __FILE__, __LINE__)