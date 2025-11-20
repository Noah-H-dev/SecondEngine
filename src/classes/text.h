//
// Created by griff on 11/17/2025.
//

#ifndef OPENGLCLION_TEXT_H
#define OPENGLCLION_TEXT_H
#include <iostream>
#include <map>

#include "shader.h"
#include "grid.h"
#include "freetype/freetype.h"
#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size; // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    unsigned int advance;
};
struct text {
    shader Shader;
    glm::vec3 backgroundColor;
    glm::vec3 textColor;
    unsigned int VAO, VBO;
    std::map<char, Character> Characters;
    unsigned int SCR_WIDTH, SCR_HEIGHT;
    grid world;

    text(const std::string& fontPath, int scr_width, int scr_height, std::string vsShader, std::string fsShader,grid Grid)
    :Shader(vsShader.c_str(),fsShader.c_str()),
    world (Grid) {
        SCR_WIDTH = scr_width;
        SCR_HEIGHT = scr_height;
        world = Grid;
        backgroundColor = glm::vec3(1.0f, 0.0f, 0.5f);
        textColor = glm::vec3(1.0f, 1.0f, 1.0f);
        FT_Library ft;
        FT_Face face;
        int error = FT_Init_FreeType( &ft );
        if (error){
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" <<
            std::endl;
        }
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        }
        FT_Set_Pixel_Sizes(face, 0, world.tileSize);
        loadFont(face);
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    ~text() {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
    }
    void update (int width, int height) {
        SCR_HEIGHT = height;
        SCR_WIDTH = width;
    }
    void draw(std::string text, float x, float y, float scale) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        this->Shader.use();
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH),0.0f , static_cast<float>(SCR_HEIGHT));
        this->Shader.setMat4("projection", projection);
        this->Shader.setVec3("textColor", textColor);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        float xpos_pixel = world.gridPos(x);
        float ypos_pixel = world.gridPos(y);
        std::string::const_iterator c;

        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];

            float xpos = xpos_pixel + ch.Bearing.x * scale;
            float ypos = ypos_pixel - (ch.Size.y - ch.Bearing.y) * scale;
            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // might have to play with this
            float vertices[6][4] = {
                { xpos,     ypos+h,     0.0f, 0.0f },
                { xpos+w,   ypos+h,     1.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },

                { xpos,     ypos,       0.0f, 1.0f },
                { xpos+w,   ypos+h,     1.0f, 0.0f },
                { xpos+w,   ypos,       1.0f, 1.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            xpos_pixel += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
    }

    void loadFont( FT_Face face) {
        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            this->Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
struct GUI_Element {
    shader Shader;
    int x, y;
    float xScale, yScale;
    glm::vec3 color;
    unsigned int VBO, VAO;
    std::vector<float> vertecies;
    float *displayValue1;
    float *displayValue2;
    text Text;
    std::vector<float> container = { // pivot bottom left
        1.0f, 0.0f, 0.0f,      1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,      1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,      1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,      0.0f, 1.0f,
    };
    GUI_Element(shader &GUIShader, int x, int y, float xScale, float yScale, glm::vec3 UIColor,
        float &display1, float &display2,
        const std::string& fontPath,
        int scr_width, int scr_height,
        std::string vsShader, std::string fsShader,grid Grid):
        Shader(GUIShader),
        Text(fontPath, scr_width, scr_height,vsShader,fsShader, Grid)  {
        this->x = x, this->y = y;
        this->xScale = xScale,this-> yScale = yScale;
        this->displayValue1 = &display1;
        this->displayValue2 = &display2;
        this->color = UIColor;
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);

        // Bind VAO first - it will capture all subsequent state
        glBindVertexArray(this->VAO);

        // Setup VBO
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->container.size()*sizeof(float), this->container.data(), GL_STATIC_DRAW);

        // Configure vertex attributes (while VAO is bound)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        this->Shader.use();
        this->Shader.setVec3("aColor", this->color);
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(scr_width),0.0f , static_cast<float>(scr_height));
        this->Shader.setMat4("projection", projection);
        for (int i = 0; i < container.size() - 5; i+= 5) {
            container.at(i) * xScale;
            container.at(i+1) * yScale;
        }
    }
    GUI_Element(shader &GUIShader, int x, int y, float xScale, float yScale, glm::vec3 UIColor, float &display1,const std::string& fontPath,
        int scr_width, int scr_height,
        std::string vsShader, std::string fsShader,grid Grid):
        Shader(GUIShader),
        Text(fontPath, scr_width, scr_height,vsShader,fsShader, Grid){
        this->x = x, this->y = y;
        this->xScale = xScale,this-> yScale = yScale;
        this->displayValue1 = &display1;
        this->displayValue2 = nullptr;
        this->color = UIColor;
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);

        // Bind VAO first - it will capture all subsequent state
        glBindVertexArray(this->VAO);

        // Setup VBO
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->container.size()*sizeof(float), this->container.data(), GL_STATIC_DRAW);

        // Configure vertex attributes (while VAO is bound)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        this->Shader.use();
        this->Shader.setVec3("aColor", this->color);
        //Add ability to resize window through a "resize/update function" currently will just warp with sceen size change
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(scr_width),0.0f , static_cast<float>(scr_height));
        this->Shader.setMat4("projection", projection);
        for (int i = 0; i < container.size() - 5; i+= 5) {
            container.at(i) * xScale;
            container.at(i+1) * yScale;
        }
    }
    void draw() {
        this->Shader.use();
        glBindVertexArray(this->VAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        Text.draw(std::to_string(*displayValue1), x, y, 1);

    }
};


#endif //OPENGLCLION_TEXT_H