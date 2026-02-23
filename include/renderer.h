#ifndef RENDERER_H
#define RENDERER_H
#include "shader.h"
typedef struct {
    float pos[3];
    float col[4];
    float tex_coords[2];
    int sample;
} Vertex;

typedef struct {
    int width;
    int height;
    int id;
} Sprite;

typedef struct {
    int window_width;
    int window_height;
    unsigned int VAO;
    unsigned int quadVAO;
    unsigned int FBO;
    unsigned int RBO;
    unsigned int VBO;
    unsigned int quadVBO;
    unsigned int EBO;
    unsigned int quadEBO;
    unsigned int framebufferTexture;
    unsigned int noise_texture;
    Shader shader;
    Shader FBOshader;
    unsigned int textures[16];
    int textures_loaded;
    unsigned int batch_limit;
    int draw_count;
    Vertex* vertices;
} Batch_Renderer;

extern float WHITE[4];

Sprite create_sprite(const char *path);
void init_renderer(Batch_Renderer* r, int w, int h, int max);
void renderer_update(Batch_Renderer *r, int w, int h, int off, float delta);
void render_batch();
void atlas_draw(Sprite s, float a_x, float a_y, float a_w, float a_h, float x, float y, float r, float s_x, float s_y, float x_offset, float y_offset, float color[4]);
void quad_draw(float x, float y, float width, float height, float rotation, float scale_x, float scale_y, float x_offset, float y_offset, float color[4]);
void cleanup_renderer(Batch_Renderer* r);

#endif 
