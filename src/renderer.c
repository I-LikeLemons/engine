#include "utils.h"
#include <glad/glad.h>
#include <renderer.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <shader.h>
#include <cglm/cglm.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static Batch_Renderer* c_r;

float WHITE[4] = {1,1,1,1};

void init_renderer(Batch_Renderer *r, int w, int h, int max) {
    //initilize some variables
    r->textures_loaded = 0;
    r->draw_count = 0;
    r->batch_limit = max;
    r->window_width = w;
    r->window_height = h;
    r->vertices = (Vertex*)malloc(max * sizeof(Vertex) * 4);

    //background stuff setup :)
    const float quad_verts[24] = {
	1.0f,-1.0f, 1.0f,0.0f,
	-1.0f,-1.0f, 0.0f,0.0f,
	-1.0f,1.0f, 0.0f,1.0f,

	1.0f, 1.0f, 1.0f,1.0f,
	1.0f,-1.0f, 1.0f,0.0f,
	-1.0f,1.0f, 0.0f,1.0f
    };

    glGenVertexArrays(1, &r->quadVAO);
    glGenBuffers(1, &r->quadVBO);
    glBindVertexArray(r->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, r->quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_verts), &quad_verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    //shader creation
    create_shader(&r->shader,"shaders/vertex_shader.vert", "shaders/fragment_shader.frag");
    create_shader(&r->FBOshader,"shaders/basic.vert", "shaders/framebuffer.frag");

    //set indicies
    int indicies[6 * r->batch_limit];
    for (int i = 0; i < r->batch_limit; i++) {
	indicies[0+(i*6)] = 0 + 4*i;
	indicies[1+(i*6)] = 1 + 4*i;
	indicies[2+(i*6)] = 3 + 4*i;
	indicies[3+(i*6)] = 1 + 4*i;
	indicies[4+(i*6)] = 2 + 4*i;
	indicies[5+(i*6)] = 3 + 4*i;
    }

    //create quad texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    GLubyte pixel_data[4] = {255,255,255,255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1,1,0,GL_RGBA, GL_UNSIGNED_BYTE, pixel_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    r->textures[0] = texture;
    r->textures_loaded += 1;

    //create and setup batch rendering VAO VBO and EBO
    glGenVertexArrays(1, &r->VAO);

    glGenBuffers(1, &r->VBO);
    glGenBuffers(1, &r->EBO);

    glBindVertexArray(r->VAO);

    glBindBuffer(GL_ARRAY_BUFFER,r->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * r->batch_limit, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,r->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, col));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, sample));
    glEnableVertexAttribArray(3);

    //set samples
    int samples[16];
    for (int i = 0; i < 16; i++) {
	samples[i] = i;
    }

    glUseProgram(r->shader.id);
    int idLoc = glGetUniformLocation(r->shader.id, "uTextures");
    glUniform1iv(idLoc, 16, samples);
    glUseProgram(0);

    //Framebuffer setup
    glGenFramebuffers(1, &r->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, r->FBO);
    
    glGenTextures(1,&r->framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, r->framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, r->window_width, r->window_height, 0,GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, r->framebufferTexture, 0);

    glGenRenderbuffers(1, &r->RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, r->RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, r->window_width, r->window_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, r->RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) printf("something went wrong womp womp!!!!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //set the global renderer to the current renderer
    c_r = r;
}

Sprite create_sprite(const char *path) {
    Sprite sprite;
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width,height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height,0,GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
    } else {
	printf("failed to create sprite\n");
	stbi_image_free(data);
	return sprite;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    sprite.height = height;
    sprite.width = width;
    sprite.id = c_r->textures_loaded;
    c_r->textures[c_r->textures_loaded] = texture;
    c_r->textures_loaded += 1;
    return sprite;
}


void quad_draw(float x, float y, float width, float height, float rotation, float scale_x, float scale_y, float x_offset, float y_offset, float color[4]) {
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3){x,y,1});
    glm_rotate(model,glm_rad(rotation), (vec3){0,0,1});
    glm_translate(model, (vec3){x_offset, y_offset, 0});
    glm_scale(model,(vec3){scale_x,scale_y,0});

    vec4 p1 = {width,height,0,1};
    vec4 p2 = {width,0,0,1};
    vec4 p3 = {0,0,0,1};
    vec4 p4 = {0,height,0,1};

    glm_mat4_mulv(model, p1, p1);
    glm_mat4_mulv(model, p2, p2);
    glm_mat4_mulv(model, p3, p3);
    glm_mat4_mulv(model, p4, p4);

    c_r->vertices[0 + (c_r->draw_count * 4)] = (Vertex){{p1[0], p1[1],0}, {color[0],color[1],color[2],color[3]}, {1,1}, 0};
    c_r->vertices[1 + (c_r->draw_count * 4)] = (Vertex){{p2[0], p2[1],0}, {color[0],color[1],color[2],color[3]}, {1,0}, 0};
    c_r->vertices[2 + (c_r->draw_count * 4)] = (Vertex){{p3[0], p3[1],0}, {color[0],color[1],color[2],color[3]}, {0,0}, 0};
    c_r->vertices[3 + (c_r->draw_count * 4)] = (Vertex){{p4[0], p4[1],0}, {color[0],color[1],color[2],color[3]}, {0,1},0};
    c_r->draw_count += 1;
}
void atlas_draw(Sprite s, float a_x, float a_y, float a_w, float a_h, float x, float y, float r, float s_x, float s_y, float x_offset, float y_offset, float color[4]) { 
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, (vec3){x,y,1});
    glm_rotate(model,glm_rad(r), (vec3){0,0,1});
    glm_translate(model, (vec3){x_offset, y_offset, 0});
    glm_scale(model,(vec3){s_x,s_y,0});

    vec4 p1 = {a_w,a_h,0,1};
    vec4 p2 = {a_w,0,0,1};
    vec4 p3 = {0,0,0,1};
    vec4 p4 = {0,a_h,0,1};

    glm_mat4_mulv(model, p1, p1);
    glm_mat4_mulv(model, p2, p2);
    glm_mat4_mulv(model, p3, p3);
    glm_mat4_mulv(model, p4, p4);

    c_r->vertices[0 + (c_r->draw_count * 4)] = (Vertex){{p1[0], p1[1],0}, {color[0],color[1],color[2],color[3]}, {(a_x + a_w)/(float)s.width,(a_y + a_h)/(float)s.height}, s.id};
    c_r->vertices[1 + (c_r->draw_count * 4)] = (Vertex){{p2[0], p2[1],0}, {color[0],color[1],color[2],color[3]}, {(a_x + a_w)/(float)s.width,a_y/(float)s.height}, s.id};
    c_r->vertices[2 + (c_r->draw_count * 4)] = (Vertex){{p3[0], p3[1],0}, {color[0],color[1],color[2],color[3]}, {a_x/(float)s.width,a_y/(float)s.height}, s.id};
    c_r->vertices[3 + (c_r->draw_count * 4)] = (Vertex){{p4[0], p4[1],0}, {color[0],color[1],color[2],color[3]}, {a_x/(float)s.width,(a_y + a_h)/(float)s.height}, s.id};

    c_r->draw_count += 1;
}

void render_batch() {
    glBindVertexArray(c_r->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, c_r->VBO);

    for (int i = 0; i < c_r->textures_loaded; i++) {
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, c_r->textures[i]);
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * c_r->draw_count * 4, c_r->vertices);
    glDrawElements(GL_TRIANGLES, 6 * c_r->draw_count, GL_UNSIGNED_INT, NULL);

    c_r->draw_count = 0;
    glBindTexture(GL_TEXTURE_2D, 0);
}


#define LENGTH 100
void renderer_update(Batch_Renderer *r, int w, int h, int off, float delta) {
    glViewport(0,0, r->window_width,r->window_height);

    //draw to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, r->FBO);

    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    

    glUseProgram(r->shader.id);
    mat4 view, proj, u_transform;
    glm_mat4_identity(view);
    glm_mat4_identity(proj);
    glm_translate(view, (vec3){r->window_width/2.0,r->window_height/2.0,0});
    glm_ortho(0, r->window_width, r->window_height, 0, -1, 1, proj);
    glm_mat4_mul(proj, view, u_transform);
    int transformLoc = glGetUniformLocation(r->shader.id,"u_transform");
    glUniformMatrix4fv(transformLoc,1, 	GL_FALSE, &u_transform[0][0]);

    render_batch();

    //draw framebuffer
    glViewport(0,off,w,h);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(r->FBOshader.id);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(r->FBOshader.id,"screenTexture"),0);
    glBindVertexArray(r->quadVAO);
    glBindTexture(GL_TEXTURE_2D, r->framebufferTexture);
    glDrawArrays(GL_TRIANGLES, 0,6);

}

void cleanup_renderer(Batch_Renderer* r) {
    glDeleteProgram(r->shader.id);
    glDeleteProgram(r->FBOshader.id);

    glDeleteVertexArrays(1, &r->VAO);
    glDeleteVertexArrays(1, &r->quadVAO);

    glDeleteBuffers(1, &r->VBO);
    glDeleteBuffers(1, &r->quadVBO);

    glDeleteFramebuffers(1, &r->FBO);
    glDeleteRenderbuffers(1, &r->RBO);

    glDeleteBuffers(1, &r->EBO);
    free(r->vertices);
}
