#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

void bindVertices(GLuint &VAO, GLuint &VBO, const GLfloat *vertices, size_t vertices_sz) 
{
    // bind the VAO
    glBindVertexArray(VAO);
    // send vertex data to vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_sz, vertices, GL_STATIC_DRAW);
    // tell OpenGL how to interpret the vertex data
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    // Texture attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // unbind the VBO - we can do this because it was registered as VBO
    // when we called glVertexAttribPointer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // unbind the VAO
    glBindVertexArray(0);

}

void drawCubeVertices(GLuint &VAO, GLuint nVertices)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, nVertices);
    glBindVertexArray(0);
}

void drawLineVertixes(GLuint &VAO, GLuint nVertices)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, nVertices);
    glBindVertexArray(0);
}

void drawPointVertices(GLuint &VAO, GLuint nVertices)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, nVertices);
    glBindVertexArray(0);
}

#endif