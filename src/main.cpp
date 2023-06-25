#include "public.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


glm::vec3 g_camera_front = glm::vec3(0, 0, -1);
glm::vec3 g_camera_up = glm::vec3(0, 1, 0);
glm::vec3 g_camer_pos = glm::vec3(0, 0, 3);

float g_last_x = 400;
float g_last_y = 300;
float g_yaw = -90.0;
float g_pitch = 0;
float g_fov = 45;

void frameSizeChange(GLFWwindow *window, int width, int height) {
    ::std::cout << "width X height = " << width << " X " << height << ::std::endl;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static int firstMouse = true;
    if (firstMouse) // 这个bool变量初始时是设定为true的
    {
        g_last_x = xpos;
        g_last_y = ypos;
        firstMouse = false;
    }
    
    
    float xoffset = xpos - g_last_x;
    float yoffset = g_last_y - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
    g_last_x = xpos;
    g_last_y = ypos;

    float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    g_yaw += xoffset;
    g_pitch += yoffset;

    if (g_pitch > 89.0f)
        g_pitch = 89.0f;
    if (g_pitch < -89.0f)
        g_pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(g_pitch)) * cos(glm::radians(g_yaw));
    front.y = sin(glm::radians(g_pitch));
    front.z = cos(glm::radians(g_pitch)) * sin(glm::radians(g_yaw));
    g_camera_front = glm::normalize(front);

    ::std::cout << g_yaw << "\t "<< g_pitch << ::std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (g_fov >= 1.0f && g_fov <= 45.0f)
        g_fov -= yoffset;
    if (g_fov <= 1.0f)
        g_fov = 1.0f;
    if (g_fov >= 45.0f)
        g_fov = 45.0f;
}
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_KEY_SPACE) {
        glfwSetWindowShouldClose(window, true);
    }
    
    
    float cameraSpeed = 0.05f; // adjust accordingly
     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
         g_camer_pos += cameraSpeed * g_camera_front;
     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
         g_camer_pos -= cameraSpeed * g_camera_front;
     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
         g_camer_pos -= glm::normalize(glm::cross(g_camera_front, g_camera_up)) * cameraSpeed;
     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
         g_camer_pos += glm::normalize(glm::cross(g_camera_front, g_camera_up)) * cameraSpeed;
    
}

class Texture {
private:
    
#define TEXTURE_INVALID_ID_NUM 0
    int width_{0};
    int height_{0};
    int channels_{0};
    unsigned int tex_id_off_{0};
    unsigned int id_{TEXTURE_INVALID_ID_NUM};
public:
    void Create(::std::string file_path, unsigned int tex_id_off = 0) {
        tex_id_off_ = tex_id_off;
        
        stbi_set_flip_vertically_on_load(true);
        unsigned char*  data = stbi_load(file_path.c_str(), &width_, &height_, &channels_, 0);
        if (!data)
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        
        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        stbi_image_free(data);
        
    }

    void Enable() {
        glActiveTexture( GL_TEXTURE0 + tex_id_off_);
        glBindTexture(GL_TEXTURE_2D, id_);
    }
    
    unsigned int GetTexIdOff() {
        return tex_id_off_;
    }
};




class ShaderSource {
private:
#define SHADER_INVALID_ID_NUM 0
    ::std::string src_;
    unsigned int id_{SHADER_INVALID_ID_NUM};

public:
    ~ShaderSource() {
        if (id_ != SHADER_INVALID_ID_NUM) {
            glDeleteShader(id_);
        }
    }

    void Load(::std::string file_path) {
        src_ = "";
        ::std::fstream fp(file_path);

        char data[256] = {0};
        while (!fp.eof()) {
            fp.getline(data, 256);
            src_ += ::std::string(data) + "\n";
        }
        fp.close();

        ::std::cout << src_ << ::std::endl;
    }

    void Create(::std::string file_path, int shader_type) {
        Load(file_path);

        id_ = glCreateShader(shader_type);
        const char *src_data = src_.c_str();
        glShaderSource(id_, 1, &src_data, NULL);
        glCompileShader(id_);

        int success;
        char infoLog[512];
        glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(id_, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
    }


    unsigned int GetShaderId() {
        return id_;
    }
};

class ShaderProgram {
private:
#define PROGRAM_INVALID_ID_NUM 0

    unsigned int id_{PROGRAM_INVALID_ID_NUM};

public:
    ~ShaderProgram() {
        if (id_ != SHADER_INVALID_ID_NUM) {
            glDeleteProgram(id_);
        }
    }

    void Create(unsigned int vs_id, unsigned int fs_id) {
        id_ = glCreateProgram();

        glAttachShader(id_, vs_id);
        glAttachShader(id_, fs_id);
        glLinkProgram(id_);

        int success;
        char infoLog[512];
        glGetProgramiv(id_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id_, 512, NULL, infoLog);
            std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
    }
    
    void SetTexture(Texture& texture, ::std::string name) {
        Enable();
        texture.Enable();
        glUniform1i( glGetUniformLocation(id_, name.c_str()), texture.GetTexIdOff());
        
    }
    
    void SetMatrix(glm::mat4 trans, ::std::string name) {
        Enable();
        glUniformMatrix4fv( glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
    }

    void Enable() {
        glUseProgram(id_);
    }
};

class VertexArray {
private:
#define VAO_INVALID_ID_NUM 0
#define VBO_INVALID_ID_NUM 0
#define EBO_INVALID_ID_NUM 0
    unsigned int vao_{VAO_INVALID_ID_NUM};
    unsigned int vbo_{VBO_INVALID_ID_NUM};
    unsigned int ebo_{EBO_INVALID_ID_NUM};

    unsigned int vao_ele_num_{0};
    unsigned int ebo_ele_num_{0};

public:
    ~VertexArray() {
        glDeleteBuffers(1, &vbo_);
        glDeleteVertexArrays(1, &vao_);
    }

    struct Attrib {
        Attrib(unsigned int location, unsigned int size, unsigned int offset) :
            location_(location), size_(size), offset_(offset) {
        }

        unsigned int location_;
        unsigned int size_;
        unsigned int offset_;
    };

    void Create(float *data, unsigned int size, unsigned int step, ::std::vector<Attrib> vertex_attrib) {
        vao_ele_num_ = size / step;
        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);

        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        for (auto &iter : vertex_attrib) {
            glVertexAttribPointer(iter.location_, iter.size_, GL_FLOAT, GL_FALSE, step,
                                  (void *)iter.offset_);
            glEnableVertexAttribArray(iter.location_);
        }
    }

    void Create(float *data, unsigned int size, unsigned int step,
                ::std::vector<Attrib> vertex_attrib,
                unsigned int *indices,
                unsigned int indices_size) {
        vao_ele_num_ = size / step;
        ebo_ele_num_ = indices_size / sizeof(unsigned int);

        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);

        glGenBuffers(1, &ebo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices,
                     GL_STATIC_DRAW);

        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

        for (auto &iter : vertex_attrib) {
            glVertexAttribPointer(iter.location_, iter.size_, GL_FLOAT, GL_FALSE, step,
                                  (void *)iter.offset_);
            glEnableVertexAttribArray(iter.location_);
        }
    }

    void Draw(unsigned int draw_type = GL_TRIANGLES,
              unsigned int draw_num = INT_MAX) {
        glBindVertexArray(vao_);
        if (ebo_ele_num_ == 0) {
            glDrawArrays(draw_type, 0,
                         vao_ele_num_ > draw_num ? draw_num : vao_ele_num_);
        } else {
            glDrawElements(draw_type,
                           ebo_ele_num_ > draw_num ? draw_num : ebo_ele_num_,
                           GL_UNSIGNED_INT, 0);
        }
    }
};

class Matrix {
public:
    glm::mat4 model_{glm::mat4(1.0f)};
    glm::mat4 view_{glm::mat4(1.0f)};
    glm::mat4 projection_{glm::mat4(1.0f)};
 
    void SetModel(glm::vec3 location,  glm::vec3 rotate_axis, float radians) {
        model_ = glm::mat4(1.0f);
        model_ = glm::translate(model_, location);
          model_ = glm::rotate(model_, glm::radians(radians), rotate_axis);
    }
    
    void SetView(glm::vec3 camara_pos, glm::vec3 front, glm::vec3 head_up ) {
        view_ = glm::lookAt(camara_pos, camara_pos + front, head_up);
    }
    
    void SetProjection(float fov, float aspect, float near, float far) {
        projection_ = glm::perspective(glm::radians(fov), aspect, near, far);
    }
    
    
};


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 计算窗口中心位置
    int centerX = 800 / 2;
    int centerY = 600 / 2;

    // 设置鼠标光标位置为窗口中心
    glfwSetCursorPos(window, centerX, centerY);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, frameSizeChange);

    ShaderSource vs;
    vs.Create(::std::string(RESOURCE_DIR) + "/basic.vs", GL_VERTEX_SHADER);
    ShaderSource fs;
    fs.Create(::std::string(RESOURCE_DIR) + "/basic.fs", GL_FRAGMENT_SHADER);

    ShaderProgram program;
    program.Create(vs.GetShaderId(), fs.GetShaderId());

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        // 注意索引从0开始!
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    VertexArray vao;
    vao.Create(
        vertices, sizeof(vertices), 5 * sizeof(float),
        ::std::vector<VertexArray::Attrib>{
            VertexArray::Attrib(0, 3, 0),
            VertexArray::Attrib(2, 2, 3 * sizeof(float))
            });

    Texture texture1;
    texture1.Create(::std::string(RESOURCE_DIR) + "/texture.jpeg", 0);
    Texture texture2;
    texture2.Create(::std::string(RESOURCE_DIR) + "/texture.jpg", 1);

    program.SetTexture(texture1, "texture1");
    program.SetTexture(texture2, "texture2");

    
    Matrix matrix;
    matrix.SetView(g_camer_pos, g_camera_front, g_camera_up);
    matrix.SetProjection(g_fov, 800 * 1.f/ 600, 0.1, 100);

    program.SetMatrix(matrix.model_, "model");
    program.SetMatrix(matrix.view_, "view");
    program.SetMatrix(matrix.projection_, "projection");
   
    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        program.Enable();
        matrix.SetView(g_camer_pos, g_camera_front, g_camera_up);
        matrix.SetProjection(g_fov, 800 * 1.f / 600, 0.1, 100);

        texture1.Enable();
        texture2.Enable();
        
        for(unsigned int i = 0; i < 10; i++)
        {
            matrix.SetModel(cubePositions[i], glm::vec3(1.0f, 0.3f, 0.5f), 20.0f * (i + 1) * (float)glfwGetTime());

    


            program.SetMatrix(matrix.model_, "model");
            program.SetMatrix(matrix.view_, "view");
            program.SetMatrix(matrix.projection_, "projection");

          glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        
        

        
        
        vao.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
